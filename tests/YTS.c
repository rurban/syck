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

extern const TestNode end_node;

/*
 * Example : Trailing tab in plains
 * YTS: DC7X Various trailing tabs
 */
static void YtsFoldedScalars_7(CuTest *tc) {
  TestNode map[] = {
    {T_STR, 0, "a", NULL, 0}, {T_STR, 0, "b", NULL, 0}, end_node};
  TestNode stream[] = {{T_MAP, 0, 0, map, 0}, end_node};

  CuStreamCompare(tc,

                  /* YAML document */
                  "a: b\t  \n",

                  /* C structure of validations */
                  stream);

  CuRoundTrip(tc, stream);
}
/*
 * Example : Empty Sequence
 * YTS: 7ZZ5 (partially) Empty flow collections
 */
static void YtsNullsAndEmpties_0(CuTest *tc) {
  TestNode seq[] = {end_node};
  TestNode map[] = {
      {T_STR, 0, "empty", NULL, 0}, {T_SEQ, 0, 0, seq, 0}, end_node};
  TestNode stream[] = {{T_MAP, 0, 0, map, 0}, end_node};

  CuStreamCompare(tc,

                  /* YAML document */
                  "empty: [] \n",

                  /* C structure of validations */
                  stream);

  CuRoundTrip(tc, stream);
}
/*
 * Example : Empty Mapping
 * YTS: 7ZZ5 (partially) Empty flow collections
 */
static void YtsNullsAndEmpties_1(CuTest *tc) {
  TestNode map2[] = {end_node};
  TestNode map1[] = {
    {T_STR, 0, "empty", NULL, 0}, {T_MAP, 0, 0, map2, 0}, end_node};
  TestNode stream[] = {{T_MAP, 0, 0, map1, 0}, end_node};

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
  TestNode seq[] = {end_node};
  TestNode stream[] = {{T_SEQ, 0, 0, seq, 0}, end_node};

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
  TestNode map[] = {end_node};
  TestNode stream[] = {{T_MAP, 0, 0, map, 0}, end_node};

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
  TestNode stream[] = {{T_STR, 0, "~", NULL, 0}, end_node};

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
  TestNode stream[] = {{T_STR, 0, "", NULL, 0}, end_node};

  CuStreamCompare(tc,

                  /* YAML document */
                  "--- '' \n",

                  /* C structure of validations */
                  stream);

  CuRoundTrip(tc, stream);
}
/*
 * Example 2.1: Sequence of scalars (ball players)
 * YTS: FQ7F
 */
static void YtsSpecificationExamples_2_1(CuTest *tc) {
  TestNode seq[] = {{T_STR, 0, "Mark McGwire", NULL, 0},
                            {T_STR, 0, "Sammy Sosa", NULL, 0},
                            {T_STR, 0, "Ken Griffey", NULL, 0},
                            end_node};
  TestNode stream[] = {{T_SEQ, 0, 0, seq, 0}, end_node};

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
 * Example 2.2: Mapping of scalars to scalars (player statstics)
 * YTS: SYW4
 */
static void YtsSpecificationExamples_2_2(CuTest *tc) {
  TestNode map[] = {{T_STR, 0, "hr", NULL, 0},
                            {T_STR, 0, "65", NULL, 0},
                            {T_STR, 0, "avg", NULL, 0},
                            {T_STR, 0, "0.278", NULL, 0},
                            {T_STR, 0, "rbi", NULL, 0},
                            {T_STR, 0, "147", NULL, 0},
                            end_node};
  TestNode stream[] = {{T_MAP, 0, 0, map, 0}, end_node};

  CuStreamCompare(tc,

                  /* YAML document */
                  "hr:  65    # Home runs\n"
                  "avg: 0.278 # Batting average\n"
                  "rbi: 147   # Runs Batted In\n",

                  /* C structure of validations */
                  stream);

  CuRoundTrip(tc, stream);
}
/*
 * Example 2.3: Mapping Scalars to Sequences (ball clubs in each league)
 * YTS: PBJ2
 */
static void YtsSpecificationExamples_2_3(CuTest *tc) {
  TestNode seq1[] = {{T_STR, 0, "Boston Red Sox", NULL, 0},
                             {T_STR, 0, "Detroit Tigers", NULL, 0},
                             {T_STR, 0, "New York Yankees", NULL, 0},
                             end_node};
  TestNode seq2[] = {{T_STR, 0, "New York Mets", NULL, 0},
                             {T_STR, 0, "Chicago Cubs", NULL, 0},
                             {T_STR, 0, "Atlanta Braves", NULL, 0},
                             end_node};
  TestNode map[] = {{T_STR, 0, "american", NULL, 0},
                            {T_SEQ, 0, 0, seq1, 0},
                            {T_STR, 0, "national", NULL, 0},
                            {T_SEQ, 0, 0, seq2, 0},
                            end_node};
  TestNode stream[] = {{T_MAP, 0, 0, map, 0}, end_node};

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
 * Example 2.4: Sequence of Mappings (players’ statistics)
 * YTS: 229Q
 */
static void YtsSpecificationExamples_2_4(CuTest *tc) {
  TestNode map1[] = {{T_STR, 0, "name", NULL, 0},
                             {T_STR, 0, "Mark McGwire", NULL, 0},
                             {T_STR, 0, "hr", NULL, 0},
                             {T_STR, 0, "65", NULL, 0},
                             {T_STR, 0, "avg", NULL, 0},
                             {T_STR, 0, "0.278", NULL, 0},
                             end_node};
  TestNode map2[] = {{T_STR, 0, "name", NULL, 0},
                             {T_STR, 0, "Sammy Sosa", NULL, 0},
                             {T_STR, 0, "hr", NULL, 0},
                             {T_STR, 0, "63", NULL, 0},
                             {T_STR, 0, "avg", NULL, 0},
                             {T_STR, 0, "0.288", NULL, 0},
                             end_node};
  TestNode seq[] = {{T_MAP, 0, 0, map1, 0}, {T_MAP, 0, 0, map2, 0}, end_node};
  TestNode stream[] = {{T_SEQ, 0, 0, seq, 0}, end_node};

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
 * Example legacy_A5: Legacy A5 (see 2.11 Mapping between Sequences)
 * YTS: similar to M5DY
 */
static void YtsSpecificationLegacy_A5(CuTest *tc) {
  TestNode seq1[] = {{T_STR, 0, "New York Yankees", NULL, 0},
                             {T_STR, 0, "Atlanta Braves", NULL, 0},
                             end_node};
  TestNode seq2[] = {{T_STR, 0, "2001-07-02", NULL, 0},
                             {T_STR, 0, "2001-08-12", NULL, 0},
                             {T_STR, 0, "2001-08-14", NULL, 0},
                             end_node};
  TestNode seq3[] = {{T_STR, 0, "Detroit Tigers", NULL, 0},
                             {T_STR, 0, "Chicago Cubs", NULL, 0},
                             end_node};
  TestNode seq4[] = {{T_STR, 0, "2001-07-23", NULL, 0}, end_node};
  TestNode map[] = {{T_SEQ, 0, 0, seq1, 0},
                            {T_SEQ, 0, 0, seq2, 0},
                            {T_SEQ, 0, 0, seq3, 0},
                            {T_SEQ, 0, 0, seq4, 0},
                            end_node};
  TestNode stream[] = {{T_MAP, 0, 0, map, 0}, end_node};

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
 * YTS: YD5X
 */
static void YtsSpecificationExamples_2_5(CuTest *tc) {
  TestNode seq1[] = {{T_STR, 0, "name", NULL, 0},
                             {T_STR, 0, "hr", NULL, 0},
                             {T_STR, 0, "avg", NULL, 0},
                             end_node};
  TestNode seq2[] = {{T_STR, 0, "Mark McGwire", NULL, 0},
                             {T_STR, 0, "65", NULL, 0},
                             {T_STR, 0, "0.278", NULL, 0},
                             end_node};
  TestNode seq3[] = {{T_STR, 0, "Sammy Sosa", NULL, 0},
                             {T_STR, 0, "63", NULL, 0},
                             {T_STR, 0, "0.288", NULL, 0},
                             end_node};
  TestNode seq[] = {
    {T_SEQ, 0, 0, seq1, 0}, {T_SEQ, 0, 0, seq2, 0}, {T_SEQ, 0, 0, seq3, 0}, end_node};
  TestNode stream[] = {{T_SEQ, 0, 0, seq, 0}, end_node};

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
 * YTS: ZF4X
 */
static void YtsSpecificationExamples_2_6(CuTest *tc) {
  TestNode map1[] = {{T_STR, 0, "hr", NULL, 0},
                             {T_STR, 0, "65", NULL, 0},
                             {T_STR, 0, "avg", NULL, 0},
                             {T_STR, 0, "0.278", NULL, 0},
                             end_node};
  TestNode map2[] = {{T_STR, 0, "hr", NULL, 0},
                             {T_STR, 0, "63", NULL, 0},
                             {T_STR, 0, "avg", NULL, 0},
                             {T_STR, 0, "0.288", NULL, 0},
                             end_node};
  TestNode map[] = {{T_STR, 0, "Mark McGwire", NULL, 0},
                            {T_MAP, 0, 0, map1, 0},
                            {T_STR, 0, "Sammy Sosa", NULL, 0},
                            {T_MAP, 0, 0, map2, 0},
                            end_node};
  TestNode stream[] = {{T_MAP, 0, 0, map, 0}, end_node};

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
 * YTS: JHB9
 */
static void YtsSpecificationExamples_2_7(CuTest *tc) {
  TestNode seq1[] = {{T_STR, 0, "Mark McGwire", NULL, 0},
                             {T_STR, 0, "Sammy Sosa", NULL, 0},
                             {T_STR, 0, "Ken Griffey", NULL, 0},
                             end_node};
  TestNode seq2[] = {{T_STR, 0, "Chicago Cubs", NULL, 0},
                             {T_STR, 0, "St Louis Cardinals", NULL, 0},
                             end_node};
  TestNode stream[] = {
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
 * YTS: 
 */
static void YtsSpecificationExamples_2_8(CuTest *tc) {
  TestNode map1[] = {{T_STR, 0, "time", NULL, 0},
                             {T_STR, 0, "20:03:20", NULL, 0},
                             {T_STR, 0, "player", NULL, 0},
                             {T_STR, 0, "Sammy Sosa", NULL, 0},
                             {T_STR, 0, "action", NULL, 0},
                             {T_STR, 0, "strike (miss)", NULL, 0},
                             end_node};
  TestNode map2[] = {{T_STR, 0, "time", NULL, 0},
                             {T_STR, 0, "20:03:47", NULL, 0},
                             {T_STR, 0, "player", NULL, 0},
                             {T_STR, 0, "Sammy Sosa", NULL, 0},
                             {T_STR, 0, "action", NULL, 0},
                             {T_STR, 0, "grand slam", NULL, 0},
                             end_node};
  TestNode stream[] = {
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
static void YtsSpecificationExamples_2_9(CuTest *tc) {
  TestNode seq1[] = {{T_STR, 0, "Mark McGwire", NULL, 0},
                             {T_STR, 0, "Sammy Sosa", NULL, 0},
                             end_node};
  TestNode seq2[] = {{T_STR, 0, "Sammy Sosa", NULL, 0},
                             {T_STR, 0, "Ken Griffey", NULL, 0},
                             end_node};
  TestNode map[] = {{T_STR, 0, "hr", NULL, 0},
                            {T_SEQ, 0, 0, seq1, 0},
                            {T_STR, 0, "rbi", NULL, 0},
                            {T_SEQ, 0, 0, seq2, 0},
                            end_node};
  TestNode stream[] = {{T_MAP, 0, 0, map, 0}, end_node};

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
static void YtsSpecificationExamples_2_10(CuTest *tc) {
  TestNode seq1[] = {{T_STR, 0, "Mark McGwire", NULL, 0},
                             {T_STR, 0, "Sammy Sosa", NULL, 0},
                             end_node};
  TestNode seq2[] = {{T_STR, 0, "Sammy Sosa", NULL, 0},
                             {T_STR, 0, "Ken Griffey", NULL, 0},
                             end_node};
  TestNode map[] = {{T_STR, 0, "hr", NULL, 0},
                            {T_SEQ, 0, 0, seq1, 0},
                            {T_STR, 0, "rbi", NULL, 0},
                            {T_SEQ, 0, 0, seq2, 0},
                            end_node};
  TestNode stream[] = {{T_MAP, 0, 0, map, 0}, end_node};

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
 * YTS: M5DY
 */
static void YtsSpecificationExamples_2_11(CuTest *tc) {
  TestNode seq1[] = {{T_STR, 0, "New York Yankees", NULL, 0},
                             {T_STR, 0, "Atlanta Braves", NULL, 0},
                             end_node};
  TestNode seq2[] = {{T_STR, 0, "2001-07-02", NULL, 0},
                             {T_STR, 0, "2001-08-12", NULL, 0},
                             {T_STR, 0, "2001-08-14", NULL, 0},
                             end_node};
  TestNode seq3[] = {{T_STR, 0, "Detroit Tigers", NULL, 0},
                             {T_STR, 0, "Chicago Cubs", NULL, 0},
                             end_node};
  TestNode seq4[] = {{T_STR, 0, "2001-07-23", NULL, 0}, end_node};
  TestNode map[] = {{T_SEQ, 0, 0, seq3, 0},
                            {T_SEQ, 0, 0, seq4, 0},
                            {T_SEQ, 0, 0, seq1, 0},
                            {T_SEQ, 0, 0, seq2, 0},
                            end_node};
  TestNode stream[] = {{T_MAP, 0, 0, map, 0}, end_node};

  CuStreamCompare(tc,

                  /* YAML document */
                  "? - Detroit Tigers \n"
                  "  - Chicago Cubs \n"
                  ": - 2001-07-23 \n"
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
 * Example 2.12: Compact Nested Mapping (was: Sequence key shortcut)
 * YTS: 9U5K
 */
static void YtsSpecificationExamples_2_12(CuTest *tc) {
  TestNode map1[] = {{T_STR, 0, "item", NULL, 0},
                             {T_STR, 0, "Super Hoop", NULL, 0},
                             {T_STR, 0, "quantity", NULL, 0},
                             {T_STR, 0, "1", NULL, 0},
                             end_node};
  TestNode map2[] = {{T_STR, 0, "item", NULL, 0},
                             {T_STR, 0, "Basketball", NULL, 0},
                             {T_STR, 0, "quantity", NULL, 0},
                             {T_STR, 0, "4", NULL, 0},
                             end_node};
  TestNode map3[] = {{T_STR, 0, "item", NULL, 0},
                             {T_STR, 0, "Big Shoes", NULL, 0},
                             {T_STR, 0, "quantity", NULL, 0},
                             {T_STR, 0, "1", NULL, 0},
                             end_node};
  TestNode seq[] = {
    {T_MAP, 0, 0, map1, 0}, {T_MAP, 0, 0, map2, 0}, {T_MAP, 0, 0, map3, 0}, end_node};
  TestNode stream[] = {{T_SEQ, 0, 0, seq, 0}, end_node};

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
 * YTS: 6JQW
 */
static void YtsSpecificationExamples_2_13(CuTest *tc) {
  TestNode stream[] = {{T_STR, 0, "\\//||\\/||\n// ||  ||__\n", NULL, 0},
                               end_node};

  CuStreamCompare(tc,

                  /* YAML document */
                  "# ASCII Art\n"
                  "--- | \n"
                  "  \\//||\\/||\n"
                  "  // ||  ||__\n",

                  /* C structure of validations */
                  stream);

  CuRoundTrip(tc, stream);
}
/*
 * Example 2.14: Folded treats newlines as a space
 * YTS: FP8R
 */
static void YtsSpecificationExamples_2_14(CuTest *tc) {
  TestNode stream[] = {
      {T_STR, 0, "Mark McGwire's year was crippled by a knee injury.\n", NULL, 0},
      end_node};

  CuStreamCompare(tc,

                  /* YAML document */
                  "--- >\n"
                  "  Mark McGwire's\n"
                  "  year was crippled\n"
                  "  by a knee injury.\n",

                  /* C structure of validations */
                  stream);

  CuRoundTrip(tc, stream);
}
/*
 * Example 2.15: Newlines preserved for indented and blank lines
 * YTS: 6VJK
 */
static void YtsSpecificationExamples_2_15(CuTest *tc) {
  TestNode stream[] = {
      {T_STR, 0,
       "Sammy Sosa completed another fine season with great stats.\n\n  63 "
       "Home Runs\n  0.288 Batting Average\n\nWhat a year!\n",
       NULL, 0},
      end_node};

  CuStreamCompare(tc,

                  /* YAML document */
                  "> \n"
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
 * YTS: HMK4
 */
static void YtsSpecificationExamples_2_16(CuTest *tc) {
  TestNode map[] = {
      {T_STR, 0, "name", NULL, 0},
      {T_STR, 0, "Mark McGwire", NULL, 0},
      {T_STR, 0, "accomplishment", NULL, 0},
      {T_STR, 0, "Mark set a major league home run record in 1998.\n", NULL, 0},
      {T_STR, 0, "stats", NULL, 0},
      {T_STR, 0, "65 Home Runs\n0.278 Batting Average\n", NULL, 0},
      end_node};
  TestNode stream[] = {{T_MAP, 0, 0, map, 0}, end_node};

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
 * Example 2.17: Quoted Scalars
 * YTS: G4RS (fix quoting)
 */
static void YtsSpecificationExamples_2_17_todo(CuTest *tc) {
  TestNode map[] = {
      {T_STR, 0, "unicode", NULL, 0},
      {T_STR, 0, "Sosa did fine.\\u263A", NULL, 0},
#ifdef DEBUG_QUOTE
      {T_STR, 0, "control", NULL, 0},
      {T_STR, 0, "\"\\b1998\\t1999\\t2000\\n\"", NULL, 0},
      {T_STR, 0, "hex esc", NULL, 0},
      {T_STR, 0, "\"\\r\\n is \\r\\n\"", NULL, 0},
      {T_STR, 0, "single", NULL, 0},
      {T_STR, 0, "'\"Howdy!\" he cried.'", NULL, 0},
      {T_STR, 0, "quoted", NULL, 0},
      {T_STR, 0, " # Not a ''comment''.", NULL, 0},
#endif
      {T_STR, 0, "tie-fighter", NULL, 0},
      {T_STR, 0, "|\\-*-/|", NULL, 0},
      end_node};
  TestNode stream[] = {{T_MAP, 0, 0, map, 0}, end_node};

  CuStreamCompare(tc,

                  /* YAML document */
                  "unicode: \"Sosa did fine.\\u263A\"\n"
#ifdef DEBUG_QUOTE
                  "control: \"\\b1998\\t1999\\t2000\\n\"\n"
                  "hex esc: \"\x0d\x0a is \\r\\n\"\n"
                  "\n"
                  "single: '\"Howdy!\" he cried.'\n"
                  "quoted: ' # Not a ''comment''.'\n"
#endif
                  "tie-fighter: '|\\-*-/|'\n",

                  /* C structure of validations */
                  stream);

  CuRoundTrip(tc, stream);
}
/*
 * Example 2.18: Multiline flow scalars
 * YTS: 4CQQ
 */
static void YtsSpecificationExamples_2_18(CuTest *tc) {
  TestNode map[] = {
      {T_STR, 0, "plain", NULL, 0},
      {T_STR, 0, "This unquoted scalar spans many lines.", NULL, 0},
      {T_STR, 0, "quoted", NULL, 0},
      {T_STR, 0, "So does this quoted scalar.\n", NULL, 0},
      end_node};
  TestNode stream[] = {{T_MAP, 0, 0, map, 0}, end_node};

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
 * YTS: none
 */
static void YtsSpecificationExamples_2_19(CuTest *tc) {
  TestNode map[] = {{T_STR, 0, "canonical", NULL, 0},
                            {T_STR, 0, "12345", NULL, 0},
                            {T_STR, 0, "decimal", NULL, 0},
                            {T_STR, 0, "+12,345", NULL, 0},
                            {T_STR, 0, "sexagecimal", NULL, 0},
                            {T_STR, 0, "3:25:45", NULL, 0},
                            {T_STR, 0, "octal", NULL, 0},
                            {T_STR, 0, "0o14", NULL, 0},
                            {T_STR, 0, "octal_old", NULL, 0},
                            {T_STR, 0, "014", NULL, 0},
                            {T_STR, 0, "hexadecimal", NULL, 0},
                            {T_STR, 0, "0xC", NULL, 0},
                            end_node};
  TestNode stream[] = {{T_MAP, 0, 0, map, 0}, end_node};

  CuStreamCompare(tc,

                  /* YAML document */
                  "canonical: 12345 \n"
                  "decimal: +12,345 \n"
                  "sexagecimal: 3:25:45\n"
                  "octal: 0o14 \n"
                  "octal_old: 014 \n"
                  "hexadecimal: 0xC \n",

                  /* C structure of validations */
                  stream);

  CuRoundTrip(tc, stream);
}
/*
 * Example 2.20: Floating point
 * YTS: none
 */
static void YtsSpecificationExamples_2_20(CuTest *tc) {
  TestNode map[] = {{T_STR, 0, "canonical", NULL, 0},
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
  TestNode stream[] = {{T_MAP, 0, 0, map, 0}, end_node};

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
 * YTS: none
 */
static void YtsSpecificationExamples_2_21(CuTest *tc) {
  TestNode seq[] = {{T_STR, 0, "true", NULL, 0},
                            {T_STR, 0, "false", NULL, 0}, end_node};
  TestNode map[] = {{T_STR, 0, "null", NULL, 0},
                            {T_STR, 0, "~", NULL, 0},
                            {T_STR, 0, "null", NULL, 0},
                            {T_STR, 0, "", NULL, 0},
                            {T_STR, 0, "booleans", NULL, 0},
                            {T_SEQ, 0, 0, seq, 0},
                            {T_STR, 0, "true", NULL, 0},
                            {T_STR, 0, "y", NULL, 0},
                            {T_STR, 0, "false", NULL, 0},
                            {T_STR, 0, "n", NULL, 0},
                            {T_STR, 0, "string", NULL, 0},
                            {T_STR, 0, "12345", NULL, 0},
                            end_node};
  TestNode stream[] = {{T_MAP, 0, 0, map, 0}, end_node};

  CuStreamCompare(tc,

                  /* YAML document */
                  "null: ~\n"
                  "null:\n"
                  "booleans: [ true, false ]\n"
                  "true: y\n"
                  "false: n\n"
                  "string: '12345'\n",

                  /* C structure of validations */
                  stream);

  CuRoundTrip(tc, stream);
}
/*
 * Example 2.22: Timestamps
 * YTS: none
 */
static void YtsSpecificationExamples_2_22(CuTest *tc) {
  TestNode map[] = {{T_STR, 0, "canonical", NULL, 0},
                            {T_STR, 0, "2001-12-15T02:59:43.1Z", NULL, 0},
                            {T_STR, 0, "iso8601", NULL, 0},
                            {T_STR, 0, "2001-12-14t21:59:43.10-05:00", NULL, 0},
                            {T_STR, 0, "spaced", NULL, 0},
                            {T_STR, 0, "2001-12-14 21:59:43.10 -5", NULL, 0},
                            {T_STR, 0, "date", NULL, 0},
                            {T_STR, 0, "2002-12-14", NULL, 0},
                            end_node};
  TestNode stream[] = {{T_MAP, 0, 0, map, 0}, end_node};

  CuStreamCompare(tc,

                  /* YAML document */
                  "canonical: 2001-12-15T02:59:43.1Z\n"
                  "iso8601:  2001-12-14t21:59:43.10-05:00\n"
                  "spaced:  2001-12-14 21:59:43.10 -5\n"
                  "date:   2002-12-14 # Time is noon UTC\n",

                  /* C structure of validations */
                  stream);

  CuRoundTrip(tc, stream);
}

/*
 * Example 2.23: Various explicit tags (old style)
 * YTS: none
 */
static void YtsSpecificationExamples_2_23_old(CuTest *tc) {
  TestNode map[] = {
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
  TestNode stream[] = {{T_MAP, 0, 0, map, 0}, end_node};

  CuStreamCompare(tc,

                  /* YAML document */
                  "not-date: !str 2002-04-28\n"
                  "\n"
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
 * YTS: C4HZ (almost)
 */
static void YtsSpecificationExamples_2_24_old(CuTest *tc) {
  TestNode point1[] = {{T_STR, 0, "x", NULL, 0},
                               {T_STR, 0, "73", NULL, 0},
                               {T_STR, 0, "y", NULL, 0},
                               {T_STR, 0, "129", NULL, 0},
                               end_node};
  TestNode point2[] = {{T_STR, 0, "x", NULL, 0},
                               {T_STR, 0, "89", NULL, 0},
                               {T_STR, 0, "y", NULL, 0},
                               {T_STR, 0, "102", NULL, 0},
                               end_node};
  TestNode map1[] = {{T_STR, 0, "center", NULL, 0},
                             {T_MAP, 0, 0, point1, 0},
                             {T_STR, 0, "radius", NULL, 0},
                             {T_STR, 0, "7", NULL, 0},
                             end_node};
  TestNode map2[] = {{T_STR, 0, "start", NULL, 0},
                             {T_MAP, 0, 0, point1, 0},
                             {T_STR, 0, "finish", NULL, 0},
                             {T_MAP, 0, 0, point2, 0},
                             end_node};
  TestNode map3[] = {{T_STR, 0, "start", NULL, 0},
                             {T_MAP, 0, 0, point1, 0},
                             {T_STR, 0, "color", NULL, 0},
                             {T_STR, 0, "0xFFEEBB", NULL, 0},
                             {T_STR, 0, "value", NULL, 0},
                             {T_STR, 0, "Pretty vector drawing.", NULL, 0},
                             end_node};
  TestNode seq[] = {
      {T_MAP, "tag:clarkevans.com,2002:graph/circle", 0, map1, 0},
      {T_MAP, "tag:clarkevans.com,2002:graph/line", 0, map2, 0},
      {T_MAP, "tag:clarkevans.com,2002:graph/label", 0, map3, 0},
      end_node};
  TestNode stream[] = {
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
#ifdef DEBUG_SET
/*
 * Example 2.25: Unordered Sets
 * YTS: 2XXW (almost)
 * FIXME parse
 */
static void YtsSpecificationExamples_2_25_old(CuTest *tc) {
  TestNode map1[] = {
      {T_STR, 0, "Mark McGwire", NULL, 0}, {T_STR, 0, "~", NULL, 0}, end_node};
  TestNode map2[] = {
      {T_STR, 0, "Sammy Sosa", NULL, 0}, {T_STR, 0, "~", NULL, 0}, end_node};
  TestNode map3[] = {
      {T_STR, 0, "Ken Griffy", NULL, 0}, {T_STR, 0, "~", NULL, 0}, end_node};
  TestNode seq[] = {
      {T_MAP, 0, 0, map1, 0}, {T_MAP, 0, 0, map2, 0}, {T_MAP, 0, 0, map3, 0}, end_node};
  TestNode stream[] = {{T_SEQ, "tag:yaml.org,2002:set", 0, seq, 0},
                               end_node};

  CuStreamCompare(tc,

                  /* YAML document */
                  "# Sets are represented as a\n"
                  "# Mapping where each key is\n"
                  "# associated with a null value\n"
                  "--- !set\n"
                  "? Mark McGwire\n"
                  "? Sammy Sosa\n"
                  "? Ken Griffy\n",

                  /* C structure of validations */
                  stream);

  CuRoundTrip(tc, stream);
}
#endif
/*
 * Example 2.26: Ordered mappings
 * YTS: J7PZ (almost)
 */
static void YtsSpecificationExamples_2_26_old(CuTest *tc) {
  TestNode map1[] = {
      {T_STR, 0, "Mark McGwire", NULL, 0}, {T_STR, 0, "65", NULL, 0}, end_node};
  TestNode map2[] = {
      {T_STR, 0, "Sammy Sosa", NULL, 0}, {T_STR, 0, "63", NULL, 0}, end_node};
  TestNode map3[] = {
      {T_STR, 0, "Ken Griffy", NULL, 0}, {T_STR, 0, "58", NULL, 0}, end_node};
  TestNode seq[] = {
      {T_MAP, 0, 0, map1, 0}, {T_MAP, 0, 0, map2, 0}, {T_MAP, 0, 0, map3, 0}, end_node};
  TestNode stream[] = {{T_SEQ, "tag:yaml.org,2002:omap", 0, seq, 0},
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
 * YTS: UGM3 (almost)
 */
static void YtsSpecificationExamples_2_27_old(CuTest *tc) {
  TestNode prod1[] = {{T_STR, 0, "sku", NULL, 0},
                              {T_STR, 0, "BL394D", NULL, 0},
                              {T_STR, 0, "quantity", NULL, 0},
                              {T_STR, 0, "4", NULL, 0},
                              {T_STR, 0, "description", NULL, 0},
                              {T_STR, 0, "Basketball", NULL, 0},
                              {T_STR, 0, "price", NULL, 0},
                              {T_STR, 0, "450.00", NULL, 0},
                              end_node};
  TestNode prod2[] = {{T_STR, 0, "sku", NULL, 0},
                              {T_STR, 0, "BL4438H", NULL, 0},
                              {T_STR, 0, "quantity", NULL, 0},
                              {T_STR, 0, "1", NULL, 0},
                              {T_STR, 0, "description", NULL, 0},
                              {T_STR, 0, "Super Hoop", NULL, 0},
                              {T_STR, 0, "price", NULL, 0},
                              {T_STR, 0, "2392.00", NULL, 0},
                              end_node};
  TestNode products[] = {
      {T_MAP, 0, 0, prod1, 0}, {T_MAP, 0, 0, prod2, 0}, end_node};
  TestNode address[] = {
      {T_STR, 0, "lines", NULL, 0},
      {T_STR, 0, "458 Walkman Dr.\nSuite #292\n", NULL, 0},
      {T_STR, 0, "city", NULL, 0},
      {T_STR, 0, "Royal Oak", NULL, 0},
      {T_STR, 0, "state", NULL, 0},
      {T_STR, 0, "MI", NULL, 0},
      {T_STR, 0, "postal", NULL, 0},
      {T_STR, 0, "48046", NULL, 0},
      end_node};
  TestNode id001[] = {{T_STR, 0, "given", NULL, 0},
                              {T_STR, 0, "Chris", NULL, 0},
                              {T_STR, 0, "family", NULL, 0},
                              {T_STR, 0, "Dumars", NULL, 0},
                              {T_STR, 0, "address", NULL, 0},
                              {T_MAP, 0, 0, address, 0},
                              end_node};
  TestNode map[] = {
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
  TestNode stream[] = {
      {T_MAP, "tag:clarkevans.com,2002:invoice", 0, map, 0}, end_node};

  CuStreamCompare(tc,

                  /* YAML document */
                  // FIXME
                 //--- !<tag:clarkevans.com,2002:invoice>
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
 * YTS: RZT7
 */
static void YtsSpecificationExamples_2_28(CuTest *tc) {
  TestNode map1[] = {
      {T_STR, 0, "Time", NULL, 0},
      {T_STR, 0, "2001-11-23 15:01:42 -5", NULL, 0},
      {T_STR, 0, "User", NULL, 0},
      {T_STR, 0, "ed", NULL, 0},
      {T_STR, 0, "Warning", NULL, 0},
      {T_STR, 0, "This is an error message for the log file\n", NULL, 0},
      end_node};
  TestNode map2[] = {
      {T_STR, 0, "Time", NULL, 0},
      {T_STR, 0, "2001-11-23 15:02:31 -5", NULL, 0},
      {T_STR, 0, "User", NULL, 0},
      {T_STR, 0, "ed", NULL, 0},
      {T_STR, 0, "Warning", NULL, 0},
      {T_STR, 0, "A slightly different error message.\n", NULL, 0},
      end_node};
  TestNode file1[] = {{T_STR, 0, "file", NULL, 0},
                              {T_STR, 0, "TopClass.py", NULL, 0},
                              {T_STR, 0, "line", NULL, 0},
                              {T_STR, 0, "23", NULL, 0},
                              {T_STR, 0, "code", NULL, 0},
                              {T_STR, 0, "x = MoreObject(\"345\\n\")\n", NULL, 0},
                              end_node};
  TestNode file2[] = {{T_STR, 0, "file", NULL, 0},
                              {T_STR, 0, "MoreClass.py", NULL, 0},
                              {T_STR, 0, "line", NULL, 0},
                              {T_STR, 0, "58", NULL, 0},
                              {T_STR, 0, "code", NULL, 0},
                              {T_STR, 0, "foo = bar", NULL, 0},
                              end_node};
  TestNode stack[] = {
      {T_MAP, 0, 0, file1, 0}, {T_MAP, 0, 0, file2, 0}, end_node};
  TestNode map3[] = {{T_STR, 0, "Date", NULL, 0},
                             {T_STR, 0, "2001-11-23 15:03:17 -5", NULL, 0},
                             {T_STR, 0, "User", NULL, 0},
                             {T_STR, 0, "ed", NULL, 0},
                             {T_STR, 0, "Fatal", NULL, 0},
                             {T_STR, 0, "Unknown variable \"bar\"\n", NULL, 0},
                             {T_STR, 0, "Stack", NULL, 0},
                             {T_SEQ, 0, 0, stack, 0},
                             end_node};
  TestNode stream[] = {
      {T_MAP, 0, 0, map1, 0}, {T_MAP, 0, 0, map2, 0}, {T_MAP, 0, 0, map3, 0}, end_node};

  CuStreamCompare(tc,

                  /* YAML document */
                  "---\n"
                  "Time: 2001-11-23 15:01:42 -5\n"
                  "User: ed\n"
                  "Warning: >\n"
                  "  This is an error message\n"
                  "  for the log file\n"
                  "---\n"
                  "Time: 2001-11-23 15:02:31 -5\n"
                  "User: ed\n"
                  "Warning: >\n"
                  "  A slightly different error\n"
                  "  message.\n"
                  "---\n"
                  "Date: 2001-11-23 15:03:17 -5\n"
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
/* TODO
 * Example 5.1 Byte Order Mark
 * YTS: none
 */
static void YtsSpecificationExamples_5_1_todo(CuTest *tc) {
#ifdef DEBUG_BOM
  TestNode stream[] = {
    {T_STR, 0, "", NULL, 0},
    end_node};
  CuStreamCompare(tc, /* UTF-8 YAML document */
                  "﻿\xef\xbb\xbf# Comment only.\n", stream);
  CuStreamCompare(tc, /* UTF-16LE YAML document */
                  "﻿\xff\xfe#\x00", stream);
  CuStreamCompare(tc, /* UTF-16BE YAML document */
                  "﻿\xfe\xff\x00#", stream);
  CuStreamCompare(tc, /* UTF-32BE YAML document */
                  "﻿\x00\x00\xfe\xff\x00\x00\x00#", stream);
  CuStreamCompare(tc, /* UTF-32LE YAML document */
                  "﻿\xff\xfe\x00\x00#\x00\x00\x00", stream);
  CuRoundTrip(tc, stream);
#endif
}
/* TODO
 * Example 5.2 Invalid Byte Order Mark
 * YTS: none
 */
static void YtsSpecificationExamples_5_2_todo(CuTest *tc) {
#ifdef DEBUG_BOM
  TestNode stream[] = {end_node};
  CuStreamCompare(tc, /* UTF-8 YAML document */
                  "﻿- invalid use of BOM\n\xef\xbb\xbf# Comment only.\n- inside doc", stream);
  CuStreamCompare(tc, /* UTF-16LE YAML document */
                  "﻿- invalid use of BOM\n\xff\xfe#\x00\n- inside doc", stream);
  CuStreamCompare(tc, /* UTF-16BE YAML document */
                  "﻿- invalid use of BOM\n\xfe\xff\x00#\n- inside doc", stream);
  CuStreamCompare(tc, /* UTF-32BE YAML document */
                  "﻿- invalid use of BOM\n\x00\x00\xfe\xff\x00\x00\x00#\n- inside doc", stream);
  CuStreamCompare(tc, /* UTF-32LE YAML document */
                  "﻿- invalid use of BOM\n\xff\xfe\x00\x00#\x00\x00\x00\n- inside doc", stream);
  CuRoundTrip(tc, stream);
#endif
}
/* TODO
 * Example 5.3 Block Structure Indicators
 * Example 5.4 Flow Collection Indicators
 * Example 5.5 Comment Indicator (empty doc, only comments)
 * Example 5.6 Node Property Indicators
 * Example 5.7 Block Scalar Indicators
 * Example 5.8 Quoted Scalar Indicators
 * Example 5.9 Directive Indicator
 * Example 5.10 Invalid use of Reserved Indicators
 * Example 5.11 Line Break Characters
 * Example 5.12 Tabs and Spaces
 * Example 5.13 Escaped Characters
 * Example 5.14 Invalid Escaped Characters
 * Example 6.1 Indentation Spaces
 * Example 6.2 Indentation Indicators
 * Example 6.3 Separation Spaces
 * Example 6.4 Line Prefixes
 * Example 6.5 Empty Lines
 * Example 6.6 Line Folding
 * Example 6.7 Block Folding
 * Example 6.8 Flow Folding
 * Example 6.9 Separated Comment
 */

/*
 * Example 6.11 Multi-Line Comments (extended)
 * YTS: none
 */
static void YtsSpecificationExamples_6_11(CuTest *tc) {
  TestNode map[] = {
      {T_STR, 0, "this", NULL, 0},
      {T_STR, 0,
       "contains three lines of text.\nThe third one starts with a\n# "
       "character. This isn't a comment.\n",
       NULL, 0},
      end_node};
  TestNode stream[] = {{T_MAP, 0, 0, map, 0}, end_node};

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

/* TODO
 * Example 6.12 Separation Spaces
 * Example 6.13 Reserved Directives
 *    Should be ignored with a warning
 * Example 6.14 “YAML” directive
 * Example 6.15 Invalid Repeated YAML directive
 *    Must error
 * Example 6.16 “TAG” directive
 * Example 6.17 Invalid Repeated TAG directive
 *    Must error
 * Example 6.18 Primary Tag Handle
 * Example 6.19 Secondary Tag Handle
 * Example 6.20 Tag Handles
 * Example 6.21 Local Tag Prefix
 * Example 6.22 Global Tag Prefix
 * Example 6.23 Node Properties
 * Example 6.24 Verbatim Tags
 * Example 6.25 Invalid Verbatim Tags
 * Example 6.26 Tag Shorthands
 * Example 6.27 Invalid Tag Shorthands
 * Example 6.28 Non-Specific Tags
 * Example 6.29 Node Anchors
 * Example 7.1 Alias Nodes
 * Example 7.2 Empty Content
 * Example 7.3 Completely Empty Flow Nodes
 * Example 7.4 Double Quoted Implicit Keys
 * Example 7.5 Double Quoted Line Breaks
 * Example 7.6 Double Quoted Lines
 * Example 7.7 Single Quoted Characters
 * Example 7.8 Single Quoted Implicit Keys
 * Example 7.9 Single Quoted Lines
 * Example 7.10 Plain Characters
 * Example 7.11 Plain Implicit Keys
 * Example 7.12 Plain Lines
 * Example 7.13 Flow Sequence
 * Example 7.14 Flow Sequence Entries
 * Example 7.15 Flow Mappings
 * Example 7.16 Flow Mapping Entries
 * Example 7.17 Flow Mapping Separate Values
 * Example 7.18 Flow Mapping Adjacent Values
 * Example 7.19 Single Pair Flow Mappings
 * Example 7.20 Single Pair Explicit Entry
 * Example 7.21 Single Pair Implicit Entries
 * Example 7.22 Invalid Implicit Keys
 * Example 7.23 Flow Content
 * Example 7.24 Flow Nodes
* Example 8.1 Block Scalar Header
* Example 8.2 Block Indentation Indicator
* Example 8.3 Invalid Block Scalar Indentation Indicators
* Example 8.4 Chomping Final Line Break
* Example 8.5 Chomping Trailing Lines
* Example 8.6 Empty Scalar Chomping
* Example 8.7 Literal Scalar
* Example 8.8 Literal Content
* Example 8.9 Folded Scalar
* Example 8.10 Folded Lines
* Example 8.11 More Indented Lines
* Example 8.12 Empty Separation Lines
* Example 8.13 Final Empty Lines
* Example 8.14 Block Sequence
* Example 8.15 Block Sequence Entry Types
* Example 8.16 Block Mappings
* Example 8.17 Explicit Block Mapping Entries
* Example 8.18 Implicit Block Mapping Entries
* Example 8.19 Compact Block Mappings
* Example 8.20 Block Node Types
* Example 8.21 Block Scalar Nodes
* Example 8.22 Block Collection Nodes
* Example 9.1 Document Prefix
* Example 9.2 Document Markers
* Example 9.3 Bare Documents
* Example 9.4 Explicit Documents
* Example 9.5 Directives Documents
* Example 9.6 Stream
* Example 10.1 !!map Examples
* Example 10.2 !!seq Examples
* Example 10.3 !!str Examples
* Example 10.4 !!null Examples
* Example 10.5 !!bool Examples
* Example 10.6 !!int Examples
* Example 10.7 !!float Examples
* Example 10.8 JSON Tag Resolution
* Example 10.9 Core Tag Resolution
*/

/*
 * Example : Zero indented block scalar (was: Document with a single value)
 * YTS: FP8R
 */
static void YtsSpecification_FP8R(CuTest *tc) {
  TestNode stream[] = {
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
  TestNode map1[] = {{T_STR, 0, "at", NULL, 0},
                             {T_STR, 0, "2001-08-12 09:25:00.00 Z", NULL, 0},
                             {T_STR, 0, "type", NULL, 0},
                             {T_STR, 0, "GET", NULL, 0},
                             {T_STR, 0, "HTTP", NULL, 0},
                             {T_STR, 0, "1.0", NULL, 0},
                             {T_STR, 0, "url", NULL, 0},
                             {T_STR, 0, "/index.html", NULL, 0},
                             end_node};
  TestNode map2[] = {{T_STR, 0, "at", NULL, 0},
                             {T_STR, 0, "2001-08-12 09:25:10.00 Z", NULL, 0},
                             {T_STR, 0, "type", NULL, 0},
                             {T_STR, 0, "GET", NULL, 0},
                             {T_STR, 0, "HTTP", NULL, 0},
                             {T_STR, 0, "1.0", NULL, 0},
                             {T_STR, 0, "url", NULL, 0},
                             {T_STR, 0, "/toc.html", NULL, 0},
                             end_node};
  TestNode stream[] = {
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
  TestNode map[] = {{T_STR, 0, "invoice", NULL, 0},
                            {T_STR, 0, "34843", NULL, 0},
                            {T_STR, 0, "date", NULL, 0},
                            {T_STR, 0, "2001-01-23", NULL, 0},
                            {T_STR, 0, "total", NULL, 0},
                            {T_STR, 0, "4443.52", NULL, 0},
                            end_node};
  TestNode stream[] = {{T_MAP, 0, 0, map, 0}, end_node};

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
  TestNode map[] = {end_node};
  TestNode seq[] = {end_node};
  TestNode stream[] = {
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
  TestNode map1[] = {{T_STR, 0, "sent at", NULL, 0},
                             {T_STR, 0, "2002-06-06 11:46:25.10 Z", NULL, 0},
                             {T_STR, 0, "payload", NULL, 0},
                             {T_STR, 0, "Whatever", NULL, 0},
                             end_node};
  TestNode map2[] = {{T_STR, 0, "sent at", NULL, 0},
                             {T_STR, 0, "2002-06-06 12:05:53.47 Z", NULL, 0},
                             {T_STR, 0, "payload", NULL, 0},
                             {T_STR, 0, "Whatever", NULL, 0},
                             end_node};
  TestNode stream[] = {
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
  TestNode map[] = {{T_STR, 0, "integer", NULL, 0},
                            {T_STR, "tag:yaml.org,2002:int", "12", NULL, 0},
                            {T_STR, 0, "also int", NULL, 0},
                            {T_STR, "tag:yaml.org,2002:int", "12", NULL, 0},
                            {T_STR, 0, "string", NULL, 0},
                            {T_STR, "tag:yaml.org,2002:str", "12", NULL, 0},
                            end_node};
  TestNode stream[] = {{T_MAP, 0, 0, map, 0}, end_node};

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
  TestNode pool[] = {{T_STR, 0, "number", NULL, 0},
                             {T_STR, 0, "8", NULL, 0},
                             {T_STR, 0, "color", NULL, 0},
                             {T_STR, 0, "black", NULL, 0},
                             end_node};
  TestNode map1[] = {
      {T_STR, 0, "pool", NULL, 0}, {T_MAP, "x-private:ball", 0, pool, 0}, end_node};
  TestNode bearing[] = {
      {T_STR, 0, "material", NULL, 0}, {T_STR, 0, "steel", NULL, 0}, end_node};
  TestNode map2[] = {{T_STR, 0, "bearing", NULL, 0},
                             {T_MAP, "x-private:ball", 0, bearing, 0},
                             end_node};
  TestNode stream[] = {
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
  TestNode seq[] = {
      {T_STR, "tag:yaml.org,2002:str", "a Unicode string", NULL, 0}, end_node};
  TestNode stream[] = {{T_SEQ, 0, 0, seq, 0}, end_node};

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
  TestNode map[] = {end_node};
  TestNode seq[] = {
      {T_MAP, "tag:perl.yaml.org,2002:Text::Tabs", 0, map, 0}, end_node};
  TestNode stream[] = {{T_SEQ, 0, 0, seq, 0}, end_node};

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
  TestNode map[] = {end_node};
  TestNode seq[] = {
      {T_MAP, "tag:clarkevans.com,2003-02:timesheet", 0, map, 0}, end_node};
  TestNode stream[] = {{T_SEQ, 0, 0, seq, 0}, end_node};

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
  TestNode same[] = {
      {T_STR, "tag:domain.tld,2002:type0", "value", NULL, 0},
      {T_STR, "tag:domain.tld,2002:type0", "value", NULL, 0},
      end_node};
  TestNode diff[] = {
      {T_STR, "tag:domain.tld,2002:type%30", "value", NULL, 0},
      {T_STR, "tag:domain.tld,2002:type0", "value", NULL, 0},
      end_node};
  TestNode map[] = {{T_STR, 0, "same", NULL, 0},
                            {T_SEQ, 0, 0, same, 0},
                            {T_STR, 0, "different", NULL, 0},
                            {T_SEQ, 0, 0, diff, 0},
                            end_node};
  TestNode stream[] = {{T_MAP, 0, 0, map, 0}, end_node};

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
  TestNode map[] = {
      {T_STR, 0, "anchor", NULL, 0},
      {T_STR, 0, "This scalar has an anchor.", NULL, 0},
      {T_STR, 0, "override", NULL, 0},
      {T_STR, 0, "The alias node below is a repeated use of this value.\n",
       NULL, 0},
      {T_STR, 0, "alias", NULL, 0},
      {T_STR, 0, "The alias node below is a repeated use of this value.\n",
       NULL, 0},
      end_node};
  TestNode stream[] = {{T_MAP, 0, 0, map, 0}, end_node};

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
  TestNode empty[] = {end_node};
  TestNode flow[] = {
      {T_STR, 0, "one", NULL, 0},   {T_STR, 0, "two", NULL, 0},
      {T_STR, 0, "three", NULL, 0}, {T_STR, 0, "four", NULL, 0},
      {T_STR, 0, "five", NULL, 0},  end_node};
  TestNode inblock[] = {{T_STR, 0, "Subordinate sequence entry", NULL, 0},
                                end_node};
  TestNode block[] = {{T_STR, 0, "First item in top sequence", NULL, 0},
                              {T_SEQ, 0, 0, inblock, 0},
                              {T_STR, 0, "A folded sequence entry\n", NULL, 0},
                              {T_STR, 0, "Sixth item in top sequence", NULL, 0},
                              end_node};
  TestNode map[] = {{T_STR, 0, "empty", NULL, 0},
                            {T_SEQ, 0, 0, empty, 0},
                            {T_STR, 0, "flow", NULL, 0},
                            {T_SEQ, 0, 0, flow, 0},
                            {T_STR, 0, "block", NULL, 0},
                            {T_SEQ, 0, 0, block, 0},
                            end_node};
  TestNode stream[] = {{T_MAP, 0, 0, map, 0}, end_node};

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
  TestNode map[] = {
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
  TestNode stream[] = {{T_MAP, 0, 0, map, 0}, end_node};

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

#ifdef DEBUG_NEW
/*
 * Example 6.12 Separation Spaces
 */
static void YtsSpecificationExamples_6_12(CuTest *tc) {
  TestNode map1[] = {
       {T_MAP, 0, 0, map1, 0},
       {T_STR, 0, "first", NULL, 0},{T_STR, 0, "Sammy", NULL, 0},
       {T_STR, 0, "last", NULL, 0},{T_STR, 0, "Sosa", NULL, 0},
       end_node};
  TestNode map2[] = {
      {T_STR, 0, "hr", NULL, 0},{T_STR, 0, "65", NULL, 0},
      {T_STR, 0, "avg", NULL, 0},{T_STR, 0, "0.270", NULL, 0},
      end_node};
  TestNode stream[] = {
      {T_MAP, 0, 0, map1, 0}, {T_MAP, 0, 0, map2, 0}, end_node};

  CuStreamCompare(tc,

                  /* YAML document */
                  "{ first: Sammy, last: Sosa }:\n"
                  "# Statistics:\n"
                  "  hr:  # Home runs\n"
                  "     65\n"
                  "  avg: # Average\n"
                  "   0.278\n",

                  /* C structure of validations */
                  stream);

  CuRoundTrip(tc, stream);
}
#endif

static CuSuite *SyckGetSuite(void) {
  CuSuite *suite = CuSuiteNew();
  SUITE_ADD_TEST(suite, YtsFoldedScalars_7);
  SUITE_ADD_TEST(suite, YtsNullsAndEmpties_0);
  SUITE_ADD_TEST(suite, YtsNullsAndEmpties_1);
  SUITE_ADD_TEST(suite, YtsNullsAndEmpties_2);
  SUITE_ADD_TEST(suite, YtsNullsAndEmpties_3);
  SUITE_ADD_TEST(suite, YtsNullsAndEmpties_4);
  SUITE_ADD_TEST(suite, YtsNullsAndEmpties_5);
  SUITE_ADD_TEST(suite, YtsSpecificationExamples_2_1);
  SUITE_ADD_TEST(suite, YtsSpecificationExamples_2_2);
  SUITE_ADD_TEST(suite, YtsSpecificationExamples_2_3);
  SUITE_ADD_TEST(suite, YtsSpecificationExamples_2_4);
  SUITE_ADD_TEST(suite, YtsSpecificationLegacy_A5);
  SUITE_ADD_TEST(suite, YtsSpecificationExamples_2_5);
  SUITE_ADD_TEST(suite, YtsSpecificationExamples_2_6);
  SUITE_ADD_TEST(suite, YtsSpecificationExamples_2_7);
  SUITE_ADD_TEST(suite, YtsSpecificationExamples_2_8);
  SUITE_ADD_TEST(suite, YtsSpecificationExamples_2_9);
  SUITE_ADD_TEST(suite, YtsSpecificationExamples_2_10);
  SUITE_ADD_TEST(suite, YtsSpecificationExamples_2_11);
  SUITE_ADD_TEST(suite, YtsSpecificationExamples_2_12);
  SUITE_ADD_TEST(suite, YtsSpecificationExamples_2_13);
  SUITE_ADD_TEST(suite, YtsSpecificationExamples_2_14);
  SUITE_ADD_TEST(suite, YtsSpecificationExamples_2_15);
  SUITE_ADD_TEST(suite, YtsSpecificationExamples_2_16);
  SUITE_ADD_TEST(suite, YtsSpecificationExamples_2_17_todo);
  SUITE_ADD_TEST(suite, YtsSpecificationExamples_2_18);
  SUITE_ADD_TEST(suite, YtsSpecificationExamples_2_19);
  SUITE_ADD_TEST(suite, YtsSpecificationExamples_2_20);
  SUITE_ADD_TEST(suite, YtsSpecificationExamples_2_21);
  SUITE_ADD_TEST(suite, YtsSpecificationExamples_2_22);
  SUITE_ADD_TEST(suite, YtsSpecificationExamples_2_23_old);
  SUITE_ADD_TEST(suite, YtsSpecificationExamples_2_24_old);
#ifdef DEBUG_SET
  SUITE_ADD_TEST(suite, YtsSpecificationExamples_2_25_old); // does not poarse yet
#endif
  SUITE_ADD_TEST(suite, YtsSpecificationExamples_2_26_old);
  SUITE_ADD_TEST(suite, YtsSpecificationExamples_2_27_old);
  SUITE_ADD_TEST(suite, YtsSpecificationExamples_2_28);
  SUITE_ADD_TEST(suite, YtsSpecificationExamples_5_1_todo);
  SUITE_ADD_TEST(suite, YtsSpecificationExamples_5_2_todo);
  SUITE_ADD_TEST(suite, YtsSpecificationExamples_6_11);
  SUITE_ADD_TEST(suite, YtsSpecification_FP8R);
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
#ifdef DEBUG_NEW
  SUITE_ADD_TEST(suite, YtsSpecificationExamples_6_12);
#endif
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
