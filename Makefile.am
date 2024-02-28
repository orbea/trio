SUBDIRS = doc

LDADD = libtrio.la

check_PROGRAMS = regression
EXTRA_PROGRAMS = compare example
TESTS = $(check_PROGRAMS)

compare_SOURCES = compare.c
example_SOURCES = example.c strio.h
regression_SOURCES = regression.c

EXTRA_DIST = CHANGES INSTALL LICENSE test-build.sh

dist_doc_DATA = README

include_HEADERS = trio.h triop.h triodef.h trionan.h triostr.h

lib_LTLIBRARIES = libtrio.la
libtrio_la_LIBADD = libtrionan.la $(LIBM)
libtrio_la_LDFLAGS = -no-undefined -export-symbols-regex trio_ -version-info 2:0:0
libtrio_la_SOURCES = triostr.c

noinst_LTLIBRARIES = libtrionan.la
libtrionan_la_CFLAGS = -fno-finite-math-only -fno-unsafe-math-optimizations
libtrionan_la_SOURCES = trio.c trionan.c

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = lib$(PACKAGE).pc
