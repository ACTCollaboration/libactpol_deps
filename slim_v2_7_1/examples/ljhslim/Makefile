#
# Warning: This Makefile is not the result of autoconf (obviously), and your
# mileage may vary.
# 

MYCXXFLAGS += -O3 -g
DESTDIR = /usr/local
ifdef DEBUG
  MYCXXFLAGS = -O0 -g
endif
CXXFLAGS += $(MYCXXFLAGS)

.PHONY: all install clean

all: ljhslim

OBJECTS=ljhslim_control.o

ljhslim: ljhslim.cpp $(OBJECTS) Makefile
	$(CXX) -o $@ $(CXXFLAGS) $< $(OBJECTS) -lslim

%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) $<

install:
	sudo install -p ljhslim $(DESTDIR)/bin/ljhslim

clean:
	rm -rf *~ *.dSYM *.o ljhslim

