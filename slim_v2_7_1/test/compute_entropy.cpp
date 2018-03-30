/// \file compute_entropy.cpp
/// Find the shannon entropy of one or more data files, treating the files
/// as a series of N-bit words (and repeating for N from 1 to 24).
///
/// J. Fowler, Princeton
/// January 9, 2008

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


#include <math.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "bitstream.h"

/// Compute the log-base-2 of a number
/// \param x  The argument to be logged.
inline double alog2(double x) {
  return log(x)/M_LN2;
}

/// Return the size (bytes) of a given file.
/// \param filename  The path of the file to test.
size_t filesize(const char *filename) {
  struct stat s;
  stat(filename, &s);
  return s.st_size;
}

const int MAX_BITS = 24;   ///< Max number of bits to consider at once.
const bool VERBOSE=false;  ///< Whether to report on every histogram bin.


/// Compute the Shannon entropy of a file, treated as raw binary words.
/// \param  The path to the file to be tested.
/// \param  The word size in bits.
/// \return The Shannon entropy in bits per bit. 
double compute_entropy(const char *filename, int bits) {
  if (bits <= 0 || bits > MAX_BITS) {
    fprintf(stderr,"Cannot compute entropy with %d bits: "
	    "must be between 0 and %d\n", bits, MAX_BITS);
    return 0.0;
  }

  unsigned int nbins = 1;
  for (int i=0; i<bits; i++)
    nbins *= 2;

  int *hist = new int[nbins];
  ibitstream *ib = new ibitstream(filename);
  size_t size = filesize(filename);
  
  int nwords = size*8/bits-1;
  unsigned int data;
  for (int i=0; i<nwords; i++) {
    data = ib->readbits(bits);
    if (data < nbins)
      hist[data] ++;
  }

  int nentries = 0;
  for (unsigned int i=0; i<nbins; i++)
    nentries += hist[i];


  // Take N partial sums of the entropy N at a time, where N = sqrt(nbits)
  // This mitigates the problem of summing large += small, thought that
  // turns out not to have been an issue.
  double prob;
  double entropy = 0, entropy_partial=0;
  int partial_marker = 1;
  for (unsigned int m=1; m<nbins; m*=4)
    partial_marker *= 2;
  for (unsigned int i=0; i<nbins; i++) {
    prob = double(hist[i])/nentries;
    if (prob > 0)
      entropy_partial -= prob*alog2(prob);
    if ((i+1)%partial_marker == 0) {
      entropy += entropy_partial;
      entropy_partial = 0;
    }
    if (VERBOSE && nbins < 100)
      printf("%5d %5d %8.6f %8.6f  %9.6f\n", i, hist[i], 
	     prob, -prob*alog2(prob), entropy);
  }

  return entropy;
  delete [] hist;
  delete ib;
}



int main(int argc, const char *argv[]) {
  char default_name[] = "DATA/tescat";
  const char *default_filelist = default_name;
  const char **filelist = &default_filelist;
  int num_files = argc-1;
  if (argc > 1)
    filelist = argv+1;
  else
    num_files = 1;

  FILE *fp=fopen("entropy.pro","w");
  fprintf(fp,"PRO entropy\n");

  for (int j=0; j<num_files; j++) {
    printf ("File is %s\n", filelist[j]);
    fprintf(fp,"  %s=[",filelist[j]);
    for (int i=1; i<=MAX_BITS; i++) {
      double e=compute_entropy(filelist[j], i);
      printf("%2d: %9.6f %8.6f\n", i, e, e/i);
      fprintf(fp,"%8.6f,", e/i);
    }
    fseek(fp, -1, SEEK_CUR);
    fprintf(fp,"]\n");
  }

  fprintf(fp,
	  "  plot,[0,25],[.5,1],/nodata\n"
	  "  b=1+findgen(24)\n"
	  "  oplot,b,tescat\n"
	  "  oplot,b,thermom\n"
	  "  oplot,b,tescat_slm\n"
	  "end\n");
  fclose(fp);
}
