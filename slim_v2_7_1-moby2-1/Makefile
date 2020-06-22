# Makefile.  Generated from Makefile.in by configure.

# From autoconf
prefix = /mnt/so1/shared/software/moby2_deps
exec_prefix = ${prefix}
datarootdir = ${prefix}/share
INSTALL = /usr/bin/install -c
INSTALL_PROGRAM = ${INSTALL}
INSTALL_DATA = ${INSTALL} -m 644

SHELL = /bin/sh
DIRS =  src doc share

define loop-over-dirs
	@status=0; \
	for f in $(DIRS); do \
	    if [ -d $$f ]; then \
	        if [ ! \( $$f = test -a $1 = all \) ]; then \
	            (cd $$f; echo $$f; $(MAKE) $(MFLAGS) $1) || status=$$?; \
	        fi; \
	    else \
	        echo No such directory: $$f >&2; \
	    fi \
	done; \
	if [ $$status = 2 ]; then false; fi
endef

.PHONY: again all debug doc test

all :
	$(call loop-over-dirs,$@)
clean:
	$(call loop-over-dirs,$@)
	$(RM) -fr *~ *.bak svn*.tmp bin/* lib/*

distclean:
	$(call loop-over-dirs,$@)
	$(RM) -fr *~ *.bak svn*.tmp Makefile config.log config.status slim*.tgz

doc: 
	$(MAKE) -C doc doc

debug:
	$(MAKE) -C src debug

again: clean all


#
# Figure out where to install
#

# Targets to install
LOCAL_MANDIR=share/man/man1
MANPAGES=$(LOCAL_MANDIR)/slim.1 $(LOCAL_MANDIR)/unslim.1 $(LOCAL_MANDIR)/slimcat.1
TARGETLIB_ST=libslim.a
TARGETLIB_SO=libslim.so
MAJOR=0
MINOR=0
TARGETLIB=lib/$(TARGETLIB_ST) lib/$(TARGETLIB_SO).$(MAJOR).$(MINOR)

.PHONY : install
install: all
	@:
	@: Check the inode number for . and $(prefix) to find out if two
	@: directories are the same\; they may have different names due to
	@: symbolic links and automounters
	@:
	@if [ -d $(prefix) ]; then \
		if [ `ls -id $(prefix) | awk '{print $$1}'` = `ls -id . | awk '{print $$1}'` ]; then \
	        echo "The destination directory is the same" \
	                "as the current directory; aborting." >&2; \
	        echo ""; \
	        exit 1; \
	   fi; \
	fi

	mkdir -p $(DESTDIR)${exec_prefix}/bin
	$(INSTALL_PROGRAM) -p bin/slim bin/slim_dump $(DESTDIR)${exec_prefix}/bin
	ln -f $(DESTDIR)${exec_prefix}/bin/slim $(DESTDIR)${exec_prefix}/bin/unslim
	ln -f $(DESTDIR)${exec_prefix}/bin/slim $(DESTDIR)${exec_prefix}/bin/slimcat

	mkdir -p $(DESTDIR)${datarootdir}/man/man1
	$(INSTALL_DATA) -p $(MANPAGES) $(DESTDIR)${datarootdir}/man/man1
	mkdir -p $(DESTDIR)${prefix}/include
	$(INSTALL_DATA) -p include/*.h $(DESTDIR)${prefix}/include
	mkdir -p $(DESTDIR)${exec_prefix}/lib
	$(INSTALL_DATA) -p $(TARGETLIB) $(DESTDIR)${exec_prefix}/lib
	ln -f -s $(TARGETLIB_SO).$(MAJOR).$(MINOR) \
	   $(DESTDIR)${exec_prefix}/lib/$(TARGETLIB_SO).$(MAJOR)
	ln -f -s $(TARGETLIB_SO).$(MAJOR) \
	   $(DESTDIR)${exec_prefix}/lib/$(TARGETLIB_SO)

ifdef NEED_LDCONFIG
	@echo "Running deferred ldconfig"; \
	/sbin/ldconfig
endif


test:
	$(MAKE) -Ctest test

