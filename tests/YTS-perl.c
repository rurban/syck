//
// YTS-perl.c
//
// Copyright (C) 2026 Reini Urban
// See ../COPYING (MIT and D&R)
//
// Write down the ext/perl tests to see if they extend ot deviate from the YTS.
//

#include "CuTest.h"
#include "syck.h"
#include "YTS-lib.h"
#include <string.h>

extern const struct test_node end_node;

/* ext/perl/t/2-scalars.t 2: Dump a simple number */
static void YtsScalars_1(CuTest *tc) {
  struct test_node stream[] = {{T_STR, 0, "42", NULL, 0}, end_node};
  CuStreamCompare(tc, "--- 42\n", stream);
  CuRoundTrip(tc, stream);
}
/* ext/perl/t/2-scalars.t 4: Dump a ref to a number */
static void YtsScalars_2(CuTest *tc) {
  struct test_node stream[] = {{T_STR, 0, "42", NULL, 0}, end_node};
  CuStreamCompare(tc, "--- !!ref\n=: 42\n", stream);
  CuRoundTrip(tc, stream);
}

static CuSuite *SyckGetSuite(void) {
  CuSuite *suite = CuSuiteNew();
  SUITE_ADD_TEST(suite, YtsScalars_1);
  SUITE_ADD_TEST(suite, YtsScalars_2);
  return suite;
}

int main(void) {
  CuString *output = CuStringNew();
  CuSuite *suite = SyckGetSuite();
  int count;

  CuSuiteRunNoJmp(suite); // this leaks the parser on errors
  CuSuiteSummary(suite, output);
  CuSuiteDetails(suite, output);

  printf("%s\n", output->buffer);
  count = suite->failCount;

  CuStringFree(output);
  CuSuiteFree(suite);

  return count;
}
