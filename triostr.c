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

/*************************************************************************
 * Include files
 */

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "triostr.h"

/*************************************************************************
 * Definitions
 */

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
# if defined(TRIO_PLATFORM_QNX)
#  define strcasecmp(x,y) stricmp(x,y)
#  define strncasecmp(x,y,n) strnicmp(x,y,n)
# endif
#elif defined(TRIO_PLATFORM_WIN32)
# define USE_STRCASECMP
# define strcasecmp(x,y) strcmpi(x,y)
#endif

/*************************************************************************
 * Structures
 */

struct _trio_string_t
{
  char *content;
  size_t length;
  size_t allocated;
};

/*************************************************************************
 * Static String Functions
 */

#if defined(TRIO_DOCUMENTATION)
# include "doc/doc_static.h"
#endif
/** @addtogroup StaticStrings
    @{
*/

/*
 * TrioDuplicate
 */
TRIO_PRIVATE char *
TrioDuplicateMax(const char *source, size_t size)
{
  char *target;

  assert(source);

  /* Make room for string plus a terminating zero */
  size++;
  target = trio_create(size);
  if (target)
    {
      trio_copy_max(target, size, source);
    }
  return target;
}


/**
   Append @p source at the end of @p target.
   
   @param target Target string.
   @param source Source string.
   @return Boolean value indicating success or failure.
   
   @pre @p target must point to a memory chunk with sufficient room to
   contain the @p target string and @p source string.
   @pre No boundary checking is performed, so insufficient memory will
   result in a buffer overrun.
   @post @p target will be zero terminated.
*/
TRIO_PUBLIC int
trio_append(char *target,
	    const char *source)
{
  assert(target);
  assert(source);
  
  return (strcat(target, source) != NULL);
}


/**
   Append at most @p max characters from @p source to @p target.
   
   @param target Target string.
   @param max Maximum number of characters to append.
   @param source Source string.
   @return Boolean value indicating success or failure.
   
   @pre @p target must point to a memory chuck with sufficient room to
   contain the @p target string and the @p source string (at most @p max
   characters).
   @pre No boundary checking is performed, so insufficient memory will
   result in a buffer overrun.
   @post @p target will be zero terminated.
*/
TRIO_PUBLIC int
trio_append_max(char *target,
		size_t max,
		const char *source)
{
  size_t length;
  
  assert(target);
  assert(source);

  length = trio_length(target);
  
  if (max > length)
    {
      strncat(target, source, max - length - 1);
    }
  return TRUE;
}


/**
   Copy @p source to @p target.
   
   @param target Target string.
   @param source Source string.
   @return Boolean value indicating success or failure.
   
   @pre @p target must point to a memory chunk with sufficient room to
   contain the @p source string.
   @pre No boundary checking is performed, so insufficient memory will
   result in a buffer overrun.
   @post @p target will be zero terminated.
*/
TRIO_PUBLIC int
trio_copy(char *target,
	  const char *source)
{
  assert(target);
  assert(source);

  (void)strcpy(target, source);
  return TRUE;
}


/**
   Copy at most @p max characters from @p source to @p target.
   
   @param target Target string.
   @param max Maximum number of characters to append.
   @param source Source string.
   @return Boolean value indicating success or failure.
   
   @pre @p target must point to a memory chunk with sufficient room to
   contain the @p source string (at most @p max characters).
   @pre No boundary checking is performed, so insufficient memory will
   result in a buffer overrun.
   @post @p target will be zero terminated.
*/
TRIO_PUBLIC int
trio_copy_max(char *target,
	      size_t max,
	      const char *source)
{
  assert(target);
  assert(source);
  assert(max > 0); /* Includes != 0 */

  (void)strncpy(target, source, max - 1);
  target[max - 1] = (char)0;
  return TRUE;
}


/**
   Duplicate @p source.
   
   @param source Source string.
   @return A copy of the @p source string.
   
   @post @p target will be zero terminated.
*/
TRIO_PUBLIC char *
trio_duplicate(const char *source)
{
  return TrioDuplicateMax(source, trio_length(source));
}


/**
   Duplicate at most @p max characters of @p source.
   
   @param source Source string.
   @param max Maximum number of characters to duplicate.
   @return A copy of the @p source string.
   
   @post @p target will be zero terminated.
*/
TRIO_PUBLIC char *
trio_duplicate_max(const char *source,
		   size_t max)
{
  size_t length;

  assert(source);
  assert(max > 0);

  length = trio_length(source);
  if (length > max)
    {
      length = max;
    }
  return TrioDuplicateMax(source, length);
}


/**
   Compare if two strings are equal.
   
   @param first First string.
   @param second Second string.
   @return Boolean indicating whether the two strings are equal or not.
   
   Case-insensitive comparison.
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


/*
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


/*
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


/*
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


/*
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


/*
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


/*
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


/*
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


/*
 * trio_lower
 */
TRIO_PUBLIC int
trio_lower(char *target)
{
  assert(target);

  return trio_span_function(target, target, tolower);
}


/*
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


/*
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


/*
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


/*
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


/*
 * trio_to_double
 *
 * @internal
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


/*
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


/*
 * trio_upper
 */
TRIO_PUBLIC int
trio_upper(char *target)
{
  assert(target);

  return trio_span_function(target, target, toupper);
}


/** @} End of StaticStrings */


/*************************************************************************
 * Dynamic String Functions
 */

#if defined(TRIO_DOCUMENTATION)
# include "doc/doc_dynamic.h"
#endif
/** @addtogroup DynamicStrings
    @{
*/

/*
 * TrioStringAlloc
 */
TRIO_PRIVATE trio_string_t *
TrioStringAlloc(void)
{
  trio_string_t *self;
  
  self = (trio_string_t *)TRIO_MALLOC(sizeof(trio_string_t));
  if (self)
    {
      self->content = NULL;
      self->length = 0;
      self->allocated = 0;
    }
  return self;
}


/*
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
  char *new_content;
  size_t new_size;

  new_size = (delta == 0)
    ? ( (self->allocated == 0) ? 1 : self->allocated * 2 )
    : self->allocated + delta;
  
  new_content = (char *)TRIO_REALLOC(self->content, new_size);
  if (new_content)
    {
      self->content = new_content;
      self->allocated = new_size;
      status = TRUE;
    }
  return status;
}


/**
   Create a new dynamic string.
   
   @param initial_size Initial size of the buffer.
   @return Newly allocated dynamic string, or NULL if memory allocation failed.
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
	  self->content[0] = (char)0;
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


/**
   Deallocate the dynamic string and its contents.
   
   @param self Dynamic string
*/
TRIO_PUBLIC void
trio_string_destroy(trio_string_t *self)
{
  assert(self);
  
  if (self)
    {
      trio_destroy(self->content);
      TRIO_FREE(self);
    }
}


/**
   Get a pointer to the content.
   
   @param self Dynamic string.
   @param offset Offset into content.
   @return Pointer to the content.
   
   @p Offset can be zero, positive, or negative. If @p offset is zero,
   then the start of the content will be returned. If @p offset is positive,
   then a pointer to @p offset number of characters from the beginning of the
   content is returned. If @p offset is negative, then a pointer to @p offset
   number of characters from the ending of the string, starting at the
   terminating zero, is returned.
*/
TRIO_PUBLIC char *
trio_string_get(trio_string_t *self, int offset)
{
  char *result = NULL;
  
  assert(self);

  if (self->content != NULL)
    {
      if (self->length == 0)
	{
	  (void)trio_string_length(self);
	}
      if (offset >= 0)
	{
	  if (offset > self->length)
	    {
	      offset = self->length;
	    }
	}
      else
	{
	  offset += self->length + 1;
	  if (offset < 0)
	    {
	      offset = 0;
	    }
	}
      result = &(self->content[offset]);
    }
  return result;
}


/**
   Extract the content.
   
   @param self Dynamic String
   @return Content of dynamic string.
   
   The content is removed from the dynamic string. This enables destruction
   of the dynamic string without deallocation of the content.
*/
TRIO_PUBLIC char *
trio_string_extract(trio_string_t *self)
{
  char *result;
  
  assert(self);

  result = self->content;
  /* FIXME: Allocate new empty buffer? */
  self->content = NULL;
  self->length = self->allocated = 0;
  return result;
}


/**
   Set the content of the dynamic string.
   
   @param self Dynamic String
   @param buffer The new content.
   
   Sets the content of the dynamic string to a copy @p buffer.
   An existing content will be deallocated first, if necessary.
   
   @remark
   This function will make a copy of @p buffer.
   You are responsible for deallocating @p buffer yourself.
*/
TRIO_PUBLIC void
trio_xstring_set(trio_string_t *self,
		 char *buffer)
{
  assert(self);

  trio_destroy(self->content);
  self->content = trio_duplicate(buffer);
}


/*
 * trio_string_size
 */
TRIO_PUBLIC int
trio_string_size(trio_string_t *self)
{
  assert(self);

  return self->allocated;
}


/*
 * trio_string_terminate
 */
TRIO_PUBLIC void
trio_string_terminate(trio_string_t *self)
{
  char *end;
  
  assert(self);

  end = trio_string_get(self, -1);
  if (end)
    {
      *end = NIL;
    }
}


/**
   Append the second string to the first.
   
   @param self Dynamic string to be modified.
   @param other Dynamic string to copy from.
   @return Boolean value indicating success or failure.
*/
TRIO_PUBLIC int
trio_string_append(trio_string_t *self,
		   trio_string_t *other)
{
  int delta;
  
  assert(self);
  assert(other);

  delta = trio_string_size(self) - (self->length + other->length);
  if (delta < 0)
    {
      if (!TrioStringGrow(self, -delta))
	goto error;
    }
  trio_copy(&self->content[self->length], other->content);
  self->length += other->length;
  return TRUE;
  
 error:
  return FALSE;
}


/*
 * trio_xstring_append
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
  delta = trio_string_size(self) - (self->length + length);
  if (delta < 0)
    {
      if (!TrioStringGrow(self, -delta))
	goto error;
    }
  trio_copy(&self->content[self->length], other);
  self->length += length;
  return TRUE;
  
 error:
  return FALSE;
}


/*
 * trio_xstring_append_char
 */
TRIO_PUBLIC int
trio_xstring_append_char(trio_string_t *self,
			 char character)
{
  assert(self);

  if (self->length >= trio_string_size(self))
    {
      if (!TrioStringGrow(self, 0))
	goto error;
    }
  self->content[self->length] = character;
  self->length++;
  return TRUE;
  
 error:
  return FALSE;
}


/**
   Search for the first occurrence of second parameter in the first.
   
   @param self Dynamic string to be modified.
   @param other Dynamic string to copy from.
   @return Boolean value indicating success or failure.
*/
TRIO_PUBLIC int
trio_string_contains(trio_string_t *self,
		     trio_string_t *other)
{
  assert(self);
  assert(other);

  return trio_contains(self->content, other->content);
}


/*
 * trio_xstring_contains
 */
TRIO_PUBLIC int
trio_xstring_contains(trio_string_t *self,
		      const char *other)
{
  assert(self);
  assert(other);

  return trio_contains(self->content, other);
}


/*
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


/*
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


/*
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
      self->content = TrioDuplicateMax(other->content, other->length);
      if (self->content)
	{
	  self->length = other->length;
	  self->allocated = self->length + 1;
	}
      else
	{
	  self->length = self->allocated = 0;
	}
    }
  return self;
}


/*
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
      self->content = TrioDuplicateMax(other, trio_length(other));
      if (self->content)
	{
	  self->length = trio_length(self->content);
	  self->allocated = self->length + 1;
	}
      else
	{
	  self->length = self->allocated = 0;
	}
    }
  return self;
}


/*
 * trio_string_equal
 */
TRIO_PUBLIC int
trio_string_equal(trio_string_t *self,
		  trio_string_t *other)
{
  assert(self);
  assert(other);

  return trio_equal(self->content, other->content);
}


/*
 * trio_xstring_equal
 */
TRIO_PUBLIC int
trio_xstring_equal(trio_string_t *self,
		   const char *other)
{
  assert(self);
  assert(other);

  return trio_equal(self->content, other);
}


/*
 * trio_string_equal_max
 */
TRIO_PUBLIC int
trio_string_equal_max(trio_string_t *self,
		      size_t max,
		      trio_string_t *other)
{
  assert(self);
  assert(other);

  return trio_equal_max(self->content, max, other->content);
}


/*
 * trio_xstring_equal_max
 */
TRIO_PUBLIC int
trio_xstring_equal_max(trio_string_t *self,
		       size_t max,
		       const char *other)
{
  assert(self);
  assert(other);

  return trio_equal_max(self->content, max, other);
}


/*
 * trio_string_equal_case
 */
TRIO_PUBLIC int
trio_string_equal_case(trio_string_t *self,
		       trio_string_t *other)
{
  assert(self);
  assert(other);

  return trio_equal_case(self->content, other->content);
}


/*
 * trio_xstring_equal_case
 */
TRIO_PUBLIC int
trio_xstring_equal_case(trio_string_t *self,
			const char *other)
{
  assert(self);
  assert(other);

  return trio_equal_case(self->content, other);
}


/*
 * trio_string_equal_case_max
 */
TRIO_PUBLIC int
trio_string_equal_case_max(trio_string_t *self,
			   size_t max,
			   trio_string_t *other)
{
  assert(self);
  assert(other);

  return trio_equal_case_max(self->content, max, other->content);
}


/*
 * trio_xstring_equal_case_max
 */
TRIO_PUBLIC int
trio_xstring_equal_case_max(trio_string_t *self,
			    size_t max,
			    const char *other)
{
  assert(self);
  assert(other);

  return trio_equal_case_max(self->content, max, other);
}


/*
 * trio_string_format_data_max
 */
TRIO_PUBLIC size_t
trio_string_format_date_max(trio_string_t *self,
			    size_t max,
			    const char *format,
			    const struct tm *datetime)
{
  assert(self);

  return trio_format_date_max(self->content, max, format, datetime);
}


/*
 * trio_string_index
 */
TRIO_PUBLIC char *
trio_string_index(trio_string_t *self,
		  int character)
{
  assert(self);

  return trio_index(self->content, character);
}


/*
 * trio_string_index_last
 */
TRIO_PUBLIC char *
trio_string_index_last(trio_string_t *self,
		       int character)
{
  assert(self);

  return trio_index_last(self->content, character);
}


/*
 * trio_string_length
 */
TRIO_PUBLIC int
trio_string_length(trio_string_t *self)
{
  assert(self);

  if (self->length == 0)
    {
      self->length = trio_length(self->content);
    }
  return self->length;
}


/*
 * trio_string_lower
 */
TRIO_PUBLIC int
trio_string_lower(trio_string_t *self)
{
  assert(self);

  return trio_lower(self->content);
}


/*
 * trio_string_match
 */
TRIO_PUBLIC int
trio_string_match(trio_string_t *self,
		  trio_string_t *other)
{
  assert(self);
  assert(other);

  return trio_match(self->content, other->content);
}


/*
 * trio_xstring_match
 */
TRIO_PUBLIC int
trio_xstring_match(trio_string_t *self,
		   const char *other)
{
  assert(self);
  assert(other);

  return trio_match(self->content, other);
}


/*
 * trio_string_match_case
 */
TRIO_PUBLIC int
trio_string_match_case(trio_string_t *self,
		       trio_string_t *other)
{
  assert(self);
  assert(other);

  return trio_match_case(self->content, other->content);
}


/*
 * trio_xstring_match_case
 */
TRIO_PUBLIC int
trio_xstring_match_case(trio_string_t *self,
			const char *other)
{
  assert(self);
  assert(other);

  return trio_match_case(self->content, other);
}


/*
 * trio_string_substring
 */
TRIO_PUBLIC char *
trio_string_substring(trio_string_t *self,
		      trio_string_t *other)
{
  assert(self);
  assert(other);

  return trio_substring(self->content, other->content);
}


/*
 * trio_xstring_substring
 */
TRIO_PUBLIC char *
trio_xstring_substring(trio_string_t *self,
		       const char *other)
{
  assert(self);
  assert(other);

  return trio_substring(self->content, other);
}


/*
 * trio_string_upper
 */
TRIO_PUBLIC int
trio_string_upper(trio_string_t *self)
{
  assert(self);

  return trio_upper(self->content);
}

/** @} End of DynamicStrings */
