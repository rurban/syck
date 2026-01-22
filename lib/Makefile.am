AUTOMAKE_OPTIONS = 1.4 foreign

AM_CPPFLAGS = -I$(top_srcdir)
WARN_CFLAGS = @WARN_CFLAGS@
AM_CFLAGS   = -I. $(WARN_CFLAGS)

LEXLIB = @LEXLIB@
SUFFIXES = .re

YACC = @YACC@
AM_YFLAGS = -d -t -v -p syck

lib_LTLIBRARIES = libsyck.la
include_HEADERS = syck.h syck_st.h
noinst_HEADERS = yamlbyte.h gram.h bytecode.h
DISTCLEANFILES = gram.c gram.h token.c implicit.c bytecode.c \
	token16.c implicit16.c token32.c implicit32.c

noinst_LTLIBRARIES = libsyck_nowarn.la libsyck_rec16.la libsyck_rec32.la
libsyck_nowarn_la_SOURCES = \
	gram.c emitter.c bytecode.c token.c
libsyck_nowarn_la_CFLAGS = $(AM_CFLAGS) @WNO_SWITCH_ENUM@
libsyck_rec16_la_SOURCES = token16.c implicit16.c
libsyck_rec32_la_SOURCES = token32.c implicit32.c
libsyck_rec16_la_CFLAGS = $(AM_CFLAGS) -DYYCSZ=16 @WNO_SWITCH_ENUM@
libsyck_rec32_la_CFLAGS = $(AM_CFLAGS) -DYYCSZ=32 @WNO_SWITCH_ENUM@

libsyck_a_SOURCES = \
	handler.c \
	node.c \
	syck.c \
	syck_st.c \
	implicit.c \
	yaml2byte.c
EXTRA_DIST = bytecode.re \
	token.re \
	implicit.re \
	gram.y

libsyck_la_CFLAGS = $(AM_CFLAGS)
libsyck_la_LDFLAGS = -version-info $(SYCK_SO_VERSION)
libsyck_la_SOURCES = $(libsyck_a_SOURCES)
libsyck_la_LIBADD = libsyck_nowarn.la libsyck_rec16.la libsyck_rec32.la

.c.i:
	$(AM_V_CC)depbase=`echo $@ | sed 's|[^/]*$$|$(DEPDIR)/&|;s|\.o$$||'`;\
	$(COMPILE) -E -o $@ $<

REC = @REC@
RECFLAGS = -g --case-ranges --no-generation-date
RECSZ = 8
COMPILE.re = if test -n "$(REC)"; then \
	  chmod +w $@ 2>/dev/null || true; \
	  $(REC) $(RECFLAGS) --utf$(RECSZ) $< -o $@ && \
	  echo -n "/* ex: set ro ft=c: -*- mode: c; buffer-read-only: t -*- */" > $@.prep && \
	  if test -n "@UNIFDEF@"; then unifdef -DYYCSZ=$(RECSZ) -m $@; fi; \
	  cat $@.prep $@ >$@.new && mv $@.new $@; rm $@.prep; chmod -w $@; \
	fi

.re.c:
	$(COMPILE.re)

token16.c: RECSZ=16
token16.c: token.re
	$(COMPILE.re)
implicit16.c: RECSZ=16
implicit16.c: implicit.re
	$(COMPILE.re)
token32.c: RECSZ=32
token32.c: token.re
	$(COMPILE.re)
implicit32.c: RECSZ=32
implicit32.c: implicit.re
	$(COMPILE.re)

# emacs flymake-mode
check-syntax:
	test -n "$(CHK_SOURCES)" && \
	  nice $(COMPILE) -I. -O0 -o /dev/null -S $(CHK_SOURCES)
.PHONY: check-syntax

LINT = splint

SYCK_SPLINT_SRCS = \
	emitter.c \
	handler.c \
	node.c \
	syck.c \
	syck_st.c \
	gram.c \
	yaml2byte.c \
	bytecode.c \
	token.c \
	implicit.c

gram.c:	gram.y
	-chmod +w gram.c gram.h
	$(YACC) $(AM_YFLAGS) -o gram.c $<
	-@if test -f gram.c; then \
	  chmod +w gram.c gram.h; \
	  sed -i 's,int syckparse (void),int syckparse (void*),' gram.h; \
	  sed -i 's,^yyparse (void),yyparse (void *parser),' gram.c; \
	  sed -i 's/yylex (\&yylval)/yylex (\&yylval, parser)/' gram.c; \
	  chmod -w gram.c gram.h; \
	fi

BUILT_SOURCES = gram.c gram.h bytecode.c \
	token.c token16.c token16.c \
	implicit.c implicit32.c implicit32.c

.PHONY: sources
sources:
	@echo $(SYCK_SPLINT_SRCS:%=syck/lib/%)

.PHONY: lint
lint:
	$(LINT) $(DEFS) $(AM_CPPFLAGS) $(SYCK_SPLINT_SRCS)
