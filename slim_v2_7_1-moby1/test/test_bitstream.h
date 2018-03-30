/// \file test_bitstream.h
/// Header for some undocumented tests of the obitstream and ibitstream 
/// classes.

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

#ifndef TEST_BITSTREAM_H
#define TEST_BITSTREAM_H

const char filename[]="test_bits.bin";

typedef uint32_t rawdata_t;

const int N_TEST_VALUES=4;
const rawdata_t TEST_VALUES[N_TEST_VALUES] = {
  0u, 1u, ~0u, 0x55555555u
};

const int SIZE_STEPS=7, MAXSIZE=32;

#endif // TEST_BITSTREAM_H
