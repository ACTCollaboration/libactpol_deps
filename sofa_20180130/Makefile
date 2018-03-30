#
# Set PREFIX before installing.
#

libdir ?= $(PREFIX)/lib
includedir ?= $(PREFIX)/include

SRCDIR = c/src/

LIBNAME = libsofa_c.so
HEADERS = $(SRCDIR)/sofa.h


SRC = $(wildcard $(SRCDIR)/*.c)

OBJECTS = $(SRC:.c=.o)

CFLAGS += -fPIC -Wall -I$(SRCDIR)

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
