/// \file build_bit_constants_h.cpp
/// Create the file bit_constants.h, which is way too structured for a human to
/// maintain it.

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

#include <cstdlib>
#include <stdio.h>
#include <stdint.h>
#include <limits.h>


void fail_bit_count() {
  printf("Can only write 32 or 64 bits.  Default is 32.\n"
	 "usage: build_bit_constants_h [32|64]\n");
  throw "Can only write 32 or 64 bits.\n";
}

/// Print the complete header file.
/// \param fp  The file open for writing.
void headerfile(FILE *fp, int nbits=32) {
  fprintf(fp, "// -*- mode: c++; -*-\n\n");
  fprintf(fp, "/// \\file bit_constants.h\n");
  fprintf(fp, "/// Include file to point to pre-defined constant bit patterns\n");
  fprintf(fp, "/// All the constants are designed to speed slim/unslim\n\n");
  fprintf(fp, "// --------------------------------------------------------\n");
  fprintf(fp, "// WARNING!  This file is automatically generated by\n"
	  "// program "__FILE__ "  Do not edit directly.\n");
  fprintf(fp, "// See that program for copyright information.\n");
  fprintf(fp, "// --------------------------------------------------------\n");
  fprintf(fp, "\n");
  fprintf(fp, "#ifndef SLIM_BIT_CONSTANTS_H\n");
  fprintf(fp, "#define SLIM_BIT_CONSTANTS_H\n");
  fprintf(fp, "\n\n#include <stdint.h>\n\n");
  fprintf(fp, "typedef uint8_t Byte_t;   ///< Byte-oriented buffer for I/O\n");
  fprintf(fp, "typedef uint%d_t Word_t;  ///< Word-oriented type for using buffer.\n", nbits);
  fprintf(fp, "\n");
  fprintf(fp, "extern const Word_t bitNset[%d], highestNset[%d], lowestNset[%d];\n",
	  nbits, nbits+1, nbits+1);
  fprintf(fp, "extern const int32_t *lowestNset32bits;\n\n");
  fprintf(fp, "#endif // SLIM_BIT_CONSTANTS_H\n");
}


/// Print the C++ file preface.
/// \param fp  The file open for writing.
void preface(FILE *fp, int nbits=32) {
  fprintf(fp, "/// \\file bit_constants.cpp\n");
  fprintf(fp, "/// Source file with pre-defined constant bit patterns\n");
  fprintf(fp, "/// All the constants are designed to speed slim/unslim\n\n");
  fprintf(fp, "// --------------------------------------------------------\n");
  fprintf(fp, "// WARNING!  This file is automatically generated by\n"
	  "// program "__FILE__ "  Do not edit directly.\n");
  fprintf(fp, "// --------------------------------------------------------\n");
  fprintf(fp, "\n#include \"bit_constants.h\"\n\n");
}



/// Print constants having exactly one bit set.
/// \param fp    The file open for writing.
/// \param nbits The number of bits in the fundamental word
void single_bit_set(FILE *fp, int nbits=32) {
  fprintf(fp,"/// Constants with only bit #N set.\n");
  fprintf(fp,"const Word_t bitNset[%d] = {\n", nbits);
  long long value=1;
  int i;

  switch(nbits) {
  case 32:
    for ( i=0; i<8; i++) {
      fprintf(fp,"  0x%8.8llx, ",value);    value <<= 1;
      fprintf(fp,"0x%8.8llx, ",value);    value <<= 1;
      fprintf(fp,"0x%8.8llx, ",value);    value <<= 1;
      fprintf(fp,"0x%8.8llx,\n",value);    value <<= 1;
    }
    break;

  case 64:
    for ( i=0; i<8; i++) {
      fprintf(fp,"  0x%8.8llxll, ",value);    value <<= 1ll;
      fprintf(fp,"0x%8.8llxll, ",  value);    value <<= 1ll;
      fprintf(fp,"0x%8.8llxll, ",  value);    value <<= 1ll;
      fprintf(fp,"0x%8.8llxll,\n", value);    value <<= 1ll;
    }
    for ( i=0; i<16; i++) {
      fprintf(fp,"  0x%16.16llxll, ",value);    value <<= 1ll;
      fprintf(fp,"0x%16.16llxll,\n", value);    value <<= 1ll;
    }
    break;

  default:
    fail_bit_count();
    break;

  }  fprintf(fp,"};\n\n\n");
}



/// Print the header file preface.
/// \param fp  The file open for writing.
/// \param nbits The number of bits in the fundamental word
void lowest_bits_set(FILE *fp, int nbits=32) {
  fprintf(fp,"/// Constants with their lowest N bits set.\n");
  fprintf(fp,"const Word_t lowestNset[%d] = {\n", nbits+1);
  fprintf(fp,"  0x0,\n");
  long long value=1;

  switch (nbits) {
  case 32:
    for (int i=0; i<8; i++) {
      fprintf(fp,"  0x%8.8llx, ",value);    value = 1+(value<<1);
      fprintf(fp,"0x%8.8llx, ",value);      value = 1+(value<<1);
      fprintf(fp,"0x%8.8llx, ",value);      value = 1+(value<<1);
      fprintf(fp,"0x%8.8llx,\n",value);     value = 1+(value<<1);
    }
    fprintf(fp,"};\n");
    fprintf(fp,"const int32_t *lowestNset32bits = "
	    "reinterpret_cast<const int32_t *>(lowestNset);\n\n\n");
    break;

  case 64:
    for (int i=0; i<8; i++) {
      fprintf(fp,"  0x%8.8llxll, ",value);    value = 1+(value<<1);
      fprintf(fp,"0x%8.8llxll, ",value);      value = 1+(value<<1);
      fprintf(fp,"0x%8.8llxll, ",value);      value = 1+(value<<1);
      fprintf(fp,"0x%8.8llxll,\n",value);     value = 1+(value<<1);
    }
    for (int i=0; i<16; i++) {
      fprintf(fp,"  0x%16.16llxll, ",value);    value = 1+(value<<1);
      fprintf(fp,"0x%16.16llxll,\n",value);     value = 1+(value<<1);
    }
    
    fprintf(fp,"};\n\n\n/// 32-bit constants with their lowest N bits set.\n");
    fprintf(fp,"const int32_t _dummy_lowestNset32bits[33] = {\n");
    fprintf(fp,"  0x0,\n");
    value=1;
    for (int i=0; i<8; i++) {
      fprintf(fp,"  0x%8.8llx, ",value);    value = 1+(value<<1);
      fprintf(fp,"0x%8.8llx, ",value);      value = 1+(value<<1);
      fprintf(fp,"0x%8.8llx, ",value);      value = 1+(value<<1);
      fprintf(fp,"0x%8.8llx,\n",value);     value = 1+(value<<1);
    }
    fprintf(fp,"};\n");
    fprintf(fp,"const int32_t *lowestNset32bits = _dummy_lowestNset32bits;\n\n\n");
    break;

  default:
    fail_bit_count();
    break;
  }
}



/// Print the header file preface.
/// \param fp  The file open for writing.
/// \param nbits The number of bits in the fundamental word
void highest_bits_set(FILE *fp, int nbits=32) {
  fprintf(fp,"/// Constants with their highest N bit set.\n");
  fprintf(fp,"const Word_t highestNset[%d] = {\n", nbits+1);
  fprintf(fp,"  0x0,\n");

  switch(nbits) {
  case 32:
    {
      int32_t value = 0x80000000;
      for (int i=0; i<8; i++) {
	fprintf(fp,"  0x%8.8x, ",value);    value >>= 1;
	fprintf(fp,"0x%8.8x, ",value);      value  >>= 1;
	fprintf(fp,"0x%8.8x, ",value);      value  >>= 1;
	fprintf(fp,"0x%8.8x,\n",value);     value  >>= 1;
      }
    }
    break;

  case 64:
    {
      long long value = 0x8000000000000000ll;
      for (int i=0; i<32; i++) {
	fprintf(fp,"  0x%16.16llxll, ",value);    value >>= 1;
	fprintf(fp,"0x%16.16llxll,\n",value);     value  >>= 1;
      }
    }
    break;

  default:
    fail_bit_count();
    break;
  }
  fprintf(fp,"};\n\n\n");
}



int main(int argc, char *argv[]) {
  const int nbits_native = 8*sizeof(long);
  int nbits = nbits_native;
  if (argc > 1) {
    nbits = atoi(argv[1]);
    if (nbits != 32 && nbits != 64) {
      fail_bit_count();
    }
    if (nbits != nbits_native) {
      printf("********************************************************************\n");
      printf("Warning!  Native word size seems to be %d bits, but you specified %d "
	     "bits.\n", nbits_native, nbits);
      printf("This choice is probably only appropriate for testing or cross-compiling.\n");
      printf("Proceeding with %d-bit words.\n", nbits);
      printf("********************************************************************\n");
    }
  } else
    printf("Building bit_constants.{h,cpp} using %d-bit words.\n", nbits);
	  
  FILE *fph = fopen("bit_constants.h", "w");
  FILE *fpc = fopen("bit_constants.cpp", "w");
  if (fph == NULL || fpc == NULL) {
    printf("Could not open output files in %s\n", __FILE__);
    if (fph)
      fclose(fph);
    if (fpc)
      fclose(fpc);
    exit(1);
  }

  headerfile(fph, nbits);
  fclose(fph);

  preface(fpc, nbits);
  single_bit_set(fpc, nbits);
  lowest_bits_set(fpc, nbits);
  highest_bits_set(fpc, nbits);
  fclose(fpc);
}
