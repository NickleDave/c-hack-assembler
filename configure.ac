AC_INIT([hackc], [0.1], [nickledave@users.noreply.github.com])

AM_INIT_AUTOMAKE([-Wall -Werror foreign])

AC_PROG_CC

PKG_CHECK_MODULES([CRITERION], [criterion], [], [
  AC_MSG_ERROR([unable to find Criterion])
])

AC_SUBST([CRITERION_CFLAGS])
AC_SUBST([CRITERION_LIBS])

AC_CONFIG_FILES([
    Makefile
    src/Makefile
    tests/Makefile
])

AC_OUTPUT