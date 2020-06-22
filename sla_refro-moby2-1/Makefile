# 
# Set PREFIX before installing.
#

FC ?= gfortran

FSOURCE = $(wildcard *.f)
FOBJECT = $(FSOURCE:%.f=%.o)

CSOURCE = wrap.c
COBJECT = wrap.o

OBJECT = $(FOBJECT) $(COBJECT)

NAME = slarefro
LIBNAME = lib$(NAME).so
HDRNAME = $(NAME).h

default: $(LIBNAME) $(HDRNAME)

$(LIBNAME): $(OBJECT)
	 $(CC) -shared -fPIC $^ -o $@ -fPIC -shared

$(HDRNAME): wrap.h
	cp wrap.h $(HDRNAME)

%.o : %.c
	$(CC) -fPIC -g -c -o $@ $<

%.o : %.f
	$(FC) -fPIC -g -c -o $@ $<

install:
	install -d $(PREFIX)/lib $(PREFIX)/include
	install $(LIBNAME) $(PREFIX)/lib
	install $(HDRNAME) $(PREFIX)/include


clean:
	-rm *.o $(LIBNAME) $(HDRNAME)

