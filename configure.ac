#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([smaSHeM], [0.3], [timb@nth-dimension.org.uk])
AM_INIT_AUTOMAKE(smaSHeM, 0.3)
AC_CONFIG_SRCDIR([src/smaSHeM.h])

# Checks for programs.
AC_PROG_CXXCPP
AC_PROG_CXX
PKG_PROG_PKG_CONFIG

# We're using C++ so that we get the benefit of QtGui
AC_LANG([C++])

# Checks for pkg-conf.
AC_ARG_WITH([qtgui], AS_HELP_STRING([--with-qtgui], [Build with QtGui for image dumping]))
if test "x$with_qtgui" = "xyes"; then
  PKG_CHECK_MODULES([QTGUI], [QtGui])
  CPPFLAGS="$CPPFLAGS $QTGUI_CFLAGS"
fi

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([stdio.h stdlib.h errno.h unistd.h stdlib.h string.h sys/types.h sys/shm.h ctype.h, AC_MSG_ERROR([required header file missing]))
if test "x$with_qtgui" = "xyes"; then
  # waitpid is only useful if we have QtGui
  AC_CHECK_HEADERS([QtGui/qimage.h sys/wait.h, AC_MSG_ERROR([optional, but requested header file missing]))
fi

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
AC_FUNC_ERROR_AT_LINE

AC_CONFIG_FILES([Makefile src/Makefile doc/Makefile])
AC_OUTPUT
