/*************************************************************************
 * Regression test
 */

static const char rcsid[] = "@(#)$Id$";

#include <stdarg.h>
#include <math.h>
#include <float.h>
#include "strio.h"
#include "trio.h"
#undef printf


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

  if (! StrEqualCase(result, buffer))
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
  if (!StrEqualCase(result, expected))
    {
      nerr++;
      Report(__FILE__, __LINE__, expected, result);
    }
  
  rc = trio_snprintf(buffer, sizeof(buffer), "%s%n", "0123456789", &count);
  trio_sprintf(result, "%d %d %s", rc, count, buffer);
  expected = "10 10 0123456789";
  if (!StrEqualCase(result, expected))
    {
      nerr++;
      Report(__FILE__, __LINE__, expected, result);
    }
  
  rc = trio_snprintf(buffer, 4, "%s%n", "0123456789", &count);
  trio_sprintf(result, "%d %d %s", rc, count, buffer);
  expected = "10 3 012";
  if (!StrEqualCase(result, expected))
    {
      nerr++;
      Report(__FILE__, __LINE__, expected, result);
    }

  /* The output buffer contains the empty string */
  rc = trio_snprintf(buffer, 1, "%s%n", "0123456789", &count);
  trio_sprintf(result, "%d %d %s", rc, count, buffer);
  expected = "10 0 ";
  if (!StrEqualCase(result, expected))
    {
      nerr++;
      Report(__FILE__, __LINE__, expected, result);
    }

  /* The output buffer should be left untouched when max size is 0 */
  trio_sprintf(buffer, "DO NOT TOUCH");
  rc = trio_snprintf(buffer, 0, "%s%n", "0123456789", &count);
  trio_sprintf(result, "%d %d %s", rc, count, buffer);
  expected = "10 0 DO NOT TOUCH";
  if (!StrEqualCase(result, expected))
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
  else if (!StrEqualCase(string, TEST_STRING))
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
  nerrors += Verify(__FILE__, __LINE__, "3141",
		    "%f", 3141.0);
  nerrors += Verify(__FILE__, __LINE__, "3141.5",
		    "%f", 3141.5);
  nerrors += Verify(__FILE__, __LINE__, "3.141e+3",
		    "%e", 3141.0);
  nerrors += Verify(__FILE__, __LINE__, "3.141E-44",
		    "%E", 3.141e-44);
  nerrors += Verify(__FILE__, __LINE__, "3141.5",
		    "%g", 3141.5);
  nerrors += Verify(__FILE__, __LINE__, "3.1415E-6",
		    "%G", 3.1415e-6);
  /* Double decimal point */
  nerrors += Verify(__FILE__, __LINE__, "3141",
		    "%.0f", 3141.0);
  nerrors += Verify(__FILE__, __LINE__, "3141.",
		    "%#f", 3141.0);
  nerrors += Verify(__FILE__, __LINE__, "0.",
		    "%#.4f", 0.0);
  nerrors += Verify(__FILE__, __LINE__, "0.",
		    "%#.4g", 0.0);
  nerrors += Verify(__FILE__, __LINE__, "3141.",
		    "%#.4f", 3141.0);
  nerrors += Verify(__FILE__, __LINE__, "3141.",
		    "%#.0f", 3141.0);
  nerrors += Verify(__FILE__, __LINE__, "11.",
		    "%#.4f", 11.0);
  nerrors += Verify(__FILE__, __LINE__, "100",
		    "%.2f", 99.9999);
  nerrors += Verify(__FILE__, __LINE__, "100",
		    "%.2g", 99.9999);
  nerrors += Verify(__FILE__, __LINE__, "100.",
		    "%#.2g", 99.9999);
  /* Double width, precision, and zero padding */
  nerrors += Verify(__FILE__, __LINE__, "00000003.1415e+3",
		    "%016e", 3141.5);
  nerrors += Verify(__FILE__, __LINE__, "       3.1415e+3",
		    "%16e", 3141.5);
  nerrors += Verify(__FILE__, __LINE__, "3.1415e+3       ",
		    "%-16e", 3141.5);
  nerrors += Verify(__FILE__, __LINE__, "003.142e+3",
		    "%010.3e", 3141.5);
#if 0 /* The output depends on the locale settings */
  nerrors += Verify(__FILE__, __LINE__, "31,415.200000",
		    "%'f", 31415.2);
#endif
  /* Infinite */
  nerrors += Verify(__FILE__, __LINE__, "inf",
		    "%f", HUGE_VAL);
  nerrors += Verify(__FILE__, __LINE__, "-inf",
		    "%f", -HUGE_VAL);
  nerrors += Verify(__FILE__, __LINE__, "INF",
		    "%F", HUGE_VAL);
  nerrors += Verify(__FILE__, __LINE__, "-INF",
		    "%F", -HUGE_VAL);
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
  rc = StrFormatMax(buffer, sizeof(buffer), "%d %r", 42, "text");
  StrFormatMax(buffer, sizeof(buffer), "Err = %d (%s), Pos = %d",
	       TRIO_ERROR_CODE(rc),
	       TRIO_ERROR_NAME(rc),
	       TRIO_ERROR_POSITION(rc));
  nerrors += Verify(__FILE__, __LINE__, "Err = 2 (Invalid argument), Pos = 5",
		    "%s", buffer);
  /* Error: Invalid argument 2 */
  rc = StrFormatMax(buffer, sizeof(buffer), "%#");
  StrFormatMax(buffer, sizeof(buffer), "Err = %d (%s), Pos = %d",
	       TRIO_ERROR_CODE(rc),
	       TRIO_ERROR_NAME(rc),
	       TRIO_ERROR_POSITION(rc));
  nerrors += Verify(__FILE__, __LINE__, "Err = 2 (Invalid argument), Pos = 3",
		    "%s", buffer);
  /* Error: Invalid argument 3 */
  rc = StrFormatMax(buffer, sizeof(buffer), "%hhhd", 42);
  StrFormatMax(buffer, sizeof(buffer), "Err = %d (%s), Pos = %d",
	       TRIO_ERROR_CODE(rc),
	       TRIO_ERROR_NAME(rc),
	       TRIO_ERROR_POSITION(rc));
  nerrors += Verify(__FILE__, __LINE__, "Err = 2 (Invalid argument), Pos = 4",
		    "%s", buffer);
  /* Error: Double reference */
  rc = StrFormatMax(buffer, sizeof(buffer), "hello %1$d %1$d", 31, 32);
  StrFormatMax(buffer, sizeof(buffer), "Err = %d (%s), Pos = %d",
	       TRIO_ERROR_CODE(rc),
	       TRIO_ERROR_NAME(rc),
	       TRIO_ERROR_POSITION(rc));
  nerrors += Verify(__FILE__, __LINE__, "Err = 4 (Double reference), Pos = 0",
		    "%s", buffer);
  /* Error: Reference gap */
  rc = StrFormatMax(buffer, sizeof(buffer), "%3$d %1$d", 31, 32, 33);
  StrFormatMax(buffer, sizeof(buffer), "Err = %d (%s), Pos = %d",
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
  if (!StrEqual(buffer, "Find me now"))
    nerrors++;
  if (!StrEqualMax(buffer, sizeof("Find me") - 1, "Find ME"))
    nerrors++;
  if (!StrContains(buffer, "me"))
    nerrors++;
  if (StrContains(buffer, "and me"))
    nerrors++;
  if (StrSubstring(buffer, "me") == NULL)
    nerrors++;
  if (StrSubstringMax(buffer, 4, "me") != NULL)
    nerrors++;
  
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

