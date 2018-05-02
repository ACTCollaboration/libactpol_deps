#
# Set PREFIX before installing.
#

libdir ?= $(PREFIX)/lib
includedir ?= $(PREFIX)/include

SRCDIR = c/src/

LIBNAME = libsofa_c.so
HEADERS = $(SRCDIR)/sofa.h $(SRCDIR)/sofam.h


SRC = $(wildcard $(SRCDIR)/*.c)

OBJECTS = $(SRC:.c=.o)

# Note you might want to empty CFLAGS/CPPFLAGS, before compiling, if
# there are installed header files that conflict with the new ones.
CPPFLAGS += -I$(SRCDIR)
CFLAGS += -fPIC -Wall

default: $(LIBNAME)

$(LIBNAME): $(OBJECTS)
	gcc -shared -Wl,-soname,$@.1 \
	    -o $@ $(OBJECTS)

clean:
	-rm $(OBJECTS)
	-rm $(LIBNAME)

$(OBJECTS): $(C_SRC)

install:
	install -t $(libdir) $(LIBNAME)
	install -t $(includedir) $(HEADERS)
