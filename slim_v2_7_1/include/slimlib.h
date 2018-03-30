/// \file slimlib.h
/// Function prototypes for the slimlib C library.
/// Include this in order to use the C wrappers for slim
/// compress/expand operations and the SLIMFILE opaque type.

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

#ifndef _SLIMLIB_H
#define _SLIMLIB_H

#include <stdio.h> // for size_t

#ifdef __cplusplus /* If this is a C++ compiler, use C linkage */
extern "C" {
#endif

typedef struct slimlib_file_t  SLIMFILE;

extern SLIMFILE *slimopen(const char *filename,
			  const char *modes);

extern SLIMFILE *slimdopen(int fd, const char *modes);

extern int slimclose(SLIMFILE *sf);

extern size_t slimread(void *ptr, size_t size, size_t nmemb, SLIMFILE *sf);

extern long slimtell(SLIMFILE *sf);

extern void slimrewind(SLIMFILE *sf);

extern int slimseek(SLIMFILE *sf, long offset, int whence);

extern long slimrawsize(const char *filename);

extern long slimdrawsize(int fd);

#ifdef __cplusplus /* If this is a C++ compiler, end C linkage */
}
#endif

#endif // #ifndef _SLIMLIB_H
