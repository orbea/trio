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
#if defined(TRIO_PLATFORM_UNIX)
# include <sys/types.h>
#endif

#undef TRIO_BUILD_WIDECHAR

#define TRIO_CHAR char
#define TRIO_INT int
#define TRIO_STRING_T trio_string_t
#define TEXT(x) x

#define TOLOWER tolower
#define TOUPPER toupper
#define ISDIGIT isdigit
#define ISXDIGIT isxdigit

#define STRLEN(x) strlen((x))
#define STRCAT(x,y) strcat((x),(y))
#define STRNCAT(x,y,n) strncat((x),(y),(n))
#define STRSTR(x,y) strstr((x),(y))
#define STRCPY(x,y) strcpy((x),(y))
#define STRNCPY(x,y,n) strncpy((x),(y),(n))
#define STRCMP(x,y) strcmp((x),(y))
#define STRNCMP(x,y,n) strncmp((x),(y),(n))
#define STRCOLL(x,y) strcoll((x),(y))
#define STRFTIME(x,n,y,t) strftime((x),(n),(y),(t))
#define STRCHR(x,c) strchr((x),(c))
#define STRRCHR(x,c) strrchr((x),(c))
#define STRTOK(x,y,z) strtok_r((x),(y),(z)) /* FIXME */
#define STRTOL(x,y,n) strtol((x),(y),(n))
#define STRTOUL(x,y,n) strtoul((x),(y),(n))

#define TRIO_CREATE trio_create
#define TRIO_DESTROY trio_destroy
#define TRIO_LENGTH trio_length
#define TRIO_APPEND trio_append
#define TRIO_APPEND_MAX trio_append_max
#define TRIO_CONTAINS trio_contains
#define TRIO_COPY trio_copy
#define TRIO_COPY_MAX trio_copy_max
#define TRIO_DESCRIPTION trio_description
#define TRIO_DUPLICATE trio_duplicate
#define TRIO_DUPLICATE_MAX trio_duplicate_max
#define TRIO_EQUAL trio_equal
#define TRIO_EQUAL_CASE trio_equal_case
#define TRIO_EQUAL_CASE_MAX trio_equal_case_max
#define TRIO_EQUAL_LOCALE trio_equal_locale
#define TRIO_EQUAL_MAX trio_equal_max
#define TRIO_FORMAT_DATE_MAX trio_format_date_max
#define TRIO_HASH trio_hash
#define TRIO_INDEX trio_index
#define TRIO_INDEX_LAST trio_index_last
#define TRIO_LOWER trio_lower
#define TRIO_MATCH trio_match
#define TRIO_MATCH_CASE trio_match_case
#define TRIO_SPAN_FUNCTION trio_span_function
#define TRIO_SUBSTRING trio_substring
#define TRIO_SUBSTRING_MAX trio_substring_max
#define TRIO_TOKENIZE trio_tokenize
#define TRIO_TO_DOUBLE trio_to_double
#define TRIO_TO_FLOAT trio_to_float
#define TRIO_TO_LONG trio_to_long
#define TRIO_TO_UNSIGNED_LONG trio_to_unsigned_long
#define TRIO_UPPER trio_upper

#define TRIO_STRING_CREATE trio_string_create
#define TRIO_STRING_DESTROY trio_string_destroy
#define TRIO_STRING_GET trio_string_get
#define TRIO_STRING_EXTRACT trio_string_extract
#define TRIO_XSTRING_SET trio_xstring_set
#define TRIO_STRING_SIZE trio_string_size
#define TRIO_STRING_TERMINATE trio_string_terminate
#define TRIO_STRING_APPEND trio_string_append
#define TRIO_XSTRING_APPEND trio_xstring_append
#define TRIO_XSTRING_APPEND_CHAR trio_xstring_append_char
#define TRIO_STRING_CONTAINS trio_string_contains
#define TRIO_XSTRING_CONTAINS trio_xstring_contains
#define TRIO_STRING_COPY trio_string_copy
#define TRIO_XSTRING_COPY trio_xstring_copy
#define TRIO_STRING_DUPLICATE trio_string_duplicate
#define TRIO_XSTRING_DUPLICATE trio_xstring_duplicate
#define TRIO_STRING_EQUAL trio_string_equal
#define TRIO_XSTRING_EQUAL trio_xstring_equal
#define TRIO_STRING_EQUAL_MAX trio_string_equal_max
#define TRIO_XSTRING_EQUAL_MAX trio_xstring_equal_max
#define TRIO_STRING_EQUAL_CASE trio_string_equal_case
#define TRIO_XSTRING_EQUAL_CASE trio_xstring_equal_case
#define TRIO_STRING_EQUAL_CASE_MAX trio_string_equal_case_max
#define TRIO_XSTRING_EQUAL_CASE_MAX trio_xstring_equal_case_max
#define TRIO_STRING_FORMAT_DATE_MAX trio_string_format_date_max
#define TRIO_STRING_INDEX trio_string_index
#define TRIO_STRING_INDEX_LAST trio_string_index_last
#define TRIO_STRING_LENGTH trio_string_length
#define TRIO_STRING_LOWER trio_string_lower
#define TRIO_STRING_MATCH trio_string_match
#define TRIO_XSTRING_MATCH trio_xstring_match
#define TRIO_STRING_MATCH_CASE trio_string_match_case
#define TRIO_XSTRING_MATCH_CASE trio_xstring_match_case
#define TRIO_STRING_SUBSTRING trio_string_substring
#define TRIO_XSTRING_SUBSTRING trio_xstring_substring
#define TRIO_STRING_UPPER trio_string_upper

/*************************************************************************
 * Structures
 */

struct _trio_string_t
{
  char *content;
  size_t length;
  size_t allocated;
};

#define TRIO_STRING_T trio_string_t

/*************************************************************************
 * Implementation
 */

#include "triostr.c"
