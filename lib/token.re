/*
 * token.re
 *
 * $Author$
 * $Date$
 *
 * Copyright (C) 2003 why the lucky stiff
 */
#include "syck.h"
#include "gram.h"

/*
 * Allocate quoted strings in chunks
 */
#define QUOTELEN    1024

/*
 * They do my bidding...
 */
#define YYCTYPE     char
#define YYCURSOR    parser->cursor
#define YYMARKER    parser->marker
#define YYLIMIT     parser->limit
#define YYTOKEN     parser->token
#define YYTOKTMP    parser->toktmp
#define YYLINEPTR   parser->lineptr
#define YYLINE      parser->linect
#define YYFILL(n)   syck_parser_read(parser)

/*
 * Repositions the cursor at `n' offset from the token start.
 * Only works in `Header' and `Document' sections.
 */
#define YYPOS(n)    YYCURSOR = YYTOKEN + n

/*
 * Track line numbers
 */
#define NEWLINE(ptr)    YYLINE++; YYLINEPTR = ptr + 1

/*
 * I like seeing the level operations as macros...
 */
#define ADD_LEVEL(len, status)  syck_parser_add_level( parser, len, status )
#define POP_LEVEL()     syck_parser_pop_level( parser )
#define CURRENT_LEVEL() syck_parser_current_level( parser )

/*
 * Force a token next time around yylex()
 */
#define FORCE_NEXT_TOKEN(tok)    parser->force_token = tok;

/*
 * Nice little macro to ensure we're IOPENed to the current level.
 * * Only use this macro in the "Document" section *
 */
#define ENSURE_IOPEN(last_lvl, to_len, reset) \
        if ( last_lvl->spaces < to_len ) \
        { \
            if ( last_lvl->status == syck_lvl_inline ) \
            { \
                goto Document; \
            } \
            else \
            { \
                ADD_LEVEL( to_len, syck_lvl_doc ); \
                if ( reset == 1 ) YYPOS(0); \
                return IOPEN; \
            } \
        } 

/*
 * Nice little macro to ensure closure of levels.
 * * Only use this macro in the "Document" section *
 */
#define ENSURE_IEND(last_lvl, to_len) \
        if ( last_lvl->spaces > to_len ) \
        { \
            syck_parser_pop_level( parser ); \
            YYPOS(0); \
            return IEND; \
        }

/*
 * Concatenates quoted string items and manages allocation
 * to the quoted string
 */
#define QUOTECAT(s, c, i, l) \
        { \
            if ( i + 1 >= c ) \
            { \
                c += QUOTELEN; \
                S_REALLOC_N( s, char, c ); \
            } \
            s[i++] = l; \
            s[i] = '\0'; \
        }

/*
 * Tags a plain scalar with a transfer method
 * * Use only in "Plain" section *
 */
#define RETURN_IMPLICIT(fold) \
    { \
        SyckLevel *i_lvl = CURRENT_LEVEL(); \
        YYCURSOR = YYTOKTMP; \
        yylval->nodeData = syck_new_str2( YYTOKEN, YYCURSOR - YYTOKEN ); \
        if ( fold ) { \
            syck_fold_format( yylval->nodeData->data.str, BLOCK_PLAIN, i_lvl->spaces, NL_CHOMP ); \
        } \
        if ( parser->implicit_typing == 1 ) \
        { \
            try_tag_implicit( yylval->nodeData, parser->taguri_expansion ); \
        } \
        return PLAIN; \
    }

/*
 * Handles newlines, calculates indent
 */
#define GOBBLE_UP_INDENT( ict, start ) \
    char *indent = start; \
    NEWLINE(indent); \
    while ( indent < YYCURSOR ) \
    { \
        if ( *(++indent) == '\n' ) \
        { \
            NEWLINE(indent); \
        } \
    } \
    ict = 0; \
    if ( *YYCURSOR == '\0' ) \
    { \
        ict = -1; \
        start = YYCURSOR - 1; \
    } \
    else if ( *YYLINEPTR == ' ' ) \
    { \
        ict = YYCURSOR - YYLINEPTR; \
    }

/*
 * If an indent exists at the current level, back up.
 */
#define GET_TRUE_INDENT(indt_len) \
    { \
        SyckLevel *lvl_deep = CURRENT_LEVEL(); \
        indt_len = lvl_deep->spaces; \
        if ( indt_len == YYTOKEN - YYLINEPTR ) \
        { \
            SyckLevel *lvl_over; \
            parser->lvl_idx--; \
            lvl_over = CURRENT_LEVEL(); \
            indt_len = lvl_over->spaces; \
            parser->lvl_idx++; \
        } \
    }

/*
 * Argjh!  I hate globals!  Here for yyerror() only!
 */
SyckParser *syck_parser_ptr = NULL;

/*
 * Accessory funcs later in this file.
 */
void eat_comments( SyckParser * );

/*
 * My own re-entrant yylex() using re2c.
 * You really get used to the limited regexp.
 * It's really nice to not rely on backtracking and such.
 */
int
yylex( YYSTYPE *yylval, SyckParser *parser )
{
    syck_parser_ptr = parser;
    if ( YYCURSOR == NULL ) 
    {
        syck_parser_read( parser );
    }

    if ( parser->force_token != 0 )
    {
        int t = parser->force_token;
        parser->force_token = 0;
        return t;
    }

/*!re2c

WORDC = [A-Za-z0-9_-\.] ;
LF = [\n]+ ;
ENDSPC = ( [ ]+ | LF );
INDENT = LF [ \n]* ;
NULL = [\000] ;
ANY = [\001-\377] ;
ODELIMS = [\{\[] ;
CDELIMS = [\}\]] ;
INLINEX = ( CDELIMS | "," ENDSPC ) ;
ALLX = ( ":" ENDSPC ) ;
DIR = "%" WORDC+ ":" WORDC+ ;
BLOCK = [>|] [-+0-9]* ENDSPC ; 

*/

    if ( YYLINEPTR != YYCURSOR )
    {
        goto Document;
    }

Header:

    YYTOKEN = YYCURSOR;

/*!re2c

"---" ENDSPC        {   SyckLevel *lvl = CURRENT_LEVEL();
                        if ( lvl->status == syck_lvl_header )
                        {
                            YYPOS(3);
                            goto Directive; 
                        }
                        else
                        {
                            ENSURE_IEND(lvl, -1);
                            YYPOS(0);
                            return 0; 
                        }
                    }

"#"                 {   eat_comments( parser ); 
                        goto Header;
                    }

NULL                {   SyckLevel *lvl = CURRENT_LEVEL();
                        ENSURE_IEND(lvl, -1);
                        return 0; 
                    }

INDENT              {   int indt_len;
                        GOBBLE_UP_INDENT( indt_len, YYTOKEN );
                        goto Header; 
                    }

ANY                 {   YYPOS(0);
                        goto Document; 
                    }

*/

Document:
    {
        SyckLevel *lvl = CURRENT_LEVEL();
        if ( lvl->status == syck_lvl_header )
        {
            lvl->status = syck_lvl_doc;
        }

        YYTOKEN = YYCURSOR;

/*!re2c

INDENT              {   // Isolate spaces
                        int indt_len;
                        GOBBLE_UP_INDENT( indt_len, YYTOKEN );
                        lvl = CURRENT_LEVEL();

                        // Check for open indent
                        ENSURE_IEND(lvl, indt_len);
                        ENSURE_IOPEN(lvl, indt_len, 0);
                        if ( indt_len == -1 )
                        {
                            return 0;
                        }
                        return INDENT;
                    }

ODELIMS             {   ENSURE_IOPEN(lvl, 0, 1);
                        lvl = CURRENT_LEVEL();
                        ADD_LEVEL(lvl->spaces + 1, syck_lvl_inline);
                        return YYTOKEN[0]; 
                    }

CDELIMS             {   POP_LEVEL();
                        return YYTOKEN[0]; 
                    }

[:,] ENDSPC         {   YYPOS(1); 
                        return YYTOKEN[0]; 
                    }

[-?] ENDSPC         {   ENSURE_IOPEN(lvl, YYTOKEN - YYLINEPTR, 1);
                        FORCE_NEXT_TOKEN(IOPEN);
                        if ( *( YYCURSOR - 1 ) == '\n' )
                        {
                            YYCURSOR--; 
                        }
                        ADD_LEVEL(YYCURSOR - YYLINEPTR, syck_lvl_doc);
                        return YYTOKEN[0]; 
                    }

"&" WORDC+          {   yylval->name = syck_strndup( YYTOKEN + 1, YYCURSOR - YYTOKEN - 1 );
                        return ANCHOR;
                    }

"*" WORDC+          {   yylval->name = syck_strndup( YYTOKEN + 1, YYCURSOR - YYTOKEN - 1 );
                        return ALIAS;
                    }

"!"                 {   goto TransferMethod; }

"'"                 {   goto SingleQuote; }

"\""                {   goto DoubleQuote; }

BLOCK               {   if ( *( YYCURSOR - 1 ) == '\n' ) 
                        {
                            YYCURSOR--;
                        }
                        goto ScalarBlock; 
                    }

"#"                 {   eat_comments( parser ); 
                        goto Document;
                    }

[ ]+                {   goto Document; }

NULL                {   ENSURE_IEND(lvl, -1);
                        return 0; 
                    }

ANY                 {   ENSURE_IOPEN(lvl, 0, 1);
                        goto Plain; 
                    }

*/
    }

Directive:
    {
        YYTOKTMP = YYCURSOR;

/*!re2c

DIR                 {   goto Directive; }

[ ]+                {   goto Directive; }

ANY                 {   YYCURSOR = YYTOKTMP;
                        return DOCSEP;
                    }
*/

    }

Plain:
    {
        SyckLevel *plvl;
        int parentIndent;
        int multiLine = 0;
        YYCURSOR = YYTOKEN;
        plvl = CURRENT_LEVEL();
        GET_TRUE_INDENT(parentIndent);

Plain2: 
        YYTOKTMP = YYCURSOR;

Plain3:

/*!re2c

INDENT              {   int indt_len;
                        SyckLevel *lvl;
                        char *tok = YYTOKTMP;
                        GOBBLE_UP_INDENT( indt_len, tok );
                        lvl = CURRENT_LEVEL();

                        if ( indt_len <= parentIndent )
                        {
                            RETURN_IMPLICIT(multiLine);
                        }

                        multiLine = 1;
                        goto Plain2; 
                    }

ALLX                {   RETURN_IMPLICIT(multiLine); }

INLINEX             {   if ( plvl->status != syck_lvl_inline ) goto Plain2;
                        RETURN_IMPLICIT(multiLine);
                    }

NULL                {   RETURN_IMPLICIT(multiLine); }

" #"                {   eat_comments( parser ); 
                        RETURN_IMPLICIT(multiLine);
                    }

[ ]                 {   goto Plain3; }

ANY                 {   goto Plain2; }

*/
    }

SingleQuote:
    {
        int qidx = 0;
        int qcapa = 100;
        char *qstr = S_ALLOC_N( char, qcapa );

SingleQuote2:
        YYTOKTMP = YYCURSOR;

/*!re2c

INDENT              {   int indt_len;
                        int nl_count = 0;
                        SyckLevel *lvl;
                        GOBBLE_UP_INDENT( indt_len, YYTOKTMP );
                        lvl = CURRENT_LEVEL();

                        if ( lvl->status != syck_lvl_str )
                        {
                            ADD_LEVEL( indt_len, syck_lvl_str );
                        }
                        else if ( indt_len < lvl->spaces )
                        {
                            // Error!
                        }

                        while ( YYTOKTMP < YYCURSOR )
                        {
                            if ( *YYTOKTMP++ == '\n' )
                                nl_count++;
                        }
                        if ( nl_count <= 1 )
                        {
                            QUOTECAT(qstr, qcapa, qidx, ' ');
                        }
                        else
                        {
                            int i;
                            for ( i = 0; i < nl_count - 1; i++ )
                            {
                                QUOTECAT(qstr, qcapa, qidx, '\n');
                            }
                        }

                        goto SingleQuote2; 
                    }

"''"                {   QUOTECAT(qstr, qcapa, qidx, '\'');
                        goto SingleQuote2; 
                    }

( "'" | NULL )      {   SyckLevel *lvl;
                        SyckNode *n = syck_alloc_str();
                        lvl = CURRENT_LEVEL();

                        if ( lvl->status == syck_lvl_str )
                        {
                            POP_LEVEL();
                        }
                        n->data.str->ptr = qstr;
                        n->data.str->len = qidx;
                        yylval->nodeData = n;
                        return PLAIN; 
                    }

ANY                 {   QUOTECAT(qstr, qcapa, qidx, *(YYCURSOR - 1)); 
                        goto SingleQuote2; 
                    }

*/

    }


DoubleQuote:
    {
        int keep_nl = 1;
        int qidx = 0;
        int qcapa = 100;
        char *qstr = S_ALLOC_N( char, qcapa );

DoubleQuote2:
        YYTOKTMP = YYCURSOR;


/*!re2c

INDENT              {   int indt_len;
                        int nl_count = 0;
                        SyckLevel *lvl;
                        GOBBLE_UP_INDENT( indt_len, YYTOKTMP );
                        lvl = CURRENT_LEVEL();

                        if ( lvl->status != syck_lvl_str )
                        {
                            ADD_LEVEL( indt_len, syck_lvl_str );
                        }
                        else if ( indt_len < lvl->spaces )
                        {
                            // Error!
                        }

                        if ( keep_nl == 1 )
                        {
                            while ( YYTOKTMP < YYCURSOR )
                            {
                                if ( *YYTOKTMP++ == '\n' )
                                    nl_count++;
                            }
                            if ( nl_count <= 1 )
                            {
                                QUOTECAT(qstr, qcapa, qidx, ' ');
                            }
                            else
                            {
                                int i;
                                for ( i = 0; i < nl_count - 1; i++ )
                                {
                                    QUOTECAT(qstr, qcapa, qidx, '\n');
                                }
                            }
                        }

                        keep_nl = 1;
                        goto DoubleQuote2; 
                    }

"\\" ["\\abefnrtv]   {   char ch = *( YYCURSOR - 1 );
                        switch ( ch )
                        {
                            case 'a': ch = 7; break;
                            case 'b': ch = '\010'; break;
                            case 'e': ch = '\033'; break;
                            case 'f': ch = '\014'; break;
                            case 'n': ch = '\n'; break;
                            case 'r': ch = '\015'; break;
                            case 't': ch = '\t'; break;
                            case 'v': ch = '\013'; break;
                        }
                        QUOTECAT(qstr, qcapa, qidx, ch);
                        goto DoubleQuote2; 
                    }

"\\" [ ]* "\n"      {   keep_nl = 0;
                        YYCURSOR--;
                        goto DoubleQuote2; 
                    }

( "\"" | NULL )     {   SyckLevel *lvl;
                        SyckNode *n = syck_alloc_str();
                        lvl = CURRENT_LEVEL();

                        if ( lvl->status == syck_lvl_str )
                        {
                            POP_LEVEL();
                        }
                        n->data.str->ptr = qstr;
                        n->data.str->len = qidx;
                        yylval->nodeData = n;
                        return PLAIN; 
                    }

ANY                 {   QUOTECAT(qstr, qcapa, qidx, *(YYCURSOR - 1)); 
                        goto DoubleQuote2; 
                    }

*/
    }

TransferMethod:
    {
        YYTOKTMP = YYCURSOR;

/*!re2c

ENDSPC              {   SyckLevel *lvl;
                        YYCURSOR = YYTOKTMP;
                        if ( YYCURSOR == YYTOKEN + 1 )
                        {
                            return ITRANSFER;
                        }

                        lvl = CURRENT_LEVEL();

                        //
                        // URL Prefixing
                        //
                        if ( *(YYTOKEN + 1) == '^' )
                        {
                            yylval->name = S_ALLOC_N( char, ( YYCURSOR - YYTOKEN ) + strlen( lvl->domain ) );
                            yylval->name[0] = '\0';
                            strcat( yylval->name, lvl->domain );
                            strncat( yylval->name, YYTOKEN + 2, ( YYCURSOR - YYTOKEN ) - 2 );
                        }
                        else
                        {
                            char *carat = YYTOKEN;
                            while ( (++carat) < YYCURSOR )
                            {
                                if ( *carat == '^' )
                                    break;
                            }

                            if ( carat < YYCURSOR )
                            {
                                free( lvl->domain );
                                lvl->domain = syck_strndup( YYTOKEN + 1, ( carat - YYTOKEN ) - 1 );
                                yylval->name = S_ALLOC_N( char, ( YYCURSOR - carat ) + strlen( lvl->domain ) );
                                yylval->name[0] = '\0';
                                strcat( yylval->name, lvl->domain );
                                strncat( yylval->name, carat + 1, ( YYCURSOR - carat ) - 1 );
                            }
                            else
                            {
                                yylval->name = syck_strndup( YYTOKEN + 1, ( YYCURSOR - YYTOKEN ) - 1 );
                            }
                        }

                        return TRANSFER; 
                    }

ANY                 {   goto TransferMethod; }

*/
    }

ScalarBlock:
    {
        int blockType = 0;
        int nlDoWhat = 0;
        int forceIndent = -1;
        char *yyt = YYTOKEN;
        SyckLevel *lvl = CURRENT_LEVEL();
        int parentIndent;
        GET_TRUE_INDENT(parentIndent);

        switch ( *yyt )
        {
            case '|': blockType = BLOCK_LIT; break;
            case '>': blockType = BLOCK_FOLD; break;
        }

        while ( ++yyt <= YYCURSOR )
        {
            if ( *yyt == '-' )
            {
                nlDoWhat = NL_CHOMP;
            }
            else if ( *yyt == '+' )
            {
                nlDoWhat = NL_KEEP;
            }
            else if ( isdigit( *yyt ) )
            {
                forceIndent = strtol( yyt, NULL, 10 ) + parentIndent;
            }
        }

        YYTOKEN = YYCURSOR;

ScalarBlock2:
        YYTOKTMP = YYCURSOR;

/*!re2c

INDENT              {   int indt_len;
                        GOBBLE_UP_INDENT( indt_len, YYTOKTMP );
                        lvl = CURRENT_LEVEL();

                        if ( indt_len > parentIndent && lvl->status != syck_lvl_block )
                        {
                            ADD_LEVEL( forceIndent > 0 ? forceIndent : indt_len, syck_lvl_block );
                        }

                        lvl = CURRENT_LEVEL();
                        if ( lvl->status != syck_lvl_block )
                        {
                            yylval->nodeData = syck_new_str2( YYTOKEN, 0 );  
                            YYCURSOR = YYTOKTMP;
                            return BLOCK;
                        }
                        else if ( indt_len < lvl->spaces )
                        {
                            YYCURSOR--;
                            yylval->nodeData = syck_new_str2( YYTOKEN, YYCURSOR - YYTOKEN );  
                            syck_fold_format( yylval->nodeData->data.str, blockType, lvl->spaces, nlDoWhat );
                            if ( lvl->status == syck_lvl_block )
                            {
                                POP_LEVEL();
                            }
                            YYCURSOR = YYTOKTMP;
                            return BLOCK;
                        }
                        goto ScalarBlock2;
                    }


NULL                {   lvl = CURRENT_LEVEL();
                        YYCURSOR--;
                        yylval->nodeData = syck_new_str2( YYTOKEN, YYCURSOR - YYTOKEN );  
                        syck_fold_format( yylval->nodeData->data.str, blockType, lvl->spaces, nlDoWhat );
                        POP_LEVEL();
                        return BLOCK; 
                    }

"#"                 {   lvl = CURRENT_LEVEL();
                        if ( lvl->status != syck_lvl_block )
                        {
                            eat_comments( parser );
                            YYTOKEN = YYCURSOR;
                        }
                        goto ScalarBlock2;
                    }

ANY                 {   goto ScalarBlock2; }

*/
    }

}

void
eat_comments( SyckParser *parser )
{
    char *tok;

Comment:
    {
        tok = YYCURSOR;

/*!re2c

( LF | NULL )       {   SyckLevel *lvl = CURRENT_LEVEL();
                        YYCURSOR = tok;
                        return;
                    }

ANY                 {   goto Comment; 
                    }

*/

    }

}

int 
yywrap()
{
    return 1;
}

void 
yyerror( char *msg )
{
    if ( syck_parser_ptr->error_handler == NULL )
        syck_parser_ptr->error_handler = syck_default_error_handler;

    syck_parser_ptr->root = syck_parser_ptr->root_on_error;
    (syck_parser_ptr->error_handler)(syck_parser_ptr, msg);
}

