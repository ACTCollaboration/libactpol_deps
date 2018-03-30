/// \file slim_dump.cpp
/// A standalone tool to read the slim file header info and print it.

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


#include <iostream>
#include <cstring>
#include "slim.h"

/// Dump the info from a single file
/// \param path  The path to the file.
void dump_one_file(const char *path) {
  FILE *fp;
  fp = fopen(path, "rb");
  if (fp == NULL) {
    cerr << "Could not open " << path << " for reading.\n";
    return;
  }
  
  char buf[2] = {};
  fread(buf, sizeof(char), 2, fp);
  fclose(fp);
  if (strncmp(buf, FILE_MAGIC, 2) != 0) {
    cerr << "File " <<path << " is not a slim file.  Skipping...\n";
    return;
  }

  slim_expander_t *sx = new slim_expander_t(path);
  try {
    sx->dump_sliminfo();
  } catch (const char *s) {
    cerr << "Caught error: "<<s<<"\n";
    exit(1);
  }

  delete sx;
}



/// Main program
/// \param argc Number of cmd-line tokens.
/// \param argv Command-line tokens.
/// \return Status value.
int main(int argc, char *argv[]) {

  for (int i=1; i<argc; i++)
    dump_one_file(argv[i]);

  return 0;
}
