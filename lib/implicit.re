/*
 * implicit.re
 *
 * $Author: why $
 * $Date: 2005/11/13 23:43:56 $
 *
 * Copyright (C) 2003 why the lucky stiff
 * Copyright (C) 2026 Reini Urban
 */

#include "syck.h"
#include <string.h>
#include <assert.h>

#if !defined(YYCSZ) || YYCSZ == 8
#define YYCSZ 8
#define YYCTYPE     unsigned char
#elif YYCSZ == 16
#include <stdint.h>
#define YYCTYPE     uint16_t
#elif YYCSZ == 32
#include <stdint.h>
#define YYCTYPE     uint32_t
#endif

#define YYCURSOR    cursor
#define YYMARKER    marker
#define YYLIMIT     limit
#define YYFILL(n)   {}

void try_tag_implicit8( SyckNode *n, int taguri );
void try_tag_implicit16( SyckNode *n, int taguri );
void try_tag_implicit32( SyckNode *n, int taguri );

#if YYCSZ == 8
void
try_tag_implicit(const SyckParser *p, SyckNode *n )
{
    switch(p->input_type) {
    case syck_yaml_utf8:
        return try_tag_implicit8(n, p->taguri_expansion);
    case syck_yaml_utf16:
        return try_tag_implicit16(n, p->taguri_expansion);
    case syck_yaml_utf32:
        return try_tag_implicit32(n, p->taguri_expansion);
    case syck_bytecode_utf8:
    case syck_yaml_utf32be:
    case syck_yaml_utf16be:
    default:
        /* unsupported */
        assert(0);
        return;
    }
}
#endif

void
#if YYCSZ == 8
try_tag_implicit8( SyckNode *n, int taguri )
#elif YYCSZ == 16
try_tag_implicit16( SyckNode *n, int taguri )
#elif YYCSZ == 32
try_tag_implicit32( SyckNode *n, int taguri )
#endif
{
    const char *tid = "";
    switch ( n->kind )
    {
        case syck_str_kind:
#if YYCSZ == 8
            tid = syck_match_implicit8( (uint8_t*)n->data.str->ptr, n->data.str->len );
#elif YYCSZ == 16
            tid = syck_match_implicit16( (uint16_t*)n->data.str->ptr, n->data.str->len );
#elif YYCSZ == 32
            tid = syck_match_implicit32( (uint32_t*)n->data.str->ptr, n->data.str->len );
#endif
            break;

        case syck_seq_kind:
            tid = "seq";
            break;

        case syck_map_kind:
            tid = "map";
            break;

        default:
            break;
    }
    if ( n->type_id != NULL ) {
       S_FREE( n->type_id );
    }
    if ( taguri == 1 )
    {
        n->type_id = syck_taguri( YAML_DOMAIN, tid, strlen( tid ) );
    } else {
        n->type_id = syck_strndup( tid, strlen( tid ) );
    }
}

const char*
#if YYCSZ == 8
syck_match_implicit8
#elif YYCSZ == 16
syck_match_implicit16
#elif YYCSZ == 32
syck_match_implicit32
#endif
                      ( const YYCTYPE *str, size_t len )
{
    YYCTYPE *marker;
    YYCTYPE *cursor = (YYCTYPE *)str;
    const YYCTYPE *limit = str + len;

/*!re2c

NULL = [\000] ;
ANY = [\001-\377] ;
DIGIT = [0-9] ;
DIGITSC = [0-9,] ;
DIGITSP = [0-9.] ;
YEAR = DIGIT DIGIT DIGIT DIGIT ;
MON = DIGIT DIGIT ;
SIGN = [-+] ;
HEX = [0-9a-fA-F,] ;
OCT = [0-7,] ;
INTHEX = SIGN? "0x" HEX+ ;
INTOCT = SIGN? "0" OCT+ ;
INTSIXTY = SIGN? DIGIT DIGITSC* ( ":" [0-5]? DIGIT )+ ;
INTCANON = SIGN? ( "0" | [1-9] DIGITSC* ) ;
FLOATFIX = SIGN? DIGIT DIGITSC* "." DIGITSC* ;
FLOATEXP = SIGN? DIGIT DIGITSC* "." DIGITSP* [eE] SIGN DIGIT+ ;
FLOATSIXTY = SIGN? DIGIT DIGITSC* ( ":" [0-5]? DIGIT )+ "." DIGITSC* ;
INF = ( "inf" | "Inf" | "INF" ) ;
FLOATINF = [+]? "." INF ;
FLOATNEGINF = [-] "." INF ;
FLOATNAN = "." ( "nan" | "NaN" | "NAN" ) ;
NULLTYPE = ( "~" | "null" | "Null" | "NULL" )? ;
BOOLYES = ( "yes" | "Yes" | "YES" | "true" | "True" | "TRUE" | "on" | "On" | "ON" ) ;
BOOLNO = ( "no" | "No" | "NO" | "false" | "False" | "FALSE" | "off" | "Off" | "OFF" ) ;
TIMEZ = ( "Z" | [-+] DIGIT DIGIT ( ":" DIGIT DIGIT )? ) ;
TIMEYMD = YEAR "-" MON "-" MON ;
TIMEISO = YEAR "-" MON "-" MON [Tt] MON ":" MON ":" MON ( "." DIGIT* )? TIMEZ ;
TIMESPACED = YEAR "-" MON "-" MON [ \t]+ MON ":" MON ":" MON ( "." DIGIT* )? [ \t]+ TIMEZ ;
TIMECANON = YEAR "-" MON "-" MON "T" MON ":" MON ":" MON ( "." DIGIT* [1-9]+ )? "Z" ;
MERGE = "<<" ;
DEFAULTKEY = "=" ;

NULLTYPE NULL       {   return "null"; }
BOOLYES NULL        {   return "bool#yes"; }
BOOLNO NULL         {   return "bool#no"; }
INTHEX NULL         {   return "int#hex"; }
INTOCT NULL         {   return "int#oct"; }
INTSIXTY NULL       {   return "int#base60"; }
INTCANON NULL       {   return "int"; }
FLOATFIX NULL       {   return "float#fix"; }
FLOATEXP NULL       {   return "float#exp"; }
FLOATSIXTY NULL     {   return "float#base60"; }
FLOATINF NULL       {   return "float#inf"; }
FLOATNEGINF NULL    {   return "float#neginf"; }
FLOATNAN NULL       {   return "float#nan"; }
TIMEYMD NULL        {   return "timestamp#ymd"; }
TIMEISO NULL        {   return "timestamp#iso8601"; }
TIMESPACED NULL     {   return "timestamp#spaced"; }
TIMECANON NULL      {   return "timestamp"; }
DEFAULTKEY NULL     {   return "default"; }
MERGE NULL          {   return "merge"; }
ANY                 {   return "str"; }

*/

}

#if YYCSZ == 8
/* Remove ending fragment and compare types */
int
syck_tagcmp( const char *tag1, const char *tag2 )
{
    if ( tag1 == tag2 ) return 1;
    if ( tag1 == NULL || tag2 == NULL ) return 0;
    else {
        int i;
        char *othorpe;
        char *tmp1 = syck_strndup( tag1, strlen( tag1 ) );
        char *tmp2 = syck_strndup( tag2, strlen( tag2 ) );
        othorpe = strstr( tmp1, "#" );
		if ( othorpe != NULL ) {
            othorpe[0] = '\0';
        }
        othorpe = strstr( tmp2, "#" );
		if ( othorpe != NULL ) {
            othorpe[0] = '\0';
        }
        i = strcmp( tmp1, tmp2 );
        S_FREE( tmp1 ); S_FREE( tmp2 );
        return i;
    }
}

char *
syck_type_id_to_uri( char *type_id )
{
    char *cursor, *limit, *marker = NULL;

    cursor = type_id;
    limit = type_id + strlen( type_id );

/*!re2c

TAG = "tag" ;
XPRIVATE = "x-private" ;
WD = [A-Za-z0-9_] ;
WDD = [A-Za-z0-9_-] ;
DNSCOMPRE = WD ( WDD* WD )? ;
DNSNAMERE = ( ( DNSCOMPRE "." )+ DNSCOMPRE | DNSCOMPRE ) ;
TAGDATE = YEAR ( "-" MON )? ( "-" MON )? ;

TAG ":" DNSNAMERE "," TAGDATE ":"    {   return syck_strndup( type_id, strlen( type_id ) ); }

XPRIVATE ":"    {   return syck_strndup( type_id, strlen( type_id ) ); }

"!"             {   return syck_xprivate( type_id + 1, strlen( type_id ) - 1 ); }

DNSNAMERE "/"   {   char *domain = S_ALLOC_N( char, ( YYCURSOR - type_id ) + 15 );
                    char *uri;

                    domain[0] = '\0';
                    strncat( domain, type_id, ( YYCURSOR - type_id ) - 1 );
                    strcat( domain, "." );
                    strcat( domain, YAML_DOMAIN );
                    uri = syck_taguri( domain, YYCURSOR, YYLIMIT - YYCURSOR );

                    S_FREE( domain );
                    return uri;
                }

DNSNAMERE "," TAGDATE "/"  {   char *domain = S_ALLOC_N( char, YYCURSOR - type_id );
                               char *uri;

                               domain[0] = '\0';
                               strncat( domain, type_id, ( YYCURSOR - type_id ) - 1 );
                               uri = syck_taguri( domain, YYCURSOR, YYLIMIT - YYCURSOR );

                               S_FREE( domain );
                               return uri;
                            }

ANY             {   return syck_taguri( YAML_DOMAIN, type_id, strlen( type_id ) ); }

*/

}
#endif
