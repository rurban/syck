#include "syck.h"
#include "CuTest.h"

#define strEQc(a,c) (strncmp((a),(c),(sizeof(c)-1))==0)

/* YAML test node structures */
enum test_types {
    T_DOC,
    T_STR,
    T_SEQ,
    T_MAP,
    T_END,
};
#define ILEN 2

struct test_node {
  enum test_types type;
  const char *tag;
  const char *key;
  struct test_node *value;
  enum scalar_style style;
};
struct test_node_dyn {
  enum test_types type;
  char *tag;
  char *key;
  struct test_node *value;
  enum scalar_style style;
};

int file_exists(const char *fn);
int dir_exists(const char *fn);
SYMID syck_copy_handler(SyckParser *p, SyckNode *n);
enum st_retval syck_free_copies(SHIM(const char *key), void *_tn, SHIM(void *arg));
void CuStreamCompareX(CuTest *tc, const struct test_node *s1, struct test_node *s2);
void CuStreamCompare(CuTest *tc, const char *yaml, const struct test_node *stream);
SYMID build_symbol_table(SyckEmitter *emitter, struct test_node *node);
void test_output_handler(SyckEmitter *emitter, const char *str, long len);
void test_emitter_handler(SyckEmitter *emitter, st_data_t data);
void CuRoundTrip(CuTest *tc, struct test_node *stream);
void emit_stream(CuString *cs, struct test_node *s);
void test_yaml_and_stream(CuString *cs, const char *yaml, CuString *ev, int should_fail);
// max linelength 256!
__attribute__malloc__
CuString *CuSlurpFile(FILE *fh);
int compare_cs(CuTest *tc, FILE *fh, const CuString *cs);
