# Documentation: writeup and a doxygen site.

.PHONY: doc
all:

.PHONY: doc
doc: doxygen pdfdoc

.PHONY: doxygen
doxygen: ../src/*.h ../src/*.cpp clean_doxygen
	doxygen slim.doc

.PHONY: pdfdoc
pdfdoc: slim_format.pdf

%.pdf: %.tex Makefile
	pdflatex $<; pdflatex $<


.PHONY: clean clean_doxygen distclean
clean: clean_doxygen
	$(RM) -f *.log *.dvi *.aux *.ps

clean_doxygen:
	$(RM) -r html latex

distclean: clean
