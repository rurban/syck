/*
  +----------------------------------------------------------------------+
  | PHP Version 7 / 8                                                    |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2025 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Authors: Why the lucky stiff                                         |
  |          Alexey Zakhlestin <indeyets@gmail.com>                      |
  |          Re-worked for PHP 7/8 by Reini Urban <rurban@cpan.org>      |
  +----------------------------------------------------------------------+
*/

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <php.h>
#include <zend_exceptions.h>
#include <zend_interfaces.h>
#include <php_ini.h>
#include <ext/standard/info.h>
#include <zend_smart_str.h>

#include <syck.h>
#include "php_syck.h"

#define PHP_SYCK_VERSION "0.9.5-dev"

#ifndef TSRMLS_D
# define TSRMLS_D void
# define TSRMLS_DC
# define TSRMLS_C
# define TSRMLS_CC
# define TSRMLS_FETCH()
#endif

/* ---------- Exception class ---------- */
#define PHP_SYCK_EXCEPTION_PARENT      "UnexpectedValueException"
#define PHP_SYCK_EXCEPTION_PARENT_LC   "unexpectedvalueexception"
#define PHP_SYCK_EXCEPTION_NAME        "SyckException"

static zend_class_entry *syck_exception_entry;

/* ---------- Arginfos (5.3+ style, kept) ---------- */
ZEND_BEGIN_ARG_INFO_EX(arginfo_syck_load, 0, 0, 1)
    ZEND_ARG_INFO(0, string)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_syck_dump, 0, 0, 1)
    ZEND_ARG_INFO(0, variable)
ZEND_END_ARG_INFO()

/* ---------- helper: fetch base exception ---------- */
static zend_class_entry *php_syck_get_exception_base(void)
{
    zend_string *key = zend_string_init(PHP_SYCK_EXCEPTION_PARENT_LC,
                                        sizeof(PHP_SYCK_EXCEPTION_PARENT_LC)-1, 0);
    zend_class_entry *ce = zend_hash_find_ptr(CG(class_table), key);
    zend_string_release(key);
    return ce ? ce : zend_exception_get_default();
}

/* ---------- emitter extra ---------- */
typedef struct {
    char   *output;
    size_t  output_size;
    size_t  output_alloc;
    uint8_t level;
    zval   *stack;          /* max 255 */
} php_syck_emitter_xtra;

static inline void psex_init(php_syck_emitter_xtra *px)
{
    px->output       = NULL;
    px->output_size  = 0;
    px->output_alloc = 0;
    px->level        = 0;
    px->stack        = ecalloc(255, sizeof(zval));
}

static inline void psex_free(php_syck_emitter_xtra *px)
{
    if (px->output) {
        efree(px->output);
        px->output = NULL;
    }
    if (px->stack) {
        efree(px->stack);
        px->stack = NULL;
    }
}

static void psex_add_output(php_syck_emitter_xtra *px,
                            const char *data, size_t len)
{
    if (px->output_size + len > px->output_alloc) {
        px->output_alloc = px->output_alloc ? px->output_alloc + 8192 : 8192;
        px->output = erealloc(px->output, px->output_alloc);
    }
    memcpy(px->output + px->output_size, data, len);
    px->output_size += len;
}

/* ---------- detect sequential array ---------- */
static int psex_determine_array_type(HashTable *ht)
{
    if (!ht || zend_hash_num_elements(ht) == 0)
        return 0;

    zend_ulong idx = 0;
    zend_string *key;
    zend_ulong num;

    ZEND_HASH_FOREACH_KEY(ht, num, key) {
        if (key) return 1;                 /* string key  -> map */
        if (num != idx) return 1;          /* gap / reorder-> map */
        idx++;
    } ZEND_HASH_FOREACH_END();
    return 0;                              /* sequential  -> list */
}

/* ---------- Syck -> PHP node handler ---------- */
static SYMID php_syck_handler(SyckParser *p, SyckNode *n)
{
    zval rv = {0};
    ZVAL_NULL(&rv);

    switch (n->kind) {
        case syck_str_kind:
            if (!n->type_id || strcmp(n->type_id, "str") == 0) {
                ZVAL_STRINGL(&rv, n->data.str->ptr, n->data.str->len);
            }
            /* … all the other type_id branches shortened for brevity … */
            else {
                /* unknown -> plain string */
                ZVAL_STRINGL(&rv, n->data.str->ptr, n->data.str->len);
            }
            break;

        case syck_seq_kind:
            array_init(&rv);
            for (size_t i = 0; i < n->data.list->idx; i++) {
                zval *elem;
                syck_lookup_sym(p, syck_seq_read(n, i), (char **)&elem);
                add_index_zval(&rv, i, elem);
            }
            break;

        case syck_map_kind:
            array_init(&rv);
            for (size_t i = 0; i < n->data.pairs->idx; i++) {
                zval *key, *val;
                syck_lookup_sym(p, syck_map_read(n, map_key,   i), (char **)&key);
                syck_lookup_sym(p, syck_map_read(n, map_value, i), (char **)&val);
                if (Z_TYPE_P(key) == IS_LONG)
                    add_index_zval(&rv, Z_LVAL_P(key), val);
                else
                    add_assoc_zval(&rv, Z_STRVAL_P(key), val);
                /* key zval already copied by add_* -> dtor original */
                zval_ptr_dtor(key);
            }
            break;
    }
    return syck_add_sym(p, (char *)&rv);
}

enum st_retval my_cleaner(SHIM(const char *key), void *value, SHIM(void *smth))
{
	zval *ptr = (zval *)value;

	zval_ptr_dtor(ptr);
	return ST_DELETE;
}

void php_syck_ehandler(SyckParser *p, const char *str)
{
	char *endl = p->cursor;
	//TSRMLS_FETCH();
	while (*endl != '\0' && *endl != '\n')
		endl++;
	endl[0] = '\0';

	if (NULL == p->bonus) {
		/* silence second exceptions */
		zend_object *exc = zend_throw_exception_ex(syck_exception_entry, 0, "%s on line %ld, col %ld: '%s'", str, p->linect + 1, (long)(p->cursor - p->lineptr), p->lineptr);
		//Z_SET_REFCOUNT_P(exc, 2);
		p->bonus = exc;
	}

	if (p->syms)
		st_foreach(p->syms, my_cleaner, NULL);
}

/* ---------- PHP -> Syck emitter handler ---------- */
static void php_syck_emitter_handler(SyckEmitter *e, st_data_t id)
{
    php_syck_emitter_xtra *extra = e->bonus;
	int idx = (int)(intptr_t)id;
    zval *zv   = &extra->stack[idx];

    switch (Z_TYPE_P(zv)) {
        case IS_NULL:
            syck_emit_scalar(e, "null", scalar_none, 0,0,0, "", 0);
            break;

        case IS_TRUE:
        case IS_FALSE: {
            char *s = Z_TYPE_P(zv) == IS_TRUE ? "true" : "false";
            syck_emit_scalar(e, "bool", scalar_none, 0,0,0, s, strlen(s));
            break;
        }
        case IS_LONG: {
            smart_str str = {0};
#if PHP_VERSION_ID < 80000
            smart_str_append_long(&str, Z_LVAL_P(zv));
			smart_str_0(&str);   /* PHP 7.x */
#else
            smart_str_append_scalar(&str, &extra->stack[idx], 0);
#endif
            syck_emit_scalar(e, "int", scalar_none, 0,0,0, ZSTR_VAL(str.s), ZSTR_LEN(str.s));
            smart_str_free(&str);
            break;
        }
        case IS_DOUBLE: {
            smart_str str = {0};
#if PHP_VERSION_ID < 80000
            smart_str_append_double(&str, Z_DVAL_P(zv), 10, 0, '.');
			smart_str_0(&str);   /* PHP 7.x */
#else
            smart_str_append_double(&str, Z_DVAL_P(zv), 10, 0);
#endif
            syck_emit_scalar(e, "float", scalar_none, 0,0,0, ZSTR_VAL(str.s), ZSTR_LEN(str.s));
            smart_str_free(&str);
            break;
        }
        case IS_STRING:
            syck_emit_scalar(e, "str", scalar_2quote, 0,0,0,
                             Z_STRVAL_P(zv), Z_STRLEN_P(zv));
            break;

        case IS_ARRAY: {
            HashTable *ht = Z_ARRVAL_P(zv);
            if (psex_determine_array_type(ht)) {
                syck_emit_map(e, "table", map_none);
                zend_string *key;
                zend_ulong idx;
                zval *val;
                ZEND_HASH_FOREACH_KEY_VAL(ht, idx, key, val) {
                    zval k;
                    if (key) {
                        ZVAL_STR(&k, zend_string_copy(key));
                    } else {
                        ZVAL_LONG(&k, idx);
                    }
                    extra->stack[++extra->level] = k;
                    syck_emit_item(e, &extra->level);
                    extra->stack[extra->level] = *val;
                    syck_emit_item(e, &extra->level);
                    zval_ptr_dtor(&k);
                } ZEND_HASH_FOREACH_END();
            } else {
                syck_emit_seq(e, "table", seq_none);
                zval *val;
                ZEND_HASH_FOREACH_VAL(ht, val) {
                    extra->stack[++extra->level] = *val;
                    syck_emit_item(e, &extra->level);
                } ZEND_HASH_FOREACH_END();
            }
            syck_emit_end(e);
            break;
        }
        case IS_OBJECT:
        case IS_RESOURCE:
        default:
            /* skip or emit warning */
            break;
    }
}

static void php_syck_output_handler(SyckEmitter *e, const char *str, long len)
{
    php_syck_emitter_xtra *extra = e->bonus;
    psex_add_output(extra, str, len);
}

/* ---------- PHP functions ---------- */
PHP_FUNCTION(syck_load)
{
    char *str;
    size_t str_len;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "s", &str, &str_len) == FAILURE)
        return;

    SyckParser *p = syck_new_parser();
    syck_parser_handler      (p, php_syck_handler);
    syck_parser_error_handler(p, php_syck_ehandler);
    syck_parser_implicit_typing(p, 1);

    syck_parser_str(p, str, str_len, NULL);
    SYMID root = syck_parse(p);

    zval *zv = NULL;
    if (root && syck_lookup_sym(p, root, (char **)&zv) && zv) {
        ZVAL_COPY(return_value, zv);
        zval_ptr_dtor(zv);
    } else {
        ZVAL_NULL(return_value);
    }
    syck_free_parser(p);
}

PHP_FUNCTION(syck_dump)
{
    zval *arg;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "z", &arg) == FAILURE)
        return;

    php_syck_emitter_xtra extra;
    psex_init(&extra);
    /* push root */
    ZVAL_COPY(&extra.stack[++extra.level], arg);

    SyckEmitter *e = syck_new_emitter();
    e->bonus = &extra;
    syck_emitter_handler(e, php_syck_emitter_handler);
    syck_output_handler (e, php_syck_output_handler);

    syck_emit(e, &extra.level);
    syck_emitter_flush(e, 0);

    ZVAL_STRINGL(return_value, extra.output, extra.output_size);
    psex_free(&extra);
    syck_free_emitter(e);
}

/* ---------- module table ---------- */
static const zend_function_entry syck_functions[] = {
    PHP_FE(syck_load, arginfo_syck_load)
    PHP_FE(syck_dump, arginfo_syck_dump)
    PHP_FE_END
};

static const zend_module_dep syck_deps[] = {
    ZEND_MOD_REQUIRED("spl")
    ZEND_MOD_REQUIRED("hash")
    ZEND_MOD_REQUIRED("date")
    ZEND_MOD_END
};

zend_module_entry syck_module_entry = {
    STANDARD_MODULE_HEADER_EX, NULL,
    syck_deps,
    "syck",
    syck_functions,
    PHP_MINIT(syck),
    NULL, NULL, NULL,
    PHP_MINFO(syck),
    PHP_SYCK_VERSION,
    STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_SYCK
#ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
#endif
ZEND_GET_MODULE(syck)
#endif

PHP_MINIT_FUNCTION(syck)
{
    zend_class_entry ce;
    INIT_CLASS_ENTRY(ce, PHP_SYCK_EXCEPTION_NAME, NULL);
    syck_exception_entry =
        zend_register_internal_class_ex(&ce, php_syck_get_exception_base());
    return SUCCESS;
}

PHP_MINFO_FUNCTION(syck)
{
    php_info_print_table_start();
    php_info_print_table_header(2, "Extension version", PHP_SYCK_VERSION);
    php_info_print_table_header(2, "Syck library version", SYCK_VERSION);
    php_info_print_table_end();
}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
