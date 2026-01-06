AM_CPPFLAGS = -I$(top_srcdir)/lib
WARN_CFLAGS = @WARN_CFLAGS@
YTS_WFLAGS = @WNO_DISCARDED_QUALIFIERS@
YTS_FULL_WFLAGS = @WNO_FORMAT_TRUNCATION@ @WNO_SWITCH_ENUM@
AM_CFLAGS   = -I. $(WARN_CFLAGS)
AM_LDFLAGS  = -static
AM_LIBTOOLFLAGS = --tag=CC
LDADD = $(top_builddir)/lib/.libs/libsyck.la
CUSRC = CuTest.c CuTest.h

EXTRA_DIST = YTS-lib.h
TESTS = test-basic test-parse test-emit test-yts test-yts-full
noinst_PROGRAMS = test-basic test-parse test-emit test-yts test-yts-full

test_basic_SOURCES = Basic.c $(CUSRC)
test_parse_SOURCES = Parse.c $(CUSRC)
test_emit_SOURCES = Emit.c $(CUSRC)
test_yts_SOURCES = YTS.c YTS-lib.c $(CUSRC)
test_yts_CFLAGS = $(AM_CFLAGS) $(YTS_WFLAGS)
test_yts_full_SOURCES = YTS-full.c YTS-lib.c $(CUSRC)
test_yts_full_CFLAGS = $(AM_CFLAGS) $(YTS_FULL_WFLAGS)

test-yts-full$(EXTEXT): yaml-test-suite
yaml-test-suite: 
	git submodule update --init
