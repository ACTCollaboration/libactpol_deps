#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT([slim], [v2_7_0], [drjoefowler@gmail.com])
AC_REVISION($Revision$)
AC_CONFIG_SRCDIR([src/bitstream.cpp])

# Checks for programs.
AC_PROG_AWK
AC_PROG_CPP
AC_PROG_CXX
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET

# Checks for libraries.
AC_CHECK_LIB([m], [sqrt])
AC_CHECK_LIB([slim], [ibitstream])

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([limits.h stdint.h stdlib.h string.h unistd.h utime.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_C_INLINE
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_ERROR_AT_LINE
AC_FUNC_SETVBUF_REVERSED
AC_FUNC_STAT
AC_FUNC_UTIME_NULL
AC_CHECK_FUNCS([memset sqrt strchr strrchr strstr utime])

AC_ARG_WITH([zzip],
    [AS_HELP_STRING([--with-zzip],
        [support reading from zip archives])],
    [],
    [with_zzip=no])

LIBZZIP=
AS_IF([test "x$with_zzip" != xno],
    [AC_CHECK_LIB([zzip], [zzip_fopen],
        [AC_SUBST([LIBZZIP], ["-lzzip"])
            AC_DEFINE([HAVE_LIBZZIP], [1],
            [Define if you have libzzip])],
        [AC_MSG_FAILURE(
            [--with-zzip was given, but test for zzip failed])],
        [])])

AS_IF([test "x$with_zzip" != xno],
    [AC_CHECK_LIB([zzip], [zzip_fopen],
        [AC_SUBST([TEST_ZZIP], ["./test_zirfile;"])
            AC_DEFINE([HAVE_LIBZZIP], [1],
            [Define if you have libzzip])],
        [AC_MSG_FAILURE(
            [--with-zzip was given, but test for zzip failed])],
        [])])

AC_ARG_WITH([lz4],
    [AS_HELP_STRING([--with-lz4],
        [support reading from LZ4 compressed files])],
    [],
    [with_lz4=no])

LIBLZ4=
AS_IF([test "x$with_lz4" != xno],
    [AC_CHECK_LIB([lz4], [LZ4F_decompress],
        [AC_SUBST([LIBLZ4], ["-llz4"])
            AC_DEFINE([HAVE_LIBLZ4], [1],
            [Define if you have liblz4])],
        [AC_MSG_FAILURE(
            [--with-lz4 was given, but test for LZ4 failed])],
        [])])

AC_CONFIG_FILES([Makefile
                 src/Makefile
                 test/Makefile])
AC_OUTPUT
