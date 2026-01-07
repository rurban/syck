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
#include "YTS-lib.h"
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

static void yts_test_func(CuTest *tc) {
  char s[128];
  char path[64];
  char fn[128];
  struct yts_node *stream = NULL;
  CuString *cs, *ev;
  char *yaml;
  FILE *fh, *outfh = NULL, *testfh = NULL;
  size_t fsize, nread;

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
  CuAssert(tc, "in.yaml not found", fh != NULL);
  fseek(fh, 0, SEEK_END);
  fsize = ftell(fh);
  yaml = S_ALLOC_N(char, fsize + 1);
  fseek(fh, 0, SEEK_SET);
  nread = fread(yaml, 1, fsize, fh);
  yaml[nread] = '\0';
  if (nread != fsize)
    fprintf(stderr, "Unexpected shortened file %s: %zu != %zu\n", fn, nread, fsize);

  snprintf(fn, sizeof(fn)-1, DATA_DIR "%s/out.yaml", path);
  outfh = fopen(fn, "r");
  CuAssert(tc, "out.yaml not found", outfh != NULL);
  snprintf(fn, sizeof(fn)-1, DATA_DIR "%s/test.event", path);
  testfh = fopen(fn, "r");
  CuAssert(tc, "test.event not found", testfh != NULL);

  cs = CuStringNew();
  ev = CuStringNew();
  test_yaml_and_stream(cs, yaml, ev);

  if (!compare_cs(tc, outfh, cs))
    printf("OK out.yaml matches\n");
  else {
    printf("FAIL out.yaml does not match\n");
  }
  fclose(outfh);

  if (!compare_cs(tc, testfh, ev))
    printf("OK test.event matches\n");
  else {
    printf("FAIL test.event does not match\n");
  }
  fclose(testfh);

  CuStringFree(cs);
  CuStringFree(ev);
  S_FREE(yaml);
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
