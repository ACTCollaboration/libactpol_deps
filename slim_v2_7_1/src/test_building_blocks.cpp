/// \file test_building_blocks.cpp
/// A really old, unmaintained program to test some stuff in the
/// obitstream and ibitstream classes.  To be deleted?  (JWF 3/17/2009)

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


#include <limits.h>
#include <iostream>
#include <iomanip>

#include "bitstream.h"
#include "slim_single_codec.h"

const char *binary_str(unsigned int u, int n, char implied='\0') {
  static char str[40];
  char *sp=str;
  if (implied)
    *sp++ = implied;
  for (int i=n-1; i>=0; i--) {
    if (u & bitNset[i])
      *sp++ = '1';
    else
      *sp++ = '0';
    if (i%4 == 0)
      *sp++ = ' ';
  }
  *sp = '\0';
  return str;
}

const char *binary_str(unsigned int u) {
  return binary_str(u, bit_size(u));
}
  
const char *binary_str( int i) {
  return binary_str(*(unsigned int *)(&i), bit_size(i));
}
  

int main() {
  unsigned int u=0;
  int i, k;

  obitstream *ob = new obitstream("test_bb.bin");

  const unsigned int two30=1073741824u;
  const unsigned int two31=2147483648u;
  const unsigned int testval[]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,
			  two31-2, two31-1, two31, two31+1, two31+2,
			  UINT_MAX-2, UINT_MAX-1, UINT_MAX, 0};

  const int NI=46;
  const int itestval[] = {
    INT_MIN, INT_MIN+1, INT_MIN+2, INT_MIN+3,
    -two30-1, -two30, -two30+1, -two30+2,
    -33, -32, -31, -17, -16, -15,
    -9, -8, -7, -6,
    -5, -4, -3, -2, -1, 0, 1, 2, 3, 4, 5, 
    7,8,9,15,16,17,31,32,33,
    two30-2, two30-1, two30, two30+1,
    INT_MAX-3, INT_MAX-2, INT_MAX-1, INT_MAX};

  for (i=0; i<27; i++)
    for (k=1; k<7; k++)
      mexp_golomb_write(ob, testval[i], k);
  for (i=0; i<NI; i++)
    for (k=1; k<7; k++)
      mexp_golomb_write(ob, itestval[i], k);
  ob->writebits(0,1);
//   ob->print();
  delete ob;

  ibitstream *ib = new ibitstream("test_bb.bin");
  cout << endl << "Unsigned integers:" << endl;
  for (i=0; i<27; i++) {
    for (k=1; k<7; k++)
      cout << setw(10) << mexp_golomb_read(ib, u, k) << " ";
    cout << "  " << setw(2) <<bit_size(testval[i]) << endl;
  }
  cout << endl << "Signed integers:" << endl;
  for (i=0; i<NI; i++) {
    for (k=1; k<7; k++)
      cout << setw(11) << mexp_golomb_read(ib, i, k) << " ";
    cout << "  " << setw(2) <<bit_size(itestval[i]) << endl;
  }
  delete ib;
  

//   ob = new obitstream("test_ob.bin");
//   ob->writebits(~0u, 32);
//   ob->print();
//   delete ob;
  return 0;
}
