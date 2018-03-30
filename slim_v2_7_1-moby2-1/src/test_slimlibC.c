/// \file test_slimlibC.c
/// Test of slimlib linked from a C program.
/// Not really maintained (??)  TO DO: better test package!

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

#include "slimlib.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
  if (argc <= 1) {
    printf("usage: test_slimlibC slimfile\n");
    return 1;
  }
    
  SLIMFILE *sf = slimopen(argv[1], "r");
  if (sf == NULL) {
    fprintf(stderr,"Cannot open file!\n");
    return 1;
  }

  printf("file %s raw size is %ld\n", argv[1], slimrawsize(argv[1]));
  const int NPERLINE=4;
  unsigned int buf[NPERLINE];
  size_t n;
  do {
    long st = slimtell(sf);
    n = slimread(buf, 4, NPERLINE, sf);
    if (n>0)
      printf("%8.8ld : %11d %11d %11d %11d \n", st,
	     buf[0], buf[1], buf[2], buf[3]);
  } while (n>0);
  slimclose(sf);

  return 0;
}
