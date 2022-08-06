SUBDIRS = doc

LDADD = libtrio.la

check_PROGRAMS = regression
EXTRA_PROGRAMS = compare example
TESTS = $(check_PROGRAMS)

compare_SOURCES = compare.c
example_SOURCES = example.c
regression_SOURCES = regression.c

EXTRA_DIST = CHANGES INSTALL LICENSE test-build.sh

dist_doc_DATA = README

include_HEADERS = trio.h triop.h triodef.h trionan.h triostr.h

lib_LTLIBRARIES = libtrio.la
libtrio_la_LIBADD = -lm
libtrio_la_LDFLAGS = -no-undefined -version-info 2
libtrio_la_SOURCES = triostr.c trio.c trionan.c strio.h

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = lib$(PACKAGE).pc
