#include <assert.h>
#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "syck.h"
#include "CuTest.h"

/*-------------------------------------------------------------------------*
 * CuStr
 *-------------------------------------------------------------------------*/

char *CuStrAlloc(int size) {
  char *new = (char *)malloc(sizeof(char) * (size));
  return new;
}

char *CuStrCopy(const char *old) {
  size_t len = strlen(old);
  char *new = CuStrAlloc(len + 1);
  strcpy(new, old);
  return new;
}

/*-------------------------------------------------------------------------*
 * CuString
 *-------------------------------------------------------------------------*/

void CuStringInit(CuString *str) {
  str->length = 0;
  str->size = STRING_MAX;
  str->buffer = (char *)malloc(sizeof(char) * str->size);
  str->buffer[0] = '\0';
}

CuString *CuStringNew(void) {
  CuString *str = (CuString *)malloc(sizeof(CuString));
  str->length = 0;
  str->size = STRING_MAX;
  str->buffer = (char *)malloc(sizeof(char) * str->size);
  str->buffer[0] = '\0';
  return str;
}

void CuStringResize(CuString *str, size_t newSize) {
  str->buffer = (char *)realloc(str->buffer, sizeof(char) * newSize);
  str->size = newSize;
}

void CuStringAppend(CuString *str, const char *text) {
  size_t length = strlen(text);
  CuStringAppendLen(str, text, length);
}

void CuStringAppendLen(CuString *str, const char *text, size_t length) {
  if (str->length + length + 1 >= str->size)
    CuStringResize(str, str->length + length + 1 + STRING_INC);
  str->length += length;
  strcat(str->buffer, text);
}

void CuStringAppendChar(CuString *str, char ch) {
  char text[2];
  text[0] = ch;
  text[1] = '\0';
  CuStringAppend(str, text);
}

__attribute__format__(2,3)
void CuStringAppendFormat(CuString *str, const char *format, ...) {
  va_list argp;
  char buf[HUGE_STRING_LEN];
  va_start(argp, format);
  vsprintf(buf, format, argp);
  va_end(argp);
  CuStringAppend(str, buf);
}

void CuStringAppendEscaped(CuString *str, const char *text) {
  unsigned char *p = (unsigned char *)text;
  while(*p) {
    switch (*p) {
    //case '"':  CuStringAppendLen(str, "\\\"", 2); break;
    case '\\': CuStringAppendLen(str, "\\\\", 2); break;
    case '\0': CuStringAppendLen(str, "\\0",  2); break;
    case '\a': CuStringAppendLen(str, "\\a",  2); break;
    case '\b': CuStringAppendLen(str, "\\b",  2); break;
    case '\f': CuStringAppendLen(str, "\\f",  2); break;
    case '\r': CuStringAppendLen(str, "\\r",  2); break;
    case '\t': CuStringAppendLen(str, "\\t",  2); break;
    case '\v': CuStringAppendLen(str, "\\v",  2); break;
    case 0x1b: CuStringAppendLen(str, "\\e",  2); break;
    case '\n': CuStringAppendLen(str, "\\n",  2); break;
    default: CuStringAppendChar(str, *p); break;
    }
    p++;
  }
}

void CuStringFree(CuString *str) {
  if (str != NULL) {
    free(str->buffer);
    free(str);
  }
}

/*-------------------------------------------------------------------------*
 * CuTest
 *-------------------------------------------------------------------------*/

void CuTestInit(CuTest *t, const char *name, TestFunction function) {
  t->name = CuStrCopy(name);
  t->function = function;
  t->failed = 0;
  t->ran = 0;
  t->message = NULL;
  t->jumpBuf = NULL;
}

CuTest *CuTestNew(const char *name, TestFunction function) {
  CuTest *tc = CU_ALLOC(CuTest);
  CuTestInit(tc, name, function);
  return tc;
}

void CuTestFree(CuTest *t) {
  if (t != NULL) {
    free(t->message);
    free(t->name);
    free(t);
  }
}

void CuFail(CuTest *tc, const char *message) {
  tc->failed = 1;
  tc->message = CuStrCopy(message);
  if (tc->jumpBuf != 0)
    longjmp(*(tc->jumpBuf), 0);
}

void CuAssert(CuTest *tc, const char *message, int condition) {
  if (condition)
    return;
  CuFail(tc, message);
}

void CuAssertTrue(CuTest *tc, int condition) {
  if (condition)
    return;
  CuFail(tc, "assert failed");
}

void CuAssertStrEquals(CuTest *tc, const char *expected, char *actual) {
  CuString *message;
  char *s;
  if (strcmp(expected, actual) == 0)
    return;
  message = CuStringNew();
  CuStringAppend(message, "expected <");
  CuStringAppend(message, expected);
  CuStringAppend(message, "> but was <");
  CuStringAppend(message, actual);
  CuStringAppend(message, ">");
  s = strdup(message->buffer);
  CuStringFree(message);
  CuFail(tc, s);
  free(s);
}

void CuAssertIntEquals(CuTest *tc, int expected, int actual) {
  char buf[STRING_MAX];
  if (expected == actual)
    return;
  sprintf(buf, "expected <%d> but was <%d>", expected, actual);
  CuFail(tc, buf);
}

void CuAssertPtrEquals(CuTest *tc, void *expected, void *actual) {
  char buf[STRING_MAX];
  if (expected == actual)
    return;
  sprintf(buf, "expected pointer <0x%p> but was <0x%p>", expected, actual);
  CuFail(tc, buf);
}

void CuAssertPtrNotNull(CuTest *tc, void *pointer) {
  char buf[STRING_MAX];
  if (pointer != NULL)
    return;
  sprintf(buf, "null pointer unexpected");
  CuFail(tc, buf);
}

void CuTestRun(CuTest *tc) {
  jmp_buf buf;
  tc->jumpBuf = &buf;
  if (setjmp(buf) == 0) {
    tc->ran = 1;
    (tc->function)(tc);
  }
  if (tc->ran) {
    free(tc->message);
    tc->message = NULL;
  }
  tc->jumpBuf = 0;
}

void CuTestRunNoJmp(CuTest *tc) {
  tc->jumpBuf = 0;
  (tc->function)(tc);
  tc->ran = 1;
}

/*-------------------------------------------------------------------------*
 * CuSuite
 *-------------------------------------------------------------------------*/

void CuSuiteInit(CuSuite *testSuite) {
  testSuite->count = 0;
  testSuite->failCount = 0;
}

CuSuite *CuSuiteNew(void) {
  CuSuite *testSuite = CU_ALLOC(CuSuite);
  CuSuiteInit(testSuite);
  return testSuite;
}

void CuSuiteFree(CuSuite *testSuite) {
  int i;
  for (i = 0; i < testSuite->count; ++i) {
    CuTestFree(testSuite->list[i]);
  }
  free(testSuite);
}

void CuSuiteAdd(CuSuite *testSuite, CuTest *testCase) {
  assert(testSuite->count < MAX_TEST_CASES);
  testSuite->list[testSuite->count] = testCase;
  testSuite->count++;
}

void CuSuiteAddSuite(CuSuite *testSuite, CuSuite *testSuite2) {
  int i;
  for (i = 0; i < testSuite2->count; ++i) {
    CuTest *testCase = testSuite2->list[i];
    CuSuiteAdd(testSuite, testCase);
  }
}

void CuSuiteRun(CuSuite *testSuite) {
  int i;
  for (i = 0; i < testSuite->count; ++i) {
    CuTest *testCase = testSuite->list[i];
    CuTestRun(testCase);
    if (testCase->failed) {
      testSuite->failCount += 1;
    }
  }
}

void CuSuiteRunNoJmp(CuSuite *testSuite) {
  int i;
  for (i = 0; i < testSuite->count; ++i) {
    CuTest *testCase = testSuite->list[i];
    CuTestRunNoJmp(testCase);
    if (testCase->failed) {
      testSuite->failCount += 1;
    }
  }
}

void CuSuiteSummary(CuSuite *testSuite, CuString *summary) {
  int i;
  for (i = 0; i < testSuite->count; ++i) {
    CuTest *testCase = testSuite->list[i];
    CuStringAppend(summary, testCase->failed ? "F" : ".");
  }
  CuStringAppend(summary, "\n\n");
}

void CuSuiteDetails(CuSuite *testSuite, CuString *details) {
  int i;
  int failCount = 0;

  if (testSuite->failCount == 0) {
    int passCount = testSuite->count - testSuite->failCount;
    const char *testWord = passCount == 1 ? "test" : "tests";
    CuStringAppendFormat(details, "OK (%d %s)\n", passCount, testWord);
  } else {
    if (testSuite->failCount == 1)
      CuStringAppend(details, "There was 1 failure:\n");
    else
      CuStringAppendFormat(details, "There were %d failures:\n",
                           testSuite->failCount);

    for (i = 0; i < testSuite->count; ++i) {
      CuTest *testCase = testSuite->list[i];
      if (testCase->failed) {
        failCount++;
        CuStringAppendFormat(details, "%d) %s: %s\n", failCount, testCase->name,
                             testCase->message);
      }
    }
    CuStringAppend(details, "\n!!!FAILURES!!!\n");

    CuStringAppendFormat(details, "Runs: %d ", testSuite->count);
    CuStringAppendFormat(details, "Passes: %d ",
                         testSuite->count - testSuite->failCount);
    CuStringAppendFormat(details, "Fails: %d\n", testSuite->failCount);
  }
}
