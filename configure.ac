AC_PREREQ([2.69])
AC_INIT([trio],[1.17.1])
AC_CONFIG_AUX_DIR([.])
AC_CONFIG_SRCDIR([compare.c])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE([foreign])

AC_PROG_CC
AC_PROG_MKDIR_P
LT_INIT
LT_LIB_M

AC_CHECK_HEADERS([float.h inttypes.h limits.h locale.h stddef.h stdint.h \
	stdlib.h string.h strings.h unistd.h wchar.h wctype.h])

AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_TYPE_INT8_T
AC_TYPE_SIZE_T
AC_CHECK_TYPES([ptrdiff_t])

AC_FUNC_STRCOLL
AC_FUNC_STRTOD
AC_CHECK_FUNCS([isascii localeconv mblen memcpy memset modf strcasecmp strchr \
	strcpy strerror strlen strncasecmp strstr strtol strtoul])

AC_ARG_ENABLE([doc],
	AS_HELP_STRING([--enable-doc], [Enable building doxygen docs]))

AS_IF([test "x$enable_doc" = "xyes"], [
	AC_CHECK_PROGS([DOXYGEN], [doxygen])
	AS_IF([test -z "$DOXYGEN"], [
		AC_MSG_ERROR([Doxygen not found])
	])
])
AM_CONDITIONAL([HAVE_DOXYGEN], [test "x$enable_doc" = "xyes"])

AC_CONFIG_FILES([libtrio.pc doc/Doxyfile doc/Makefile Makefile])
AC_OUTPUT
