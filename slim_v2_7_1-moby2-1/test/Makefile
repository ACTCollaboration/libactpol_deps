# test/Makefile.  Generated from Makefile.in by configure.

SHELL = /bin/sh
.SUFFIXES:
.SUFFIXES: .c .o

# define DEBUG to have any value to get -O0 -g code.
OPT     = -O3
ifdef DEBUG
  OPT = -O0 -g
endif

# Set by autoconf
CC = gcc
CXX = g++
CPP = gcc -E
INSTALL = /usr/bin/install -c
srcdir = .

CFLAGS  := -g -O2  -I../include -I../src $(OPT) -Wall -fPIC -std=c99
CXXFLAGS:= -g -O2 $(OPT) -Wall -fPIC
CPPFLAGS:=  -I../include -I../src -DPACKAGE_NAME=\"slim\" -DPACKAGE_TARNAME=\"slim\" -DPACKAGE_VERSION=\"v2_7_0\" -DPACKAGE_STRING=\"slim\ v2_7_0\" -DPACKAGE_BUGREPORT=\"drjoefowler@gmail.com\" -DPACKAGE_URL=\"\" -DHAVE_LIBM=1 -DSTDC_HEADERS=1 -DHAVE_SYS_TYPES_H=1 -DHAVE_SYS_STAT_H=1 -DHAVE_STDLIB_H=1 -DHAVE_STRING_H=1 -DHAVE_MEMORY_H=1 -DHAVE_STRINGS_H=1 -DHAVE_INTTYPES_H=1 -DHAVE_STDINT_H=1 -DHAVE_UNISTD_H=1 -DHAVE_LIMITS_H=1 -DHAVE_STDINT_H=1 -DHAVE_STDLIB_H=1 -DHAVE_STRING_H=1 -DHAVE_UNISTD_H=1 -DHAVE_UTIME_H=1 -DHAVE__BOOL=1 -DHAVE_STDBOOL_H=1 -DHAVE_UTIME_H=1 -DHAVE_UTIME_NULL=1 -DHAVE_MEMSET=1 -DHAVE_SQRT=1 -DHAVE_STRCHR=1 -DHAVE_STRRCHR=1 -DHAVE_STRSTR=1 -DHAVE_UTIME=1

BINDIR  := ../bin
LIBDIR  := ../lib
LDFLAGS :=   -fPIC -L$(LIBDIR)
LIBS    := -lslim -lm 
DEPEND  := .Makefile.depend

%.o: %.cpp Makefile $(DEPEND)
	$(COMPILE.cpp) $<

%.o: %.c Makefile $(DEPEND)
	$(COMPILE.c) $<

%: %.c $(LIBRARY) Makefile
	$(LINK.c) $(LIBS) -o $@ $<
	@echo

%: %.cpp $(LIBRARY) Makefile
	$(LINK.cpp) $(LIBS) -o $@ $<
	@echo

# Don't want to link this against the library!
generate_random_data: generate_random_data.c Makefile
	$(LINK.c) -o $@ $<
	@echo


##########

TARGETS     :=  generate_random_data test_slimdopen test_zirfile \
		make_test_bitstream verify_test_bitstream
# Targets can use implicit rules, along with the override flags added above

CXXOBJS    := compute_entropy.o \
		make_test_bitstream.o \
		test_bitstream.o \
		test_zirfile.o \
		verify_test_bitstream.o
COBJS      := generate_random_data.o \
		test_slimdopen.o

FAKE_DATA   := /tmp/fake_test_data.bin
FAKE_ZIP_DATA := /tmp/fake_zip_data.zip

all: $(TARGETS)
fake_data: $(FAKE_ZIP_DATA) $(FAKE_DATA)

$(FAKE_DATA): generate_random_data
	@echo "Generating random data..."
	./generate_random_data 5000000 $@
	@echo "Done generating random data\n\n"

$(FAKE_ZIP_DATA): $(FAKE_DATA)
	-cp $(FAKE_DATA) $(FAKE_DATA).copy1 && \
	cp $(FAKE_DATA) $(FAKE_DATA).copy2 && \
	rm -f $(FAKE_ZIP_DATA)
	zip --junk-paths $(FAKE_ZIP_DATA) $(FAKE_DATA) $(FAKE_DATA).copy*

test: all $(FAKE_ZIP_DATA) $(FAKE_DATA)
	@export LD_LIBRARY_PATH=$(LIBDIR); \
	export LIBRARY_PATH=$(LIBDIR); \
	export DYLD_LIBRARY_PATH=$(LIBDIR); \
	export PATH=$(BINDIR):$(PATH); \
	./test_slim.py; \
	rm $(FAKE_DATA).raw; \
	./test_slimdopen && cmp $(FAKE_DATA) $(FAKE_DATA).raw; \
	 \
	./regression.py




TEST_RESULT := test_bits.bin

test2: all
	@$(RM) -f $(TEST_RESULT)
	./make_test_bitstream; \
	md5sum -c .md5sum; \
	./verify_test_bitstream; \
	$(RM) -f $(TEST_RESULT)

clean:
	$(RM) $(TARGETS) *~

distclean: clean
	$(RM) $(TEST_RESULT) $(FAKE_DATA)

again: clean all

.PHONY: test clean all again fake_data

########


# Dependencies.  When cleaning, don't try to include
depend-exists := $(wildcard $(DEPEND))
ifeq  (depend-exists,$(DEPEND))
  include $(DEPEND)
endif

depend: $(DEPEND)
$(DEPEND):
	@echo Rebuilding make dependencies
	$(CPP) -MM $(CXXOBJS:.o=.cpp)  > .Makefile.depend
	$(CC) -MM $(COBJS:.o=.c)  >> .Makefile.depend
	@echo


tags: TAGS
TAGS: *.cpp $(HEADER) Makefile
	etags.emacs *.cpp $(HEADER)

debug:
	make DEBUG=1 all

include $(DEPEND)
