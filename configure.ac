AC_PRERQ([2.73])
AC_INIT([hackc], [0.1])

AC_CONFIG_SRCDIR([src/main.c])

AM_INIT_AUTOMAKE([-Wall -Werror foreign subdir-objects])

AC_PROG_CC

PKG_CHECK_MODULES([CRITERION], [criterion], [], [
  AC_MSG_ERROR([unable to find Criterion])
])

AC_SUBST([CRITERION_CFLAGS])
AC_SUBST([CRITERION_LIBS])

AC_CONFIG_FILES([
    Makefile
])

AC_OUTPUT