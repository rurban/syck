#include "syck.h"
#include "CuTest.h"

/* YAML test node structures */
#define T_STR 10
#define T_SEQ 20
#define T_MAP 30
#define T_END 40
#define ILEN 2

struct test_node {
  int type;
  const char *tag;
  const char *key;
  struct test_node *value;
};
struct test_node_dyn {
  int type;
  char *tag;
  char *key;
  struct test_node *value;
};

SYMID syck_copy_handler(SyckParser *p, SyckNode *n);
enum st_retval syck_free_copies(SHIM(const char *key), void *_tn, SHIM(void *arg));
void CuStreamCompareX(CuTest *tc, struct test_node *s1, struct test_node *s2);
void CuStreamCompare(CuTest *tc, const char *yaml, struct test_node *stream);
SYMID build_symbol_table(SyckEmitter *emitter, struct test_node *node);
void test_output_handler(SyckEmitter *emitter, const char *str, long len);
void test_emitter_handler(SyckEmitter *emitter, st_data_t data);
void CuRoundTrip(CuTest *tc, struct test_node *stream);
