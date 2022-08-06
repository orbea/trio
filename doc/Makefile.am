if HAVE_DOXYGEN

all-local: html
install-data-local: install-html
uninstall-hook: uninstall-html

endif

CLEANFILES = doxyfile.stamp

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
	trio.css

directory = $(top_builddir)/doc/html

.PHONY: html install-html uninstall-html

doxyfile.stamp: Doxyfile
	$(DOXYGEN) $<
	touch $@

html: doxyfile.stamp

clean-local:
	-rm -rf html

install-html: doxyfile.stamp
	$(MKDIR_P) $(DESTDIR)$(htmldir)
	for file in $(directory)/*; do \
		$(INSTALL_DATA) $$file "$(DESTDIR)$(htmldir)" || exit 1; \
	done;

uninstall-html:
	-rm -f -- $(DESTDIR)$(htmldir)/*.png $(DESTDIR)$(htmldir)/*.svg
	-rm -f -- $(DESTDIR)$(htmldir)/*.html $(DESTDIR)$(htmldir)/*.js
	-rm -f -- $(DESTDIR)$(htmldir)/*.css
