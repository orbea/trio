/*************************************************************************
 * Regression test
 */

static const char rcsid[] = "@(#)$Id$";

#include <stdarg.h>
#include <math.h>
#include <float.h>
#include "trio.h"
#include "trionan.h"
#include "triostr.h"
#undef printf

#if TRIO_WIDECHAR
# include <wchar.h>
#endif

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

  /* Normal text */
  nerrors += Verify(__FILE__, __LINE__, "Hello world",
		   "Hello world");
  /* String */
  nerrors += Verify(__FILE__, __LINE__, "Hello world",
		   "%s", "Hello world");
  /* Pointer */
  /* It is ok, if this fails on 64 bit machines */
  nerrors += Verify(__FILE__, __LINE__, "Pointer 0x01234567",
		   "Pointer %p", 0x1234567);
  /* Nil pointer */
  nerrors += Verify(__FILE__, __LINE__, "Pointer (nil)",
		   "Pointer %p", NULL);
  /* Integer */
  nerrors += Verify(__FILE__, __LINE__, "Number 42",
		   "Number %d", 42);
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
  nerrors += Verify(__FILE__, __LINE__, "Number 42 == 1120 (base 3)",
		    "Number %d == %..3i (base 3)", 42, 42);
  /* Ignoring integer base */
  nerrors += Verify(__FILE__, __LINE__, "42",
		    "%..3d", 42);
  nerrors += Verify(__FILE__, __LINE__, "52",
		    "%..3o", 42);
  nerrors += Verify(__FILE__, __LINE__, "2a",
		    "%..3x", 42);
  /* Octal */
  nerrors += Verify(__FILE__, __LINE__, "052",
		   "%#o", 42);
  /* Hex */
  nerrors += Verify(__FILE__, __LINE__, "0X2A",
		   "%#X", 42);
  nerrors += Verify(__FILE__, __LINE__, "0x00c ",
		   "%-#6.3x", 12);
  nerrors += Verify(__FILE__, __LINE__, "ffffffff",
		    "%x", -1);
  /* Double */
  nerrors += Verify(__FILE__, __LINE__, "3141.000000",
		    "%f", 3141.0);
  nerrors += Verify(__FILE__, __LINE__, "3141.500000",
		    "%f", 3141.5);
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
  /* Infinite */
  nerrors += Verify(__FILE__, __LINE__, "inf",
		    "%f", trio_pinf());
  nerrors += Verify(__FILE__, __LINE__, "-inf",
		    "%f", trio_ninf());
  nerrors += Verify(__FILE__, __LINE__, "INF",
		    "%F", trio_pinf());
  nerrors += Verify(__FILE__, __LINE__, "-INF",
		    "%F", trio_ninf());
  /* These two may fail if NaN is unsupported */
  nerrors += Verify(__FILE__, __LINE__, "nan",
		    "%f", trio_nan());
  nerrors += Verify(__FILE__, __LINE__, "NAN",
		    "%F", trio_nan());
  /* Quote flag */
  nerrors += Verify(__FILE__, __LINE__, "Another \"quoted\" string",
		   "Another %'s string", "quoted");
  /* Char width alignment */
  nerrors += Verify(__FILE__, __LINE__, "Char X   .",
	 "Char %-4c.", 'X');
  /* String precision */
  nerrors += Verify(__FILE__, __LINE__, "test",
		    "%.*s", 4, "testing");
  /* Non-printable */
  nerrors += Verify(__FILE__, __LINE__, "NonPrintable \\x01 \\a \\\\",
		    "NonPrintable %#s", "\01 \07 \\");
  /* Positional */
  nerrors += Verify(__FILE__, __LINE__, "222 111",
		    "%2$s %1$s", "111", "222");
  nerrors += Verify(__FILE__, __LINE__, "123456    12345 0001234  00123",
		    "%4$d %3$*8$d %2$.*7$d %1$*6$.*5$d",
		    123, 1234, 12345, 123456, 5, 6, 7, 8);

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
int VerifyScanning(void)
{
  int nerrors = 0;

  printf("  not implemented yet\n");
  return nerrors;
}

/*************************************************************************
 *
 */
int VerifyStrings(void)
{
  int nerrors = 0;
  char buffer[512];

  sprintf(buffer, "Find me now");
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

  trio_upper(buffer);
  if (!trio_equal_case(buffer, "FIND ME NOW"))
    nerrors++;
  trio_lower(buffer);
  if (!trio_equal_case(buffer, "find me now"))
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

