/*************************************************************************
 *
 * $Id$
 *
 * Copyright (C) 2002 Bjorn Reese and Daniel Stenberg.
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED WARRANTIES OF
 * MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. THE AUTHORS AND
 * CONTRIBUTORS ACCEPT NO RESPONSIBILITY IN ANY CONCEIVABLE MANNER.
 *
 ************************************************************************/

#include "triodef.h"

#undef TRIO_BUILD_WIDECHAR

#define TRIO_CHAR char
#define TRIO_INT int
#define TRIO_STRING_T trio_string_t
#define TEXT(x) x

/*
 * Ctype
 */
#define TRIO_ISALNUM(x) isalnum(x)
#define TRIO_ISALPHA(x) isalpha(x)
#define TRIO_ISCNTRL(x) iscntrl(x)
#define TRIO_ISDIGIT(x) isdigit(x)
#define TRIO_ISGRAPH(x) isgraph(x)
#define TRIO_ISLOWER(x) islower(x)
#define TRIO_ISPRINT(x) isprint(x)
#define TRIO_ISPUNCT(x) ispunct(x)
#define TRIO_ISSPACE(x) isspace(x)
#define TRIO_ISUPPER(x) isupper(x)
#define TRIO_ISXDIGIT(x) isxdigit(x)

/*
 * Trio strings
 */
#define TRIO_COPY_MAX(x,n,y) trio_copy_max((x),(n),(y))
#define TRIO_XCOPY_MAX(x,n,y) trio_copy_max((x),(n),(y))
#define TRIO_DESCRIPTION(n,x) trio_description((n),(x))
#define TRIO_DESTROY(x) trio_destroy((x))
#define TRIO_DUPLICATE(x) trio_duplicate((x))
#define TRIO_EQUAL(x,y) trio_equal((x),(y))
#define TRIO_EQUAL_CASE(x,y) trio_equal_case((x),(y))
#define TRIO_EQUAL_LOCALE(x,y) trio_equal_locale((x),(y))
#define TRIO_EQUAL_MAX(x,n,y) trio_equal_max((x),(n),(y))
#define TRIO_LENGTH(x) trio_length(x)
#define TRIO_TO_DOUBLE(x,y) trio_to_double((x),(y))
#define TRIO_TO_LONG(x,y,n) trio_to_long((x),(y),(n))
#define TRIO_STRING_DESTROY(x) trio_string_destroy((x))
#define TRIO_STRING_EXTRACT(x) trio_string_extract((x))
#define TRIO_STRING_TERMINATE(x) trio_string_terminate((x))
#define TRIO_XSTRING_APPEND_CHAR(x,y) trio_xstring_append_char((x),(y))
#define TRIO_XSTRING_DUPLICATE(x) trio_xstring_duplicate((x))

/*
 * Trio functions
 */
#define TRIO_STRERROR trio_strerror

#define TRIO_PRINTF trio_printf
#define TRIO_VPRINTF trio_vprintf
#define TRIO_PRINTFV trio_printfv
#define TRIO_FPRINTF trio_fprintf
#define TRIO_VFPRINTF trio_vfprintf
#define TRIO_FPRINTFV trio_fprintfv
#define TRIO_DPRINTF trio_dprintf
#define TRIO_VDPRINTF trio_vdprintf
#define TRIO_DPRINTFV trio_dprintfv
#define TRIO_SPRINTF trio_sprintf
#define TRIO_VSPRINTF trio_vsprintf
#define TRIO_SPRINTFV trio_sprintfv
#define TRIO_SNPRINTF trio_snprintf
#define TRIO_VSNPRINTF trio_vsnprintf
#define TRIO_SNPRINTFV trio_snprintfv
#define TRIO_SNPRINTFCAT trio_snprintfcat
#define TRIO_VSNPRINTFCAT trio_vsnprintfcat
#define TRIO_APRINTF trio_aprintf
#define TRIO_VAPRINTF trio_vaprintf
#define TRIO_ASPRINTF trio_asprintf
#define TRIO_VASPRINTF trio_vasprintf
#define TRIO_SCANF trio_scanf
#define TRIO_VSCANF trio_vscanf
#define TRIO_SCANFV trio_scanfv
#define TRIO_FSCANF trio_fscanf
#define TRIO_VFSCANF trio_vfscanf
#define TRIO_FSCANFV trio_fscanfv
#define TRIO_DSCANF trio_dscanf
#define TRIO_VDSCANF trio_vdscanf
#define TRIO_DSCANFV trio_dscanfv
#define TRIO_SSCANF trio_sscanf
#define TRIO_VSSCANF trio_vsscanf
#define TRIO_SSCANFV trio_sscanfv

#define TRIO_REGISTER trio_register
#define TRIO_UNREGISTER trio_unregister
#define TRIO_GET_FORMAT trio_get_format
#define TRIO_GET_ARGUMENT trio_get_argument
#define TRIO_GET_WIDTH trio_get_width
#define TRIO_SET_WIDTH trio_set_width
#define TRIO_GET_PRECISION trio_get_precision
#define TRIO_SET_PRECISION trio_set_precision
#define TRIO_GET_BASE trio_get_base
#define TRIO_SET_BASE trio_set_base
#define TRIO_GET_LONG trio_get_long
#define TRIO_SET_LONG trio_set_long
#define TRIO_GET_LONGLONG trio_get_longlong
#define TRIO_SET_LONGLONG trio_set_longlong
#define TRIO_GET_LONGDOUBLE trio_get_longdouble
#define TRIO_SET_LONGDOUBLE trio_set_longdouble
#define TRIO_GET_SHORT trio_get_short
#define TRIO_SET_SHORT trio_set_short
#define TRIO_GET_SHORTSHORT trio_get_shortshort
#define TRIO_SET_SHORTSHORT trio_set_shortshort
#define TRIO_GET_ALTERNATIVE trio_get_alternative
#define TRIO_SET_ALTERNATIVE trio_set_alternative
#define TRIO_GET_ALIGNMENT trio_get_alignment
#define TRIO_SET_ALIGNMENT trio_set_alignment
#define TRIO_GET_SPACING trio_get_spacing
#define TRIO_SET_SPACING trio_set_spacing
#define TRIO_GET_SIGN trio_get_sign
#define TRIO_SET_SIGN trio_set_sign
#define TRIO_GET_PADDING trio_get_padding
#define TRIO_SET_PADDING trio_set_padding
#define TRIO_GET_QUOTE trio_get_quote
#define TRIO_SET_QUOTE trio_set_quote
#define TRIO_GET_UPPER trio_get_upper
#define TRIO_SET_UPPER trio_set_upper
#define TRIO_GET_LARGEST trio_get_largest
#define TRIO_SET_LARGEST trio_set_largest
#define TRIO_GET_PTRDIFF trio_get_ptrdiff
#define TRIO_SET_PTRDIFF trio_set_ptrdiff
#define TRIO_GET_SIZE trio_get_size
#define TRIO_SET_SIZE trio_set_size
#define TRIO_PRINT_INT trio_print_int
#define TRIO_PRINT_UINT trio_print_uint
#define TRIO_PRINT_DOUBLE trio_print_double
#define TRIO_PRINT_STRING trio_print_string
#define TRIO_PRINT_POINTER trio_print_pointer
#define TRIO_PRINT_REF trio_print_ref
#define TRIO_VPRINT_REF trio_vprint_ref
#define TRIO_PRINTV_REF trio_printv_ref

#include "trio.c"
