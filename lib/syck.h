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

#ifdef DEBUG
# include <stdio.h>
# define DPRINTF(Args)                          \
    do {                                        \
        if (syckdebug)                          \
            fprintf Args;                       \
} while (0)
#else /* !DEBUG */
# define DPRINTF(Args) ((void) 0)
#endif

/*
 * Compiler attributes
 */
#ifndef __attribute__nonnull__
# if defined(HAVE_FUNC_ATTRIBUTE_NONNULL) && !defined(__cplusplus)
/* g++ has some problem with this attribute */
#  define __attribute__nonnull__(a)         __attribute__((__nonnull__(a)))
#  define HAVE_NONNULL
# else
#  define __attribute__nonnull__(a)
#  undef HAVE_NONNULL
# endif
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
#    define UNUSED(a) if (0) (void)(a)
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
#ifdef HAVE_FUNC_ATTRIBUTE_FORMAT
#  define __attribute__format__(x,y)              __attribute__((format(printf, x, y)))
#else
#  define __attribute__format__(x,y)
#endif
#define SHIM(a) a __attribute__unused__

#ifdef DEBUG
__attribute__noreturn__
void syck_assert( const char *file_name, unsigned line_num );
# define ASSERT(f) if ( !f ) syck_assert( __FILE__, __LINE__ )
#else
# define ASSERT(f)
#endif

#ifndef NULL
# define NULL (void *)0
#endif

/*
 * Node definitions
 */

#define BLOCK_FOLD  10
#define BLOCK_LIT   20
#define BLOCK_PLAIN 30
#define NL_CHOMP    40
#define NL_KEEP     50

/* set to 0 to avoid stack-overflows on dumping circular refs */
#ifndef EMITTER_PERMIT_DUPLICATE_REFS
#define EMITTER_PERMIT_DUPLICATE_REFS 1
#endif

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
    scalar_plain,
    scalar_2quote_1 /* Added by Audrey Tang to support JSON's single quoting */
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
    char *type_id;
    /* Anchor name */
    const char *anchor;
    union {
        /* Storage for map data */
        struct SyckMap {
            enum map_style style;
            SYMID *keys;
            SYMID *values;
            long capa;
            long idx;
        } *pairs;
        /* Storage for sequence data */
        struct SyckSeq {
            enum seq_style style;
            SYMID *items;
            long capa;
            long idx;
        } *list;
        /* Storage for string data */
        struct SyckStr {
            enum scalar_style style;
            char *ptr;
            long len;
        } *str;
    } data;
    /* Shortcut node */
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
    char *buffer;
    char *linectptr;
    char *lineptr;
    char *toktmp;
    char *token;
    char *cursor;
    char *marker;
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
        SyckIoFile *file;
        SyckIoStr *str;
    } io;
    /* Symbol table for anchors */
    st_table *anchors;
    st_table *bad_anchors;
    /* Optional symbol table for SYMIDs */
    st_table *syms;
    /* Levels of indentation */
    SyckLevel *levels;
    int lvl_idx;
    int lvl_capa;
    /* Pointer for extension's use */
    void *bonus;
};

/*
 * Emitter definitions
 */
typedef struct _syck_emitter SyckEmitter;
typedef struct _syck_emitter_node SyckEmitterNode;

typedef void (*SyckOutputHandler)(SyckEmitter *e, const char *, long);
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
    st_table *markers;
    st_table *anchors;
    st_table *anchored;
    /* Custom buffer size */
    size_t bufsize;
    /* Buffer */
    char *buffer;
    char *marker;
    /* Absolute position of the buffer */
    long bufpos;
    /* Handler for emitter nodes */
    SyckEmitterHandler emitter_handler;
    /* Handler for output */
    SyckOutputHandler output_handler;
    /* Levels of indentation */
    SyckLevel *levels;
    int lvl_idx;
    int lvl_capa;
    int max_depth;
    int depth;
    int permit_duplicate_refs;
    /* Pointer for extension's use */
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

#ifndef EXPORT
# ifdef SWIG
#  define EXPORT extern
# elif defined(_WIN32) && defined(ENABLE_SHARED)
#  ifdef DLL_EXPORT
#    define EXPORT  __declspec(dllexport)
#  else
#    define EXPORT  __declspec(dllimport)
#  endif
# elif defined(__clang__) || defined(__clang) || \
        (defined( __GNUC__) && ((__GNUC__ * 100) + __GNUC_MINOR__) >= 303)
#  define EXPORT __attribute__((visibility("default")))
# else
#  define EXPORT
# endif
#endif

/*
 * Handler prototypes
 */
EXPORT __attribute__warn_unused_result__
SYMID syck_hdlr_add_node( SyckParser *p, SyckNode *n );
EXPORT __attribute__warn_unused_result__
SyckNode *syck_hdlr_add_anchor( SyckParser *p, char *a, SyckNode *n );
EXPORT void syck_hdlr_remove_anchor( SyckParser *p, char *a );
EXPORT __attribute__warn_unused_result__
__attribute__returns_nonnull__
SyckNode *syck_hdlr_get_anchor( SyckParser *p, char *a );
EXPORT void syck_add_transfer( char *uri, SyckNode *n, int taguri );
EXPORT __attribute__warn_unused_result__
char *syck_xprivate( const char *type_id, int type_len );
EXPORT __attribute__malloc__
__attribute__warn_unused_result__
char *syck_taguri( const char *domain, const char *type_id, int type_len );
EXPORT __attribute__warn_unused_result__
int syck_tagcmp( const char *tag1, const char *tag2 );
EXPORT int syck_add_sym( SyckParser *p, void *data );
EXPORT int syck_lookup_sym( SyckParser *p, SYMID, char **out );
EXPORT __attribute__warn_unused_result__
int syck_try_implicit( SyckNode *n );
EXPORT __attribute__warn_unused_result__
char *syck_type_id_to_uri( char *type_id );
EXPORT void try_tag_implicit( SyckNode *n, int taguri );
EXPORT __attribute__warn_unused_result__
__attribute__returns_nonnull__
const char *syck_match_implicit( const char *str, size_t len );

/*
 * API prototypes
 */
EXPORT __attribute__malloc__
__attribute__warn_unused_result__
char *syck_strndup( const char *buf, long len );
EXPORT long syck_io_file_read( char *buf, SyckIoFile *file, long max_size, long skip );
EXPORT __attribute__warn_unused_result__
long syck_io_str_read( char *buf, SyckIoStr *str, long max_size, long skip );
EXPORT __attribute__warn_unused_result__
char *syck_base64enc( char *, long );
EXPORT __attribute__warn_unused_result__
char *syck_base64dec( char *, long, long * );
EXPORT __attribute__malloc__
__attribute__warn_unused_result__
__attribute__returns_nonnull__
SyckEmitter *syck_new_emitter(void);
EXPORT void syck_emitter_permit_duplicate_refs( SyckEmitter *e, int );
EXPORT __attribute__warn_unused_result__
SYMID syck_emitter_mark_node( SyckEmitter *e, st_data_t );
EXPORT void syck_emitter_ignore_id( SyckEmitter *e, SYMID );
EXPORT void syck_output_handler( SyckEmitter *e, SyckOutputHandler );
EXPORT void syck_emitter_handler( SyckEmitter *e, SyckEmitterHandler );
EXPORT void syck_free_emitter( SyckEmitter *e );
EXPORT void syck_emitter_clear( SyckEmitter *e );
EXPORT void syck_emitter_write( SyckEmitter *e, const char *, long );
EXPORT void syck_emitter_escape( SyckEmitter *e, const unsigned char *, long );
EXPORT void syck_emitter_flush( SyckEmitter *e, long );
EXPORT void syck_emit( SyckEmitter *e, st_data_t );
EXPORT void syck_emit_scalar( SyckEmitter *e, const char *, enum scalar_style force_style, int, int, char, const char *, long );
EXPORT void syck_emit_1quoted( SyckEmitter *e, int, const char *, long );
EXPORT void syck_emit_2quoted( SyckEmitter *e, int, const char *, long );
EXPORT void syck_emit_2quoted_1( SyckEmitter *e, int, const char *, long );
EXPORT void syck_emit_folded( SyckEmitter *e, int, char, const char *, long );
EXPORT void syck_emit_literal( SyckEmitter *e, char, const char *, long );
EXPORT void syck_emit_seq( SyckEmitter *e, const char *, enum seq_style style );
EXPORT void syck_emit_item( SyckEmitter *e, st_data_t );
EXPORT void syck_emit_map( SyckEmitter *e, const char *, enum map_style style );
EXPORT void syck_emit_end( SyckEmitter *e );
EXPORT void syck_emit_tag( SyckEmitter *e, const char *tag, const char *ignore );
EXPORT void syck_emit_indent( SyckEmitter *e );
EXPORT __attribute__warn_unused_result__
__attribute__returns_nonnull__
SyckLevel *syck_emitter_current_level( SyckEmitter *e );
EXPORT __attribute__warn_unused_result__
__attribute__returns_nonnull__
SyckLevel *syck_emitter_parent_level( SyckEmitter *e );
EXPORT void syck_emitter_pop_level( SyckEmitter *e );
EXPORT void syck_emitter_add_level( SyckEmitter *e, int, enum syck_level_status );
EXPORT void syck_emitter_reset_levels( SyckEmitter *e );
EXPORT __attribute__malloc__
__attribute__warn_unused_result__
__attribute__returns_nonnull__
SyckParser *syck_new_parser(void);
EXPORT void syck_free_parser( SyckParser *p );
EXPORT void syck_parser_set_root_on_error( SyckParser *p, SYMID );
EXPORT void syck_parser_implicit_typing( SyckParser *p, int );
EXPORT void syck_parser_taguri_expansion( SyckParser *p, int );
__attribute__warn_unused_result__
int syck_scan_scalar( int, const char *, long );
EXPORT void syck_parser_handler( SyckParser *p, SyckNodeHandler );
EXPORT void syck_parser_error_handler( SyckParser *p, SyckErrorHandler );
EXPORT void syck_parser_bad_anchor_handler( SyckParser *p, SyckBadAnchorHandler );
EXPORT void syck_parser_set_input_type( SyckParser *p, enum syck_parser_input input_type );
EXPORT void syck_parser_file( SyckParser *p, FILE *, SyckIoFileRead );
EXPORT void syck_parser_str( SyckParser *p, char *, long, SyckIoStrRead );
EXPORT void syck_parser_str_auto( SyckParser *p, const char *, SyckIoStrRead );
EXPORT __attribute__returns_nonnull__
SyckLevel *syck_parser_current_level( SyckParser *p );
EXPORT void syck_parser_add_level( SyckParser *p, int, enum syck_level_status );
EXPORT void syck_parser_pop_level( SyckParser *p );
EXPORT void free_any_io( SyckParser *p );
EXPORT long syck_parser_read( SyckParser *p );
EXPORT long syck_parser_readlen( SyckParser *p, long );
EXPORT SYMID syck_parse( SyckParser *p );
EXPORT void syck_default_error_handler( SyckParser *p, const char * );
EXPORT int syck_str_is_unquotable_integer(const char* str, long len);
EXPORT SYMID syck_yaml2byte_handler( SyckParser *p, SyckNode *n );
EXPORT char *syck_yaml2byte( char *yamlstr );

/*
 * Allocation prototypes
 */
EXPORT __attribute__malloc__
SyckNode *syck_alloc_map(void);
EXPORT __attribute__malloc__
SyckNode *syck_alloc_seq(void);
EXPORT __attribute__malloc__
SyckNode *syck_alloc_str(void);
EXPORT __attribute__warn_unused_result__
__attribute__returns_nonnull__
const char *syck_node_kind( SyckNode *n );
EXPORT void syck_safe_free_node( SyckParser *p, SyckNode **np );
EXPORT void syck_free_node( SyckNode **np );
EXPORT void syck_free_members( SyckNode *n );
EXPORT void syck_free_name( char *name );

EXPORT __attribute__malloc__
__attribute__warn_unused_result__
SyckNode *syck_new_str( const char *str, enum scalar_style style );
EXPORT __attribute__malloc__
__attribute__warn_unused_result__
SyckNode *syck_new_str2( char *str, long len, enum scalar_style style );
EXPORT void syck_replace_str( SyckNode *n, char *str, enum scalar_style style );
EXPORT void syck_replace_str2( SyckNode *n, char *str, long len, enum scalar_style style );
EXPORT void syck_str_blow_away_commas( SyckNode *n );
#ifdef HAVE_RUBY_ST_H
EXPORT int
syck_st_free_nodes( st_data_t key, st_data_t record,
                    st_data_t arg );
#else
EXPORT enum st_retval
syck_st_free_nodes( SHIM(const char *key), void *record,
                    SHIM(void *arg) );
#endif

char *syck_str_read( SyckNode *n );
EXPORT __attribute__malloc__
__attribute__warn_unused_result__
SyckNode *syck_new_map( SYMID key, SYMID value );
EXPORT void syck_map_empty( SyckNode *n );
EXPORT void syck_map_add( SyckNode *map, SYMID key, SYMID value );
EXPORT SYMID syck_map_read( SyckNode *map, enum map_part p, long idx );
EXPORT void syck_map_assign( SyckNode *map, enum map_part p, long idx, SYMID id );
EXPORT long syck_map_count( SyckNode *map );
EXPORT void syck_map_update( SyckNode *map1, SyckNode *map2 );
EXPORT SyckNode *syck_new_seq( SYMID value );
EXPORT void syck_seq_empty( SyckNode *n );
EXPORT void syck_seq_add( SyckNode *arr, SYMID value );
EXPORT void syck_seq_assign( SyckNode *seq, long idx, SYMID id );
EXPORT SYMID syck_seq_read( SyckNode *seq, long idx );
EXPORT long syck_seq_count( SyckNode *seq );

/*
 * Lexer prototypes
 */
EXPORT void syckerror( SyckParser *parser, const char *msg );
#if defined(__APPLE__) && defined(__MACH__)
/* defined in gram.h. but for some unknown reason macOS does not
   see that decl in syck.c */
int syckparse(void *parser);
#endif

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
EXPORT int sycklex( YYSTYPE *_sycklval, SyckParser *parser );

#if defined(__cplusplus)
}  /* extern "C" { */
#endif

#endif /* ifndef SYCK_H */
