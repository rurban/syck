AM_CPPFLAGS = -I$(top_srcdir)/lib
AM_LDFLAGS = -L$(top_builddir)/lib
LIBS = -lsyck
CUSRC = CuTest.c CuTest.h

TESTS = test-basic test-parse test-emit test-yts test-yts-full
noinst_PROGRAMS = test-basic test-parse test-emit test-yts test-yts-full

test_basic_SOURCES = Basic.c $(CUSRC)
test_parse_SOURCES = Parse.c $(CUSRC)
test_emit_SOURCES = Emit.c $(CUSRC)
test_yts_SOURCES = YTS.c $(CUSRC)
test_yts_full_SOURCES = YTS-full.c $(CUSRC)

yaml-test-suite: Makefile.am
	git submodule update --init
