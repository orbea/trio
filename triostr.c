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

static const char rcsid[] = "@(#)$Id$";

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "triostr.h"

#ifndef NULL
# define NULL 0
#endif
#define NIL ((char)0)
#ifndef FALSE
# define FALSE (1 == 0)
# define TRUE (! FALSE)
#endif
#define BOOLEAN_T int

#if defined(TRIO_PLATFORM_UNIX)
# define USE_STRCASECMP
# define USE_STRNCASECMP
# define USE_STRERROR
# if defined(__QNX__)
#  define strcasecmp(x,y) stricmp(x,y)
#  define strncasecmp(x,y,n) strnicmp(x,y,n)
# endif
#elif defined(TRIO_PLATFORM_WIN32)
# define USE_STRCASECMP
# define strcasecmp(x,y) strcmpi(x,y)
#endif

/*************************************************************************
 * Dynamic string
 */

struct _trio_string_t
{
  char *buffer;
  size_t length;
  size_t allocated;
};

/*************************************************************************
 *
 * @STATIC STRING FUNCTIONS
 *
 ************************************************************************/

/*************************************************************************
 * trio_append_max
 */
TRIO_PUBLIC char *
trio_append_max(char *target,
		size_t max,
		const char *source)
{
  assert(target);
  assert(source);
  assert(max > 0);

  max -= trio_length(target) + 1;
  return (max > 0) ? strncat(target, source, max) : target;
}

/*************************************************************************
 * trio_copy_max
 */
TRIO_PUBLIC char *
trio_copy_max(char *target,
	      size_t max,
	      const char *source)
{
  assert(target);
  assert(source);
  assert(max > 0); /* Includes != 0 */

  target = strncpy(target, source, max - 1);
  target[max - 1] = (char)0;
  return target;
}

/*************************************************************************
 * trio_duplicate
 */
TRIO_PUBLIC char *
trio_duplicate(const char *source)
{
  char *target;

  assert(source);

  target = trio_create(trio_length(source) + 1);
  if (target)
    {
      trio_copy(target, source);
    }
  return target;
}

/*************************************************************************
 * trio_duplicate_max
 */
TRIO_PUBLIC char *
trio_duplicate_max(const char *source,
		   size_t max)
{
  char *target;
  size_t length;

  assert(source);
  assert(max > 0);

  /* Make room for string plus a terminating zero */
  length = trio_length(source) + 1;
  if (length > max)
    {
      length = max;
    }
  target = trio_create(length);
  if (target)
    {
      trio_copy_max(target, length, source);
    }
  return target;
}

/*************************************************************************
 * trio_equal
 */
TRIO_PUBLIC int
trio_equal(const char *first,
	   const char *second)
{
  assert(first);
  assert(second);

  if ((first != NULL) && (second != NULL))
    {
#if defined(USE_STRCASECMP)
      return (0 == strcasecmp(first, second));
#else
      while ((*first != NIL) && (*second != NIL))
	{
	  if (toupper(*first) != toupper(*second))
	    {
	      break;
	    }
	  first++;
	  second++;
	}
      return ((*first == NIL) && (*second == NIL));
#endif
    }
  return FALSE;
}

/*************************************************************************
 * trio_equal_case
 */
TRIO_PUBLIC int
trio_equal_case(const char *first,
		const char *second)
{
  assert(first);
  assert(second);

  if ((first != NULL) && (second != NULL))
    {
      return (0 == strcmp(first, second));
    }
  return FALSE;
}

/*************************************************************************
 * trio_equal_case_max
 */
TRIO_PUBLIC int
trio_equal_case_max(const char *first,
		    size_t max,
		    const char *second)
{
  assert(first);
  assert(second);

  if ((first != NULL) && (second != NULL))
    {
      return (0 == strncmp(first, second, max));
    }
  return FALSE;
}

/*************************************************************************
 * trio_equal_locale
 */
TRIO_PUBLIC int
trio_equal_locale(const char *first,
		  const char *second)
{
  assert(first);
  assert(second);

#if defined(LC_COLLATE)
  return (strcoll(first, second) == 0);
#else
  return trio_equal(first, second);
#endif
}

/*************************************************************************
 * trio_equal_max
 */
TRIO_PUBLIC int
trio_equal_max(const char *first,
	       size_t max,
	       const char *second)
{
  assert(first);
  assert(second);

  if ((first != NULL) && (second != NULL))
    {
#if defined(USE_STRNCASECMP)
      return (0 == strncasecmp(first, second, max));
#else
      /* Not adequately tested yet */
      size_t cnt = 0;
      while ((*first != NIL) && (*second != NIL) && (cnt <= max))
	{
	  if (toupper(*first) != toupper(*second))
	    {
	      break;
	    }
	  first++;
	  second++;
	  cnt++;
	}
      return ((cnt == max) || ((*first == NIL) && (*second == NIL)));
#endif
    }
  return FALSE;
}

/*************************************************************************
 * trio_error
 */
TRIO_PUBLIC const char *
trio_error(int errorNumber)
{
#if defined(USE_STRERROR)
  return strerror(errorNumber);
#else
  return "unknown";
#endif
}

/*************************************************************************
 * trio_format_date_max
 */
TRIO_PUBLIC size_t
trio_format_date_max(char *target,
		     size_t max,
		     const char *format,
		     const struct tm *datetime)
{
  assert(target);
  assert(format);
  assert(datetime);
  assert(max > 0);
  
  return strftime(target, max, format, datetime);
}

/*************************************************************************
 * trio_hash
 */
TRIO_PUBLIC unsigned long
trio_hash(const char *string,
	  int type)
{
  unsigned long value = 0L;
  char ch;

  assert(string);
  
  switch (type)
    {
    case TRIO_HASH_PLAIN:
      while ( (ch = *string++) != NIL )
	{
	  value *= 31;
	  value += (unsigned long)ch;
	}
      break;
    default:
      assert(FALSE);
      break;
    }
  return value;
}

/*************************************************************************
 * trio_lower
 */
TRIO_PUBLIC int
trio_lower(char *target)
{
  assert(target);

  return trio_span_function(target, target, tolower);
}

/*************************************************************************
 * trio_match
 */
TRIO_PUBLIC int
trio_match(const char *string,
	   const char *pattern)
{
  assert(string);
  assert(pattern);
  
  for (; ('*' != *pattern); ++pattern, ++string)
    {
      if (NIL == *string)
	{
	  return (NIL == *pattern);
	}
      if ((toupper((int)*string) != toupper((int)*pattern))
	  && ('?' != *pattern))
	{
	  return FALSE;
	}
    }
  /* two-line patch to prevent *too* much recursiveness: */
  while ('*' == pattern[1])
    pattern++;

  do
    {
      if ( trio_match(string, &pattern[1]) )
	{
	  return TRUE;
	}
    }
  while (*string++);
  
  return FALSE;
}

/*************************************************************************
 * trio_match_case
 */
TRIO_PUBLIC int
trio_match_case(const char *string,
		const char *pattern)
{
  assert(string);
  assert(pattern);
  
  for (; ('*' != *pattern); ++pattern, ++string)
    {
      if (NIL == *string)
	{
	  return (NIL == *pattern);
	}
      if ((*string != *pattern)
	  && ('?' != *pattern))
	{
	  return FALSE;
	}
    }
  /* two-line patch to prevent *too* much recursiveness: */
  while ('*' == pattern[1])
    pattern++;

  do
    {
      if ( trio_match_case(string, &pattern[1]) )
	{
	  return TRUE;
	}
    }
  while (*string++);
  
  return FALSE;
}

/*************************************************************************
 * trio_span_function
 *
 * Untested
 */
TRIO_PUBLIC size_t
trio_span_function(char *target,
		   const char *source,
		   int (*Function)(int))
{
  size_t count = 0;

  assert(target);
  assert(source);
  assert(Function);
  
  while (*source != NIL)
    {
      *target++ = Function(*source++);
      count++;
    }
  return count;
}

/*************************************************************************
 * trio_substring_max
 */
TRIO_PUBLIC char *
trio_substring_max(const char *string,
		   size_t max,
		   const char *find)
{
  size_t count;
  size_t size;
  char *result = NULL;

  assert(string);
  assert(find);
  
  size = trio_length(find);
  if (size <= max)
    {
      for (count = 0; count <= max - size; count++)
	{
	  if (trio_equal_max(find, size, &string[count]))
	    {
	      result = (char *)&string[count];
	      break;
	    }
	}
    }
  return result;
}

/*************************************************************************
 * trio_to_double
 *
 * double ::= [ <sign> ]
 *            ( <number> |
 *              <number> <decimal_point> <number> |
 *              <decimal_point> <number> )
 *            [ <exponential> [ <sign> ] <number> ]
 * number ::= 1*( <digit> )
 * digit ::= ( '0' | '1' | '2' | '3' | '4' | '5' | '6' | '7' | '8' | '9' )
 * exponential ::= ( 'e' | 'E' )
 * sign ::= ( '-' | '+' )
 * decimal_point ::= '.'
 */
TRIO_PUBLIC double
trio_to_double(const char *source,
	       const char **endp)
{
#if defined(TRIO_COMPILER_SUPPORTS_C99)
  return strtod(source, (char **)endp);
#else
  /* Preliminary code */
  int isNegative = FALSE;
  int isExponentNegative = FALSE;
  unsigned long integer = 0;
  unsigned long fraction = 0;
  unsigned long fracdiv = 1;
  unsigned long exponent = 0;
  double value = 0.0;

  /* First try hex-floats */
  if ((source[0] == '0') && ((source[1] == 'x') || (source[1] == 'X')))
    {
      source += 2;
      while (isxdigit((int)*source))
	{
	  integer *= 16;
	  integer += (isdigit((int)*source)
		      ? (*source - '0')
		      : 10 + (toupper((int)*source) - 'A'));
	  source++;
	}
      if (*source == '.')
	{
	  source++;
	  while (isxdigit((int)*source))
	    {
	      fraction *= 16;
	      fraction += (isdigit((int)*source)
			   ? (*source - '0')
			   : 10 + (toupper((int)*source) - 'A'));
	      fracdiv *= 16;
	      source++;
	    }
	  if ((*source == 'p') || (*source == 'P'))
	    {
	      source++;
	      if ((*source == '+') || (*source == '-'))
		{
		  isExponentNegative = (*source == '-');
		  source++;
		}
	      while (isdigit((int)*source))
		{
		  exponent *= 10;
		  exponent += (*source - '0');
		  source++;
		}
	    }
	}
    }
  else /* Then try normal decimal floats */
    {
      isNegative = (*source == '-');
      /* Skip sign */
      if ((*source == '+') || (*source == '-'))
	source++;

      /* Integer part */
      while (isdigit((int)*source))
	{
	  integer *= 10;
	  integer += (*source - '0');
	  source++;
	}

      if (*source == '.')
	{
	  source++; /* skip decimal point */
	  while (isdigit((int)*source))
	    {
	      fraction *= 10;
	      fraction += (*source - '0');
	      fracdiv *= 10;
	      source++;
	    }
	}
      if ((*source == 'e') || (*source == 'E'))
	{
	  source++; /* Skip exponential indicator */
	  isExponentNegative = (*source == '-');
	  if ((*source == '+') || (*source == '-'))
	    source++;
	  while (isdigit((int)*source))
	    {
	      exponent *= 10;
	      exponent += (*source - '0');
	      source++;
	    }
	}
    }
  
  value = (double)integer;
  if (fraction != 0)
    {
      value += (double)fraction / (double)fracdiv;
    }
  if (exponent != 0)
    {
      if (isExponentNegative)
	value /= pow((double)10, (double)exponent);
      else
	value *= pow((double)10, (double)exponent);
    }
  if (isNegative)
    value = -value;

  if (endp)
    *endp = source;
  return value;
#endif
}

/*************************************************************************
 * trio_to_float
 */
TRIO_PUBLIC float
trio_to_float(const char *source,
	      const char **endp)
{
#if defined(TRIO_COMPILER_SUPPORTS_C99)
  return strtof(source, (char **)endp);
#else
  return (float)trio_to_double(source, endp);
#endif
}

/*************************************************************************
 * trio_upper
 */
TRIO_PUBLIC int
trio_upper(char *target)
{
  assert(target);

  return trio_span_function(target, target, toupper);
}

/*************************************************************************
 *
 * @DYNAMIC STRING FUNCTIONS
 *
 ************************************************************************/

/*************************************************************************
 * TrioStringAlloc
 */
TRIO_PRIVATE trio_string_t *
TrioStringAlloc(void)
{
  trio_string_t *self;
  
  self = (trio_string_t *)TRIO_MALLOC(sizeof(trio_string_t));
  if (self)
    {
      self->buffer = NULL;
      self->length = 0;
      self->allocated = 0;
    }
  return self;
}

/*************************************************************************
 * TrioStringGrow
 *
 * The size of the string will be increased by 'delta' characters. If
 * 'delta' is zero, the size will be doubled.
 */
TRIO_PRIVATE BOOLEAN_T
TrioStringGrow(trio_string_t *self,
	       size_t delta)
{
  BOOLEAN_T status = FALSE;
  char *new_buffer;
  size_t new_size;

  new_size = (delta == 0)
    ? self->allocated * 2
    : self->allocated + delta;
  
  new_buffer = (char *)TRIO_REALLOC(self->buffer, new_size);
  if (new_buffer)
    {
      self->buffer = new_buffer;
      self->allocated = new_size;
      status = TRUE;
    }
  return status;
}

/*************************************************************************
 * trio_string_create
 */
TRIO_PUBLIC trio_string_t *
trio_string_create(int initial_size)
{
  trio_string_t *self;

  self = TrioStringAlloc();
  if (self)
    {
      if (TrioStringGrow(self,
			 (size_t)((initial_size > 0) ? initial_size : 1)))
	{
	  self->buffer[0] = (char)0;
	  self->allocated = initial_size;
	}
      else
	{
	  trio_string_destroy(self);
	  self = NULL;
	}
    }
  return self;
}

/*************************************************************************
 * trio_string_destroy
 */
TRIO_PUBLIC void
trio_string_destroy(trio_string_t *self)
{
  assert(self);
  
  if (self)
    {
      if (self->buffer)
	{
	  trio_destroy(self->buffer);
	}
      TRIO_FREE(self);
    }
}

/*************************************************************************
 * trio_string_get
 */
TRIO_PUBLIC char *
trio_string_get(trio_string_t *self)
{
  assert(self);
  
  return self->buffer;
}

/*************************************************************************
 * trio_string_extract
 */
TRIO_PUBLIC char *
trio_string_extract(trio_string_t *self)
{
  char *result;
  
  assert(self);

  result = self->buffer;
  self->buffer = NULL;
  self->length = self->allocated = 0;
  return result;
}

/*************************************************************************
 * trio_string_set
 */
TRIO_PUBLIC void
trio_string_set(trio_string_t *self,
		char *buffer)
{
  assert(self);

  if (self->buffer)
    {
      trio_destroy(self->buffer);
    }
  self->buffer = buffer;
}

/*************************************************************************
 * trio_string_terminate
 */
TRIO_PUBLIC void
trio_string_terminate(trio_string_t *self)
{
  assert(self);

  if (self->buffer)
    {
      self->buffer[self->length] = NIL;
    }
}

/*************************************************************************
 * trio_string_append
 */
TRIO_PUBLIC int
trio_string_append(trio_string_t *self,
		   trio_string_t *other)
{
  int delta;
  
  assert(self);
  assert(other);

  delta = self->allocated - (self->length + other->length);
  if (delta < 0)
    {
      if (!TrioStringGrow(self, delta))
	goto error;
    }
  trio_copy(&self->buffer[self->length], other->buffer);
  self->length += other->length;
  return TRUE;
  
 error:
  return FALSE;
}

/*************************************************************************
 * trio_string_append
 */
TRIO_PUBLIC int
trio_xstring_append(trio_string_t *self,
		    const char *other)
{
  size_t length;
  int delta;
  
  assert(self);
  assert(other);

  length = trio_length(other);
  delta = self->allocated - (self->length + length);
  if (delta < 0)
    {
      if (!TrioStringGrow(self, delta))
	goto error;
    }
  trio_copy(&self->buffer[self->length], other);
  self->length += length;
  return TRUE;
  
 error:
  return FALSE;
}

/*************************************************************************
 * trio_xstring_append_char
 */
TRIO_PUBLIC void
trio_xstring_append_char(trio_string_t *self,
			 char character)
{
  assert(self);

  if (self->length > self->allocated)
    {
      if (!TrioStringGrow(self, 0))
	goto error;
    }
  self->buffer[self->length] = character;
  self->length++;
 error:
  ;
}

/*************************************************************************
 * trio_string_contains
 */
TRIO_PUBLIC int
trio_string_contains(trio_string_t *self,
		     trio_string_t *other)
{
  assert(self);
  assert(other);

  return trio_contains(self->buffer, other->buffer);
}

/*************************************************************************
 * trio_xstring_contains
 */
TRIO_PUBLIC int
trio_xstring_contains(trio_string_t *self,
		      const char *other)
{
  assert(self);
  assert(other);

  return trio_contains(self->buffer, other);
}

/*************************************************************************
 * trio_string_copy
 */
TRIO_PUBLIC int
trio_string_copy(trio_string_t *self,
		 trio_string_t *other)
{
  assert(self);
  assert(other);

  self->length = 0;
  return trio_string_append(self, other);
}

/*************************************************************************
 * trio_xstring_copy
 */
TRIO_PUBLIC int
trio_xstring_copy(trio_string_t *self,
		  const char *other)
{
  assert(self);
  assert(other);

  self->length = 0;
  return trio_xstring_append(self, other);
}

/*************************************************************************
 * trio_string_duplicate
 */
TRIO_PUBLIC trio_string_t *
trio_string_duplicate(trio_string_t *other)
{
  trio_string_t *self;
  
  assert(other);

  self = TrioStringAlloc();
  if (self)
    {
      self->buffer = trio_duplicate(other->buffer);
      self->length = self->allocated = (self->buffer)
	? other->length
	: 0;
    }
  return self;
}

/*************************************************************************
 * trio_xstring_duplicate
 */
TRIO_PUBLIC trio_string_t *
trio_xstring_duplicate(const char *other)
{
  trio_string_t *self;
  
  assert(other);

  self = TrioStringAlloc();
  if (self)
    {
      self->buffer = trio_duplicate(other);
      self->length = self->allocated = (self->buffer)
	? trio_length(self->buffer)
	: 0;
    }
  return self;
}

/*************************************************************************
 * trio_string_equal
 */
TRIO_PUBLIC int
trio_string_equal(trio_string_t *self,
		  trio_string_t *other)
{
  assert(self);
  assert(other);

  return trio_equal(self->buffer, other->buffer);
}

/*************************************************************************
 * trio_xstring_equal
 */
TRIO_PUBLIC int
trio_xstring_equal(trio_string_t *self,
		   const char *other)
{
  assert(self);
  assert(other);

  return trio_equal(self->buffer, other);
}

/*************************************************************************
 * trio_string_equal_max
 */
TRIO_PUBLIC int
trio_string_equal_max(trio_string_t *self,
		      size_t max,
		      trio_string_t *other)
{
  assert(self);
  assert(other);

  return trio_equal_max(self->buffer, max, other->buffer);
}

/*************************************************************************
 * trio_xstring_equal_max
 */
TRIO_PUBLIC int
trio_xstring_equal_max(trio_string_t *self,
		       size_t max,
		       const char *other)
{
  assert(self);
  assert(other);

  return trio_equal_max(self->buffer, max, other);
}

/*************************************************************************
 * trio_string_equal_case
 */
TRIO_PUBLIC int
trio_string_equal_case(trio_string_t *self,
		       trio_string_t *other)
{
  assert(self);
  assert(other);

  return trio_equal_case(self->buffer, other->buffer);
}

/*************************************************************************
 * trio_xstring_equal_case
 */
TRIO_PUBLIC int
trio_xstring_equal_case(trio_string_t *self,
			const char *other)
{
  assert(self);
  assert(other);

  return trio_equal_case(self->buffer, other);
}

/*************************************************************************
 * trio_string_equal_case_max
 */
TRIO_PUBLIC int
trio_string_equal_case_max(trio_string_t *self,
			   size_t max,
			   trio_string_t *other)
{
  assert(self);
  assert(other);

  return trio_equal_case_max(self->buffer, max, other->buffer);
}

/*************************************************************************
 * trio_xstring_equal_case_max
 */
TRIO_PUBLIC int
trio_xstring_equal_case_max(trio_string_t *self,
			    size_t max,
			    const char *other)
{
  assert(self);
  assert(other);

  return trio_equal_case_max(self->buffer, max, other);
}

/*************************************************************************
 * trio_string_format_data_max
 */
TRIO_PUBLIC size_t
trio_string_format_date_max(trio_string_t *self,
			    size_t max,
			    const char *format,
			    const struct tm *datetime)
{
  assert(self);

  return trio_format_date_max(self->buffer, max, format, datetime);
}

/*************************************************************************
 * trio_string_index
 */
TRIO_PUBLIC char *
trio_string_index(trio_string_t *self,
		  int character)
{
  assert(self);

  return trio_index(self->buffer, character);
}

/*************************************************************************
 * trio_string_index_last
 */
TRIO_PUBLIC char *
trio_string_index_last(trio_string_t *self,
		       int character)
{
  assert(self);

  return trio_index_last(self->buffer, character);
}

/*************************************************************************
 * trio_string_length
 */
TRIO_PUBLIC int
trio_string_length(trio_string_t *self)
{
  assert(self);

  return (self->length == 0)
    ? trio_length(self->buffer)
    : self->length;
}

/*************************************************************************
 * trio_string_lower
 */
TRIO_PUBLIC int
trio_string_lower(trio_string_t *self)
{
  assert(self);

  return trio_lower(self->buffer);
}

/*************************************************************************
 * trio_string_match
 */
TRIO_PUBLIC int
trio_string_match(trio_string_t *self,
		  trio_string_t *other)
{
  assert(self);
  assert(other);

  return trio_match(self->buffer, other->buffer);
}

/*************************************************************************
 * trio_xstring_match
 */
TRIO_PUBLIC int
trio_xstring_match(trio_string_t *self,
		   const char *other)
{
  assert(self);
  assert(other);

  return trio_match(self->buffer, other);
}

/*************************************************************************
 * trio_string_match_case
 */
TRIO_PUBLIC int
trio_string_match_case(trio_string_t *self,
		       trio_string_t *other)
{
  assert(self);
  assert(other);

  return trio_match_case(self->buffer, other->buffer);
}

/*************************************************************************
 * trio_xstring_match_case
 */
TRIO_PUBLIC int
trio_xstring_match_case(trio_string_t *self,
			const char *other)
{
  assert(self);
  assert(other);

  return trio_match_case(self->buffer, other);
}

/*************************************************************************
 * trio_string_substring
 */
TRIO_PUBLIC char *
trio_string_substring(trio_string_t *self,
		      trio_string_t *other)
{
  assert(self);
  assert(other);

  return trio_substring(self->buffer, other->buffer);
}

/*************************************************************************
 * trio_xstring_substring
 */
TRIO_PUBLIC char *
trio_xstring_substring(trio_string_t *self,
		       const char *other)
{
  assert(self);
  assert(other);

  return trio_substring(self->buffer, other);
}

/*************************************************************************
 * trio_string_upper
 */
TRIO_PUBLIC int
trio_string_upper(trio_string_t *self)
{
  assert(self);

  return trio_upper(self->buffer);
}
