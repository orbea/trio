/*************************************************************************
 *
 * $Id$
 *
 * Copyright (C) 2000 Bjorn Reese and Daniel Stenberg.
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

#ifndef TRIO_TRIOP_H
#define TRIO_TRIOP_H

/*
 * Private functions, types, etc. used for callback functions.
 *
 * The ref pointer is an opaque type and should remain as such.
 * Private data must only be accessible through the getter and
 * setter functions.
 */

#if defined(__STDC__) && (__STDC_VERSION__ >= 199901L)
# define TRIO_C99
#endif
#define TRIO_BSD
#define TRIO_GNU
#define TRIO_MISC
#define TRIO_UNIX98
#define TRIO_EXTENSION
#define TRIO_ERRORS

enum trio_format_type {
  TRIO_FORMAT_UNKNOWN = 0,
  TRIO_FORMAT_INT,
  TRIO_FORMAT_DOUBLE,
  TRIO_FORMAT_CHAR,
  TRIO_FORMAT_STRING,
  TRIO_FORMAT_POINTER,
  TRIO_FORMAT_COUNT,
  TRIO_FORMAT_PARAMETER,
  TRIO_FORMAT_GROUP,
  TRIO_FORMAT_ERRNO,
  TRIO_FORMAT_USER_DEFINED,
  
  TRIO_FORMAT_LAST
};

typedef int (*trio_callback_t)(void *ref);

void *trio_register(trio_callback_t callback, const char *namespace);
void trio_unregister(void *handle);

const char *trio_get_format(void *ref);
void *trio_get_argument(void *ref);

/* Modifiers */
int  trio_get_width(void *ref);
void trio_set_width(void *ref, int width);
int  trio_get_precision(void *ref);
void trio_set_precision(void *ref, int precision);
int  trio_get_base(void *ref);
void trio_set_base(void *ref, int base);
int  trio_get_long(void *ref);
void trio_set_long(void *ref, int is_long);
int  trio_get_short(void *ref);
void trio_set_short(void *ref, int is_short);
int  trio_get_leftadjusted(void *ref);
void trio_set_leftadjusted(void *ref, int is_leftadjusted);
int  trio_get_space(void *ref);
void trio_set_space(void *ref, int is_space);
int  trio_get_showsign(void *ref);
void trio_set_showsign(void *ref, int is_showsign);
int  trio_get_nilpad(void *ref);
void trio_set_nilpad(void *ref, int is_nilpad);
int  trio_get_unsigned(void *ref);
void trio_set_unsigned(void *ref, int is_unsiged);
/*  int  trio_get_quote(void *ref); */
/*  void trio_set_quote(void *ref, int is_quote); */
/*  int  trio_get_upper(void *ref); */
/*  void trio_set_upper(void *ref, int is_upper); */
/*  int  trio_get_float_F(void *ref); */
/*  void trio_set_float_F(void *ref, int is_float_F); */
/*  int  trio_get_float_E(void *ref); */
/*  void trio_set_float_E(void *ref, int is_float_E); */
/*  int  trio_get_float_G(void *ref); */
/*  void trio_set_float_G(void *ref, int is_float_G); */

/* Printing */
int trio_print_ref(void *ref, const char *format, ...);
int trio_vprint_ref(void *ref, const char *format, va_list args);
int trio_printv_ref(void *ref, const char *format, void **args);

void trio_print_int(void *ref, int number);
void trio_print_uint(void *ref, unsigned int number);
/*  void trio_print_long(void *ref, long number); */
/*  void trio_print_ulong(void *ref, unsigned long number); */
void trio_print_double(void *ref, double number);
void trio_print_string(void *ref, char *string);
void trio_print_pointer(void *ref, void *pointer);

#endif /* TRIO_TRIOP_H */
