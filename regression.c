/*************************************************************************
 * Regression test
 */

static const char rcsid[] = "@(#)$Id$";

#include <stdarg.h>
#include <math.h>
#include <limits.h>
#include <float.h>
#include <errno.h>
#include "trio.h"
#include "triop.h"
#include "trionan.h"
#include "triostr.h"
#undef printf

#if TRIO_WIDECHAR
# include <wchar.h>
#endif

#define QUOTE(x) #x

#define DOUBLE_EQUAL(x,y) (((x)>(y)-DBL_EPSILON) && ((x)<(y)+DBL_EPSILON))
#define FLOAT_EQUAL(x,y) (((x)>(y)-FLT_EPSILON) && ((x)<(y)+FLT_EPSILON))

/*************************************************************************
 *
 */
void Dump(char *buffer, int rc)
{
  if (rc < 0)
    {
      printf("Err = %d (%s), Pos = %d\n",
	     TRIO_ERROR_CODE(rc),
	     TRIO_ERROR_NAME(rc),
	     TRIO_ERROR_POSITION(rc));
    }
  else if (buffer)
    printf("buffer[% 3d] = \"%s\"\n", rc, buffer);
}

/*************************************************************************
 *
 */
void Report(const char *file,
	    int line,
	    const char *expected,
	    const char *got)
{
  printf("Verification failed in %s:%d.\n", file, line);
  printf("  Expected \"%s\"\n", expected);
  printf("  Got      \"%s\"\n", got);
}

/*************************************************************************
 *
 */
int Verify(const char *file, int line,
	   const char *result,
	   const char *fmt, ...)
{
  int rc;
  va_list args;
  char buffer[4096];

  va_start(args, fmt);
  rc = trio_vsnprintf(buffer, sizeof(buffer), fmt, args);
  if (rc < 0)
    Dump(buffer, rc);
  va_end(args);

  if (!trio_equal_case(result, buffer))
    {
      Report(file, line, result, buffer);
      return 1;
    }
  return 0;
}

/*************************************************************************
 *
 */
int VerifyReturnValues()
{
  int nerr = 0;
  int rc;
  int count;
  char *expected;
  char buffer[4096];
  char result[4096];

  rc = trio_sprintf(buffer, "%s%n", "0123456789", &count);
  trio_sprintf(result, "%d %d %s", rc, count, buffer);
  expected = "10 10 0123456789";
  if (!trio_equal_case(result, expected))
    {
      nerr++;
      Report(__FILE__, __LINE__, expected, result);
    }
  
  rc = trio_snprintf(buffer, sizeof(buffer), "%s%n", "0123456789", &count);
  trio_sprintf(result, "%d %d %s", rc, count, buffer);
  expected = "10 10 0123456789";
  if (!trio_equal_case(result, expected))
    {
      nerr++;
      Report(__FILE__, __LINE__, expected, result);
    }
  
  rc = trio_snprintf(buffer, 4, "%s%n", "0123456789", &count);
  trio_sprintf(result, "%d %d %s", rc, count, buffer);
  expected = "10 3 012";
  if (!trio_equal_case(result, expected))
    {
      nerr++;
      Report(__FILE__, __LINE__, expected, result);
    }

  /* The output buffer contains the empty string */
  rc = trio_snprintf(buffer, 1, "%s%n", "0123456789", &count);
  trio_sprintf(result, "%d %d %s", rc, count, buffer);
  expected = "10 0 ";
  if (!trio_equal_case(result, expected))
    {
      nerr++;
      Report(__FILE__, __LINE__, expected, result);
    }

  /* The output buffer should be left untouched when max size is 0 */
  trio_sprintf(buffer, "DO NOT TOUCH");
  rc = trio_snprintf(buffer, 0, "%s%n", "0123456789", &count);
  trio_sprintf(result, "%d %d %s", rc, count, buffer);
  expected = "10 0 DO NOT TOUCH";
  if (!trio_equal_case(result, expected))
    {
      nerr++;
      Report(__FILE__, __LINE__, expected, result);
    }
  
  return nerr;
}

/*************************************************************************
 *
 */
#define TEST_STRING "0123456789"

int VerifyAllocate()
{
  int nerr = 0;
  int rc;
  char *string;
  int count;
  int test_size = sizeof(TEST_STRING) - 1;

  /* Allocate a string with the result */
  rc = trio_asprintf(&string, "%s%n", TEST_STRING, &count);
  if (rc < 0)
    {
      nerr++;
      Dump(string, rc);
    }
  else if (count != test_size)
    {
      nerr++;
      printf("Validation failed in %s:%d\n", __FILE__, __LINE__);
      printf("  Expected %%n = %d\n", test_size);
      printf("  Got      %%n = %d\n", count);
    }
  else if (!trio_equal_case(string, TEST_STRING))
    {
      nerr++;
      Report(__FILE__, __LINE__, TEST_STRING, string);
    }
  if (string)
    free(string);

  return nerr;
}


/*************************************************************************
 *
 */
int VerifyFormatting(void)
{
  int nerrors = 0;
  char buffer[256];

  /* Normal text */
  nerrors += Verify(__FILE__, __LINE__, "Hello world",
		   "Hello world");
  /* String */
  nerrors += Verify(__FILE__, __LINE__, "Hello world",
		   "%s", "Hello world");
  /* Pointer */
  nerrors += Verify(__FILE__, __LINE__, "Pointer (nil)",
		   "Pointer %p", 0);
  /* This may fail on 64 bit machines */
  nerrors += Verify(__FILE__, __LINE__, "Pointer 0x01234567",
		   "Pointer %p", 0x1234567);
  /* Nil pointer */
  nerrors += Verify(__FILE__, __LINE__, "Pointer (nil)",
		   "Pointer %p", NULL);
  /* Integer */
  nerrors += Verify(__FILE__, __LINE__, "Number 42",
		   "Number %d", 42);
  nerrors += Verify(__FILE__, __LINE__, "Number -42",
		   "Number %d", -42);
  nerrors += Verify(__FILE__, __LINE__, "Number 42",
		   "Number %ld", 42L);
  nerrors += Verify(__FILE__, __LINE__, "Number -42",
		   "Number %ld", -42L);
  /* Integer width */
  nerrors += Verify(__FILE__, __LINE__, "  1234",
		    "%6d", 1234);
  nerrors += Verify(__FILE__, __LINE__, "  1234",
		    "%*d", 6, 1234);
  /* Integer width overrun */
  nerrors += Verify(__FILE__, __LINE__, "123456",
		    "%4d", 123456);
  /* Integer precision */
  nerrors += Verify(__FILE__, __LINE__, "0012",
		    "%.4d", 12);
  nerrors += Verify(__FILE__, __LINE__, "0012",
		    "%.*d", 4, 12);
  nerrors += Verify(__FILE__, __LINE__, "  0012",
		    "%6.*d", 4, 12);
  nerrors += Verify(__FILE__, __LINE__, "  0012",
		    "%*.*d", 6, 4, 12);
  nerrors += Verify(__FILE__, __LINE__, "  0012",
		    "%*.*.*d", 6, 4, 2, 12);
  nerrors += Verify(__FILE__, __LINE__, "  0012",
		    "%*.*.*i", 6, 4, 10, 12);
  /* Integer sign, zero-padding, and width */
  nerrors += Verify(__FILE__, __LINE__, "+01234",
		    "%+06d", 1234);
  nerrors += Verify(__FILE__, __LINE__, " 01234",
		    "% 06d", 1234);
  nerrors += Verify(__FILE__, __LINE__, "+01234",
		    "% +06d", 1234);
  /* Integer adjust, zero-padding, and width */
  nerrors += Verify(__FILE__, __LINE__, "12      ",
		    "%-08d", 12);
  /* Integer zero-padding, width, and precision */
  nerrors += Verify(__FILE__, __LINE__, "  000012",
		    "%08.6d", 12);
#if 0 /* The output depends on the locale settings */
  /* Integer thousand separator */
  nerrors += Verify(__FILE__, __LINE__, "Number 1,000,000",
		    "Number %'d", 1000000);
#endif
  /* Integer base */
  nerrors += Verify(__FILE__, __LINE__, "42",
		   "%u", 42);
  nerrors += Verify(__FILE__, __LINE__, "-1",
		   "%d", -1);
  nerrors += Verify(__FILE__, __LINE__, "52",
		   "%o", 42);
  nerrors += Verify(__FILE__, __LINE__, "052",
		   "%#o", 42);
  nerrors += Verify(__FILE__, __LINE__, "2a",
		    "%x", 42);
  nerrors += Verify(__FILE__, __LINE__, "2A",
		    "%X", 42);
  nerrors += Verify(__FILE__, __LINE__, "0x2a",
		   "%#x", 42);
  nerrors += Verify(__FILE__, __LINE__, "0X2A",
		   "%#X", 42);
  nerrors += Verify(__FILE__, __LINE__, "0x00c ",
		   "%-#6.3x", 12);
  sprintf(buffer, "%u", UINT_MAX);
  nerrors += Verify(__FILE__, __LINE__, buffer,
		   "%u", -1);
  sprintf(buffer, "%x", UINT_MAX);
  nerrors += Verify(__FILE__, __LINE__, buffer,
		    "%x", -1);
  /* Double */
  nerrors += Verify(__FILE__, __LINE__, "3141.000000",
		    "%f", 3141.0);
  nerrors += Verify(__FILE__, __LINE__, "3141.500000",
		    "%f", 3141.5);
  nerrors += Verify(__FILE__, __LINE__, "123456789012345700000.000000",
		    "%f", 1.234567890123456789e20);
  nerrors += Verify(__FILE__, __LINE__, "0.000000",
		    "%f", 1.234567890123456789e-20);
  nerrors += Verify(__FILE__, __LINE__, "3.141000e+03",
		    "%e", 3141.0);
  nerrors += Verify(__FILE__, __LINE__, "     -2.3420e-02",
		    "%16.4e", -2.342E-02);
  nerrors += Verify(__FILE__, __LINE__, "     -2.3420e-22",
		    "%16.4e", -2.342E-22);
  nerrors += Verify(__FILE__, __LINE__, "3.141000E-44",
		    "%E", 3.141e-44);
  nerrors += Verify(__FILE__, __LINE__, "0",
		    "%g", 0.0);
  nerrors += Verify(__FILE__, __LINE__, "3141.5",
		    "%g", 3141.5);
  nerrors += Verify(__FILE__, __LINE__, "3.1415E-06",
		    "%G", 3.1415e-6);
  nerrors += Verify(__FILE__, __LINE__, "+3141.000000",
		    "%+f", 3141.0);
  nerrors += Verify(__FILE__, __LINE__, "-3141.000000",
		    "%+f", -3141.0);
  /* Double decimal point */
  nerrors += Verify(__FILE__, __LINE__, "3141",
		    "%.0f", 3141.0);
  nerrors += Verify(__FILE__, __LINE__, "3141.000",
		    "%.3f", 3141.0);
  nerrors += Verify(__FILE__, __LINE__, "3141.000000",
		    "%#f", 3141.0);
  nerrors += Verify(__FILE__, __LINE__, "0.0000",
		    "%#.4f", 0.0);
  nerrors += Verify(__FILE__, __LINE__, "0.000",
		    "%#.4g", 0.0);
  nerrors += Verify(__FILE__, __LINE__, "3141.0000",
		    "%#.4f", 3141.0);
  nerrors += Verify(__FILE__, __LINE__, "3141.",
		    "%#.0f", 3141.0);
  nerrors += Verify(__FILE__, __LINE__, "11.0000",
		    "%#.4f", 11.0);
  nerrors += Verify(__FILE__, __LINE__, "100.00",
		    "%.2f", 99.9999);
  nerrors += Verify(__FILE__, __LINE__, "100",
		    "%.2g", 99.9999);
  nerrors += Verify(__FILE__, __LINE__, "100.",
		    "%#.2g", 99.9999);
  nerrors += Verify(__FILE__, __LINE__, "0.123",
		    "%0g", 0.123);
  nerrors += Verify(__FILE__, __LINE__, "1",
		    "%.2g", 0.9999);
  nerrors += Verify(__FILE__, __LINE__, "0.01",
		    "%.2g", 0.01);
  nerrors += Verify(__FILE__, __LINE__, "0.010",
		    "%#.2g", 0.01);
  /* Double width, precision, and zero padding */
  nerrors += Verify(__FILE__, __LINE__, "00003.141500e+03",
		    "%016e", 3141.5);
  nerrors += Verify(__FILE__, __LINE__, "    3.141500e+03",
		    "%16e", 3141.5);
  nerrors += Verify(__FILE__, __LINE__, "3.141500e+03    ",
		    "%-16e", 3141.5);
  nerrors += Verify(__FILE__, __LINE__, "03.142e+03",
		    "%010.3e", 3141.5);
#if 0 /* The output depends on the locale settings */
  nerrors += Verify(__FILE__, __LINE__, "31,415.200000",
		    "%'f", 31415.2);
#endif
  /* Special cases */
  nerrors += Verify(__FILE__, __LINE__, "1.00",
		    "%.2f", 0.999);
  nerrors += Verify(__FILE__, __LINE__, "100",
		    "%.0f", 99.9);
  nerrors += Verify(__FILE__, __LINE__, "inf",
		    "%f", trio_pinf());
  nerrors += Verify(__FILE__, __LINE__, "-inf",
		    "%f", trio_ninf());
  nerrors += Verify(__FILE__, __LINE__, "INF",
		    "%F", trio_pinf());
  nerrors += Verify(__FILE__, __LINE__, "-INF",
		    "%F", trio_ninf());
  /* May fail if NaN is unsupported */
  nerrors += Verify(__FILE__, __LINE__, "nan",
		    "%f", trio_nan());
  nerrors += Verify(__FILE__, __LINE__, "NAN",
		    "%F", trio_nan());
  /* Char width alignment */
  nerrors += Verify(__FILE__, __LINE__, "Char X   .",
	 "Char %-4c.", 'X');
  /* String width / precision */
  nerrors += Verify(__FILE__, __LINE__, " testing",
		    "%8s", "testing");
  nerrors += Verify(__FILE__, __LINE__, "testing ",
		    "%-8s", "testing");
  nerrors += Verify(__FILE__, __LINE__, " testing",
		    "%*s", 8, "testing");
  nerrors += Verify(__FILE__, __LINE__, "test",
		    "%.4s", "testing");
  nerrors += Verify(__FILE__, __LINE__, "test",
		    "%.*s", 4, "testing");
  /* Quote flag */
  nerrors += Verify(__FILE__, __LINE__, "Another \"quoted\" string",
		   "Another %'s string", "quoted");
  /* Positional */
  nerrors += Verify(__FILE__, __LINE__, "222 111",
		    "%2$s %1$s", "111", "222");
  nerrors += Verify(__FILE__, __LINE__, "123456    12345 0001234  00123",
		    "%4$d %3$*8$d %2$.*7$d %1$*6$.*5$d",
		    123, 1234, 12345, 123456, 5, 6, 7, 8);

#if TRIO_GNU
  nerrors += Verify(__FILE__, __LINE__, "256",
		    "%Zd", sizeof(buffer));
  errno = EINTR;
  nerrors += Verify(__FILE__, __LINE__, "Interrupted system call",
		    "%m");
#endif
  
#if TRIO_BSD || TRIO_GNU
  nerrors += Verify(__FILE__, __LINE__, "42",
		    "%qd", 42LL);
#endif

#if TRIO_C99
  nerrors += Verify(__FILE__, __LINE__, "0xc45.000000",
		    "%a", 3141.0);
  nerrors += Verify(__FILE__, __LINE__, "0XC45.000000",
		    "%A", 3141.0);
  nerrors += Verify(__FILE__, __LINE__, "0x3.241893p-2c",
		    "%a", 3.141e-44);
  nerrors += Verify(__FILE__, __LINE__, "256",
		    "%zd", sizeof(buffer));
  nerrors += Verify(__FILE__, __LINE__, "42",
		    "%td", 42);
  nerrors += Verify(__FILE__, __LINE__, "42",
		    "%jd", 42LL);
#endif

#if TRIO_WIDECHAR
  nerrors += Verify(__FILE__, __LINE__, "Hello World",
		    "%ls", L"Hello World");
  nerrors += Verify(__FILE__, __LINE__, "\\aHello World",
		    "%#ls", L"\aHello World");
  nerrors += Verify(__FILE__, __LINE__, "A",
		    "%lc", L'A');
  nerrors += Verify(__FILE__, __LINE__, "\\a",
		    "%#lc", L'\a');
#endif

#if TRIO_MICROSOFT
  nerrors += Verify(__FILE__, __LINE__, "42",
		    "%I8d", 42);
  nerrors += Verify(__FILE__, __LINE__, "ffffffff",
		    "%I16x", -1);
#endif
  
#if TRIO_EXTENSION
  nerrors += Verify(__FILE__, __LINE__, "  42   86",
		    "%!4d %d", 42, 86);
  nerrors += Verify(__FILE__, __LINE__, "0042 0086",
		    "%!04d %d", 42, 86);
  nerrors += Verify(__FILE__, __LINE__, "42",
		    "%&d", sizeof(long), 42L);
  /* Non-printable string */
  nerrors += Verify(__FILE__, __LINE__, "NonPrintable \\x01 \\a \\\\",
		    "NonPrintable %#s", "\01 \07 \\");
  nerrors += Verify(__FILE__, __LINE__, "\\a \\b \\t \\n \\v \\f \\r",
		    "%#s", "\007 \010 \011 \012 \013 \014 \015");
  /* Integer base */
  nerrors += Verify(__FILE__, __LINE__, "Number 42 == 1120 (base 3)",
		    "Number %d == %..3i (base 3)", 42, 42);
  nerrors += Verify(__FILE__, __LINE__, "42",
		    "%..3d", 42);
  nerrors += Verify(__FILE__, __LINE__, "52",
		    "%..3o", 42);
  nerrors += Verify(__FILE__, __LINE__, "2a",
		    "%..3x", 42);
#endif
  
  return nerrors;
}

/*************************************************************************
 *
 */
int VerifyErrors(void)
{
  char buffer[512];
  int rc;
  int nerrors = 0;
  
  /* Error: Invalid argument 1 */
  rc = trio_snprintf(buffer, sizeof(buffer), "%d %r", 42, "text");
  trio_snprintf(buffer, sizeof(buffer), "Err = %d (%s), Pos = %d",
		TRIO_ERROR_CODE(rc),
		TRIO_ERROR_NAME(rc),
		TRIO_ERROR_POSITION(rc));
  nerrors += Verify(__FILE__, __LINE__, "Err = 2 (Invalid argument), Pos = 5",
		    "%s", buffer);
  /* Error: Invalid argument 2 */
  rc = trio_snprintf(buffer, sizeof(buffer), "%#");
  trio_snprintf(buffer, sizeof(buffer), "Err = %d (%s), Pos = %d",
		TRIO_ERROR_CODE(rc),
		TRIO_ERROR_NAME(rc),
		TRIO_ERROR_POSITION(rc));
  nerrors += Verify(__FILE__, __LINE__, "Err = 2 (Invalid argument), Pos = 3",
		    "%s", buffer);
  /* Error: Invalid argument 3 */
  rc = trio_snprintf(buffer, sizeof(buffer), "%hhhd", 42);
  trio_snprintf(buffer, sizeof(buffer), "Err = %d (%s), Pos = %d",
		TRIO_ERROR_CODE(rc),
		TRIO_ERROR_NAME(rc),
		TRIO_ERROR_POSITION(rc));
  nerrors += Verify(__FILE__, __LINE__, "Err = 2 (Invalid argument), Pos = 4",
		    "%s", buffer);
  /* Error: Double reference */
  rc = trio_snprintf(buffer, sizeof(buffer), "hello %1$d %1$d", 31, 32);
  trio_snprintf(buffer, sizeof(buffer), "Err = %d (%s), Pos = %d",
		TRIO_ERROR_CODE(rc),
		TRIO_ERROR_NAME(rc),
		TRIO_ERROR_POSITION(rc));
  nerrors += Verify(__FILE__, __LINE__, "Err = 4 (Double reference), Pos = 0",
		    "%s", buffer);
  /* Error: Reference gap */
  rc = trio_snprintf(buffer, sizeof(buffer), "%3$d %1$d", 31, 32, 33);
  trio_snprintf(buffer, sizeof(buffer), "Err = %d (%s), Pos = %d",
		TRIO_ERROR_CODE(rc),
		TRIO_ERROR_NAME(rc),
		TRIO_ERROR_POSITION(rc));
  nerrors += Verify(__FILE__, __LINE__, "Err = 5 (Reference gap), Pos = 1",
		    "%s", buffer);

  return nerrors;
}

/*************************************************************************
 *
 */
int VerifyScanningOneInteger(const char *file,
			     int line,
			     const char *expected,
			     const char *format,
			     int original)
{
  int number;
  char data[512];
  
  trio_snprintf(data, sizeof(data), format, original);
  trio_sscanf(data, format, &number);
  return Verify(file, line, expected, format, number);
}

int VerifyScanningIntegers(void)
{
  int nerrors = 0;

  nerrors += VerifyScanningOneInteger(__FILE__, __LINE__, "42",
				      "%i", 42);
  nerrors += VerifyScanningOneInteger(__FILE__, __LINE__, "42",
				      "%d", 42);
  nerrors += VerifyScanningOneInteger(__FILE__, __LINE__, "-42",
				      "%d", -42);
  nerrors += VerifyScanningOneInteger(__FILE__, __LINE__, "2147483647",
				      "%d", 2147483647);
  nerrors += VerifyScanningOneInteger(__FILE__, __LINE__, "42",
				      "%u", 42);
  nerrors += VerifyScanningOneInteger(__FILE__, __LINE__, "2a",
				      "%x", 42);
  nerrors += VerifyScanningOneInteger(__FILE__, __LINE__, "52",
				      "%o", 42);
  nerrors += VerifyScanningOneInteger(__FILE__, __LINE__, "101010",
				      "%..2i", 42);
  nerrors += VerifyScanningOneInteger(__FILE__, __LINE__, "0x2a",
				      "%#x", 42);
  nerrors += VerifyScanningOneInteger(__FILE__, __LINE__, "052",
				      "%#o", 42);

  return nerrors;
}

/*************************************************************************
 *
 */
int VerifyScanningOneFloat(const char *file,
			   int line,
			   const char *expected,
			   const char *format,
			   double original)
{
  double number;
  char data[512];
  
  trio_snprintf(data, sizeof(data), format, original);
  trio_sscanf(data, format, &number);
  return Verify(file, line, expected, format, number);
}

int VerifyScanningFloats(void)
{
  int nerrors = 0;

  nerrors += VerifyScanningOneFloat(__FILE__, __LINE__, "42.000000",
				      "%f", 42.0);
  nerrors += VerifyScanningOneFloat(__FILE__, __LINE__, "-42.000000",
				      "%f", -42.0);
  nerrors += VerifyScanningOneFloat(__FILE__, __LINE__, "4.200000e+01",
				      "%e", 42.0);
  nerrors += VerifyScanningOneFloat(__FILE__, __LINE__, "4.200000E+01",
				      "%E", 42.0);
  nerrors += VerifyScanningOneFloat(__FILE__, __LINE__, "42",
				      "%g", 42.0);
  nerrors += VerifyScanningOneFloat(__FILE__, __LINE__, "1.23457e+06",
				      "%g", 1234567.0);
  nerrors += VerifyScanningOneFloat(__FILE__, __LINE__, "1.23457e-06",
				      "%g", 1.234567e-6);
  nerrors += VerifyScanningOneFloat(__FILE__, __LINE__, "1.23457E+06",
				      "%G", 1234567.0);
  nerrors += VerifyScanningOneFloat(__FILE__, __LINE__, "1.234567e+06",
				      "%12e", 1234567.0);
  nerrors += VerifyScanningOneFloat(__FILE__, __LINE__, "1.234500e+00",
				      "%6e", 1234567.0);
  nerrors += VerifyScanningOneFloat(__FILE__, __LINE__, "1.234567e+06",
				      "%.6e", 1234567.0);
  nerrors += VerifyScanningOneFloat(__FILE__, __LINE__, "1.2345670000e+06",
				      "%.10e", 1234567.0);
  nerrors += VerifyScanningOneFloat(__FILE__, __LINE__, "1.23457e+06",
				      "%.6g", 1234567.0);
  nerrors += VerifyScanningOneFloat(__FILE__, __LINE__, "1234567",
				      "%.10g", 1234567.0);
  nerrors += VerifyScanningOneFloat(__FILE__, __LINE__, "0x2a.000000",
				      "%a", 42.0);
  nerrors += VerifyScanningOneFloat(__FILE__, __LINE__, "0x1.3c0c95p+06",
				      "%a", 1234567.0);
  nerrors += VerifyScanningOneFloat(__FILE__, __LINE__, "0X1.3C0C95P+06",
				      "%A", 1234567.0);
  nerrors += VerifyScanningOneFloat(__FILE__, __LINE__, "1.79769e+308",
				      "%g", 1.79769e+308);
  nerrors += VerifyScanningOneFloat(__FILE__, __LINE__, "nan",
				      "%f", trio_nan());
  nerrors += VerifyScanningOneFloat(__FILE__, __LINE__, "NAN",
				      "%F", trio_nan());
  nerrors += VerifyScanningOneFloat(__FILE__, __LINE__, "-inf",
				      "%f", trio_ninf());
  
  return nerrors;
}

/*************************************************************************
 *
 */
int VerifyScanningOneString(const char *file,
			    int line,
			    const char *expected,
			    const char *format,
			    char *original)
{
  char string[512];
  char data[512];
  
  trio_snprintf(data, sizeof(data), "%s", original);
  trio_sscanf(data, format, string);
  return Verify(file, line, expected, "%s", string);
}

int VerifyScanningStrings(void)
{
  int nerrors = 0;

  nerrors += VerifyScanningOneString(__FILE__, __LINE__, "hello",
				     "%s", "hello");
  nerrors += VerifyScanningOneString(__FILE__, __LINE__, "hello",
				     "%s", "hello world");
  nerrors += VerifyScanningOneString(__FILE__, __LINE__, "hello world",
				     "%[^\n]", "hello world");
  nerrors += VerifyScanningOneString(__FILE__, __LINE__, "(nil)",
				     "%s", NULL);
  nerrors += VerifyScanningOneString(__FILE__, __LINE__, "hello",
				     "%20s", "hello");
  nerrors += VerifyScanningOneString(__FILE__, __LINE__, "he",
				     "%2s", "hello");
  nerrors += VerifyScanningOneString(__FILE__, __LINE__, "ab",
				     "%[ab]", "abcba");
  nerrors += VerifyScanningOneString(__FILE__, __LINE__, "abcba",
				     "%[abc]", "abcba");
  nerrors += VerifyScanningOneString(__FILE__, __LINE__, "abcba",
				     "%[a-c]", "abcba");
  nerrors += VerifyScanningOneString(__FILE__, __LINE__, "abcba",
				     "%[[:alpha:]]", "abcba");
  nerrors += VerifyScanningOneString(__FILE__, __LINE__, "ba",
				     "%*[ab]c%[^\n]", "abcba");

  return nerrors;
}

/*************************************************************************
 *
 */
int VerifyScanningRegression(void)
{
  int nerrors = 0;
  int rc;
  int index;
  double dnumber;
  long lnumber;
  int number;
  char ch;

  rc = trio_sscanf("1.5", "%lf%n", &dnumber, &index);
  nerrors += Verify(__FILE__, __LINE__, "1 3 1.500000",
		    "%d %d %f", rc, index, dnumber);
  rc = trio_sscanf("q 123", "%c%ld", &ch, &lnumber);
  nerrors += Verify(__FILE__, __LINE__, "q 123",
		    "%c %ld", ch, lnumber);
  rc = trio_sscanf("abc", "%*s%n", &number);
  nerrors += Verify(__FILE__, __LINE__, "0 3",
		    "%d %d", rc, number);
  rc = trio_sscanf("abc def", "%*s%n", &number);
  nerrors += Verify(__FILE__, __LINE__, "0 3",
		    "%d %d", rc, number);
  
  return nerrors;
}

/*************************************************************************
 *
 */
int VerifyScanning(void)
{
  int nerrors = 0;

  nerrors += VerifyScanningIntegers();
  nerrors += VerifyScanningFloats();
  nerrors += VerifyScanningStrings();
  nerrors += VerifyScanningRegression();
  
  return nerrors;
}

/*************************************************************************
 *
 */
int VerifyStrings(void)
{
  int nerrors = 0;
  char buffer[512];
  double dnumber;
  float fnumber;
  char *end;

  /* Comparison */
  trio_copy(buffer, "Find me now");
  if (trio_length(buffer) != sizeof("Find me now") - 1)
    nerrors++;
  if (!trio_equal(buffer, "Find me now"))
    nerrors++;
  if (!trio_equal_case(buffer, "Find me now"))
    nerrors++;
  if (trio_equal_case(buffer, "FIND ME NOW"))
    nerrors++;
  if (!trio_equal_max(buffer, sizeof("Find me") - 1, "Find ME"))
    nerrors++;
  if (!trio_contains(buffer, "me"))
    nerrors++;
  if (trio_contains(buffer, "and me"))
    nerrors++;
  if (trio_substring(buffer, "me") == NULL)
    nerrors++;
  if (trio_substring_max(buffer, 4, "me") != NULL)
    nerrors++;
  if (!trio_match(buffer, "* me *"))
    nerrors++;
  if (trio_match_case(buffer, "* ME *"))
    nerrors++;
  if (trio_index(buffer, 'n') == NULL)
    nerrors++;
  if (trio_index(buffer, '_') != NULL)
    nerrors++;
  if (trio_index_last(buffer, 'n') == NULL)
    nerrors++;

  /* Append */
  trio_copy(buffer, "Find me now");
  if (!trio_append(buffer, " and again"))
    nerrors++;
  if (!trio_equal(buffer, "Find me now and again"))
    nerrors++;
  if (!trio_append_max(buffer, 0, "should not appear"))
    nerrors++;
  if (!trio_equal(buffer, "Find me now and again"))
    nerrors++;

  /* To upper/lower */
  trio_copy(buffer, "Find me now");
  trio_upper(buffer);
  if (!trio_equal_case(buffer, "FIND ME NOW"))
    nerrors++;
  trio_lower(buffer);
  if (!trio_equal_case(buffer, "find me now"))
    nerrors++;

  /* Double conversion */
  trio_copy(buffer, "3.1415");
  dnumber = trio_to_double(buffer, NULL);
  if (!DOUBLE_EQUAL(dnumber, 3.1415))
    nerrors++;
  fnumber = trio_to_float(buffer, NULL);
  if (!FLOAT_EQUAL(fnumber, 3.1415))
    nerrors++;

  /* Long conversion */
  trio_copy(buffer, "3.1415");
  if (trio_to_long(buffer, NULL, 10) != 3L)
    nerrors++;
  if (trio_to_long(buffer, NULL, 4) != 3L)
    nerrors++;
  trio_to_long(buffer, &end, 2);
  if (end != buffer)
    nerrors++;
  
  return nerrors;
}

/*************************************************************************
 *
 */
int VerifyDynamicStrings(void)
{
  int nerrors = 0;
  trio_string_t *string;

  string = trio_xstring_duplicate("Find me now");
  if (string == NULL) {
    nerrors++;
    goto error;
  }
  if (!trio_xstring_equal(string, "FIND ME NOW"))
    nerrors++;
  if (!trio_xstring_append(string, " and again") ||
      !trio_xstring_equal(string, "FIND ME NOW AND AGAIN"))
    nerrors++;
  if (!trio_xstring_contains(string, "me"))
    nerrors++;
  if (trio_xstring_contains(string, "ME"))
    nerrors++;
  if (!trio_xstring_match(string, "* me *"))
    nerrors++;
  if (trio_xstring_match_case(string, "* ME *"))
    nerrors++;
  
 error:
  if (string)
    trio_string_destroy(string);
  
  return nerrors;
}

/*************************************************************************
 *
 */
int main(void)
{
  int nerrors = 0;

  printf("%s\n", rcsid);

  printf("Verifying strings\n");
  nerrors += VerifyStrings();
  
  printf("Verifying dynamic strings\n");
  nerrors += VerifyDynamicStrings();
  
  printf("Verifying formatting\n");
  nerrors += VerifyFormatting();
  
  printf("Verifying scanning\n");
  nerrors += VerifyScanning();
  
  printf("Verifying return values\n");
  nerrors += VerifyErrors();
  nerrors += VerifyReturnValues();
  
  printf("Verifying allocation\n");
  nerrors += VerifyAllocate();

  if (nerrors == 0)
    printf("Regression test suceeded\n");
  else
    printf("Regression test failed in %d instance(s)\n", nerrors);
  
  return 0;
}

