# Process this file with autoconf to produce a configure script.
AC_INIT([syck],[0.70],[rurban@cpan.org],[],
        [https://github.com/rurban/syck/])
AC_CONFIG_AUX_DIR(config)
AC_PREREQ([2.50])

LT_INIT
# CURRENT:REVISION:AGE
# Bump REVISION if there's no API or ABI changes.
# Bump AGE and CURRENT and reset REVISION if there's no ABI changes.
# Bump CURRENT and reset AGE and REVISION if there's ABI changes.
AC_SUBST([SYCK_SO_VERSION], [0:55:0])

AM_INIT_AUTOMAKE([1.14 gnu])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_AWK
AM_PROG_LEX([noyywrap])
#LT_INIT

AC_CHECK_PROGS(YACC, 'bison')
if test "$YACC" != "bison"; then
  AC_MSG_ERROR([bison not found])
fi
AC_CHECK_PROGS(REC, 're2c')
if test "$REC" != "re2c"; then
  AC_MSG_WARN([re2c not found])
fi

# Checks for libraries.

# Checks for header files.
AC_CHECK_INCLUDES_DEFAULT
AC_PROG_EGREP

AC_CHECK_HEADERS(alloca.h stdlib.h string.h sal.h)
AC_CHECK_SIZEOF(int, 4)
AC_CHECK_SIZEOF(long, 4)

# Checks for typedefs, structures, and compiler characteristics.
AX_GCC_FUNC_ATTRIBUTE(returns_nonnull)
AX_GCC_FUNC_ATTRIBUTE(noreturn)
AX_GCC_FUNC_ATTRIBUTE(malloc)
AX_GCC_FUNC_ATTRIBUTE(pure)
AX_GCC_FUNC_ATTRIBUTE(warn_unused_result)
AX_GCC_FUNC_ATTRIBUTE(unused)

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_VPRINTF
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_FILES([Makefile 
                 lib/Makefile 
                 tests/Makefile])
AC_OUTPUT
