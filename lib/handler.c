/*
 * handler.c
 *
 * $Author: why $
 * $Date: 2005/09/20 05:21:06 $
 *
 * Copyright (C) 2003 why the lucky stiff
 */

#include "syck.h"
#include <assert.h>
extern int syckdebug;

SYMID
syck_hdlr_add_node( SyckParser *p, SyckNode *n )
{
    SYMID id;

    if ( ! n->id ) 
    {
        n->id = (p->handler)( p, n );
    }
    id = n->id;

    if ( n->anchor == NULL )
    {
        syck_free_node( &n );
    }
    return id;
}

/* a must be freshly allocated, owned by the node (as n->anchor).
   n is owned by the parser, not the table.
   free any previous anchor.
 */
SyckNode *
syck_hdlr_add_anchor( SyckParser *p, char *a, SyckNode *n )
{
    SyckNode *ntmp = NULL;

    if (n->anchor) {
        DPRINTF((stderr, "DEBUG %s Free stale anchor '%s' in %p\n", __FUNCTION__, a, n));
        if (p->anchors) {
            st_delete(p->anchors, (void*)&(n->anchor), (void*)&n);
        }
        S_FREE(n->anchor);
    }
    n->anchor = a;
    DPRINTF((stderr, "DEBUG %s '%s'\n", __FUNCTION__, a));
    if ( p->bad_anchors != NULL )
    {
        SyckNode *bad;
        if ( st_lookup( p->bad_anchors, (st_data_t)a, (void *)&bad ) )
        {
            if ( n->kind != syck_str_kind )
            {
                n->id = bad->id;
                (p->handler)( p, n );
            }
        }
    }
    if ( p->anchors == NULL )
    {
        p->anchors = st_init_strtable();
    }
    assert(p->anchors != NULL);
    if ( st_lookup( p->anchors, (st_data_t)a, (void *)&ntmp ) )
    {
        if ( ntmp != (void *)1 )
        {
            syck_free_node( &ntmp ); // frees the old node and its anchor
        }
    }
    // this inserts not an numeric symid, but the pointer to the anchor
    st_insert( p->anchors, (st_data_t)a, (st_data_t)n );
    return n;
}

void
syck_hdlr_remove_anchor( SyckParser *p, char *a )
{
    char *atmp = a;
    SyckNode *ntmp;
    DPRINTF((stderr, "DEBUG %s '%s'\n", __FUNCTION__, a));
    if ( p->anchors == NULL )
    {
        p->anchors = st_init_strtable();
    }
    assert(p->anchors != NULL);
    if ( st_delete( p->anchors, (void *)&atmp, (void *)&ntmp ) )
    {
        DPRINTF(( stderr, "DEBUG Removed anchor '%s'\n", atmp ));
        if ( ntmp != (void *)1 )
        {
            syck_free_node( &ntmp );
        }
    }
    st_insert( p->anchors, (st_data_t)a, (st_data_t)1 );
}

SyckNode *
syck_hdlr_get_anchor( SyckParser *p, char *a )
{
    SyckNode *n = NULL;

    DPRINTF((stderr, "DEBUG %s '%s'\n", __FUNCTION__, a));
    if ( p->anchors != NULL )
    {
        if ( st_lookup( p->anchors, (st_data_t)a, (void *)&n ) )
        {
            if ( n != (void *)1 )
            {
                S_FREE( a );
                return n;
            }
            else
            {
                if ( p->bad_anchors == NULL )
                {
                    p->bad_anchors = st_init_strtable();
                }
                assert(p->bad_anchors != NULL);
                if ( ! st_lookup( p->bad_anchors, (st_data_t)a, (void *)&n )
                     && p->bad_anchor_handler)
                {
                    n = (p->bad_anchor_handler)( p, a );
                    st_insert( p->bad_anchors, (st_data_t)a, (st_data_t)n );
                }
            }
        }
    }

    if ( n == NULL || n == (void *)1)
    {
        if (p->bad_anchor_handler)
            n = (p->bad_anchor_handler)( p, a );
        else {
            n = syck_new_str( "", scalar_plain );
            n->type_id = syck_taguri( YAML_DOMAIN, "null", 4 );
            n->anchor = a;
            DPRINTF(( stderr, "DEBUG Added empty node %p for anchor '%s'\n", n, a));
            st_insert( p->bad_anchors, (st_data_t)a, (st_data_t)n );
            return n;
        }
    }

    if ( n->anchor != NULL )
    {
        S_FREE( a );
    }
    else
    {
        n->anchor = a;
    }

    return n;
}

void
syck_add_transfer( char *uri, SyckNode *n, int taguri )
{
    if ( n->type_id != NULL )
    {
        S_FREE( n->type_id );
    }
    if ( taguri == 0 )
    {
        n->type_id = uri;
        return;
    }

    n->type_id = syck_type_id_to_uri( uri );
    S_FREE( uri );
}

char *
syck_xprivate( const char *type_id, int type_len )
{
    char *uri = S_ALLOC_N( char, type_len + 14 );
    uri[0] = '\0';
    strcat( uri, "x-private:" );
    strncat( uri, type_id, type_len );
    return uri;
}

__attribute__malloc__
__attribute__warn_unused_result__
char *
syck_taguri( const char *domain, const char *type_id, int type_len )
{
    char *uri = S_ALLOC_N( char, strlen( domain ) + type_len + 14 );
    uri[0] = '\0';
    strcat( uri, "tag:" );
    strcat( uri, domain );
    strcat( uri, ":" );
    strncat( uri, type_id, type_len );
    return uri;
}

int 
syck_try_implicit( SHIM(SyckNode *n) )
{
    UNUSED(n);
    return 1;
}

