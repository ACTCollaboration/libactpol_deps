/// \file slim.cpp
/// Supplies a main program for the slim binary and for its
/// aliases "unslim" and "slimcat".

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

#include "slim.h"
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <sysexits.h>

int main(int argc, char *argv[]) 
{
  slim_control *ctrl = new slim_control();

  // Find base filename within path
  const char *basename = strrchr(argv[0], '/');
  if (basename)
    basename++;
  else
    basename = argv[0];

  //
  if (strcmp(basename, "unslim") == 0) {
    ctrl->unslim();
  } else if (strcmp(basename, "slimcat") == 0) {
    ctrl->slimcat();
  }

  ctrl->process_args(argc, argv);
  argv += optind;
  argc -= optind;

  int n_bad_files = 0;
  if (argc <= 0) {
    ctrl->usage();
  } else {
    while (argc--) {
      try {
        ctrl->handle_one_file(*argv++);

      } catch (const bad_file &bf) {
        // Handle errors specific to this one file with a message to stderr.
        bf.mesg();
        n_bad_files++;

      } catch (char const *error) {
        // In general error case, abort.
        cerr << "Caught error: " << error << "\n";
        exit(1);
      }
    }
  }

  delete ctrl;

  if (n_bad_files) {
    cerr << "Failed on "<<n_bad_files<<" files\n";
    return EX_USAGE;
  }
  return 0;
}
