#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([smaSHeM], [0.3], [timb@nth-dimension.org.uk])
AM_INIT_AUTOMAKE(smaSHeM, 0.3)
AC_CONFIG_SRCDIR([src/smaSHeM.h])

# Checks for programs.
AC_PROG_CC

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([stdio.h stdlib.h errno.h unistd.h stdlib.h string.h sys/types.h sys/shm.h ctype.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
AC_FUNC_ERROR_AT_LINE

AC_CONFIG_FILES([Makefile src/Makefile doc/Makefile])
AC_OUTPUT
