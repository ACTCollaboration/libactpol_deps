#include <unistd.h>
#include <slim.h>
#include <sysexits.h>
#include "ljhslim.h"

int main(int argc, char *argv[]) 
{
  ljhslim_control *ctrl = new ljhslim_control();

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
