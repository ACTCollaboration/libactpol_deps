/// \file verify_test_bitstream.cpp
/// Not sure this even builds anymore.  TO DO: remove from slim.

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
#include <sys/stat.h>
#include <unistd.h>
#include "slim.h"
#include "slimlib.h"

void expand_one_file(const char *compname) 
{
  const bool preserve_input = true;
  const bool force_clobber = false;
  const char RAW_SUFFIX[] = ".raw";

  // Verify that file exists
  struct stat st;

  // Create appropriate raw file name.
  // Normally, this means stripping any trailing dot-suffix.
  // But if we have --preserve, then append a ".raw" suffix instead.
  char *rawname;
  const char *last_suffix = strrchr(compname,'.');
  size_t baselen;
  if (last_suffix)
    baselen = (last_suffix-compname);
  else
    baselen=strlen(compname);

  rawname = new char[baselen+strlen(RAW_SUFFIX)+5];
  strncpy(rawname, compname, baselen);
  if (last_suffix == NULL)
    strncpy(rawname+baselen, RAW_SUFFIX, strlen(RAW_SUFFIX));
  else if (preserve_input)
    strncpy(rawname+baselen, RAW_SUFFIX, strlen(RAW_SUFFIX));
  else
    rawname[baselen] = '\0';

  // Make sure not to clobber the output file unless desired.
  if (!force_clobber) {
    int ret = stat(rawname, &st);
    if (ret == 0) {
      bad_file bf = bad_file(rawname, ": raw file exists (use -k to force).");
      delete [] rawname;
      throw bf;
    }
  }

  // Do the expansion, deleting the compressed file (unless --preserve).
  slim_expander_t *expander = new slim_expander_t(compname);
  expander->expand_to_file(rawname);

  delete expander;
  delete [] rawname;
  if (!preserve_input)
    unlink(compname);
}

int main(int argc, char *argv[]) 
{
#ifndef HAVE_LIBZZIP
  cerr << "libzzip is not available" << endl;
  exit(1);
#else

  slim_control *ctrl = new slim_control();

  ctrl->process_args(argc, argv);
  argv += optind;
  argc -= optind;

  if (argc <= 0) {
    ctrl->usage();
  } else {

    ctrl->unslim();
    while (argc--) {
      try {
        expand_one_file(*argv++);
      } catch (const bad_file &bf) {
        // Handle errors specific to this one file with a message to stderr.
        bf.mesg();
        exit(1);
    
      } catch (char const *error) {
        // In general error case, abort.
        cerr << "Caught error: " << error << "\n";
        exit(1);
      }
    }
  }
#endif

}
