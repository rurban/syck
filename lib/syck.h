/*
 * syck.h
 *
 * $Author: why $
 * $Date: 2005/11/13 23:43:56 $
 *
 * Copyright (C) 2003 why the lucky stiff
 */

#ifndef SYCK_H
#define SYCK_H

#define SYCK_YAML_MAJOR 1
#define SYCK_YAML_MINOR 0

#define SYCK_VERSION    "0.70"
#define YAML_DOMAIN     "yaml.org,2002"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#ifdef HAVE_RUBY_ST_H
#include <ruby.h>
#else
#include "syck_st.h"
#endif

#if defined(__cplusplus)
extern "C" {
#endif

/*
 * Memory Allocation (using ruby's xmalloc with ext/ruby)
 */

#if DEBUG
__attribute__noreturn__
void syck_assert( const char *file_name, unsigned line_num )
	/*@globals fileSystem @*/
	/*@modifies fileSystem @*/;
# define ASSERT(f) if ( !f ) syck_assert( __FILE__, __LINE__ )
#else
# define ASSERT(f)
#endif

#ifndef NULL
# define NULL (void *)0
#endif

#if !defined(xmalloc)
#define	xmalloc(_n)	malloc(_n)
#define	xrealloc(_p,_n) realloc((_p), (_n))
#define	xfree(_n)       free((_n))
#endif

#define ALLOC_CT 8
#define SYCK_BUFFERSIZE 4096
#define S_ALLOC_N(type,n) (type*)xmalloc(sizeof(type)*(n))
#define S_ALLOC(type) (type*)xmalloc(sizeof(type))
#define S_REALLOC_N(var,type,n) (var)=(type*)xrealloc((char*)(var),sizeof(type)*(n))
#ifndef CU_TEST_H    
#ifdef HAVE_RUBY_ST_H
#define S_FREE(n) xfree((void *)n); n = 0UL;
#else
#define S_FREE(n) xfree((void *)n); n = NULL;
#endif
#else
#define S_FREE(n) xfree((void *)n);
#endif

#define S_ALLOCA_N(type,n) (type*)alloca(sizeof(type)*(n))

#define S_MEMZERO(p,type,n) memset((p), 0, sizeof(type)*(n))
#define S_MEMCPY(p1,p2,type,n) memcpy((p1), (p2), sizeof(type)*(n))
#define S_MEMMOVE(p1,p2,type,n) memmove((p1), (p2), sizeof(type)*(n))
#define S_MEMCMP(p1,p2,type,n) memcmp((p1), (p2), sizeof(type)*(n))

/*
 * Compiler attributes
 */
#if defined(HAVE_FUNC_ATTRIBUTE_NONNULL) && !defined(__cplusplus)
/* g++ has some problem with this attribute */
#  define __attribute__nonnull__(a)         __attribute__((__nonnull__(a)))
#  define HAVE_NONNULL
#else
#  define __attribute__nonnull__(a)
#  undef HAVE_NONNULL
#endif
#ifdef HAVE_FUNC_ATTRIBUTE_NORETURN
#  ifdef _MSC_VER
#    define __attribute__noreturn__         __declspec(noreturn)
#  else
#    define __attribute__noreturn__         __attribute__((__noreturn__))
#  endif
#else
#  define __attribute__noreturn__
#endif
#ifdef HAVE_FUNC_ATTRIBUTE_PURE
#  define __attribute__pure__               __attribute__((__pure__))
#else
#  define __attribute__pure__
#endif
#ifdef HAVE_FUNC_ATTRIBUTE_CONST
#  define __attribute__const__              __attribute__((__const__))
#else
#  define __attribute__const__
#endif
#ifdef HAVE_FUNC_ATTRIBUTE_MALLOC
#  define __attribute__malloc__              __attribute__((__malloc__))
#else
#  define __attribute__malloc__
#endif
#ifdef HAVE_FUNC_ATTRIBUTE_UNUSED
#  define __attribute__unused__             __attribute__((__unused__))
#  define UNUSED(a)
#else
#  define __attribute__unused__
#  ifdef __clang__
#    define UNUSED(a) (void)(a)
#  else
#    define UNUSED(a) /*@-noeffect*/if (0) (void)(a)/*@=noeffect*/
#  endif
#endif
#ifdef HAVE_FUNC_ATTRIBUTE_WARN_UNUSED_RESULT
#  define __attribute__warn_unused_result__ __attribute__((__warn_unused_result__))
#elif defined(_MSC_VER) && _MSC_VER > 1600 && defined(HAVE_SAL_H)
#  define __attribute__warn_unused_result__ _Check_return_
#else
#  define __attribute__warn_unused_result__
#endif
#ifdef HAVE_FUNC_ATTRIBUTE_RETURNS_NONNULL
#  define __attribute__returns_nonnull__    __attribute__((returns_nonnull))
#else
#  define __attribute__returns_nonnull__
#endif
#define SHIM(a) /*@unused@*/ a __attribute__unused__
    
/*
 * Node definitions
 */

#define BLOCK_FOLD  10
#define BLOCK_LIT   20
#define BLOCK_PLAIN 30
#define NL_CHOMP    40
#define NL_KEEP     50

#ifndef ST_DATA_T_DEFINED
typedef void * st_data_t;
#endif

#define SYMID unsigned long

typedef struct _syck_node SyckNode;

enum syck_kind_tag {
    syck_map_kind,
    syck_seq_kind,
    syck_str_kind
};

enum map_part {
    map_key,
    map_value
};

enum map_style {
    map_none,
    map_inline
};

enum seq_style {
    seq_none,
    seq_inline
};

enum scalar_style {
    scalar_none,
    scalar_1quote,
    scalar_2quote,
    scalar_fold,
    scalar_literal,
    scalar_plain
};

/*
 * Node metadata struct
 */
struct _syck_node {
    /* Symbol table ID */
    SYMID id;
    /* Underlying kind */
    enum syck_kind_tag kind;
    /* Fully qualified tag-uri for type */
/*@null@*/
    char *type_id;
    /* Anchor name */
/*@null@*/
    char *anchor;
    union {
        /* Storage for map data */
/*@relnull@*/
        struct SyckMap {
            enum map_style style;
/*@relnull@*/
            SYMID *keys;
/*@relnull@*/
            SYMID *values;
            long capa;
            long idx;
        } *pairs;
        /* Storage for sequence data */
/*@relnull@*/
        struct SyckSeq {
            enum seq_style style;
/*@relnull@*/
            SYMID *items;
            long capa;
            long idx;
        } *list;
        /* Storage for string data */
/*@relnull@*/
        struct SyckStr {
            enum scalar_style style;
/*@relnull@*/
            char *ptr;
            long len;
        } *str;
    } data;
    /* Shortcut node */
/*@null@*/
    void *shortcut;
};

/*
 * Parser definitions
 */
typedef struct _syck_parser SyckParser;
typedef struct _syck_file SyckIoFile;
typedef struct _syck_str SyckIoStr;
typedef struct _syck_level SyckLevel;

typedef SYMID (*SyckNodeHandler)(SyckParser *p, SyckNode *n);
typedef void (*SyckErrorHandler)(SyckParser *p, const char *);
typedef SyckNode * (*SyckBadAnchorHandler)(SyckParser *p, const char *);
typedef long (*SyckIoFileRead)(char *, SyckIoFile *, long, long); 
typedef long (*SyckIoStrRead)(char *, SyckIoStr *, long, long);

enum syck_io_type {
    syck_io_str,
    syck_io_file
};

/*@-enummemuse@*/
enum syck_parser_input {
    syck_yaml_utf8,
    syck_yaml_utf16,
    syck_yaml_utf32,
    syck_bytecode_utf8
};

enum syck_level_status {
    syck_lvl_header,
    syck_lvl_doc,
    syck_lvl_open,
    syck_lvl_seq,
    syck_lvl_map,
    syck_lvl_block,
    syck_lvl_str,
    syck_lvl_iseq,
    syck_lvl_imap,
    syck_lvl_end,
    syck_lvl_pause,
    syck_lvl_anctag,
    syck_lvl_mapx,
    syck_lvl_seqx
};
/*@=enummemuse@*/

/*
 * Parser structs
 */
struct _syck_file {
    /* File pointer */
    FILE *ptr;
    /* Function which FILE -> buffer */
    SyckIoFileRead read;
};

struct _syck_str {
    /* String buffer pointers */
    char *beg, *ptr, *end;
    /* Function which string -> buffer */
    SyckIoStrRead read;
};

struct _syck_level {
    /* Indent */
    int spaces;
    /* Counts nodes emitted at this level, useful for parsing 
     * keys and pairs in bytecode */
    int ncount;
    /* Does node have anchors or tags? */
    int anctag;
    /* Domain prefixing at the given level */
/*@relnull@*/
    char *domain;
    /* Keeps a node status */
    enum syck_level_status status;
};

struct _syck_parser {
    /* Root node */
    SYMID root, root_on_error;
    /* Implicit typing flag */
    int implicit_typing, taguri_expansion;
    /* Scripting language function to handle nodes */
    SyckNodeHandler handler;
    /* Error handler */
    SyckErrorHandler error_handler;
    /* InvalidAnchor handler */
    SyckBadAnchorHandler bad_anchor_handler;
    /* Parser input type */
    enum syck_parser_input input_type;
    /* IO type */
    enum syck_io_type io_type;
    /* Custom buffer size */
    size_t bufsize;
    /* Buffer pointers */
/*@relnull@*/
    char *buffer;
/*@relnull@*/
    char *linectptr;
/*@relnull@*/
    char *lineptr;
/*@relnull@*/
    char *toktmp;
/*@relnull@*/
    char *token;
/*@relnull@*/
    char *cursor;
/*@relnull@*/
    char *marker;
/*@relnull@*/
    char *limit;
    /* Line counter */
    int linect;
    /* Last token from yylex() */
    int last_token;
    /* Force a token upon next call to yylex() */
    int force_token;
    /* EOF flag */
    int eof;
    union {
/*@relnull@*/
        SyckIoFile *file;
/*@relnull@*/
        SyckIoStr *str;
    } io;
    /* Symbol table for anchors */
/*@relnull@*/
    st_table *anchors;
/*@relnull@*/
    st_table *bad_anchors;
    /* Optional symbol table for SYMIDs */
/*@relnull@*/
    st_table *syms;
    /* Levels of indentation */
/*@relnull@*/
    SyckLevel *levels;
    int lvl_idx;
    int lvl_capa;
    /* Pointer for extension's use */
/*@null@*/
    void *bonus;
};

/*
 * Emitter definitions
 */
typedef struct _syck_emitter SyckEmitter;
/*@-typeuse@*/
typedef struct _syck_emitter_node SyckEmitterNode;
/*@=typeuse@*/

typedef void (*SyckOutputHandler)(SyckEmitter *e, char *, long); 
typedef void (*SyckEmitterHandler)(SyckEmitter *e, st_data_t); 

enum doc_stage {
    doc_open,
    doc_processing
};

/*
 * Emitter struct
 */
struct _syck_emitter {
    /* Headerless doc flag */
    int headless;
    /* Force header? */
    int use_header;
    /* Force version? */
    int use_version;
    /* Sort hash keys */
    int sort_keys;
    /* Anchor format */
/*@null@*/
    char *anchor_format;
    /* Explicit typing on all collections? */
    int explicit_typing;
    /* Best width on folded scalars */
    int best_width;
    /* Use literal[1] or folded[2] blocks on all text? */
    enum scalar_style style;
    /* Stage of written document */
    enum doc_stage stage;
    /* Level counter */
    int level;
    /* Default indentation */
    int indent;
    /* Object ignore ID */
    SYMID ignore_id;
    /* Symbol table for anchors */
/*@relnull@*/
    st_table *markers;
/*@relnull@*/
    st_table *anchors;
/*@relnull@*/
    st_table *anchored;
    /* Custom buffer size */
    size_t bufsize;
    /* Buffer */
/*@null@*/
    char *buffer;
/*@relnull@*/
    char *marker;
    /* Absolute position of the buffer */
    long bufpos;
    /* Handler for emitter nodes */
/*@null@*/
    SyckEmitterHandler emitter_handler;
    /* Handler for output */
/*@null@*/
    SyckOutputHandler output_handler;
    /* Levels of indentation */
/*@relnull@*/
    SyckLevel *levels;
    int lvl_idx;
    int lvl_capa;
    /* Pointer for extension's use */
/*@null@*/
    void *bonus;
};

/*
 * Emitter node metadata struct
 */
struct _syck_emitter_node {
    /* Node buffer position */
    long pos;
    /* Current indent */
    long indent;
    /* Collection? */
    int is_shortcut;
};

/*
 * Handler prototypes
 */
__attribute__warn_unused_result__
SYMID syck_hdlr_add_node( SyckParser *p, SyckNode *n )
	/*@modifies n @*/;
__attribute__warn_unused_result__
SyckNode *syck_hdlr_add_anchor( SyckParser *p, char *a, SyckNode *n )
	/*@modifies p, n @*/;
void syck_hdlr_remove_anchor( SyckParser *p, char *a )
	/*@modifies p, a @*/;
__attribute__warn_unused_result__
__attribute__returns_nonnull__
SyckNode *syck_hdlr_get_anchor( SyckParser *p, char *a )
	/*@modifies p, a @*/;
void syck_add_transfer( /*@only@*/ char *uri, SyckNode *n, int taguri )
	/*@modifies n, uri @*/;
/*@null@*/
__attribute__warn_unused_result__
char *syck_xprivate( const char *type_id, int type_len )
	/*@*/;
/*@null@*/
__attribute__malloc__
__attribute__warn_unused_result__
char *syck_taguri( const char *domain, const char *type_id, int type_len )
	/*@*/;
__attribute__warn_unused_result__
int syck_tagcmp( /*@null@*/ const char *tag1, /*@null@*/ const char *tag2 )
	/*@*/;
__attribute__warn_unused_result__
int syck_add_sym( SyckParser *p, void *data )
	/*@modifies p @*/;
int syck_lookup_sym( SyckParser *p, SYMID, char **out )
	/*@modifies *data @*/;
__attribute__warn_unused_result__
int syck_try_implicit( SyckNode *n )
	/*@*/;
/*@null@*/
__attribute__warn_unused_result__
char *syck_type_id_to_uri( char *type_id )
	/*@*/;
void try_tag_implicit( SyckNode *n, int taguri )
	/*@modifies n @*/;
/*@observer@*/
__attribute__warn_unused_result__
__attribute__returns_nonnull__
const char *syck_match_implicit( const char *str, size_t len )
	/*@*/;

/*
 * API prototypes
 */
/*@null@*/
__attribute__malloc__
__attribute__warn_unused_result__
char *syck_strndup( const char *buf, long len )
	/*@*/;
long syck_io_file_read( char *buf, SyckIoFile *file, long max_size, long skip )
	/*@globals fileSystem @*/
	/*@modifies buf, file, fileSystem @*/;
__attribute__warn_unused_result__
long syck_io_str_read( char *buf, SyckIoStr *str, long max_size, long skip )
	/*@modifies buf, str @*/;
/*@null@*/
__attribute__warn_unused_result__
char *syck_base64enc( char *, long )
	/*@*/;
/*@null@*/
__attribute__warn_unused_result__
char *syck_base64dec( char *, long )
	/*@*/;
/*@null@*/
__attribute__malloc__
__attribute__warn_unused_result__
__attribute__returns_nonnull__
SyckEmitter *syck_new_emitter(void)
	/*@*/;
__attribute__warn_unused_result__
SYMID syck_emitter_mark_node( SyckEmitter *e, st_data_t )
	/*@modifies e @*/;
void syck_emitter_ignore_id( SyckEmitter *e, SYMID )
	/*@*/;
void syck_output_handler( SyckEmitter *e, SyckOutputHandler )
	/*@modifies e @*/;
void syck_emitter_handler( SyckEmitter *e, SyckEmitterHandler )
	/*@modifies e @*/;
void syck_free_emitter( SyckEmitter *e )
	/*@modifies e @*/;
void syck_emitter_clear( SyckEmitter *e )
	/*@modifies e @*/;
void syck_emitter_write( SyckEmitter *e, char *, long )
	/*@modifies e @*/;
void syck_emitter_escape( SyckEmitter *e, char *, long )
	/*@modifies e @*/;
void syck_emitter_flush( SyckEmitter *e, long )
	/*@modifies e @*/;
void syck_emit( SyckEmitter *e, st_data_t )
	/*@modifies e @*/;
void syck_emit_scalar( SyckEmitter *e, char *, enum scalar_style force_style, int, int, char, char *, long )
	/*@modifies e @*/;
void syck_emit_1quoted( SyckEmitter *e, int, char *, long )
	/*@modifies e @*/;
void syck_emit_2quoted( SyckEmitter *e, int, char *, long )
	/*@modifies e @*/;
void syck_emit_folded( SyckEmitter *e, int, char, char *, long )
	/*@modifies e @*/;
void syck_emit_literal( SyckEmitter *e, char, char *, long )
	/*@modifies e @*/;
void syck_emit_seq( SyckEmitter *e, char *, enum seq_style style )
	/*@modifies e @*/;
void syck_emit_item( SyckEmitter *e, st_data_t )
	/*@modifies e @*/;
void syck_emit_map( SyckEmitter *e, char *, enum map_style style )
	/*@modifies e @*/;
void syck_emit_end( SyckEmitter *e )
	/*@modifies e @*/;
void syck_emit_tag( SyckEmitter *e, /*@null@*/ char *tag, /*@null@*/ char *ignore )
	/*@modifies e @*/;
void syck_emit_indent( SyckEmitter *e )
	/*@modifies e @*/;
__attribute__warn_unused_result__
__attribute__returns_nonnull__
SyckLevel *syck_emitter_current_level( SyckEmitter *e )
	/*@*/;
__attribute__warn_unused_result__
__attribute__returns_nonnull__
SyckLevel *syck_emitter_parent_level( SyckEmitter *e )
	/*@*/;
void syck_emitter_pop_level( SyckEmitter *e )
	/*@modifies e @*/;
void syck_emitter_add_level( SyckEmitter *e, int, enum syck_level_status )
	/*@modifies e @*/;
void syck_emitter_reset_levels( SyckEmitter *e )
	/*@modifies e @*/;
/*@null@*/
__attribute__malloc__
__attribute__warn_unused_result__
__attribute__returns_nonnull__
SyckParser *syck_new_parser(void)
	/*@*/;
void syck_free_parser( SyckParser *p )
	/*@modifies p @*/;
void syck_parser_set_root_on_error( SyckParser *p, SYMID )
	/*@modifies p @*/;
void syck_parser_implicit_typing( SyckParser *p, int )
	/*@modifies p @*/;
void syck_parser_taguri_expansion( SyckParser *p, int )
	/*@modifies p @*/;
__attribute__warn_unused_result__
int syck_scan_scalar( int, char *, long )
	/*@*/;
void syck_parser_handler( SyckParser *p, SyckNodeHandler )
	/*@modifies p @*/;
void syck_parser_error_handler( SyckParser *p, /*@null@*/ SyckErrorHandler )
	/*@modifies p @*/;
void syck_parser_bad_anchor_handler( SyckParser *p, SyckBadAnchorHandler )
	/*@modifies p @*/;
void syck_parser_set_input_type( SyckParser *p, enum syck_parser_input input_type )
        /*@modifies p @*/;
void syck_parser_file( SyckParser *p, FILE *, SyckIoFileRead )
	/*@modifies p @*/;
void syck_parser_str( SyckParser *p, char *, long, /*@null@*/ SyckIoStrRead )
	/*@modifies p @*/;
void syck_parser_str_auto( SyckParser *p, char *, /*@null@*/ SyckIoStrRead )
	/*@modifies p @*/;
__attribute__returns_nonnull__
SyckLevel *syck_parser_current_level( SyckParser *p )
	/*@*/;
void syck_parser_add_level( SyckParser *p, int, enum syck_level_status )
	/*@modifies p @*/;
void syck_parser_pop_level( SyckParser *p )
	/*@modifies p @*/;
void free_any_io( SyckParser *p )
	/*@modifies p @*/;
long syck_parser_read( SyckParser *p )
	/*@modifies p @*/;
long syck_parser_readlen( SyckParser *p, long )
	/*@modifies p @*/;
SYMID syck_parse( SyckParser *p )
	/*@globals fileSystem @*/
	/*@modifies p, fileSystem @*/;
void syck_default_error_handler( SyckParser *p, const char * )
	/*@globals fileSystem @*/
	/*@modifies p, fileSystem @*/;
SYMID syck_yaml2byte_handler( SyckParser *p, SyckNode *n )
	/*@modifies p, n @*/;
/*@null@*/
char *syck_yaml2byte( char *yamlstr )
	/*@globals fileSystem @*/
	/*@modifies fileSystem @*/;

/*
 * Allocation prototypes
 */
__attribute__malloc__
/*@null@*/
SyckNode *syck_alloc_map(void)
	/*@*/;
__attribute__malloc__
/*@null@*/
SyckNode *syck_alloc_seq(void)
	/*@*/;
__attribute__malloc__
/*@null@*/
SyckNode *syck_alloc_str(void)
	/*@*/;
void syck_free_node( SyckNode *n )
	/*@modifies n @*/;
void syck_free_members( SyckNode *n )
	/*@modifies n @*/;
/*@null@*/
__attribute__malloc__
__attribute__warn_unused_result__
SyckNode *syck_new_str( char *str, enum scalar_style style )
	/*@*/;
/*@null@*/
__attribute__malloc__
__attribute__warn_unused_result__
SyckNode *syck_new_str2( char *str, long len, enum scalar_style style )
	/*@*/;
void syck_replace_str( SyckNode *n, char *str, enum scalar_style style )
	/*@modifies n @*/;
void syck_replace_str2( SyckNode *n, char *str, long len, enum scalar_style style )
	/*@modifies n @*/;
void syck_str_blow_away_commas( SyckNode *n )
	/*@modifies n @*/;
/*@null@*/
char *syck_str_read( SyckNode *n )
	/*@*/;
/*@null@*/
__attribute__malloc__
__attribute__warn_unused_result__
SyckNode *syck_new_map( SYMID key, SYMID value )
	/*@*/;
void syck_map_empty( SyckNode *n )
	/*@modifies n @*/;
void syck_map_add( SyckNode *map, SYMID key, SYMID value )
	/*@modifies map @*/;
SYMID syck_map_read( SyckNode *map, enum map_part p, long idx )
	/*@*/;
void syck_map_assign( SyckNode *map, enum map_part p, long idx, SYMID id )
	/*@modifies map @*/;
long syck_map_count( SyckNode *map )
	/*@*/;
void syck_map_update( SyckNode *map1, SyckNode *map2 )
	/*@modifies map1, map2 @*/;
/*@null@*/
SyckNode *syck_new_seq( SYMID value )
	/*@*/;
void syck_seq_empty( SyckNode *n )
	/*@modifies n @*/;
void syck_seq_add( SyckNode *arr, SYMID value )
	/*@modifies arr @*/;
void syck_seq_assign( SyckNode *seq, long idx, SYMID id )
	/*@modifies seq @*/;
SYMID syck_seq_read( SyckNode *seq, long idx )
	/*@*/;
long syck_seq_count( SyckNode *seq )
	/*@*/;

/*
 * Lexer prototypes
 */
void syckerror( const char *msg )
	/*@*/;
int syckparse( void * )
	/*@globals fileSystem @*/
	/*@modifies fileSystem @*/;
/* XXX union YYSTYPE *sycklval has issues on Mac OS X. */
/* Value type from gram.h.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
    SYMID nodeId;
    SyckNode *nodeData;
    char *name;
};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif    
int sycklex( YYSTYPE *_sycklval, SyckParser *parser )
	/*@modifies _sycklval, parser @*/;

#if defined(__cplusplus)
}  /* extern "C" { */
#endif

#endif /* ifndef SYCK_H */
