AM_CPPFLAGS = -I$(top_srcdir)/lib
WARN_CFLAGS = @WARN_CFLAGS@
AM_CFLAGS   = -I. $(WARN_CFLAGS)
AM_LDFLAGS  = -static
AM_LIBTOOLFLAGS = --tag=CC
LDADD = $(top_builddir)/lib/.libs/libsyck.la
CUSRC = CuTest.c CuTest.h

TESTS = test-basic test-parse test-emit test-yts test-yts-full
noinst_PROGRAMS = test-basic test-parse test-emit test-yts test-yts-full

test_basic_SOURCES = Basic.c $(CUSRC)
test_parse_SOURCES = Parse.c $(CUSRC)
test_emit_SOURCES = Emit.c $(CUSRC)
test_yts_SOURCES = YTS.c $(CUSRC)
test_yts_full_SOURCES = YTS-full.c $(CUSRC)
test_yts_full_CFLAGS = $(AM_CFLAGS) @WNO_FORMAT_TRUNCATION@

yaml-test-suite: Makefile.am
	git submodule update --init
