/*************************************************************************
 *
 * $Id$
 *
 * Copyright (C) 2001 Bjorn Reese and Daniel Stenberg.
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

#ifndef TRIO_TRIOSTR_H
#define TRIO_TRIOSTR_H

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "triodef.h"
#include "triop.h"

enum {
  TRIO_HASH_NONE = 0,
  TRIO_HASH_PLAIN,
  TRIO_HASH_TWOSIGNED
};

/*************************************************************************
 * String functions
 */

int trio_append TRIO_PROTO((char *target, const char *source));
int trio_append_max TRIO_PROTO((char *target, size_t max, const char *source));
int trio_contains TRIO_PROTO((const char *string, const char *substring));
int trio_copy TRIO_PROTO((char *target, const char *source));
int trio_copy_max TRIO_PROTO((char *target, size_t max, const char *source));
char *trio_create TRIO_PROTO((size_t size));
void trio_destroy TRIO_PROTO((char *string));
char *trio_duplicate TRIO_PROTO((const char *source));
char *trio_duplicate_max TRIO_PROTO((const char *source, size_t max));
int trio_equal TRIO_PROTO((const char *first, const char *second));
int trio_equal_case TRIO_PROTO((const char *first, const char *second));
int trio_equal_case_max TRIO_PROTO((const char *first, size_t max, const char *second));
int trio_equal_locale TRIO_PROTO((const char *first, const char *second));
int trio_equal_max TRIO_PROTO((const char *first, size_t max, const char *second));
TRIO_CONST char *trio_error TRIO_PROTO((int));
size_t trio_format_date_max TRIO_PROTO((char *target, size_t max, const char *format, const struct tm *datetime));
unsigned long trio_hash TRIO_PROTO((const char *string, int type));
char *trio_index TRIO_PROTO((const char *string, int character));
char *trio_index_last TRIO_PROTO((const char *string, int character));
size_t trio_length TRIO_PROTO((const char *string));
int trio_lower TRIO_PROTO((char *target));
int trio_match TRIO_PROTO((const char *string, const char *pattern));
int trio_match_case TRIO_PROTO((const char *string, const char *pattern));
size_t trio_span_function TRIO_PROTO((char *target, const char *source, int (*Function) TRIO_PROTO((int))));
char *trio_substring TRIO_PROTO((const char *string, const char *substring));
char *trio_substring_max TRIO_PROTO((const char *string, size_t max, const char *substring));
char *trio_tokenize TRIO_PROTO((char *string, const char *delimiters));
float trio_to_float TRIO_PROTO((const char *source, const char **endp));
double trio_to_double TRIO_PROTO((const char *source, const char **endp));
long trio_to_long TRIO_PROTO((const char *source, char **endp, int base));
unsigned long trio_to_unsigned_long TRIO_PROTO((const char *source, char **endp, int base));
int trio_upper TRIO_PROTO((char *target));

/*************************************************************************
 * Dynamic string functions
 */

/*
 * Opaque type for dynamic strings
 */
typedef struct _trio_string_t trio_string_t;

trio_string_t *trio_string_create TRIO_PROTO((int initial_size));
void trio_string_destroy TRIO_PROTO((trio_string_t *self));
char *trio_string_get TRIO_PROTO((trio_string_t *self, int offset));
void trio_xstring_set TRIO_PROTO((trio_string_t *self, char *buffer));
char *trio_string_extract TRIO_PROTO((trio_string_t *self));
int trio_string_size TRIO_PROTO((trio_string_t *self));
void trio_string_terminate TRIO_PROTO((trio_string_t *self));

int trio_string_append TRIO_PROTO((trio_string_t *self, trio_string_t *other));
int trio_string_contains TRIO_PROTO((trio_string_t *self, trio_string_t *other));
int trio_string_copy TRIO_PROTO((trio_string_t *self, trio_string_t *other));
trio_string_t *trio_string_duplicate TRIO_PROTO((trio_string_t *other));
int trio_string_equal TRIO_PROTO((trio_string_t *self, trio_string_t *other));
int trio_string_equal_max TRIO_PROTO((trio_string_t *self, size_t max, trio_string_t *second));
int trio_string_equal_case TRIO_PROTO((trio_string_t *self, trio_string_t *other));
int trio_string_equal_case_max TRIO_PROTO((trio_string_t *self, size_t max, trio_string_t *other));
size_t trio_string_format_date_max TRIO_PROTO((trio_string_t *self, size_t max, const char *format, const struct tm *datetime));
char *trio_string_index TRIO_PROTO((trio_string_t *self, int character));
char *trio_string_index_last TRIO_PROTO((trio_string_t *self, int character));
int trio_string_length TRIO_PROTO((trio_string_t *self));
int trio_string_lower TRIO_PROTO((trio_string_t *self));
int trio_string_match TRIO_PROTO((trio_string_t *self, trio_string_t *other));
int trio_string_match_case TRIO_PROTO((trio_string_t *self, trio_string_t *other));
char *trio_string_substring TRIO_PROTO((trio_string_t *self, trio_string_t *other));
int trio_string_upper TRIO_PROTO((trio_string_t *self));

int trio_xstring_append_char TRIO_PROTO((trio_string_t *self, char character));
int trio_xstring_append TRIO_PROTO((trio_string_t *self, const char *other));
int trio_xstring_contains TRIO_PROTO((trio_string_t *self, const char *other));
int trio_xstring_copy TRIO_PROTO((trio_string_t *self, const char *other));
trio_string_t *trio_xstring_duplicate TRIO_PROTO((const char *other));
int trio_xstring_equal TRIO_PROTO((trio_string_t *self, const char *other));
int trio_xstring_equal_max TRIO_PROTO((trio_string_t *self, size_t max, const char *other));
int trio_xstring_equal_case TRIO_PROTO((trio_string_t *self, const char *other));
int trio_xstring_equal_case_max TRIO_PROTO((trio_string_t *self, size_t max, const char *other));
int trio_xstring_match TRIO_PROTO((trio_string_t *self, const char *other));
int trio_xstring_match_case TRIO_PROTO((trio_string_t *self, const char *other));
char *trio_xstring_substring TRIO_PROTO((trio_string_t *self, const char *other));

#endif /* TRIO_TRIOSTR_H */
