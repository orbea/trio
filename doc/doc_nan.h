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

/** @addtogroup SpecialQuantities Special Quantifies.
Functions to detect and fabricate special quantities in floating-point
numbers.

@b SYNOPSIS

@verbatim
cc ... -ltrio -lm

#include <trionan.h>
@endverbatim

@b DESCRIPTION

Certain arithmetical operations does not result in normal numbers. Instead
they result in special quantities that must be handled differently by the
floating-point hardware. These includes Infinity and Not-A-Number (NaN).

For example, 0/0 (zero divided by zero) yields NaN. Any operation which
involves a NaN will result in NaN. Any comparison involving NaN will be
unsuccessful, even if NaN is compared to NaN.

These special quantities are represented with special bit patterns by the
floating-point hardware, and this bit patterns depend on the hardware.
There may even be hardware that does not support special quantities, so
the functions in this module are not guaranteed to work on all platforms.

The approach used in this module is to (in decreasing order of importance)
@li Use C99 functionality when available.
@li Use IEEE 754-1985 bit patterns if possible.
@li Use platform-specific techniques.

@b NOTES

This module does not depend on the rest of trio, and can thus be reused
separately. The following files are necessary:
@li @c triodef.h
@li @c trionan.h
@li @c trionan.c

*/
