if HAVE_DOXYGEN

CLEANFILES = doxygen.stamp

doxygen.stamp: trio.cfg
	$(DOXYGEN) $<
	touch $@

all-local: doxygen.stamp
clean-local:
	-rm -rf $(top_srcdir)/docs/html

endif

EXTRA_DIST = \
	doc.h \
	doc_dynamic.h \
	doc_nan.h \
	doc_printf.h \
	doc_register.h \
	doc_scanf.h \
	doc_static.h \
	footer.html \
	header.html \
	trio.cfg \
	trio.css
