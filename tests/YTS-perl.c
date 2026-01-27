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
  TestNode stream[] = {{T_STR, 0, "42", NULL, 0}, end_node};
  CuStreamCompare(tc, "--- 42\n", stream);
  CuRoundTrip(tc, stream);
}
/* ext/perl/t/2-scalars.t 4: Dump a ref to a number
 * =: was a syck extension for defaults.
 */
static void YtsScalars_2(CuTest *tc) {
  TestNode map[] = {{T_STR, "tag:yaml.org,2002:default", "=", NULL, 0},
                    {T_STR, 0, "42", NULL, 0}, end_node};
  TestNode stream[] = {
    {T_MAP, "tag:yaml.org,2002:ref", 0, map, 0}, end_node};
  CuStreamCompare(tc, "!ref\n=: 42\n", stream);
  CuRoundTrip(tc, stream);
}
/* ext/perl/t/2-scalars.t 6: Dump a circular ref
 * =: was a syck extension for defaults.
 * This does not parse with syck anymore, perl only.
 */
static void YtsScalars_3(CuTest *tc) {
  TestNode stream[] = {{T_STR, "tag:yaml.org,2002:null", "", NULL, 0},
                       end_node};
  CuStreamCompare(tc, "&1 !ref \n=: *1\n", stream);
  CuRoundTrip(tc, stream);
}
/* ext/perl/t/2-scalars.t 6: Dump code
 */
static void YtsScalars_4(CuTest *tc) {
  TestNode stream[] = {{T_STR, "tag:perl.yaml.org,2002:code:", "{ \"foo\" . \\$_[0] }", NULL, 0},
                       end_node};
  CuStreamCompare(tc, "!perl/code: '{ \"foo\" . \\$_[0] }'\n", stream);
  CuRoundTrip(tc, stream);
}
/* ext/perl/t/2-scalars.t Dump undef
 */
static void YtsScalars_5(CuTest *tc) {
  TestNode stream[] = {{T_STR, "tag:yaml.org,2002:null", "~", NULL, 0},
                    end_node};
  CuStreamCompare(tc, "~\n", stream);
  CuRoundTrip(tc, stream);
}
/* ext/perl/t/2-scalars.t Dump '~'
 */
static void YtsScalars_6(CuTest *tc) {
  TestNode stream[] = {{T_STR, "tag:yaml.org,2002:str", "~", NULL, 1}, // style 1quote
                    end_node};
  CuStreamCompare(tc, "'~'\n", stream);
  CuRoundTrip(tc, stream);
}
/* ext/perl/t/2-scalars.t Dump 'a:'
 */
static void YtsScalars_7(CuTest *tc) {
  TestNode map[] = {{T_STR, "tag:yaml.org,2002:str", "a", NULL, 0},
                    {T_STR, 0, "", NULL, 0}, end_node};
  TestNode stream[] = {{T_MAP, 0, 0, map, 0}, end_node};
  CuStreamCompare(tc, "a:\n", stream);
  CuRoundTrip(tc, stream);
}
/* ext/perl/t/2-scalars.t Dump 'a: '
   This deviates from perl, which keeps the ending space
 */
static void YtsScalars_8(CuTest *tc) {
  TestNode map[] = {{T_STR, "tag:yaml.org,2002:str", "a", NULL, 0},
                    {T_STR, 0, "", NULL, 0}, end_node};
  TestNode stream[] = {{T_MAP, 0, 0, map, 0}, end_node};
  CuStreamCompare(tc, "a: \n", stream);
  CuRoundTrip(tc, stream);
}
/* ext/perl/t/2-scalars.t Dump 'a '
   This deviates from perl, which keeps the ending space
 */
static void YtsScalars_9(CuTest *tc) {
  TestNode stream[] = {{T_STR, "tag:yaml.org,2002:str", "a", NULL, 0},
                       end_node};
  CuStreamCompare(tc, "a \n", stream);
  CuRoundTrip(tc, stream);
}
/* ext/perl/t/2-scalars.t Dump 'a: b' as map
 */
static void YtsScalars_10(CuTest *tc) {
  TestNode map[] = {{T_STR, "tag:yaml.org,2002:str", "a", NULL, 0},
                    {T_STR, 0, "b", NULL, 0}, end_node};
  TestNode stream[] = {{T_MAP, 0, 0, map, 0}, end_node};
  CuStreamCompare(tc, "a: b\n", stream);
  CuRoundTrip(tc, stream);
}
/* ext/perl/t/2-scalars.t Dump 'a:b' as str
 */
static void YtsScalars_11(CuTest *tc) {
  TestNode stream[] = {{T_STR, "tag:yaml.org,2002:str", "a:b", NULL, 0},
                       end_node};
  CuStreamCompare(tc, "a:b\n", stream);
  CuRoundTrip(tc, stream);
}
/* ext/perl/t/2-scalars.t Dump aliased seqs
 */
static void YtsScalars_12(CuTest *tc) {
  TestNode seq[] = {{T_STR, 0, "baz", NULL, 0},
                     {T_STR, 0, "troz", NULL, 0},
                     end_node};
  TestNode map[] = {{T_STR, 0, "bar", NULL, 0},
                    {T_SEQ, 0, 0, seq, 0},
                    {T_STR, 0, "foo", NULL, 0},
                    {T_SEQ, 0, 0, seq, 0},
                    end_node};
  TestNode stream[] = {{T_MAP, 0, 0, map, 0}, end_node};
  CuStreamCompare(tc, "--- \nbar: &1 \n  - baz\n  - troz\nfoo: *1\n", stream);
  CuRoundTrip(tc, stream);
}
/* ext/perl/t/2-scalars.t true and false
 */
static void YtsScalars_13(CuTest *tc) {
  TestNode stream[] = {{T_STR, 0, "true", NULL, 0}, end_node};
  CuStreamCompare(tc, "--- true\n", stream);
  CuRoundTrip(tc, stream);
}
static void YtsScalars_14(CuTest *tc) {
  TestNode stream[] = {{T_STR, 0, "false", NULL, 0}, end_node};
  CuStreamCompare(tc, "--- false\n", stream);
  CuRoundTrip(tc, stream);
}


static CuSuite *SyckGetSuite(void) {
  CuSuite *suite = CuSuiteNew();
  SUITE_ADD_TEST(suite, YtsScalars_1);
  SUITE_ADD_TEST(suite, YtsScalars_2);
  SUITE_ADD_TEST(suite, YtsScalars_3);
  SUITE_ADD_TEST(suite, YtsScalars_4);
  SUITE_ADD_TEST(suite, YtsScalars_5);
  SUITE_ADD_TEST(suite, YtsScalars_6);
  SUITE_ADD_TEST(suite, YtsScalars_7);
  SUITE_ADD_TEST(suite, YtsScalars_8);
  SUITE_ADD_TEST(suite, YtsScalars_9);
  SUITE_ADD_TEST(suite, YtsScalars_10);
  SUITE_ADD_TEST(suite, YtsScalars_11);
  SUITE_ADD_TEST(suite, YtsScalars_12);
  SUITE_ADD_TEST(suite, YtsScalars_13);
  SUITE_ADD_TEST(suite, YtsScalars_14);
  return suite;
}

int main(void) {
  CuString *output = CuStringNew();
  CuSuite *suite = SyckGetSuite();
  int count;

  CuSuiteRun/*NoJmp*/(suite); // this leaks the parser on errors
  CuSuiteSummary(suite, output);
  CuSuiteDetails(suite, output);

  printf("%s\n", output->buffer);
  count = suite->failCount;

  CuStringFree(output);
  CuSuiteFree(suite);

  return count;
}
