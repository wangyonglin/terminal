#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT(terminal,1.0.0,admin@wangyonglin.com)
AM_INIT_AUTOMAKE
AC_CONFIG_SRCDIR([app/main.c])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC
AC_PROG_RANLIB
LD_LIBRARY_PATH
LD_LIBRARY_PATH
LT_INIT

# Checks for libraries.
AC_CHECK_LIB([pthread], [main])

# Checks for header files.
AC_CHECK_HEADERS([stddef.h stdlib.h string.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T

# Checks for library functions.
AC_CONFIG_FILES([Makefile
                 app/Makefile
                 src/ccl/Makefile
                 src/mqtt/Makefile
                 src/wangyonglin/Makefile
                 src/cjson/Makefile])
AC_OUTPUT
