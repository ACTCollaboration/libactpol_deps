# src/Makefile.  Generated from Makefile.in by configure.

SHELL = /bin/sh
.SUFFIXES:
.SUFFIXES: .c .o

# Run "make DEBUG=1" (or any value) to get -O0 -g code.
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

CFLAGS  := -g -O2 $(OPT) -Wall -fPIC
CXXFLAGS:= -g -O2 $(OPT) -Wall -fPIC
CPPFLAGS:=  -I../include -I. -DPACKAGE_NAME=\"slim\" -DPACKAGE_TARNAME=\"slim\" -DPACKAGE_VERSION=\"v2_7_0\" -DPACKAGE_STRING=\"slim\ v2_7_0\" -DPACKAGE_BUGREPORT=\"drjoefowler@gmail.com\" -DPACKAGE_URL=\"\" -DHAVE_LIBM=1 -DSTDC_HEADERS=1 -DHAVE_SYS_TYPES_H=1 -DHAVE_SYS_STAT_H=1 -DHAVE_STDLIB_H=1 -DHAVE_STRING_H=1 -DHAVE_MEMORY_H=1 -DHAVE_STRINGS_H=1 -DHAVE_INTTYPES_H=1 -DHAVE_STDINT_H=1 -DHAVE_UNISTD_H=1 -DHAVE_LIMITS_H=1 -DHAVE_STDINT_H=1 -DHAVE_STDLIB_H=1 -DHAVE_STRING_H=1 -DHAVE_UNISTD_H=1 -DHAVE_UTIME_H=1 -DHAVE__BOOL=1 -DHAVE_STDBOOL_H=1 -DHAVE_UTIME_H=1 -DHAVE_UTIME_NULL=1 -DHAVE_MEMSET=1 -DHAVE_SQRT=1 -DHAVE_STRCHR=1 -DHAVE_STRRCHR=1 -DHAVE_STRSTR=1 -DHAVE_UTIME=1

LIBDIR  := ../lib
LDFLAGS :=  -fPIC -L$(LIBDIR)
LIBS    := -lslim -lm  
SOLIBS  := -lm  
DEPEND  := .Makefile.depend

%.o: %.cpp Makefile $(DEPEND)
	$(COMPILE.cpp) $<

%.o: %.c Makefile $(DEPEND)
	$(COMPILE.c) $<

.PHONY: all debug again tags depend test

STATIC_LIBRARY  = $(LIBDIR)/libslim.a
SO_LIBRARY= $(LIBDIR)/libslim.so
SONAME := libslim.so.0
LIBRARY  = $(SO_LIBRARY)
LINK.o  := $(LINK.cpp)

ifdef DEBUG
  LIBRARY = $(STATIC_LIBRARY)
  LDFLAGS = $(STATIC_LIBRARY) -lm
endif

TARGETBIN= ../bin
TARGET   = $(TARGETBIN)/slim
TARGETC  = slim.cpp
ALLTARGETS= \
	$(STATIC_LIBRARY) \
	$(SO_LIBRARY) \
	$(TARGET) \
	$(TARGETBIN)/slimcat \
	$(TARGETBIN)/unslim   \
	$(TARGETBIN)/slim_dump \

HEADER   = bit_constants.h bitstream.h slim.h slim_single_codec.h
CXXOBJS  = \
	bit_constants.o \
	bitstream.o \
	raw_section.o \
	slim_channel.o \
	slim_codec_constant.o \
	slim_codec_default.o \
	slim_codec_factory.o \
	slim_codec_runlength.o \
	slim_codec_reduced_binary.o \
	slim_control.o \
	slim_exceptions.o \
	slim_file.o \
	slimlib.o
COBJS    = 
OBJS     = $(CXXOBJS) $(COBJS)

all: bit_constants.h $(ALLTARGETS)

$(TARGET): $(TARGETC) $(LIBRARY) Makefile
	$(LINK.cpp) -o $@ $< $(LIBS)
	@echo

$(TARGETBIN)/slimcat: $(TARGET)
	ln -f $< $@

$(TARGETBIN)/unslim: $(TARGET)
	ln -f $< $@

$(TARGETBIN)/slim_dump: slim_dump.o $(LIBRARY) Makefile
	$(LINK.o) -o $@ $< $(LIBS)
	@echo

bit_constants.h bit_constants.cpp: build_bit_constants_h
	./$<

build_bit_constants_h: build_bit_constants_h.o
	$(LINK.o) $(FLAGS) -o $@ $<


$(STATIC_LIBRARY): $(OBJS)
	$(AR) rc $@ $(OBJS)

# Mac osX versus linux options for installing SONAME in the library:
sysname := $(shell uname -s)
ifeq ($(sysname), Darwin)
  SO_OPTIONS = -Wl,-dylib_install_name -Wl,$(SONAME)
else
  SO_OPTIONS = -Wl,-soname,$(SONAME) 
endif

$(SO_LIBRARY): $(OBJS) Makefile
	$(LINK.o) -shared  $(SO_OPTIONS) -o $@.0.0 $(OBJS) $(SOLIBS)
	ln -f $@.0.0 $@
	ln -f $@.0.0 $@.0

compute_entropy: compute_entropy.o bitstream.o Makefile
	$(LINK.o) -o $@ $<  $(OBJS) $(LIBS)

# Dependencies.  When cleaning, don't try to include
depend-exists := $(wildcard $(DEPEND))
ifeq  (depend-exists,$(DEPEND))
  include $(DEPEND)
endif

depend: $(DEPEND)
$(DEPEND):
	@echo Rebuilding make dependencies
	$(CPP) -MM $(CXXOBJS:.o=.cpp)  > .Makefile.depend
#	$(CC) -MM $(COBJS:.o=.c)  >> .Makefile.depend
	@echo


tags: TAGS
TAGS: *.cpp $(HEADER) Makefile
	etags.emacs *.cpp $(HEADER)

debug:
	make DEBUG=1 all


# Test programs (some possibly out of date)

TEST_PROGRAMS=test_bitstream test_building_blocks test_slimlib \
	test_slimlibC

test: $(TEST_PROGRAMS)

test_bitstream: test_bitstream.o  $(LIBRARY) Makefile
	$(LINK.o) -o test_bitstream test_bitstream.o $(LIBS)

test_building_blocks: test_building_blocks.o bitstream.o
	$(LINK.o) -o $@ $<   bitstream.o bit_constants.o $(LIBS)

test_slimlibC: test_slimlibC.o $(LIBRARY)  Makefile
	$(LINK.o) -o $@ $< $(LIBS)

# The test_slimlibC.c should work when compiled by g++, too.
test_slimlib.cpp: test_slimlibC.c
	ln -s $< $@

test_slimlib: test_slimlib.cpp $(LIBRARY)  Makefile
	$(LINK.o) -o $@ $<  $(LIBS)



# Cleaning
.PHONY: clean distclean nocores again

clean: nocores
	$(RM) *~ *.o *.a *.so build_bit_constants_h bit_constants.h \
	$(TEST_PROGRAMS) \
	$(LIBDIR)/*.so $(LIBDIR)/*.a bits_out_test.txt

distclean: clean
	$(RM)  ../bin/{slim,slimcat,unslim,slim_dump} Makefile

nocores:
	$(RM)  core* vgcore*

again: clean all

include $(DEPEND)
