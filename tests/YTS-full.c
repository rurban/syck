/*
 * YTS-full.c
 *
 * Copyright (C) 2026 Reini Urban
 *
 * Run the complete yaml-test-suite in plain C to check any deviations from libyaml.
 * - parse all yaml-test-suite/XXXX/in.yaml
 * - emit the node events in a special event stream format,
 *   and compare to yaml-test-suite/XXXX/test.event
 * - and emit it, comparing to yaml-test-suite/XXXX/out.yaml
 */

#define _GNU_SOURCE
#include "CuTest.h"
#include "syck.h"
#include <string.h>
#include <dirent.h>

#define DATA_DIR "yaml-test-suite/"

/* YTS node events, in a test.event format */
enum yts_event_type {
   NO_EVENT,                 // ???
   STREAM_START_EVENT,       // +STR
   STREAM_END_EVENT,         // -STR
   DOC_START_EVENT,          // +DOC
   DOC_EXPLICIT_START_EVENT, // +DOC with ---
   DOC_END_EVENT,            // -DOC
   DOC_EXPLICIT_END_EVENT,   // -DOC with ...
   MAP_START_EVENT,          // +MAP
   MAP_EMPTY_EVENT,          // +MAP with {}
   MAP_END_EVENT,            // -MAP
   SEQ_START_EVENT,          // +SEQ
   SEQ_EMPTY_EVENT,          // +SEQ with []
   SEQ_END_EVENT,            // -SEQ
   SCALAR_EVENT,             // =VAL with opt. tag/anchor and a value
};

enum yts_opt_type {
   opt_no_type,
   opt_value_type,  // :
   opt_empty_type,  // [], {}, ---, ...
};

struct yts_node {
  enum yts_event_type type;
  enum yts_opt_type opt_type;
  char *tag;
  char *anchor;
  char *value;
  struct yts_node *kids;
};
const struct yts_node yts_end_node = {STREAM_END_EVENT, opt_no_type, NULL, NULL, NULL, NULL};

/*
 * Helper to compare YAML parse node events with
 * equivalent test.event lines.
 */
static SYMID
yts_parse_event_handler(SyckParser *p, SyckNode *n) {
  int i = 0;
  struct yts_node *tn = S_ALLOC_N(struct yts_node, 1);
  memset(tn, 0, sizeof(struct yts_node));

  tn->opt_type = opt_no_type;
  if (n->type_id) {
    tn->tag = syck_strndup(n->type_id, strlen(n->type_id));
  }
  else if (n->anchor) {
    tn->anchor = syck_strndup(n->anchor, strlen(n->anchor));
  }

  switch (n->kind) {
  case syck_str_kind:
    tn->type = SCALAR_EVENT; // =VAL with opt. tag/anchor and a value
    if (!n->type_id && !n->anchor)
      tn->opt_type = opt_value_type;
    tn->value = syck_strndup(n->data.str->ptr, n->data.str->len);
    break;

  case syck_seq_kind:
    tn->type = SEQ_START_EVENT;
    tn->value = NULL;
    if (!n->data.list->idx) {
      tn->type = SEQ_EMPTY_EVENT;
      tn->opt_type = opt_empty_type;
    }
    break;

  case syck_map_kind:
    tn->type = MAP_START_EVENT;
    tn->value = NULL;
    if (!n->data.list->idx) {
      tn->type = MAP_EMPTY_EVENT;
      tn->opt_type = opt_empty_type;
    }
    break;

  default:
    exit(1);
  }

  return syck_add_sym(p, (char *)tn);
}

static enum st_retval
syck_free_copies(SHIM(const char *key), void *_tn,
                 SHIM(void *arg)) {
  const struct yts_node *tn = (const struct yts_node *)_tn;
  UNUSED(key);
  UNUSED(arg);
  if (tn != NULL) {
    S_FREE(tn->tag);
    S_FREE(tn->anchor);
    S_FREE(tn->value);
  }
  S_FREE(tn);
  tn = NULL;
  return ST_CONTINUE;
}

static void CuStreamCompareX(CuTest *tc, struct yts_node *s1, struct yts_node *s2) {
  int i = 0;
  while (1) {
    CuAssertIntEquals(tc, s1[i].type, s2[i].type);
    /*
    if (s1[i].type == T_END)
      return;
    if (s1[i].tag != 0 && s2[i].tag != 0)
      CuAssertStrEquals(tc, s1[i].tag, s2[i].tag);
    switch (s1[i].type) {
    case T_STR:
      CuAssertStrEquals(tc, s1[i].key, s2[i].key);
      break;
    case T_SEQ:
    case T_MAP:
      CuStreamCompareX(tc, s1[i].value, s2[i].value);
      break;
    }
    */
    i++;
  }
}

static void CuStreamCompare(CuTest *tc, char *yaml, struct yts_node *stream) {
  int doc_ct = 0;
  struct yts_node *ystream = S_ALLOC_N(struct yts_node, doc_ct + 1);

  /* Set up parser */
  SyckParser *parser = syck_new_parser();
  syck_parser_str_auto(parser, yaml, NULL);
  syck_parser_handler(parser, yts_parse_event_handler);
  syck_parser_error_handler(parser, NULL);
  syck_parser_implicit_typing(parser, 1);
  syck_parser_taguri_expansion(parser, 1);

  /* Parse all streams */
  while (1) {
    struct yts_node *ydoc;
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
    S_REALLOC_N(ystream, struct yts_node, doc_ct + 1);
  }
  ystream[doc_ct] = yts_end_node;

  /* Traverse the struct and the symbol table side-by-side */
  /* DEBUG: y( stream, 0 ); y( ystream, 0 ); */
  if (stream)
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
static void test_output_handler(SyckEmitter *emitter, const char *str, long len) {
  CuString *dest = (CuString *)emitter->bonus;
  CuStringAppendLen(dest, str, len);
}

static
int is_end_type(struct yts_node *node) {
  return node->type == MAP_END_EVENT || node->type == SEQ_END_EVENT;
}

static SYMID
build_symbol_table(SyckEmitter *emitter, struct yts_node *node) {
  switch (node->type) {
  case SEQ_START_EVENT:
  case MAP_START_EVENT: {
    int i = 0;
    while (!is_end_type(&(node->kids[i]))) {
      build_symbol_table(emitter, &node->kids[i]);
      i++;
    }
    return syck_emitter_mark_node(emitter, (st_data_t)node, 0);
  }
  default:
    break;
  }
  return 0;
}

static void
test_emitter_handler(SyckEmitter *emitter, st_data_t data) {
  struct yts_node *node = (struct yts_node *)data;
  switch (node->type) {
  case SCALAR_EVENT:
    syck_emit_scalar(emitter, node->tag, scalar_none, 0, 0, 0, node->value,
                     strlen(node->value));
    break;
  case SEQ_START_EVENT:
  case SEQ_EMPTY_EVENT:
  {
    int i = 0;
    syck_emit_seq(emitter, node->tag, seq_none);
    while (!is_end_type(&node->kids[i])) {
      syck_emit_item(emitter, (st_data_t)&node->kids[i]);
      i++;
    }
    syck_emit_end(emitter);
  } break;
  case MAP_START_EVENT:
  case MAP_EMPTY_EVENT:
  {
    int i = 0;
    syck_emit_map(emitter, node->tag, map_none);
    while (!is_end_type(&node->kids[i])) {
      syck_emit_item(emitter, (st_data_t)&node->kids[i]);
      i++;
    }
    syck_emit_end(emitter);
  } break;
  default:
    break;
  }
}

__attribute__unused__
static void
CuRoundTrip(CuTest *tc, struct yts_node *stream) {
  int i = 0;
  CuString *cs = CuStringNew();
  SyckEmitter *emitter = syck_new_emitter();

  /* Calculate anchors and tags */
  build_symbol_table(emitter, stream);

  /* Build the stream */
  syck_output_handler(emitter, test_output_handler);
  syck_emitter_handler(emitter, test_emitter_handler);
  emitter->bonus = cs;
  while (!is_end_type(&stream[i])) {
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

static void
yts_test_func(CuTest *tc) {
  char s[128];
  char path[64];
  char fn[128];
  struct yts_node *stream = NULL;
  char *yaml;
  FILE *fh;

  strncpy(path, tc->name, sizeof(path)-1);
  snprintf(fn, sizeof(fn)-1, DATA_DIR "%s/===", path);
  fn[sizeof(fn)-1] = '\0';
  fh = fopen(fn, "r");
  free(tc->name);
  if (!fh) {
    tc->name = strdup(path);
  } else {
    tc->name = strdup(fgets(s, sizeof(s)-1, fh));
    tc->name[strlen(tc->name)-1] = '\0';
    fclose(fh);
  }
  fprintf(stderr, "%s/in.yaml: %s\n", path, tc->name);
  snprintf(fn, sizeof(fn)-1, DATA_DIR "%s/in.yaml", path);
  fn[sizeof(fn)-1] = '\0';
  fh = fopen(fn, "r");
  if (fh) {
    size_t fsize, nread;
    fseek(fh, 0, SEEK_END);
    fsize = ftell(fh);
    yaml = S_ALLOC_N(char, fsize + 1);
    fseek(fh, 0, SEEK_SET);
    nread = fread(yaml, 1, fsize, fh);
    yaml[nread] = '\0';
    if (nread != fsize)
      fprintf(stderr, "Unexpected shortened file %s: %zu != %zu\n", fn, nread, fsize);
    CuStreamCompare(tc, yaml, stream);
    S_FREE(yaml);
  } else {
    fprintf(stderr, "no file %s\n", fn);
  }
}

static void addYTSDir(const char *prefix, struct dirent *dir, CuSuite *suite) {
  FILE *fh;
  char fn[256];
  char name[256];
  size_t off;

  if (dir->d_name[0] == '.')
    return;
  snprintf(fn, sizeof(fn)-1, "%s/%s/in.yaml", prefix, dir->d_name);
  fn[sizeof(fn)-1] = '\0';
  fh = fopen(fn, "r");
  if (!fh) {
    struct dirent **subdirs;
    int n1;
    // some subdirs of data
    snprintf(fn, sizeof(fn)-1, "%s/%s", prefix, dir->d_name);
    fn[sizeof(fn)-1] = '\0';
    n1 = scandir(fn, &subdirs, NULL, alphasort);
    if (n1) {
      for (int i=0; i<n1; i++) {
        char a = subdirs[i]->d_name[0];
        if (a == '.')
          continue;
        if ((a >= '0' && a <= '9')
            || (a >= 'A' && a <= 'Z'))
          addYTSDir(fn, subdirs[i], suite);
      }
      free(subdirs);
      subdirs = NULL;
    } else {
      fprintf(stderr, "no dir %s\n", fn);
    }
  }
  off = sizeof(DATA_DIR)-1;
  snprintf(name, sizeof(name)-1, "%s/%s", prefix, dir->d_name);
  name[sizeof(name)-1] = '\0';
  fh = fopen(fn, "r");
  if (fh) {
    CuSuiteAdd(suite, CuTestNew(&name[off], yts_test_func));
    fclose(fh);
  }
}

static CuSuite *SyckGetSuite(void) {
  CuSuite *suite = CuSuiteNew();
  struct dirent **flist;
  int n = scandir("yaml-test-suite", &flist, NULL, alphasort);
  if (n < 0) {
    perror("scandir");
    return NULL;
  }
  for (int i=0; i<n; i++) {
    char a = flist[i]->d_name[0];
    if (a == '.')
      continue;
    if ((a >= '1' && a <= '9')
        || (a >= 'A' && a <= 'Z'))
      addYTSDir("yaml-test-suite", flist[i], suite);
  }
  free(flist);
  return suite;
}

int main(void) {
  CuString *output = CuStringNew();
  CuSuite *suite = SyckGetSuite();
  int count;

  if (!suite)
    return 1;
  CuSuiteRun(suite);
  CuSuiteSummary(suite, output);
  CuSuiteDetails(suite, output);

  printf("%s\n", output->buffer);
  count = suite->failCount;

  CuStringFree(output);
  CuSuiteFree(suite);

  return count;
}
