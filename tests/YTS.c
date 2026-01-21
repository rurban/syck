//
// YTS.c
//
// Copyright (C) 2004 why the lucky stiff
// Copyright (C) 2026 Reini Urban
// See ../COPYING (MIT and D&R)
//
// Well, this is the Yaml Testing Suite in the form of a plain C
// API.  Basically, this is as good as C integration gets for Syck.
// You've got to have a symbol table around.  From there, you can
// query your data.
//

#include "CuTest.h"
#include "syck.h"
#include "YTS-lib.h"
#include <string.h>

extern const struct test_node end_node;

/*
 * ACTUAL TESTS FOR THE YAML TESTING SUITE BEGIN HERE
 *   (EVERYTHING PREVIOUS WAS SET UP FOR THE TESTS)
 */

/*
 * Example : Trailing tab in plains
 * YTS DC7X/in.yaml Various trailing tabs
 */
static void YtsFoldedScalars_7(CuTest *tc) {
  struct test_node map[] = {
    {T_STR, 0, "a", NULL, 0}, {T_STR, 0, "b", NULL, 0}, end_node};
  struct test_node stream[] = {{T_MAP, 0, 0, map, 0}, end_node};

  CuStreamCompare(tc,

                  /* YAML document */
                  "a: b\t  \n",

                  /* C structure of validations */
                  stream);

  CuRoundTrip(tc, stream);
}
/*
 * Example : Empty Sequence
 * part of 7ZZ5/in.yaml Empty flow collections
 */
static void YtsNullsAndEmpties_0(CuTest *tc) {
  struct test_node seq[] = {end_node};
  struct test_node map[] = {
      {T_STR, 0, "empty", NULL, 0}, {T_SEQ, 0, 0, seq, 0}, end_node};
  struct test_node stream[] = {{T_MAP, 0, 0, map, 0}, end_node};

  CuStreamCompare(tc,

                  /* YAML document */
                  "empty: [] \n",

                  /* C structure of validations */
                  stream);

  CuRoundTrip(tc, stream);
}
/*
 * Example : Empty Mapping
 * part of 7ZZ5/in.yaml Empty flow collections
 */
static void YtsNullsAndEmpties_1(CuTest *tc) {
  struct test_node map2[] = {end_node};
  struct test_node map1[] = {
    {T_STR, 0, "empty", NULL, 0}, {T_MAP, 0, 0, map2, 0}, end_node};
  struct test_node stream[] = {{T_MAP, 0, 0, map1, 0}, end_node};

  CuStreamCompare(tc,

                  /* YAML document */
                  "empty: {} \n",

                  /* C structure of validations */
                  stream);

  CuRoundTrip(tc, stream);
}
/*
 * Example : Empty Sequence as Entire Document
 * none in YTS
 */
static void YtsNullsAndEmpties_2(CuTest *tc) {
  struct test_node seq[] = {end_node};
  struct test_node stream[] = {{T_SEQ, 0, 0, seq, 0}, end_node};

  CuStreamCompare(tc,

                  /* YAML document */
                  "--- [] \n",

                  /* C structure of validations */
                  stream);

  CuRoundTrip(tc, stream);
}
/*
 * Example : Empty Mapping as Entire Document
 * none in YTS
 */
static void YtsNullsAndEmpties_3(CuTest *tc) {
  struct test_node map[] = {end_node};
  struct test_node stream[] = {{T_MAP, 0, 0, map, 0}, end_node};

  CuStreamCompare(tc,

                  /* YAML document */
                  "--- {} \n",

                  /* C structure of validations */
                  stream);

  CuRoundTrip(tc, stream);
}
/*
 * Example : Null as Document
 * none in YTS
 */
static void YtsNullsAndEmpties_4(CuTest *tc) {
  struct test_node stream[] = {{T_STR, 0, "~", NULL, 0}, end_node};

  CuStreamCompare(tc,

                  /* YAML document */
                  "--- ~ \n",

                  /* C structure of validations */
                  stream);

  CuRoundTrip(tc, stream);
}
/*
 * Example : Empty String
 * none in YTS
 */
static void YtsNullsAndEmpties_5(CuTest *tc) {
  struct test_node stream[] = {{T_STR, 0, "", NULL, 0}, end_node};

  CuStreamCompare(tc,

                  /* YAML document */
                  "--- '' \n",

                  /* C structure of validations */
                  stream);

  CuRoundTrip(tc, stream);
}
/*
 * Example 2.1: Sequence of scalars
 * YTS FQ7F/in.yaml
 */
static void YtsSpecificationExamples_0(CuTest *tc) {
  struct test_node seq[] = {{T_STR, 0, "Mark McGwire", NULL, 0},
                            {T_STR, 0, "Sammy Sosa", NULL, 0},
                            {T_STR, 0, "Ken Griffey", NULL, 0},
                            end_node};
  struct test_node stream[] = {{T_SEQ, 0, 0, seq, 0}, end_node};

  CuStreamCompare(tc,

                  /* YAML document */
                  "- Mark McGwire \n"
                  "- Sammy Sosa \n"
                  "- Ken Griffey \n",

                  /* C structure of validations */
                  stream);

  CuRoundTrip(tc, stream);
}
/*
 * Example 2.2: Mapping of scalars to scalars
 * YTS SYW4/in.yaml
 */
static void YtsSpecificationExamples_1(CuTest *tc) {
  struct test_node map[] = {{T_STR, 0, "hr", NULL, 0},
                            {T_STR, 0, "65", NULL, 0},
                            {T_STR, 0, "avg", NULL, 0},
                            {T_STR, 0, "0.278", NULL, 0},
                            {T_STR, 0, "rbi", NULL, 0},
                            {T_STR, 0, "147", NULL, 0},
                            end_node};
  struct test_node stream[] = {{T_MAP, 0, 0, map, 0}, end_node};

  CuStreamCompare(tc,

                  /* YAML document */
                  "hr:  65 \n"
                  "avg: 0.278 \n"
                  "rbi: 147 \n",

                  /* C structure of validations */
                  stream);

  CuRoundTrip(tc, stream);
}
/*
 * Example 2.3: Mapping of scalars to sequences
 * YTS PBJ2/in.yaml
 */
static void YtsSpecificationExamples_2(CuTest *tc) {
  struct test_node seq1[] = {{T_STR, 0, "Boston Red Sox", NULL, 0},
                             {T_STR, 0, "Detroit Tigers", NULL, 0},
                             {T_STR, 0, "New York Yankees", NULL, 0},
                             end_node};
  struct test_node seq2[] = {{T_STR, 0, "New York Mets", NULL, 0},
                             {T_STR, 0, "Chicago Cubs", NULL, 0},
                             {T_STR, 0, "Atlanta Braves", NULL, 0},
                             end_node};
  struct test_node map[] = {{T_STR, 0, "american", NULL, 0},
                            {T_SEQ, 0, 0, seq1, 0},
                            {T_STR, 0, "national", NULL, 0},
                            {T_SEQ, 0, 0, seq2, 0},
                            end_node};
  struct test_node stream[] = {{T_MAP, 0, 0, map, 0}, end_node};

  CuStreamCompare(tc,

                  /* YAML document */
                  "american: \n"
                  "   - Boston Red Sox \n"
                  "   - Detroit Tigers \n"
                  "   - New York Yankees \n"
                  "national: \n"
                  "   - New York Mets \n"
                  "   - Chicago Cubs \n"
                  "   - Atlanta Braves \n",

                  /* C structure of validations */
                  stream);

  CuRoundTrip(tc, stream);
}
/*
 * Example 2.4: Sequence of mappings
 * YTS data/229Q/in.yaml
 */
static void YtsSpecificationExamples_3(CuTest *tc) {
  struct test_node map1[] = {{T_STR, 0, "name", NULL, 0},
                             {T_STR, 0, "Mark McGwire", NULL, 0},
                             {T_STR, 0, "hr", NULL, 0},
                             {T_STR, 0, "65", NULL, 0},
                             {T_STR, 0, "avg", NULL, 0},
                             {T_STR, 0, "0.278", NULL, 0},
                             end_node};
  struct test_node map2[] = {{T_STR, 0, "name", NULL, 0},
                             {T_STR, 0, "Sammy Sosa", NULL, 0},
                             {T_STR, 0, "hr", NULL, 0},
                             {T_STR, 0, "63", NULL, 0},
                             {T_STR, 0, "avg", NULL, 0},
                             {T_STR, 0, "0.288", NULL, 0},
                             end_node};
  struct test_node seq[] = {{T_MAP, 0, 0, map1, 0}, {T_MAP, 0, 0, map2, 0}, end_node};
  struct test_node stream[] = {{T_SEQ, 0, 0, seq, 0}, end_node};

  CuStreamCompare(tc,

                  /* YAML document */
                  "-  \n"
                  "  name: Mark McGwire \n"
                  "  hr:   65 \n"
                  "  avg:  0.278 \n"
                  "-  \n"
                  "  name: Sammy Sosa \n"
                  "  hr:   63 \n"
                  "  avg:  0.288 \n",

                  /* C structure of validations */
                  stream);

  CuRoundTrip(tc, stream);
}
/*
 * Example legacy_A5: Legacy A5
 */
static void YtsSpecificationExamples_4(CuTest *tc) {
  struct test_node seq1[] = {{T_STR, 0, "New York Yankees", NULL, 0},
                             {T_STR, 0, "Atlanta Braves", NULL, 0},
                             end_node};
  struct test_node seq2[] = {{T_STR, 0, "2001-07-02", NULL, 0},
                             {T_STR, 0, "2001-08-12", NULL, 0},
                             {T_STR, 0, "2001-08-14", NULL, 0},
                             end_node};
  struct test_node seq3[] = {{T_STR, 0, "Detroit Tigers", NULL, 0},
                             {T_STR, 0, "Chicago Cubs", NULL, 0},
                             end_node};
  struct test_node seq4[] = {{T_STR, 0, "2001-07-23", NULL, 0}, end_node};
  struct test_node map[] = {{T_SEQ, 0, 0, seq1, 0},
                            {T_SEQ, 0, 0, seq2, 0},
                            {T_SEQ, 0, 0, seq3, 0},
                            {T_SEQ, 0, 0, seq4, 0},
                            end_node};
  struct test_node stream[] = {{T_MAP, 0, 0, map, 0}, end_node};

  CuStreamCompare(tc,

                  /* YAML document */
                  "? \n"
                  "    - New York Yankees \n"
                  "    - Atlanta Braves \n"
                  ": \n"
                  "  - 2001-07-02 \n"
                  "  - 2001-08-12 \n"
                  "  - 2001-08-14 \n"
                  "? \n"
                  "    - Detroit Tigers \n"
                  "    - Chicago Cubs \n"
                  ": \n"
                  "  - 2001-07-23 \n",

                  /* C structure of validations */
                  stream);

  CuRoundTrip(tc, stream);
}
/*
 * Example 2.5: Sequence of sequences
 */
static void YtsSpecificationExamples_5(CuTest *tc) {
  struct test_node seq1[] = {{T_STR, 0, "name", NULL, 0},
                             {T_STR, 0, "hr", NULL, 0},
                             {T_STR, 0, "avg", NULL, 0},
                             end_node};
  struct test_node seq2[] = {{T_STR, 0, "Mark McGwire", NULL, 0},
                             {T_STR, 0, "65", NULL, 0},
                             {T_STR, 0, "0.278", NULL, 0},
                             end_node};
  struct test_node seq3[] = {{T_STR, 0, "Sammy Sosa", NULL, 0},
                             {T_STR, 0, "63", NULL, 0},
                             {T_STR, 0, "0.288", NULL, 0},
                             end_node};
  struct test_node seq[] = {
    {T_SEQ, 0, 0, seq1, 0}, {T_SEQ, 0, 0, seq2, 0}, {T_SEQ, 0, 0, seq3, 0}, end_node};
  struct test_node stream[] = {{T_SEQ, 0, 0, seq, 0}, end_node};

  CuStreamCompare(tc,

                  /* YAML document */
                  "- [ name         , hr , avg   ] \n"
                  "- [ Mark McGwire , 65 , 0.278 ] \n"
                  "- [ Sammy Sosa   , 63 , 0.288 ] \n",

                  /* C structure of validations */
                  stream);

  CuRoundTrip(tc, stream);
}
/*
 * Example 2.6: Mapping of mappings
 */
static void YtsSpecificationExamples_6(CuTest *tc) {
  struct test_node map1[] = {{T_STR, 0, "hr", NULL, 0},
                             {T_STR, 0, "65", NULL, 0},
                             {T_STR, 0, "avg", NULL, 0},
                             {T_STR, 0, "0.278", NULL, 0},
                             end_node};
  struct test_node map2[] = {{T_STR, 0, "hr", NULL, 0},
                             {T_STR, 0, "63", NULL, 0},
                             {T_STR, 0, "avg", NULL, 0},
                             {T_STR, 0, "0.288", NULL, 0},
                             end_node};
  struct test_node map[] = {{T_STR, 0, "Mark McGwire", NULL, 0},
                            {T_MAP, 0, 0, map1, 0},
                            {T_STR, 0, "Sammy Sosa", NULL, 0},
                            {T_MAP, 0, 0, map2, 0},
                            end_node};
  struct test_node stream[] = {{T_MAP, 0, 0, map, 0}, end_node};

  CuStreamCompare(tc,

                  /* YAML document */
                  "Mark McGwire: {hr: 65, avg: 0.278}\n"
                  "Sammy Sosa: {\n"
                  "    hr: 63,\n"
                  "    avg: 0.288\n"
                  "  }\n",

                  /* C structure of validations */
                  stream);

  CuRoundTrip(tc, stream);
}
/*
 * Example 2.7: Two documents in a stream each with a leading comment
 */
static void YtsSpecificationExamples_7(CuTest *tc) {
  struct test_node seq1[] = {{T_STR, 0, "Mark McGwire", NULL, 0},
                             {T_STR, 0, "Sammy Sosa", NULL, 0},
                             {T_STR, 0, "Ken Griffey", NULL, 0},
                             end_node};
  struct test_node seq2[] = {{T_STR, 0, "Chicago Cubs", NULL, 0},
                             {T_STR, 0, "St Louis Cardinals", NULL, 0},
                             end_node};
  struct test_node stream[] = {
      {T_SEQ, 0, 0, seq1, 0}, {T_SEQ, 0, 0, seq2, 0}, end_node};

  CuStreamCompare(tc,

                  /* YAML document */
                  "# Ranking of 1998 home runs\n"
                  "---\n"
                  "- Mark McGwire\n"
                  "- Sammy Sosa\n"
                  "- Ken Griffey\n"
                  "\n"
                  "# Team ranking\n"
                  "---\n"
                  "- Chicago Cubs\n"
                  "- St Louis Cardinals\n",

                  /* C structure of validations */
                  stream);

  CuRoundTrip(tc, stream);
}
/*
 * Example 2.8: Play by play feed from a game
 */
static void YtsSpecificationExamples_8(CuTest *tc) {
  struct test_node map1[] = {{T_STR, 0, "time", NULL, 0},
                             {T_STR, 0, "20:03:20", NULL, 0},
                             {T_STR, 0, "player", NULL, 0},
                             {T_STR, 0, "Sammy Sosa", NULL, 0},
                             {T_STR, 0, "action", NULL, 0},
                             {T_STR, 0, "strike (miss)", NULL, 0},
                             end_node};
  struct test_node map2[] = {{T_STR, 0, "time", NULL, 0},
                             {T_STR, 0, "20:03:47", NULL, 0},
                             {T_STR, 0, "player", NULL, 0},
                             {T_STR, 0, "Sammy Sosa", NULL, 0},
                             {T_STR, 0, "action", NULL, 0},
                             {T_STR, 0, "grand slam", NULL, 0},
                             end_node};
  struct test_node stream[] = {
      {T_MAP, 0, 0, map1, 0}, {T_MAP, 0, 0, map2, 0}, end_node};

  CuStreamCompare(tc,

                  /* YAML document */
                  "---\n"
                  "time: 20:03:20\n"
                  "player: Sammy Sosa\n"
                  "action: strike (miss)\n"
                  "...\n"
                  "---\n"
                  "time: 20:03:47\n"
                  "player: Sammy Sosa\n"
                  "action: grand slam\n"
                  "...\n",

                  /* C structure of validations */
                  stream);

  CuRoundTrip(tc, stream);
}
/*
 * Example 2.9: Single document with two comments
 */
static void YtsSpecificationExamples_9(CuTest *tc) {
  struct test_node seq1[] = {{T_STR, 0, "Mark McGwire", NULL, 0},
                             {T_STR, 0, "Sammy Sosa", NULL, 0},
                             end_node};
  struct test_node seq2[] = {{T_STR, 0, "Sammy Sosa", NULL, 0},
                             {T_STR, 0, "Ken Griffey", NULL, 0},
                             end_node};
  struct test_node map[] = {{T_STR, 0, "hr", NULL, 0},
                            {T_SEQ, 0, 0, seq1, 0},
                            {T_STR, 0, "rbi", NULL, 0},
                            {T_SEQ, 0, 0, seq2, 0},
                            end_node};
  struct test_node stream[] = {{T_MAP, 0, 0, map, 0}, end_node};

  CuStreamCompare(tc,

                  /* YAML document */
                  "hr: # 1998 hr ranking \n"
                  "  - Mark McGwire \n"
                  "  - Sammy Sosa \n"
                  "rbi: \n"
                  "  # 1998 rbi ranking \n"
                  "  - Sammy Sosa \n"
                  "  - Ken Griffey \n",

                  /* C structure of validations */
                  stream);

  CuRoundTrip(tc, stream);
}
/*
 * Example 2.1: Node for Sammy Sosa appears twice in this document
 */
static void YtsSpecificationExamples_10(CuTest *tc) {
  struct test_node seq1[] = {{T_STR, 0, "Mark McGwire", NULL, 0},
                             {T_STR, 0, "Sammy Sosa", NULL, 0},
                             end_node};
  struct test_node seq2[] = {{T_STR, 0, "Sammy Sosa", NULL, 0},
                             {T_STR, 0, "Ken Griffey", NULL, 0},
                             end_node};
  struct test_node map[] = {{T_STR, 0, "hr", NULL, 0},
                            {T_SEQ, 0, 0, seq1, 0},
                            {T_STR, 0, "rbi", NULL, 0},
                            {T_SEQ, 0, 0, seq2, 0},
                            end_node};
  struct test_node stream[] = {{T_MAP, 0, 0, map, 0}, end_node};

  CuStreamCompare(tc,

                  /* YAML document */
                  "---\n"
                  "hr: \n"
                  "   - Mark McGwire \n"
                  "   # Following node labeled SS \n"
                  "   - &SS Sammy Sosa \n"
                  "rbi: \n"
                  "   - *SS # Subsequent occurance \n"
                  "   - Ken Griffey \n",

                  /* C structure of validations */
                  stream);

  CuRoundTrip(tc, stream);
}
/*
 * Example 2.11: Mapping between sequences
 */
static void YtsSpecificationExamples_11(CuTest *tc) {
  struct test_node seq1[] = {{T_STR, 0, "New York Yankees", NULL, 0},
                             {T_STR, 0, "Atlanta Braves", NULL, 0},
                             end_node};
  struct test_node seq2[] = {{T_STR, 0, "2001-07-02", NULL, 0},
                             {T_STR, 0, "2001-08-12", NULL, 0},
                             {T_STR, 0, "2001-08-14", NULL, 0},
                             end_node};
  struct test_node seq3[] = {{T_STR, 0, "Detroit Tigers", NULL, 0},
                             {T_STR, 0, "Chicago Cubs", NULL, 0},
                             end_node};
  struct test_node seq4[] = {{T_STR, 0, "2001-07-23", NULL, 0}, end_node};
  struct test_node map[] = {{T_SEQ, 0, 0, seq3, 0},
                            {T_SEQ, 0, 0, seq4, 0},
                            {T_SEQ, 0, 0, seq1, 0},
                            {T_SEQ, 0, 0, seq2, 0},
                            end_node};
  struct test_node stream[] = {{T_MAP, 0, 0, map, 0}, end_node};

  CuStreamCompare(tc,

                  /* YAML document */
                  "? # PLAY SCHEDULE \n"
                  "  - Detroit Tigers \n"
                  "  - Chicago Cubs \n"
                  ":   \n"
                  "  - 2001-07-23 \n"
                  "\n"
                  "? [ New York Yankees, \n"
                  "    Atlanta Braves ] \n"
                  ": [ 2001-07-02, 2001-08-12,  \n"
                  "    2001-08-14 ] \n",

                  /* C structure of validations */
                  stream);

  CuRoundTrip(tc, stream);
}
/*
 * Example 2.12: Sequence key shortcut
 */
static void YtsSpecificationExamples_12(CuTest *tc) {
  struct test_node map1[] = {{T_STR, 0, "item", NULL, 0},
                             {T_STR, 0, "Super Hoop", NULL, 0},
                             {T_STR, 0, "quantity", NULL, 0},
                             {T_STR, 0, "1", NULL, 0},
                             end_node};
  struct test_node map2[] = {{T_STR, 0, "item", NULL, 0},
                             {T_STR, 0, "Basketball", NULL, 0},
                             {T_STR, 0, "quantity", NULL, 0},
                             {T_STR, 0, "4", NULL, 0},
                             end_node};
  struct test_node map3[] = {{T_STR, 0, "item", NULL, 0},
                             {T_STR, 0, "Big Shoes", NULL, 0},
                             {T_STR, 0, "quantity", NULL, 0},
                             {T_STR, 0, "1", NULL, 0},
                             end_node};
  struct test_node seq[] = {
    {T_MAP, 0, 0, map1, 0}, {T_MAP, 0, 0, map2, 0}, {T_MAP, 0, 0, map3, 0}, end_node};
  struct test_node stream[] = {{T_SEQ, 0, 0, seq, 0}, end_node};

  CuStreamCompare(tc,

                  /* YAML document */
                  "---\n"
                  "# products purchased\n"
                  "- item    : Super Hoop\n"
                  "  quantity: 1\n"
                  "- item    : Basketball\n"
                  "  quantity: 4\n"
                  "- item    : Big Shoes\n"
                  "  quantity: 1\n",

                  /* C structure of validations */
                  stream);

  CuRoundTrip(tc, stream);
}
/*
 * Example 2.13: Literal perserves newlines
 */
static void YtsSpecificationExamples_13(CuTest *tc) {
  struct test_node stream[] = {{T_STR, 0, "\\//||\\/||\n// ||  ||_\n", NULL, 0},
                               end_node};

  CuStreamCompare(tc,

                  /* YAML document */
                  "# ASCII Art\n"
                  "--- | \n"
                  "  \\//||\\/||\n"
                  "  // ||  ||_\n",

                  /* C structure of validations */
                  stream);

  CuRoundTrip(tc, stream);
}
/*
 * Example 2.14: Folded treats newlines as a space
 */
static void YtsSpecificationExamples_14(CuTest *tc) {
  struct test_node stream[] = {
      {T_STR, 0, "Mark McGwire's year was crippled by a knee injury.", NULL, 0},
      end_node};

  CuStreamCompare(tc,

                  /* YAML document */
                  "---\n"
                  "  Mark McGwire's\n"
                  "  year was crippled\n"
                  "  by a knee injury.\n",

                  /* C structure of validations */
                  stream);

  CuRoundTrip(tc, stream);
}
/*
 * Example 2.15: Newlines preserved for indented and blank lines
 */
static void YtsSpecificationExamples_15(CuTest *tc) {
  struct test_node stream[] = {
      {T_STR, 0,
       "Sammy Sosa completed another fine season with great stats.\n\n  63 "
       "Home Runs\n  0.288 Batting Average\n\nWhat a year!\n",
       NULL, 0},
      end_node};

  CuStreamCompare(tc,

                  /* YAML document */
                  "--- > \n"
                  " Sammy Sosa completed another\n"
                  " fine season with great stats.\n"
                  "\n"
                  "   63 Home Runs\n"
                  "   0.288 Batting Average\n"
                  "\n"
                  " What a year!\n",

                  /* C structure of validations */
                  stream);

  CuRoundTrip(tc, stream);
}
/*
 * Example 2.16: Indentation determines scope
 */
static void YtsSpecificationExamples_16(CuTest *tc) {
  struct test_node map[] = {
      {T_STR, 0, "name", NULL, 0},
      {T_STR, 0, "Mark McGwire", NULL, 0},
      {T_STR, 0, "accomplishment", NULL, 0},
      {T_STR, 0, "Mark set a major league home run record in 1998.\n", NULL, 0},
      {T_STR, 0, "stats", NULL, 0},
      {T_STR, 0, "65 Home Runs\n0.278 Batting Average\n", NULL, 0},
      end_node};
  struct test_node stream[] = {{T_MAP, 0, 0, map, 0}, end_node};

  CuStreamCompare(tc,

                  /* YAML document */
                  "name: Mark McGwire \n"
                  "accomplishment: > \n"
                  "   Mark set a major league\n"
                  "   home run record in 1998.\n"
                  "stats: | \n"
                  "   65 Home Runs\n"
                  "   0.278 Batting Average\n",

                  /* C structure of validations */
                  stream);

  CuRoundTrip(tc, stream);
}
/*
 * Example 2.18: Multiline flow scalars
 */
static void YtsSpecificationExamples_18(CuTest *tc) {
  struct test_node map[] = {
      {T_STR, 0, "plain", NULL, 0},
      {T_STR, 0, "This unquoted scalar spans many lines.", NULL, 0},
      {T_STR, 0, "quoted", NULL, 0},
      {T_STR, 0, "So does this quoted scalar.\n", NULL, 0},
      end_node};
  struct test_node stream[] = {{T_MAP, 0, 0, map, 0}, end_node};

  CuStreamCompare(tc,

                  /* YAML document */
                  "plain:\n"
                  "  This unquoted scalar\n"
                  "  spans many lines.\n"
                  "\n"
                  "quoted: \"So does this\n"
                  "  quoted scalar.\\n\"\n",

                  /* C structure of validations */
                  stream);

  CuRoundTrip(tc, stream);
}
/*
 * Example 2.19: Integers
 */
static void YtsSpecificationExamples_19(CuTest *tc) {
  struct test_node map[] = {{T_STR, 0, "canonical", NULL, 0},
                            {T_STR, 0, "12345", NULL, 0},
                            {T_STR, 0, "decimal", NULL, 0},
                            {T_STR, 0, "+12,345", NULL, 0},
                            {T_STR, 0, "sexagecimal", NULL, 0},
                            {T_STR, 0, "3:25:45", NULL, 0},
                            {T_STR, 0, "octal", NULL, 0},
                            {T_STR, 0, "014", NULL, 0},
                            {T_STR, 0, "hexadecimal", NULL, 0},
                            {T_STR, 0, "0xC", NULL, 0},
                            end_node};
  struct test_node stream[] = {{T_MAP, 0, 0, map, 0}, end_node};

  CuStreamCompare(tc,

                  /* YAML document */
                  "canonical: 12345 \n"
                  "decimal: +12,345 \n"
                  "sexagecimal: 3:25:45\n"
                  "octal: 014 \n"
                  "hexadecimal: 0xC \n",

                  /* C structure of validations */
                  stream);

  CuRoundTrip(tc, stream);
}
/*
 * Example 2.20: Floating point
 */
static void YtsSpecificationExamples_20(CuTest *tc) {
  struct test_node map[] = {{T_STR, 0, "canonical", NULL, 0},
                            {T_STR, 0, "1.23015e+3", NULL, 0},
                            {T_STR, 0, "exponential", NULL, 0},
                            {T_STR, 0, "12.3015e+02", NULL, 0},
                            {T_STR, 0, "sexagecimal", NULL, 0},
                            {T_STR, 0, "20:30.15", NULL, 0},
                            {T_STR, 0, "fixed", NULL, 0},
                            {T_STR, 0, "1230.15", NULL, 0},
                            {T_STR, 0, "negative infinity", NULL, 0},
                            {T_STR, 0, "-.inf", NULL, 0},
                            {T_STR, 0, "not a number", NULL, 0},
                            {T_STR, 0, ".nan", NULL, 0},
                            end_node};
  struct test_node stream[] = {{T_MAP, 0, 0, map, 0}, end_node};

  CuStreamCompare(tc,

                  /* YAML document */
                  "canonical: 1.23015e+3 \n"
                  "exponential: 12.3015e+02 \n"
                  "sexagecimal: 20:30.15\n"
                  "fixed: 1230.15 \n"
                  "negative infinity: -.inf\n"
                  "not a number: .nan \n",

                  /* C structure of validations */
                  stream);

  CuRoundTrip(tc, stream);
}
/*
 * Example 2.21: Miscellaneous
 */
static void YtsSpecificationExamples_21(CuTest *tc) {
  struct test_node map[] = {{T_STR, 0, "null", NULL, 0},
                            {T_STR, 0, "~", NULL, 0},
                            {T_STR, 0, "true", NULL, 0},
                            {T_STR, 0, "y", NULL, 0},
                            {T_STR, 0, "false", NULL, 0},
                            {T_STR, 0, "n", NULL, 0},
                            {T_STR, 0, "string", NULL, 0},
                            {T_STR, 0, "12345", NULL, 0},
                            end_node};
  struct test_node stream[] = {{T_MAP, 0, 0, map, 0}, end_node};

  CuStreamCompare(tc,

                  /* YAML document */
                  "null: ~ \n"
                  "true: y\n"
                  "false: n \n"
                  "string: '12345' \n",

                  /* C structure of validations */
                  stream);

  CuRoundTrip(tc, stream);
}
/*
 * Example 2.22: Timestamps
 */
static void YtsSpecificationExamples_22(CuTest *tc) {
  struct test_node map[] = {{T_STR, 0, "canonical", NULL, 0},
                            {T_STR, 0, "2001-12-15T02:59:43.1Z", NULL, 0},
                            {T_STR, 0, "iso8601", NULL, 0},
                            {T_STR, 0, "2001-12-14t21:59:43.10-05:00", NULL, 0},
                            {T_STR, 0, "spaced", NULL, 0},
                            {T_STR, 0, "2001-12-14 21:59:43.10 -05:00", NULL, 0},
                            {T_STR, 0, "spaced1", NULL, 0},
                            {T_STR, 0, "2001-12-14 21:59:43.10 -5", NULL, 0},
                            {T_STR, 0, "date", NULL, 0},
                            {T_STR, 0, "2002-12-14", NULL, 0},
                            end_node};
  struct test_node stream[] = {{T_MAP, 0, 0, map, 0}, end_node};

  CuStreamCompare(tc,

                  /* YAML document */
                  "canonical: 2001-12-15T02:59:43.1Z\n"
                  "iso8601:  2001-12-14t21:59:43.10-05:00\n"
                  "spaced:  2001-12-14 21:59:43.10 -05:00\n"
                  "spaced1:  2001-12-14 21:59:43.10 -5\n"
                  "date:   2002-12-14 # Time is noon UTC\n",

                  /* C structure of validations */
                  stream);

  CuRoundTrip(tc, stream);
}

/*
 * Example 2.23: Various explicit families
 */
static void YtsSpecificationExamples_24(CuTest *tc) {
  struct test_node map[] = {
      {T_STR, 0, "not-date", NULL, 0},
      {T_STR, "tag:yaml.org,2002:str", "2002-04-28", NULL, 0},
      {T_STR, 0, "picture", NULL, 0},
      {T_STR, "tag:yaml.org,2002:binary",
       "R0lGODlhDAAMAIQAAP//9/"
       "X\n17unp5WZmZgAAAOfn515eXv\nPz7Y6OjuDg4J+"
       "fn5OTk6enp\n56enmleECcgggoBADs=\n",
       NULL, 0},
      {T_STR, 0, "application specific tag", NULL, 0},
      {T_STR, "x-private:something",
       "The semantics of the tag\nabove may be different for\ndifferent "
       "documents.\n",
       NULL, 0},
      end_node};
  struct test_node stream[] = {{T_MAP, 0, 0, map, 0}, end_node};

  CuStreamCompare(tc,

                  /* YAML document */
                  "not-date: !str 2002-04-28\n"
                  "picture: !binary |\n"
                  " R0lGODlhDAAMAIQAAP//9/X\n"
                  " 17unp5WZmZgAAAOfn515eXv\n"
                  " Pz7Y6OjuDg4J+fn5OTk6enp\n"
                  " 56enmleECcgggoBADs=\n"
                  "\n"
                  "application specific tag: !!something |\n"
                  " The semantics of the tag\n"
                  " above may be different for\n"
                  " different documents.\n",

                  /* C structure of validations */
                  stream);

  CuRoundTrip(tc, stream);
}
/*
 * Example 2.24: Application specific family
 */
static void YtsSpecificationExamples_25(CuTest *tc) {
  struct test_node point1[] = {{T_STR, 0, "x", NULL, 0},
                               {T_STR, 0, "73", NULL, 0},
                               {T_STR, 0, "y", NULL, 0},
                               {T_STR, 0, "129", NULL, 0},
                               end_node};
  struct test_node point2[] = {{T_STR, 0, "x", NULL, 0},
                               {T_STR, 0, "89", NULL, 0},
                               {T_STR, 0, "y", NULL, 0},
                               {T_STR, 0, "102", NULL, 0},
                               end_node};
  struct test_node map1[] = {{T_STR, 0, "center", NULL, 0},
                             {T_MAP, 0, 0, point1, 0},
                             {T_STR, 0, "radius", NULL, 0},
                             {T_STR, 0, "7", NULL, 0},
                             end_node};
  struct test_node map2[] = {{T_STR, 0, "start", NULL, 0},
                             {T_MAP, 0, 0, point1, 0},
                             {T_STR, 0, "finish", NULL, 0},
                             {T_MAP, 0, 0, point2, 0},
                             end_node};
  struct test_node map3[] = {{T_STR, 0, "start", NULL, 0},
                             {T_MAP, 0, 0, point1, 0},
                             {T_STR, 0, "color", NULL, 0},
                             {T_STR, 0, "0xFFEEBB", NULL, 0},
                             {T_STR, 0, "value", NULL, 0},
                             {T_STR, 0, "Pretty vector drawing.", NULL, 0},
                             end_node};
  struct test_node seq[] = {
      {T_MAP, "tag:clarkevans.com,2002:graph/circle", 0, map1, 0},
      {T_MAP, "tag:clarkevans.com,2002:graph/line", 0, map2, 0},
      {T_MAP, "tag:clarkevans.com,2002:graph/label", 0, map3, 0},
      end_node};
  struct test_node stream[] = {
      {T_SEQ, "tag:clarkevans.com,2002:graph/shape", 0, seq, 0}, end_node};

  CuStreamCompare(tc,

                  /* YAML document */
                  "# Establish a tag prefix\n"
                  "--- !clarkevans.com,2002/graph/^shape\n"
                  "  # Use the prefix: shorthand for\n"
                  "  # !clarkevans.com,2002/graph/circle\n"
                  "- !^circle\n"
                  "  center: &ORIGIN {x: 73, 'y': 129}\n"
                  "  radius: 7\n"
                  "- !^line # !clarkevans.com,2002/graph/line\n"
                  "  start: *ORIGIN\n"
                  "  finish: { x: 89, 'y': 102 }\n"
                  "- !^label\n"
                  "  start: *ORIGIN\n"
                  "  color: 0xFFEEBB\n"
                  "  value: Pretty vector drawing.\n",

                  /* C structure of validations */
                  stream);

  CuRoundTrip(tc, stream);
}
/*
 * Example 2.26: Ordered mappings
 */
static void YtsSpecificationExamples_26(CuTest *tc) {
  struct test_node map1[] = {
      {T_STR, 0, "Mark McGwire", NULL, 0}, {T_STR, 0, "65", NULL, 0}, end_node};
  struct test_node map2[] = {
      {T_STR, 0, "Sammy Sosa", NULL, 0}, {T_STR, 0, "63", NULL, 0}, end_node};
  struct test_node map3[] = {
      {T_STR, 0, "Ken Griffy", NULL, 0}, {T_STR, 0, "58", NULL, 0}, end_node};
  struct test_node seq[] = {
      {T_MAP, 0, 0, map1, 0}, {T_MAP, 0, 0, map2, 0}, {T_MAP, 0, 0, map3, 0}, end_node};
  struct test_node stream[] = {{T_SEQ, "tag:yaml.org,2002:omap", 0, seq, 0},
                               end_node};

  CuStreamCompare(tc,

                  /* YAML document */
                  "# ordered maps are represented as\n"
                  "# a sequence of mappings, with\n"
                  "# each mapping having one key\n"
                  "--- !omap\n"
                  "- Mark McGwire: 65\n"
                  "- Sammy Sosa: 63\n"
                  "- Ken Griffy: 58\n",

                  /* C structure of validations */
                  stream);

  CuRoundTrip(tc, stream);
}
/*
 * Example 2.27: Invoice
 */
static void YtsSpecificationExamples_27(CuTest *tc) {
  struct test_node prod1[] = {{T_STR, 0, "sku", NULL, 0},
                              {T_STR, 0, "BL394D", NULL, 0},
                              {T_STR, 0, "quantity", NULL, 0},
                              {T_STR, 0, "4", NULL, 0},
                              {T_STR, 0, "description", NULL, 0},
                              {T_STR, 0, "Basketball", NULL, 0},
                              {T_STR, 0, "price", NULL, 0},
                              {T_STR, 0, "450.00", NULL, 0},
                              end_node};
  struct test_node prod2[] = {{T_STR, 0, "sku", NULL, 0},
                              {T_STR, 0, "BL4438H", NULL, 0},
                              {T_STR, 0, "quantity", NULL, 0},
                              {T_STR, 0, "1", NULL, 0},
                              {T_STR, 0, "description", NULL, 0},
                              {T_STR, 0, "Super Hoop", NULL, 0},
                              {T_STR, 0, "price", NULL, 0},
                              {T_STR, 0, "2392.00", NULL, 0},
                              end_node};
  struct test_node products[] = {
      {T_MAP, 0, 0, prod1, 0}, {T_MAP, 0, 0, prod2, 0}, end_node};
  struct test_node address[] = {
      {T_STR, 0, "lines", NULL, 0},
      {T_STR, 0, "458 Walkman Dr.\nSuite #292\n", NULL, 0},
      {T_STR, 0, "city", NULL, 0},
      {T_STR, 0, "Royal Oak", NULL, 0},
      {T_STR, 0, "state", NULL, 0},
      {T_STR, 0, "MI", NULL, 0},
      {T_STR, 0, "postal", NULL, 0},
      {T_STR, 0, "48046", NULL, 0},
      end_node};
  struct test_node id001[] = {{T_STR, 0, "given", NULL, 0},
                              {T_STR, 0, "Chris", NULL, 0},
                              {T_STR, 0, "family", NULL, 0},
                              {T_STR, 0, "Dumars", NULL, 0},
                              {T_STR, 0, "address", NULL, 0},
                              {T_MAP, 0, 0, address, 0},
                              end_node};
  struct test_node map[] = {
      {T_STR, 0, "invoice", NULL, 0},
      {T_STR, 0, "34843", NULL, 0},
      {T_STR, 0, "date", NULL, 0},
      {T_STR, 0, "2001-01-23", NULL, 0},
      {T_STR, 0, "bill-to", NULL, 0},
      {T_MAP, 0, 0, id001, 0},
      {T_STR, 0, "ship-to", NULL, 0},
      {T_MAP, 0, 0, id001, 0},
      {T_STR, 0, "product", NULL, 0},
      {T_SEQ, 0, 0, products, 0},
      {T_STR, 0, "tax", NULL, 0},
      {T_STR, 0, "251.42", NULL, 0},
      {T_STR, 0, "total", NULL, 0},
      {T_STR, 0, "4443.52", NULL, 0},
      {T_STR, 0, "comments", NULL, 0},
      {T_STR, 0,
       "Late afternoon is best. Backup contact is Nancy Billsmer @ 338-4338.\n",
       NULL, 0},
      end_node};
  struct test_node stream[] = {
      {T_MAP, "tag:clarkevans.com,2002:invoice", 0, map, 0}, end_node};

  CuStreamCompare(tc,

                  /* YAML document */
                  "--- !clarkevans.com,2002/^invoice\n"
                  "invoice: 34843\n"
                  "date   : 2001-01-23\n"
                  "bill-to: &id001\n"
                  "    given  : Chris\n"
                  "    family : Dumars\n"
                  "    address:\n"
                  "        lines: |\n"
                  "            458 Walkman Dr.\n"
                  "            Suite #292\n"
                  "        city    : Royal Oak\n"
                  "        state   : MI\n"
                  "        postal  : 48046\n"
                  "ship-to: *id001\n"
                  "product:\n"
                  "    - sku         : BL394D\n"
                  "      quantity    : 4\n"
                  "      description : Basketball\n"
                  "      price       : 450.00\n"
                  "    - sku         : BL4438H\n"
                  "      quantity    : 1\n"
                  "      description : Super Hoop\n"
                  "      price       : 2392.00\n"
                  "tax  : 251.42\n"
                  "total: 4443.52\n"
                  "comments: >\n"
                  "  Late afternoon is best.\n"
                  "  Backup contact is Nancy\n"
                  "  Billsmer @ 338-4338.\n",

                  /* C structure of validations */
                  stream);

  CuRoundTrip(tc, stream);
}
/*
 * Example 2.28: Log file
 */
static void YtsSpecificationExamples_28(CuTest *tc) {
  struct test_node map1[] = {
      {T_STR, 0, "Time", NULL, 0},
      {T_STR, 0, "2001-11-23 15:01:42 -05:00", NULL, 0},
      {T_STR, 0, "User", NULL, 0},
      {T_STR, 0, "ed", NULL, 0},
      {T_STR, 0, "Warning", NULL, 0},
      {T_STR, 0, "This is an error message for the log file\n", NULL, 0},
      end_node};
  struct test_node map2[] = {
      {T_STR, 0, "Time", NULL, 0},
      {T_STR, 0, "2001-11-23 15:02:31 -05:00", NULL, 0},
      {T_STR, 0, "User", NULL, 0},
      {T_STR, 0, "ed", NULL, 0},
      {T_STR, 0, "Warning", NULL, 0},
      {T_STR, 0, "A slightly different error message.\n", NULL, 0},
      end_node};
  struct test_node file1[] = {{T_STR, 0, "file", NULL, 0},
                              {T_STR, 0, "TopClass.py", NULL, 0},
                              {T_STR, 0, "line", NULL, 0},
                              {T_STR, 0, "23", NULL, 0},
                              {T_STR, 0, "code", NULL, 0},
                              {T_STR, 0, "x = MoreObject(\"345\\n\")\n", NULL, 0},
                              end_node};
  struct test_node file2[] = {{T_STR, 0, "file", NULL, 0},
                              {T_STR, 0, "MoreClass.py", NULL, 0},
                              {T_STR, 0, "line", NULL, 0},
                              {T_STR, 0, "58", NULL, 0},
                              {T_STR, 0, "code", NULL, 0},
                              {T_STR, 0, "foo = bar", NULL, 0},
                              end_node};
  struct test_node stack[] = {
      {T_MAP, 0, 0, file1, 0}, {T_MAP, 0, 0, file2, 0}, end_node};
  struct test_node map3[] = {{T_STR, 0, "Date", NULL, 0},
                             {T_STR, 0, "2001-11-23 15:03:17 -05:00", NULL, 0},
                             {T_STR, 0, "User", NULL, 0},
                             {T_STR, 0, "ed", NULL, 0},
                             {T_STR, 0, "Fatal", NULL, 0},
                             {T_STR, 0, "Unknown variable \"bar\"\n", NULL, 0},
                             {T_STR, 0, "Stack", NULL, 0},
                             {T_SEQ, 0, 0, stack, 0},
                             end_node};
  struct test_node stream[] = {
      {T_MAP, 0, 0, map1, 0}, {T_MAP, 0, 0, map2, 0}, {T_MAP, 0, 0, map3, 0}, end_node};

  CuStreamCompare(tc,

                  /* YAML document */
                  "---\n"
                  "Time: 2001-11-23 15:01:42 -05:00\n"
                  "User: ed\n"
                  "Warning: >\n"
                  "  This is an error message\n"
                  "  for the log file\n"
                  "---\n"
                  "Time: 2001-11-23 15:02:31 -05:00\n"
                  "User: ed\n"
                  "Warning: >\n"
                  "  A slightly different error\n"
                  "  message.\n"
                  "---\n"
                  "Date: 2001-11-23 15:03:17 -05:00\n"
                  "User: ed\n"
                  "Fatal: >\n"
                  "  Unknown variable \"bar\"\n"
                  "Stack:\n"
                  "  - file: TopClass.py\n"
                  "    line: 23\n"
                  "    code: |\n"
                  "      x = MoreObject(\"345\\n\")\n"
                  "  - file: MoreClass.py\n"
                  "    line: 58\n"
                  "    code: |-\n"
                  "      foo = bar\n",

                  /* C structure of validations */
                  stream);

  CuRoundTrip(tc, stream);
}
/*
 * Example : Throwaway comments
 */
static void YtsSpecificationExamples_29(CuTest *tc) {
  struct test_node map[] = {
      {T_STR, 0, "this", NULL, 0},
      {T_STR, 0,
       "contains three lines of text.\nThe third one starts with a\n# "
       "character. This isn't a comment.\n",
       NULL, 0},
      end_node};
  struct test_node stream[] = {{T_MAP, 0, 0, map, 0}, end_node};

  CuStreamCompare(tc,

                  /* YAML document */
                  "### These are four throwaway comment  ### \n"
                  "\n"
                  "### lines (the second line is empty). ### \n"
                  "this: |   # Comments may trail lines.\n"
                  "   contains three lines of text.\n"
                  "   The third one starts with a\n"
                  "   # character. This isn't a comment.\n"
                  "\n"
                  "# These are three throwaway comment\n"
                  "# lines (the first line is empty).\n",

                  /* C structure of validations */
                  stream);

  CuRoundTrip(tc, stream);
}
/*
 * Example : Document with a single value
 */
static void YtsSpecificationExamples_30(CuTest *tc) {
  struct test_node stream[] = {
      {T_STR, 0,
       "This YAML stream contains a single text value. The next stream is a "
       "log file - a sequence of log entries. Adding an entry to the log is a "
       "simple matter of appending it at the end.\n",
       NULL, 0},
      end_node};

  CuStreamCompare(tc,

                  /* YAML document */
                  "--- > \n"
                  "This YAML stream contains a single text value.\n"
                  "The next stream is a log file - a sequence of\n"
                  "log entries. Adding an entry to the log is a\n"
                  "simple matter of appending it at the end.\n",

                  /* C structure of validations */
                  stream);

  CuRoundTrip(tc, stream);
}
/*
 * Example : Document stream
 */
static void YtsSpecificationExamples_31(CuTest *tc) {
  struct test_node map1[] = {{T_STR, 0, "at", NULL, 0},
                             {T_STR, 0, "2001-08-12 09:25:00.00 Z", NULL, 0},
                             {T_STR, 0, "type", NULL, 0},
                             {T_STR, 0, "GET", NULL, 0},
                             {T_STR, 0, "HTTP", NULL, 0},
                             {T_STR, 0, "1.0", NULL, 0},
                             {T_STR, 0, "url", NULL, 0},
                             {T_STR, 0, "/index.html", NULL, 0},
                             end_node};
  struct test_node map2[] = {{T_STR, 0, "at", NULL, 0},
                             {T_STR, 0, "2001-08-12 09:25:10.00 Z", NULL, 0},
                             {T_STR, 0, "type", NULL, 0},
                             {T_STR, 0, "GET", NULL, 0},
                             {T_STR, 0, "HTTP", NULL, 0},
                             {T_STR, 0, "1.0", NULL, 0},
                             {T_STR, 0, "url", NULL, 0},
                             {T_STR, 0, "/toc.html", NULL, 0},
                             end_node};
  struct test_node stream[] = {
      {T_MAP, 0, 0, map1, 0}, {T_MAP, 0, 0, map2, 0}, end_node};

  CuStreamCompare(tc,

                  /* YAML document */
                  "--- \n"
                  "at: 2001-08-12 09:25:00.00 Z \n"
                  "type: GET \n"
                  "HTTP: '1.0' \n"
                  "url: '/index.html' \n"
                  "--- \n"
                  "at: 2001-08-12 09:25:10.00 Z \n"
                  "type: GET \n"
                  "HTTP: '1.0' \n"
                  "url: '/toc.html' \n",

                  /* C structure of validations */
                  stream);

  CuRoundTrip(tc, stream);
}
/*
 * Example : Top level mapping
 */
static void YtsSpecificationExamples_32(CuTest *tc) {
  struct test_node map[] = {{T_STR, 0, "invoice", NULL, 0},
                            {T_STR, 0, "34843", NULL, 0},
                            {T_STR, 0, "date", NULL, 0},
                            {T_STR, 0, "2001-01-23", NULL, 0},
                            {T_STR, 0, "total", NULL, 0},
                            {T_STR, 0, "4443.52", NULL, 0},
                            end_node};
  struct test_node stream[] = {{T_MAP, 0, 0, map, 0}, end_node};

  CuStreamCompare(tc,

                  /* YAML document */
                  "# This stream is an example of a top-level mapping. \n"
                  "invoice : 34843 \n"
                  "date    : 2001-01-23 \n"
                  "total   : 4443.52 \n",

                  /* C structure of validations */
                  stream);

  CuRoundTrip(tc, stream);
}
/*
 * Example : Single-line documents
 */
static void YtsSpecificationExamples_33(CuTest *tc) {
  struct test_node map[] = {end_node};
  struct test_node seq[] = {end_node};
  struct test_node stream[] = {
      {T_MAP, 0, 0, map, 0}, {T_SEQ, 0, 0, seq, 0}, {T_STR, 0, "", NULL, 0}, end_node};

  CuStreamCompare(tc,

                  /* YAML document */
                  "# The following is a sequence of three documents. \n"
                  "# The first contains an empty mapping, the second \n"
                  "# an empty sequence, and the last an empty string. \n"
                  "--- {} \n"
                  "--- [ ] \n"
                  "--- '' \n",

                  /* C structure of validations */
                  stream);

  CuRoundTrip(tc, stream);
}
/*
 * Example : Document with pause
 */
static void YtsSpecificationExamples_34(CuTest *tc) {
  struct test_node map1[] = {{T_STR, 0, "sent at", NULL, 0},
                             {T_STR, 0, "2002-06-06 11:46:25.10 Z", NULL, 0},
                             {T_STR, 0, "payload", NULL, 0},
                             {T_STR, 0, "Whatever", NULL, 0},
                             end_node};
  struct test_node map2[] = {{T_STR, 0, "sent at", NULL, 0},
                             {T_STR, 0, "2002-06-06 12:05:53.47 Z", NULL, 0},
                             {T_STR, 0, "payload", NULL, 0},
                             {T_STR, 0, "Whatever", NULL, 0},
                             end_node};
  struct test_node stream[] = {
      {T_MAP, 0, 0, map1, 0}, {T_MAP, 0, 0, map2, 0}, end_node};

  CuStreamCompare(
      tc,

      /* YAML document */
      "# A communication channel based on a YAML stream. \n"
      "--- \n"
      "sent at: 2002-06-06 11:46:25.10 Z \n"
      "payload: Whatever \n"
      "# Receiver can process this as soon as the following is sent: \n"
      "... \n"
      "# Even if the next message is sent long after: \n"
      "--- \n"
      "sent at: 2002-06-06 12:05:53.47 Z \n"
      "payload: Whatever \n"
      "... \n",

      /* C structure of validations */
      stream);

  CuRoundTrip(tc, stream);
}
/*
 * Example : Explicit typing
 */
static void YtsSpecificationExamples_35(CuTest *tc) {
  struct test_node map[] = {{T_STR, 0, "integer", NULL, 0},
                            {T_STR, "tag:yaml.org,2002:int", "12", NULL, 0},
                            {T_STR, 0, "also int", NULL, 0},
                            {T_STR, "tag:yaml.org,2002:int", "12", NULL, 0},
                            {T_STR, 0, "string", NULL, 0},
                            {T_STR, "tag:yaml.org,2002:str", "12", NULL, 0},
                            end_node};
  struct test_node stream[] = {{T_MAP, 0, 0, map, 0}, end_node};

  CuStreamCompare(tc,

                  /* YAML document */
                  "integer: 12 \n"
                  "also int: ! \"12\" \n"
                  "string: !str 12 \n",

                  /* C structure of validations */
                  stream);

  CuRoundTrip(tc, stream);
}
/*
 * Example : Private types
 */
static void YtsSpecificationExamples_36(CuTest *tc) {
  struct test_node pool[] = {{T_STR, 0, "number", NULL, 0},
                             {T_STR, 0, "8", NULL, 0},
                             {T_STR, 0, "color", NULL, 0},
                             {T_STR, 0, "black", NULL, 0},
                             end_node};
  struct test_node map1[] = {
      {T_STR, 0, "pool", NULL, 0}, {T_MAP, "x-private:ball", 0, pool, 0}, end_node};
  struct test_node bearing[] = {
      {T_STR, 0, "material", NULL, 0}, {T_STR, 0, "steel", NULL, 0}, end_node};
  struct test_node map2[] = {{T_STR, 0, "bearing", NULL, 0},
                             {T_MAP, "x-private:ball", 0, bearing, 0},
                             end_node};
  struct test_node stream[] = {
      {T_MAP, 0, 0, map1, 0}, {T_MAP, 0, 0, map2, 0}, end_node};

  CuStreamCompare(tc,

                  /* YAML document */
                  "# Both examples below make use of the 'x-private:ball' \n"
                  "# type family URI, but with different semantics. \n"
                  "--- \n"
                  "pool: !!ball \n"
                  "  number: 8 \n"
                  "  color: black \n"
                  "--- \n"
                  "bearing: !!ball \n"
                  "  material: steel \n",

                  /* C structure of validations */
                  stream);

  CuRoundTrip(tc, stream);
}
/*
 * Example : Type family under yaml.org
 */
static void YtsSpecificationExamples_37(CuTest *tc) {
  struct test_node seq[] = {
      {T_STR, "tag:yaml.org,2002:str", "a Unicode string", NULL, 0}, end_node};
  struct test_node stream[] = {{T_SEQ, 0, 0, seq, 0}, end_node};

  CuStreamCompare(tc,

                  /* YAML document */
                  "# The URI is 'tag:yaml.org,2002:str' \n"
                  "- !str a Unicode string \n",

                  /* C structure of validations */
                  stream);

  CuRoundTrip(tc, stream);
}
/*
 * Example : Type family under perl.yaml.org
 */
static void YtsSpecificationExamples_38(CuTest *tc) {
  struct test_node map[] = {end_node};
  struct test_node seq[] = {
      {T_MAP, "tag:perl.yaml.org,2002:Text::Tabs", 0, map, 0}, end_node};
  struct test_node stream[] = {{T_SEQ, 0, 0, seq, 0}, end_node};

  CuStreamCompare(tc,

                  /* YAML document */
                  "# The URI is 'tag:perl.yaml.org,2002:Text::Tabs' \n"
                  "- !perl/Text::Tabs {} \n",

                  /* C structure of validations */
                  stream);

  CuRoundTrip(tc, stream);
}
/*
 * Example : Type family under clarkevans.com
 */
static void YtsSpecificationExamples_39(CuTest *tc) {
  struct test_node map[] = {end_node};
  struct test_node seq[] = {
      {T_MAP, "tag:clarkevans.com,2003-02:timesheet", 0, map, 0}, end_node};
  struct test_node stream[] = {{T_SEQ, 0, 0, seq, 0}, end_node};

  CuStreamCompare(tc,

                  /* YAML document */
                  "# The URI is 'tag:clarkevans.com,2003-02:timesheet' \n"
                  "- !clarkevans.com,2003-02/timesheet {}\n",

                  /* C structure of validations */
                  stream);

  CuRoundTrip(tc, stream);
}
/*
 * Example : URI Escaping
 */
static void YtsSpecificationExamples_40(CuTest *tc) {
  struct test_node same[] = {
      {T_STR, "tag:domain.tld,2002:type0", "value", NULL, 0},
      {T_STR, "tag:domain.tld,2002:type0", "value", NULL, 0},
      end_node};
  struct test_node diff[] = {
      {T_STR, "tag:domain.tld,2002:type%30", "value", NULL, 0},
      {T_STR, "tag:domain.tld,2002:type0", "value", NULL, 0},
      end_node};
  struct test_node map[] = {{T_STR, 0, "same", NULL, 0},
                            {T_SEQ, 0, 0, same, 0},
                            {T_STR, 0, "different", NULL, 0},
                            {T_SEQ, 0, 0, diff, 0},
                            end_node};
  struct test_node stream[] = {{T_MAP, 0, 0, map, 0}, end_node};

  CuStreamCompare(tc,

                  /* YAML document */
                  "same: \n"
                  "  - !domain.tld,2002/type\\x30 value\n"
                  "  - !domain.tld,2002/type0 value\n"
                  "different: # As far as the YAML parser is concerned \n"
                  "  - !domain.tld,2002/type%30 value\n"
                  "  - !domain.tld,2002/type0 value\n",

                  /* C structure of validations */
                  stream);

  CuRoundTrip(tc, stream);
}
/*
 * Example : Overriding anchors
 */
static void YtsSpecificationExamples_42(CuTest *tc) {
  struct test_node map[] = {
      {T_STR, 0, "anchor", NULL, 0},
      {T_STR, 0, "This scalar has an anchor.", NULL, 0},
      {T_STR, 0, "override", NULL, 0},
      {T_STR, 0, "The alias node below is a repeated use of this value.\n",
       NULL, 0},
      {T_STR, 0, "alias", NULL, 0},
      {T_STR, 0, "The alias node below is a repeated use of this value.\n",
       NULL, 0},
      end_node};
  struct test_node stream[] = {{T_MAP, 0, 0, map, 0}, end_node};

  CuStreamCompare(tc,

                  /* YAML document */
                  "anchor : &A001 This scalar has an anchor. \n"
                  "override : &A001 >\n"
                  " The alias node below is a\n"
                  " repeated use of this value.\n"
                  "alias : *A001\n",

                  /* C structure of validations */
                  stream);

  CuRoundTrip(tc, stream);
}
/*
 * Example : Flow and block formatting
 */
static void YtsSpecificationExamples_43(CuTest *tc) {
  struct test_node empty[] = {end_node};
  struct test_node flow[] = {
      {T_STR, 0, "one", NULL, 0},   {T_STR, 0, "two", NULL, 0},
      {T_STR, 0, "three", NULL, 0}, {T_STR, 0, "four", NULL, 0},
      {T_STR, 0, "five", NULL, 0},  end_node};
  struct test_node inblock[] = {{T_STR, 0, "Subordinate sequence entry", NULL, 0},
                                end_node};
  struct test_node block[] = {{T_STR, 0, "First item in top sequence", NULL, 0},
                              {T_SEQ, 0, 0, inblock, 0},
                              {T_STR, 0, "A folded sequence entry\n", NULL, 0},
                              {T_STR, 0, "Sixth item in top sequence", NULL, 0},
                              end_node};
  struct test_node map[] = {{T_STR, 0, "empty", NULL, 0},
                            {T_SEQ, 0, 0, empty, 0},
                            {T_STR, 0, "flow", NULL, 0},
                            {T_SEQ, 0, 0, flow, 0},
                            {T_STR, 0, "block", NULL, 0},
                            {T_SEQ, 0, 0, block, 0},
                            end_node};
  struct test_node stream[] = {{T_MAP, 0, 0, map, 0}, end_node};

  CuStreamCompare(tc,

                  /* YAML document */
                  "empty: [] \n"
                  "flow: [ one, two, three # May span lines, \n"
                  "         , four,           # indentation is \n"
                  "           five ]          # mostly ignored. \n"
                  "block: \n"
                  " - First item in top sequence \n"
                  " - \n"
                  "  - Subordinate sequence entry \n"
                  " - > \n"
                  "   A folded sequence entry\n"
                  " - Sixth item in top sequence \n",

                  /* C structure of validations */
                  stream);

  CuRoundTrip(tc, stream);
}
/*
 * Example : Literal combinations
 */
static void YtsSpecificationExamples_47(CuTest *tc) {
  struct test_node map[] = {
      {T_STR, 0, "empty", NULL, 0},
      {T_STR, 0, "", NULL, 0},
      {T_STR, 0, "literal", NULL, 0},
      {T_STR, 0,
       "The \\ ' \" characters may be\nfreely used. Leading white\n   space "
       "is significant.\n\nLine breaks are significant.\nThus this value "
       "contains one\n"
       "empty line and ends with a\nsingle line break, but does\nnot start "
       "with one.\n",
       NULL, 0},
      {T_STR, 0, "is equal to", NULL, 0},
      {T_STR, 0,
       "The \\ ' \" characters may be\nfreely used. Leading white\n   space "
       "is significant.\n\nLine breaks are significant.\nThus this value "
       "contains one\n"
       "empty line and ends with a\nsingle line break, but does\nnot start "
       "with one.\n",
       NULL, 0},
      {T_STR, 0, "indented and chomped", NULL, 0},
      {T_STR, 0, "  This has no newline.", NULL, 0},
      {T_STR, 0, "also written as", NULL, 0},
      {T_STR, 0, "  This has no newline.", NULL, 0},
      {T_STR, 0, "both are equal to", NULL, 0},
      {T_STR, 0, "  This has no newline.", NULL, 0},
      end_node};
  struct test_node stream[] = {{T_MAP, 0, 0, map, 0}, end_node};

  CuStreamCompare(tc,

                  /* YAML document */
                  "empty: |\n"
                  "\n"
                  "literal: |\n"
                  " The \\ ' \" characters may be\n"
                  " freely used. Leading white\n"
                  "    space is significant.\n"
                  "\n"
                  " Line breaks are significant.\n"
                  " Thus this value contains one\n"
                  " empty line and ends with a\n"
                  " single line break, but does\n"
                  " not start with one.\n"
                  "\n"
                  "is equal to: \"The \\\\ ' \\\" characters may \\\n"
                  " be\\nfreely used. Leading white\\n   space \\\n"
                  " is significant.\\n\\nLine breaks are \\\n"
                  " significant.\\nThus this value contains \\\n"
                  " one\\nempty line and ends with a\\nsingle \\\n"
                  " line break, but does\\nnot start with one.\\n\"\n"
                  "\n"
                  "# Comments may follow a block \n"
                  "# scalar value. They must be \n"
                  "# less indented. \n"
                  "\n"
                  "# Modifiers may be combined in any order.\n"
                  "indented and chomped: |2-\n"
                  "    This has no newline.\n"
                  "\n"
                  "also written as: |-2\n"
                  "    This has no newline.\n"
                  "\n"
                  "both are equal to: \"  This has no newline.\"\n",

                  /* C structure of validations */
                  stream);

  CuRoundTrip(tc, stream);
}

static CuSuite *SyckGetSuite(void) {
  CuSuite *suite = CuSuiteNew();
  SUITE_ADD_TEST(suite, YtsFoldedScalars_7);
  SUITE_ADD_TEST(suite, YtsNullsAndEmpties_0);
  SUITE_ADD_TEST(suite, YtsNullsAndEmpties_1);
  SUITE_ADD_TEST(suite, YtsNullsAndEmpties_2);
  SUITE_ADD_TEST(suite, YtsNullsAndEmpties_3);
  SUITE_ADD_TEST(suite, YtsNullsAndEmpties_4);
  SUITE_ADD_TEST(suite, YtsNullsAndEmpties_5);
  SUITE_ADD_TEST(suite, YtsSpecificationExamples_0);
  SUITE_ADD_TEST(suite, YtsSpecificationExamples_1);
  SUITE_ADD_TEST(suite, YtsSpecificationExamples_2);
  SUITE_ADD_TEST(suite, YtsSpecificationExamples_3);
  SUITE_ADD_TEST(suite, YtsSpecificationExamples_4);
  SUITE_ADD_TEST(suite, YtsSpecificationExamples_5);
  SUITE_ADD_TEST(suite, YtsSpecificationExamples_6);
  SUITE_ADD_TEST(suite, YtsSpecificationExamples_7);
  SUITE_ADD_TEST(suite, YtsSpecificationExamples_8);
  SUITE_ADD_TEST(suite, YtsSpecificationExamples_9);
  SUITE_ADD_TEST(suite, YtsSpecificationExamples_10);
  SUITE_ADD_TEST(suite, YtsSpecificationExamples_11);
  SUITE_ADD_TEST(suite, YtsSpecificationExamples_12);
  SUITE_ADD_TEST(suite, YtsSpecificationExamples_13);
  SUITE_ADD_TEST(suite, YtsSpecificationExamples_14);
  SUITE_ADD_TEST(suite, YtsSpecificationExamples_15);
  SUITE_ADD_TEST(suite, YtsSpecificationExamples_16);
  SUITE_ADD_TEST(suite, YtsSpecificationExamples_18);
  SUITE_ADD_TEST(suite, YtsSpecificationExamples_19);
  SUITE_ADD_TEST(suite, YtsSpecificationExamples_20);
  SUITE_ADD_TEST(suite, YtsSpecificationExamples_21);
  SUITE_ADD_TEST(suite, YtsSpecificationExamples_22);
  SUITE_ADD_TEST(suite, YtsSpecificationExamples_24);
  SUITE_ADD_TEST(suite, YtsSpecificationExamples_25);
  SUITE_ADD_TEST(suite, YtsSpecificationExamples_26);
  SUITE_ADD_TEST(suite, YtsSpecificationExamples_27);
  SUITE_ADD_TEST(suite, YtsSpecificationExamples_28);
  SUITE_ADD_TEST(suite, YtsSpecificationExamples_29);
  SUITE_ADD_TEST(suite, YtsSpecificationExamples_30);
  SUITE_ADD_TEST(suite, YtsSpecificationExamples_31);
  SUITE_ADD_TEST(suite, YtsSpecificationExamples_32);
  SUITE_ADD_TEST(suite, YtsSpecificationExamples_33);
  SUITE_ADD_TEST(suite, YtsSpecificationExamples_34);
  SUITE_ADD_TEST(suite, YtsSpecificationExamples_35);
  SUITE_ADD_TEST(suite, YtsSpecificationExamples_36);
  SUITE_ADD_TEST(suite, YtsSpecificationExamples_37);
  SUITE_ADD_TEST(suite, YtsSpecificationExamples_38);
  SUITE_ADD_TEST(suite, YtsSpecificationExamples_39);
  SUITE_ADD_TEST(suite, YtsSpecificationExamples_40);
  SUITE_ADD_TEST(suite, YtsSpecificationExamples_42);
  SUITE_ADD_TEST(suite, YtsSpecificationExamples_43);
  SUITE_ADD_TEST(suite, YtsSpecificationExamples_47);
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
