#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([smaSHeM], [0.3.9], [timb@nth-dimension.org.uk])
AC_CONFIG_HEADERS([config.h:config.in])
AC_CONFIG_SRCDIR([src/smaSHeM.h])
AM_INIT_AUTOMAKE

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
  LIBS="$CPPFLAGS $QTGUI_LIBS"
  AC_DEFINE([WITH_QTGUI], 1, [image dumping enabled])
fi

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([stdio.h errno.h sys/shm.h ctype.h],, AC_MSG_ERROR([required header file missing]))
if test "x$with_qtgui" = "xyes"; then
  # waitpid is only useful if we have QtGui
  AC_CHECK_HEADERS([QtGui/qimage.h sys/wait.h],, AC_MSG_ERROR([optional but requested header file missing]))
fi

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
AC_CHECK_FUNCS([fprintf exit perror getopt atoi strlen shmat shmdt printf],, AC_MSG_ERROR([required standard library function missing]))
if test "x$with_qtgui" = "xyes"; then
  # only useful if we have QtGui
  AC_CHECK_FUNCS([malloc memset sprintf free],, AC_MSG_ERROR([optional but requested standard library function missing]))
fi

AC_CONFIG_FILES([Makefile src/Makefile doc/Makefile man/Makefile])
AC_OUTPUT
