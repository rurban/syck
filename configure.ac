# Process this file with autoconf to produce a configure script.
AC_INIT([syck],[0.70],[rurban@cpan.org],[syck],
        [https://github.com/rurban/syck/])
AC_CONFIG_AUX_DIR(config)
AC_PREREQ([2.50])

LT_INIT
# CURRENT:REVISION:AGE
# Bump REVISION if there's no API or ABI changes.
# Bump AGE and CURRENT and reset REVISION if there's no ABI changes.
# Bump CURRENT and reset AGE and REVISION if there's ABI changes.
AC_SUBST([SYCK_SO_VERSION], [1:0:0])

AM_INIT_AUTOMAKE([1.14 silent-rules gnu])
AM_SILENT_RULES([yes])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET

AC_CHECK_PROGS(YACC, 'bison')
if test "$YACC" != "bison"; then
  AC_MSG_ERROR([bison not found])
else
  AC_MSG_CHECKING([bison version])
  [YACC_VERSION=`"$YACC" --version | head -n1 2>&1 | sed 's,bison (GNU Bison) ,,'`]
  AC_MSG_RESULT([$YACC_VERSION])
  AC_MSG_CHECKING([whether we already can %define api.pure])
  cat <<_ACEOF >_test.y
%define api.pure full  /* fails on Bison < 3.0 */
%%
start: /* empty */ ;
%%
int main(void) { return 0; }
_ACEOF
  if $YACC _test.y; then
    AC_MSG_RESULT([yes])
  else
    AC_MSG_RESULT([no, patching lib/gram.y])
    sed -i.orig 's/^%define api.pure/%pure-parser/' lib/gram.y
    sed -i.orig '/^%destructor/ s/^/\/\//' lib/gram.y
  fi
  rm -f _test.y _test.tab.c test.tab.h 2>/dev/null
fi
AC_CHECK_PROGS(REC, 're2c')
if test "$REC" != "re2c"; then
  AC_MSG_WARN([re2c not found])
fi

# Checks for libraries.

# Checks for header files.
AC_CHECK_INCLUDES_DEFAULT
AC_CHECK_HEADERS(alloca.h stdlib.h string.h sal.h ruby/st.h)

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_SIZEOF(int, 4)
AC_CHECK_SIZEOF(long, 4)
AX_COMPILER_FLAGS([WARN_CFLAGS],[AM_LDFLAGS],[yes],
    [-fno-semantic-interposition -fwrapv -fno-common -fvisibility=hidden \
     -ftrivial-auto-var-init=zero -fstrict-flex-arrays=3 \
     -fstack-protector-strong -fstack-clash-protection \
     -fcf-protection=full -fno-delete-null-pointer-checks \
     -Wformat-truncation -Wno-format-nonliteral -Wdiscarded_qualifiers])
if test "$ax_cv_check_cflags__Wformat_truncation" = "yes"; then
  WNO_FORMAT_TRUNCATION=-Wno-format-truncation
fi
if test "$ax_cv_check_cflags__Wdiscarded_qualifiers" = "yes"; then
  WNO_DISCARDED_QUALIFIERS=-Wno-discarded_qualifiers
fi
if test "$ax_cv_check_cflags__Wswitch_enum" = "yes"; then
  WNO_SWITCH_ENUM=-Wno-switch-enum
fi
AC_SUBST(WNO_FORMAT_TRUNCATION)
AC_SUBST(WNO_DISCARDED_QUALIFIERS)
AC_SUBST(WNO_SWITCH_ENUM)
AX_GCC_FUNC_ATTRIBUTE(returns_nonnull)
AX_GCC_FUNC_ATTRIBUTE(noreturn)
AX_GCC_FUNC_ATTRIBUTE(malloc)
AX_GCC_FUNC_ATTRIBUTE(pure)
AX_GCC_FUNC_ATTRIBUTE(const)
AX_GCC_FUNC_ATTRIBUTE(warn_unused_result)
AX_GCC_FUNC_ATTRIBUTE(unused)
AX_GCC_FUNC_ATTRIBUTE(format)

AC_MSG_CHECKING([for --disable-werror])
AC_ARG_ENABLE([werror],AS_HELP_STRING([--disable-werror],[
    Disable -Werror (default: no).]),[],[enable_werror=yes])
AS_IF([test x$enable_werror != xno],
  [AC_DEFINE([HAVE_WERROR],1,[If -Werror is enabled.])
   WARN_CFLAGS="-Werror $WARN_CFLAGS"
   AC_MSG_RESULT([no (default)])],
  [AC_MSG_RESULT([yes])])


# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_VPRINTF
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_FILES([Makefile
                 lib/Makefile
                 tests/Makefile])
AC_OUTPUT
