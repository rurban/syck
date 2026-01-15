//
// YTS-lib.c
//
// Copyright (C) 2004 why the lucky stiff
//
// Well, this is the Yaml Testing Suite in the form of a plain C
// API.  Basically, this is as good as C integration gets for Syck.
// You've got to have a symbol table around.  From there, you can
// query your data.
//

#include "syck.h"
#include "CuTest.h"
#include "YTS-lib.h"
#include <string.h>
#include <sys/stat.h>

const struct test_node end_node = {T_END, NULL, NULL, NULL, 0};

int file_exists(const char *fn) {
  struct stat st;
  if (stat(fn, &st) != 0)
    return 0;
  return S_ISREG(st.st_mode) || S_ISLNK(st.st_mode);
}
int dir_exists(const char *fn) {
  struct stat st;
  if (stat(fn, &st) != 0)
    return 0;
  return S_ISDIR(st.st_mode);
}

SYMID
syck_copy_handler(SyckParser *p, SyckNode *n) {
  int i = 0;
  struct test_node *tn = S_ALLOC_N(struct test_node, 1);

  switch (n->kind) {
  case syck_str_kind:
    tn->type = T_STR;
    tn->key = syck_strndup(n->data.str->ptr, n->data.str->len);
    tn->value = 0;
    tn->style = n->data.str->style;
    break;

  case syck_seq_kind: {
    struct test_node *val;
    struct test_node *seq = S_ALLOC_N(struct test_node, n->data.list->idx + 1);
    tn->type = T_SEQ;
    tn->key = NULL;
    tn->style = (enum scalar_style)n->data.list->style;
    for (i = 0; i < n->data.list->idx; i++) {
      SYMID oid = syck_seq_read(n, i);
      syck_lookup_sym(p, oid, (char **)&val);
      seq[i] = val[0];
    }
    seq[n->data.list->idx] = end_node;
    tn->value = seq;
  } break;

  case syck_map_kind: {
    struct test_node *val;
    struct test_node *map =
        S_ALLOC_N(struct test_node, (n->data.pairs->idx * 2) + 1);
    tn->type = T_MAP;
    tn->key = NULL;
    tn->style = (enum scalar_style)n->data.pairs->style;
    for (i = 0; i < n->data.pairs->idx; i++) {
      SYMID oid = syck_map_read(n, map_key, i);
      syck_lookup_sym(p, oid, (char **)&val);
      map[i * 2] = val[0];

      oid = syck_map_read(n, map_value, i);
      syck_lookup_sym(p, oid, (char **)&val);
      map[(i * 2) + 1] = val[0];
    }
    map[n->data.pairs->idx * 2] = end_node;
    tn->value = map;
  } break;
  default:
    break;
  }

  tn->tag = NULL;
  if (n->type_id != NULL) {
    tn->tag = syck_strndup(n->type_id, strlen(n->type_id));
  }

  return syck_add_sym(p, (char *)tn);
}

enum st_retval
syck_free_copies(SHIM(const char *key), void *_tn,
                 SHIM(void *arg)) {
  struct test_node_dyn *tn = (struct test_node_dyn *)_tn;
  UNUSED(key);
  UNUSED(arg);
  if (tn != NULL) {
    switch (tn->type) {
    case T_STR:
      S_FREE(tn->key);
      break;

    case T_SEQ:
    case T_MAP:
      S_FREE(tn->value);
      break;

    default:
      break;
    }
    if (tn->tag != NULL)
      S_FREE(tn->tag);
    S_FREE(tn);
  }
  tn = NULL;
  return ST_CONTINUE;
}

/*
 * Assertion which compares a YAML document with an
 * equivalent set of test_node structs.
 */
void
CuStreamCompareX(CuTest *tc, struct test_node *s1, struct test_node *s2) {
  int i = 0;
  while (1) {
    CuAssertIntEquals(tc, s1[i].type, s2[i].type);
    if (s1[i].type == T_END)
      return;
    if (s1[i].tag != 0 && s2[i].tag != 0)
      CuAssertStrEquals(tc, s1[i].tag, (char*)s2[i].tag);
    switch (s1[i].type) {
    case T_STR:
      CuAssertStrEquals(tc, s1[i].key, (char*)s2[i].key);
      break;
    case T_SEQ:
    case T_MAP:
      CuStreamCompareX(tc, s1[i].value, s2[i].value);
      break;
    default:
      break;
    }
    i++;
  }
}

void
CuStreamCompare(CuTest *tc, const char *yaml, struct test_node *stream) {
  int doc_ct = 0;
  struct test_node *ystream = S_ALLOC_N(struct test_node, doc_ct + 1);

  /* Set up parser */
  SyckParser *parser = syck_new_parser();
  syck_parser_str_auto(parser, yaml, NULL);
  syck_parser_handler(parser, syck_copy_handler);
  syck_parser_error_handler(parser, NULL);
  syck_parser_implicit_typing(parser, 1);
  syck_parser_taguri_expansion(parser, 1);

  /* Parse all streams */
  while (1) {
    struct test_node *ydoc;
    SYMID oid = syck_parse(parser);
    int res;

    if (parser->eof == 1)
      break;

    /* Add document to stream */
    res = syck_lookup_sym(parser, oid, (char **)&ydoc);
    if (0 == res)
      break;

    ystream[doc_ct] = ydoc[0];
    doc_ct++;
    S_REALLOC_N(ystream, struct test_node, doc_ct + 1);
  }
  ystream[doc_ct] = end_node;

  /* Traverse the struct and the symbol table side-by-side */
  /* DEBUG: y( stream, 0 ); y( ystream, 0 ); */
  CuStreamCompareX(tc, stream, ystream);

  /* Free the node tables and the parser */
  S_FREE(ystream);
  if (parser->syms != NULL)
    st_foreach(parser->syms, syck_free_copies, 0);
  syck_free_parser(parser);
}

/*
 * Setup for testing N->Y->N.
 */
void
test_output_handler(SyckEmitter *emitter, const char *str, long len) {
  CuString *dest = (CuString *)emitter->bonus;
  CuStringAppendLen(dest, str, len);
}

SYMID
build_symbol_table(SyckEmitter *emitter, struct test_node *node) {
  switch (node->type) {
  case T_SEQ:
  case T_MAP: {
    int i = 0;
    while (node->value[i].type != T_END) {
      build_symbol_table(emitter, &node->value[i]);
      i++;
    }
  }
    return syck_emitter_mark_node(emitter, (st_data_t)node, 0);

  default:
    break;
  }
  return 0;
}

void test_emitter_handler(SyckEmitter *emitter, st_data_t data) {
  struct test_node *node = (struct test_node *)data;
  switch (node->type) {
  case T_STR:
    syck_emit_scalar(emitter, node->tag, scalar_none, 0, 0, 0, node->key,
                     strlen(node->key));
    break;
  case T_SEQ: {
    int i = 0;
    syck_emit_seq(emitter, node->tag, seq_none);
    while (node->value[i].type != T_END) {
      syck_emit_item(emitter, (st_data_t)&node->value[i]);
      i++;
    }
    syck_emit_end(emitter);
  } break;
  case T_MAP: {
    int i = 0;
    syck_emit_map(emitter, node->tag, map_none);
    while (node->value[i].type != T_END) {
      syck_emit_item(emitter, (st_data_t)&node->value[i]);
      i++;
    }
    syck_emit_end(emitter);
  } break;
  default:
    break;
  }
}

void
CuRoundTrip(CuTest *tc, struct test_node *stream) {
  int i = 0;
  CuString *cs = CuStringNew();
  SyckEmitter *emitter = syck_new_emitter();

  /* Calculate anchors and tags */
  build_symbol_table(emitter, stream);

  /* Build the stream */
  syck_output_handler(emitter, test_output_handler);
  syck_emitter_handler(emitter, test_emitter_handler);
  emitter->bonus = cs;
  while (stream[i].type != T_END) {
    syck_emit(emitter, (st_data_t)&stream[i]);
    syck_emitter_flush(emitter, 0);
    i++;
  }

  /* Reload the stream and compare */
  /* printf( "-- output for %s --\n%s\n--- end of output --\n", tc->name,
   * cs->buffer ); */
  CuStreamCompare(tc, cs->buffer, stream);
  CuStringFree(cs);

  syck_free_emitter(emitter);
}

/* TODO: print in YTS test.event format */
void emit_stream(CuString *cs, struct test_node *s) {
  int i = 0;
  while (1) {
    struct test_node *n = &s[i];
    if (n->type == T_END)
      return;
    switch (n->type) {
    case T_STR:
      CuStringAppend(cs, "=VAL ");
      // TODO anchor
      if (n->tag) {
        // FIXME get rid of x-private internally
        if (memcmp(n->tag, "x-private:", sizeof("x-private:")-1) == 0)
          CuStringAppendFormat(cs, "<tag:yaml.org,2002:%s> ", &n->tag[sizeof("x-private:")-1]);
        else if (memcmp(n->tag, "tag:yaml.org,2002:", sizeof("tag:yaml.org,2002:")-1) != 0)
          CuStringAppendFormat(cs, "<%s> ", n->tag);
      }
      if (n->style == scalar_1quote || n->style == scalar_2quote || n->style == scalar_2quote_1)
        CuStringAppend(cs, "'");
      else if (n->style == scalar_fold)
        CuStringAppend(cs, ">");
      else if (n->style == scalar_literal)
        CuStringAppend(cs, "|");
      else // none, plain
        CuStringAppend(cs, ":");
      CuStringAppendEscaped(cs, n->key);
      CuStringAppendLen(cs, "\n", 1);
      break;
    case T_SEQ:
      if (n->style == 1)
        CuStringAppend(cs, "+SEQ []\n");
      else
        CuStringAppend(cs, "+SEQ\n");
      emit_stream(cs, n->value);
      CuStringAppend(cs, "-SEQ\n");
      break;
    case T_MAP:
      if (n->style == 1)
        CuStringAppend(cs, "+MAP {}\n");
      else
        CuStringAppend(cs, "+MAP\n");
      emit_stream(cs, n->value);
      CuStringAppend(cs, "-MAP\n");
      break;
    default:
      break;
    }
    i++;

  }
}

static void yts_parser_error_handler(SyckParser *p, const char *msg) {
  // ignore all errors when we know it should fail
  fprintf(stderr, "Error at [Line %d, Col %ld]: %s\n",
        p->linect,
        p->cursor - p->lineptr,
        msg );
}

/* parses the in.yaml, builds an event stream, and emits it to a cs string.
 */
void test_yaml_and_stream(CuString *cs, const char *yaml, CuString *ev,
                          int should_fail)
{
    SyckEmitter *emitter = syck_new_emitter();
    struct test_node *ystream = S_ALLOC_N(struct test_node, 1);
    int doc_ct = 0;
    int i = 0;

    /* Set up parser */
    SyckParser *parser = syck_new_parser();
    syck_parser_str_auto(parser, yaml, NULL);
    syck_parser_handler(parser, syck_copy_handler);
    if (should_fail)
      syck_parser_error_handler(parser, yts_parser_error_handler);
    else
      syck_parser_error_handler(parser, NULL);
    syck_parser_implicit_typing(parser, 1);
    syck_parser_taguri_expansion(parser, 1);

    emitter->headless = 1;

    /* Parse all streams */
    while (1) {
        struct test_node *ydoc;
        SYMID oid = syck_parse(parser);
        int res;

        if (!oid) // syntax error, no nodes added
            break;
        if (parser->eof == 1)
            break;

        /* Add document to stream */
        res = syck_lookup_sym(parser, oid, (char **)&ydoc);
        if (0 == res)
            break;

        ystream[doc_ct] = ydoc[0];
        doc_ct++;
        S_REALLOC_N(ystream, struct test_node, doc_ct + 1);
    }
    ystream[doc_ct] = end_node;

    // print it
    puts(cs->buffer);
 
    if (doc_ct) {
      build_symbol_table(emitter, ystream);

      /* Build the stream */
      syck_output_handler(emitter, test_output_handler);
      syck_emitter_handler(emitter, test_emitter_handler);
      emitter->bonus = cs;
      while (ystream[i].type != T_END) {
        syck_emit(emitter, (st_data_t)&ystream[i]);
        syck_emitter_flush(emitter, 0);
        i++;
      }
      //puts("\n--- # Parsed Stream");
      CuStringAppend(ev, "+STR\n+DOC\n");
      emit_stream(ev, ystream);
      CuStringAppend(ev, "-DOC\n-STR\n");
    }

    S_FREE(ystream);
    if (parser->syms != NULL)
        st_foreach(parser->syms, syck_free_copies, 0);
    syck_free_parser(parser);
    syck_free_emitter(emitter);
}

// max linelength 256!
CuString *CuSlurpFile(FILE *fh) {
    CuString *cs = CuStringNew();
    char buf[256];
    while (fgets(buf, sizeof(buf), fh)) {
        CuStringAppend(cs, buf);
    }
    return cs;
}

int compare_cs(CuTest *tc, FILE *fh, const CuString *cs) {
    CuString *file_cs = CuSlurpFile(fh);
    if (tc) {
      CuAssertStrEquals(tc, file_cs->buffer, cs->buffer);
      CuStringFree(file_cs);
      return 0;
    } else {
      if (strcmp(file_cs->buffer, cs->buffer) == 0) {
        CuStringFree(file_cs);
        return 0;
      }
      fprintf(stderr, "expected <\n%s> but was <\n%s>\n", file_cs->buffer, cs->buffer);
      CuStringFree(file_cs);
      return 1;
    }
}
