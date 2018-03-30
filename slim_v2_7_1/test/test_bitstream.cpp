/// \file test_bitstream.cpp
/// Some undocumented tests of the obitstream and ibitstream classes.

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

#include <cstdio>
#include <cstdlib>

#include "bitstream.h"

int main() {
  const int num_words=1350000; // ~ 45 MB ~ 1 sec to read AND write.
  const int num_bits[15]={29,26,2,6,4,7,10,13,16,29,19,22,25,28,31};
  int jmax = 1;
  //  goto input;

  obitstream *OB = new obitstream("bits_out_test.txt");
  
  OB->writebits(0,8);
  OB->writebits(1,8);
  OB->writebits(2,8);
  OB->writebits(3,8);

  for (int i=1; i<=8; i++) {
    jmax *= 2;
    for (int j=0; j<jmax; j++) {
      OB->writebits(j,i);
    }
  }

  //OB->print();
  //OB->flush();

  for (int i=0; i<num_words; i++)
    for (int j=0; j<15; j++)
      OB->writebits(j, num_bits[j]);
  // implied: OB->close();
  delete OB;

 input:
  ibitstream *IB= new ibitstream("bits_out_test.txt");
  for (int i=0; i<4; i++)
    printf("%u\n",IB->readbits(8));

  jmax = 1;
  for (int i=1; i<=8; i++) {
    jmax *= 2;
    for (int j=0; j<jmax; j++) {
      printf("%3u ",IB->readbits(i));
    }
    printf("\n");
  }
  
  int i, val;
  try {
    for (i=0; ; i++) 
      for (int j=0; j<15; j++) {
	val = IB->readbits(num_bits[j]);
	if (val != j) printf ("Yo! %d %d %d\n",i,j,val);
	//if (val == 10) val = IB->readbits(32);
      }
  } catch (const char *s) {
    printf("Read a total of %d frames\n",i);
  }
  // implied: IB->close();
  delete IB;

  return 0;
}
