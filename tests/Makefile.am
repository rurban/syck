AM_CPPFLAGS = -I$(top_srcdir)/lib
AM_LDFLAGS = -L$(top_builddir)/lib
LIBS = -lsyck
CUSRC = CuTest.c CuTest.h

TESTS = test-basic test-parse test-yts test-emit
noinst_PROGRAMS = test-basic test-parse test-yts test-emit

test_basic_SOURCES = Basic.c $(CUSRC)
test_parse_SOURCES = Parse.c $(CUSRC)
test_yts_SOURCES = YTS.c $(CUSRC)
test_emit_SOURCES = Emit.c $(CUSRC)
