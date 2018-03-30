###############################################################################
# Installation and declaration of slim package in the EUPS ("extended
# unix package system") is handled here, rather than in the top-level Makefile.
# This way, users can ignore EUPS functions if they wish.
#
# J. Fowler, Princeton
# September 30, 2008
###############################################################################

# Make sure eups is available
ifeq (,$(shell which eups))
  $(error eups is not in the path. use 'make install' from root dir)
endif

# It's an error to specify prefix and version
ifneq (,$(prefix))
  ifneq (,$(version))
    $(error You may not specify prefix==$(prefix) and version==$(version))
  endif
endif

EUPS_PRODUCT=slim
EUPS_ROOT = $(shell eups path -1)
EUPS_FLAVOR = $(shell eups flavor)

# If version not specified, get version from root directory name
ifneq (,$(EUPS_ROOT))
  ifeq (,$(version))
    rootdir=$(shell echo $${PWD%/*})
    version=$(shell rootdir=$(rootdir); echo $${rootdir\#\#*/})
    _INFERRED_VERSION := True
  endif
endif


# This can come from cmd-line or from upper level directory
EUPS_VERSION=$(version)

# If no prefix, install in usual EUPS spot
ifeq (,$(prefix))
    prefix = $(EUPS_ROOT)/$(EUPS_FLAVOR)/$(EUPS_PRODUCT)/$(EUPS_VERSION)
endif


.PHONY: none install current declare clean

none: 
	@echo "make [install|current|declare] [version=vtag] from this directory."

install:
	@echo "Installing to EUPS, prefix=$(prefix)"

ifdef _INFERRED_VERSION
	@echo "--> Inferred tagged version name='$(version)'."; \
	echo  "--> You have 5 seconds to consider: is this correct?" ;\
	sleep 5
endif
	@make -C.. install prefix=$(prefix) ; \
	mkdir --parents $(prefix)/ups ; \
	if [ X$(EUPS_DIR) != X ]; then \
	  echo eups expandtable slim.table ${prefix}/ups; \
	  eups expandtable slim.table ${prefix}/ups; \
	fi; echo; \
        echo "Do not forget to 'make [declare|current]' for eups registration"

declare:
	eups declare --force -f $(EUPS_FLAVOR) -r $(prefix)\
	  $(EUPS_PRODUCT) $(EUPS_VERSION)

current : declare
	eups declare --current -f $(EUPS_FLAVOR) \
	  $(EUPS_PRODUCT) $(EUPS_VERSION)

clean:
	$(RM) *~
