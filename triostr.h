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

#if defined(NDEBUG) || defined(TRIO_COMPILER_DECC)
#define trio_create(n) (char *)TRIO_MALLOC(n)
#define trio_destroy(x) TRIO_FREE(x)
#define trio_append(x,y) strcat((x), (y))
#define trio_contains(x,y) (0 != strstr((x), (y)))
#define trio_copy(x,y) strcpy((x), (y))
#define trio_index(x,y) strchr((x), (y))
#define trio_index_last(x,y) strrchr((x), (y))
#define trio_length(x) strlen((x))
#define trio_substring(x,y) strstr((x), (y))
#define trio_tokenize(x,y) strtok((x), (y))
#define trio_to_long(x,y,n) strtol((x), (y), (n))
#define trio_to_unsigned_long(x,y,n) strtoul((x), (y), (n))
#else /* DEBUG */
 /*
  * To be able to use these macros everywhere, including in
  * if() sentences, the assertions are put first in a comma
  * seperated list.
  *
  * Unfortunately the DECC compiler does not seem to like this
  * so it will use the un-asserted functions above for the
  * debugging case too.
  */
#define trio_create(n) \
     (assert((n) > 0),\
      (char *)TRIO_MALLOC(n))
#define trio_destroy(x) \
     (assert((x) != NULL),\
      TRIO_FREE(x))
#define trio_append(x,y) \
     (assert((x) != NULL),\
      assert((y) != NULL),\
      strcat((x), (y)))
#define trio_contains(x,y) \
     (assert((x) != NULL),\
      assert((y) != NULL),\
      (0 != strstr((x), (y))))
#define trio_copy(x,y) \
     (assert((x) != NULL),\
      assert((y) != NULL),\
      strcpy((x), (y)))
#define trio_index(x,c) \
     (assert((x) != NULL),\
      strchr((x), (c)))
#define trio_index_last(x,c) \
     (assert((x) != NULL),\
      strrchr((x), (c)))
#define trio_length(x) \
     (assert((x) != NULL),\
      strlen((x)))
#define trio_substring(x,y) \
     (assert((x) != NULL),\
      assert((y) != NULL),\
      strstr((x), (y)))
#define trio_tokenize(x,y) \
     (assert((y) != NULL),\
      strtok((x), (y)))
#define trio_to_long(x,y,n) \
     (assert((x) != NULL),\
      assert((y) != NULL),\
      assert((n) >= 2 && (n) <= 36),\
      strtol((x), (y), (n)))
#define trio_to_unsigned_long(x,y,n) \
     (assert((x) != NULL),\
      assert((y) != NULL),\
      assert((n) >= 2 && (n) <= 36),\
      strtoul((x), (y), (n)))
#endif /* DEBUG */

char *trio_append_max(char *target, size_t max, const char *source);
char *trio_copy_max(char *target, size_t max, const char *source);
char *trio_duplicate(const char *source);
char *trio_duplicate_max(const char *source, size_t max);
int trio_equal(const char *first, const char *second);
int trio_equal_case(const char *first, const char *second);
int trio_equal_case_max(const char *first, size_t max, const char *second);
int trio_equal_locale(const char *first, const char *second);
int trio_equal_max(const char *first, size_t max, const char *second);
const char *trio_error(int);
size_t trio_format_date_max(char *target, size_t max, const char *format, const struct tm *datetime);
unsigned long trio_hash(const char *string, int type);
int trio_lower(char *target);
int trio_match(const char *string, const char *pattern);
int trio_match_case(const char *string, const char *pattern);
size_t trio_span_function(char *target, const char *source, int (*Function)(int));
char *trio_substring_max(const char *string, size_t max, const char *find);
float trio_to_float(const char *source, const char **target);
double trio_to_double(const char *source, const char **target);
int trio_upper(char *target);

/*************************************************************************
 * Dynamic string functions
 */

typedef struct _trio_string_t trio_string_t;

trio_string_t *trio_string_create(int initial_size);
void trio_string_destroy(trio_string_t *self);

char *trio_string_get(trio_string_t *self);
void trio_string_set(trio_string_t *self, char *buffer);
char *trio_string_extract(trio_string_t *self);
void trio_string_terminate(trio_string_t *self);

char *trio_string_append(trio_string_t *self, trio_string_t *other);
int trio_string_contains(trio_string_t *self, trio_string_t *other);
char *trio_string_copy(trio_string_t *self, trio_string_t *other);
trio_string_t *trio_string_duplicate(trio_string_t *other);
int trio_string_equal(trio_string_t *self, trio_string_t *other);
int trio_string_equal_max(trio_string_t *self, size_t max, trio_string_t *second);
int trio_string_equal_case(trio_string_t *self, trio_string_t *other);
int trio_string_equal_case_max(trio_string_t *self, size_t max, trio_string_t *other);
size_t trio_string_format_date_max(trio_string_t *self, size_t max, const char *format, const struct tm *datetime);
char *trio_string_index(trio_string_t *self, int character);
char *trio_string_index_last(trio_string_t *self, int character);
int trio_string_length(trio_string_t *self);
int trio_string_lower(trio_string_t *self);
int trio_string_match(trio_string_t *self, trio_string_t *other);
int trio_string_match_case(trio_string_t *self, trio_string_t *other);
char *trio_string_substring(trio_string_t *self, trio_string_t *other);
int trio_string_upper(trio_string_t *self);

void trio_xstring_append_char(trio_string_t *self, char character);
char *trio_xstring_append(trio_string_t *self, const char *other);
int trio_xstring_contains(trio_string_t *self, const char *other);
char *trio_xstring_copy(trio_string_t *self, const char *other);
trio_string_t *trio_xstring_duplicate(const char *other);
int trio_xstring_equal(trio_string_t *self, const char *other);
int trio_xstring_equal_max(trio_string_t *self, size_t max, const char *other);
int trio_xstring_equal_case(trio_string_t *self, const char *other);
int trio_xstring_equal_case_max(trio_string_t *self, size_t max, const char *other);
int trio_xstring_match(trio_string_t *self, const char *other);
int trio_xstring_match_case(trio_string_t *self, const char *other);
char *trio_xstring_substring(trio_string_t *self, const char *other);

#endif /* TRIO_TRIOSTR_H */
