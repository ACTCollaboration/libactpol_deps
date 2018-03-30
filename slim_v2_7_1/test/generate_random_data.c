/// \file generate_random_data.c
/// Create a file of random words with length specified on the
/// command line, using the system-supplied rand().  Seed rand with
/// the unix time in seconds since epoch.

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


#include <stdlib.h>
#include <stdio.h>
#include <time.h>

void usage() {
  printf("usage: generate_random_data Nbytes OutputFile\n");
  exit(0);
}

int main(int argc, char *argv[]) {
  if (argc<=2)
    usage();

  const int nbytes = atoi(argv[1]);
  char *fname = argv[2];

  FILE *fp = fopen(fname, "wb");
  unsigned int seed = time(NULL);
  srand(seed);
  int r;
  
  for (int i=0; i<nbytes/4; i++) {
    r=0;
    for (int j=0; j<12; j++)
      r += (rand()>>14);
    fwrite(&r, sizeof(r), 1, fp);
  }

  fclose(fp);
  return 0;
}
