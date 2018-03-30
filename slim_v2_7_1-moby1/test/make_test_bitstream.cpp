/// \file make_test_bitstream.cpp
/// Make a test bitstream using the obitstream class.

//  Copyright (C) 2008, 2009 Joseph Fowler
//
//  This file is part of slim, a compression package for science data.
//
//  Slim is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  Slim is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with slim.  If not, see <http://www.gnu.org/licenses/>.


#include "bitstream.h"
#include "test_bitstream.h"


int main() {
  obitstream *ob = new obitstream(filename);

  int nbits=1;
  do {
    for (int i=0; i<N_TEST_VALUES; i++) {
      rawdata_t value = TEST_VALUES[i];
      if (nbits < 32)
	value = (value << (32-nbits)) >> (32-nbits);
      ob->writebits(TEST_VALUES[i], nbits);
      //printf("Writing size %2d value x%8.8x\n", nbits, value);
    }
    nbits = (nbits+SIZE_STEPS) % MAXSIZE;
    if (nbits==0)
      nbits=MAXSIZE;
  } while (nbits != 1);
  
  ob->writebits(0,1);
  ob->close();
  delete ob;
}
