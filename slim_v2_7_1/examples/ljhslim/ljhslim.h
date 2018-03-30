#ifndef LJHSLIM_H
#define LJHSLIM_H

#include <slim.h>

class ljhslim_control : public slim_control
{
 public:
  ljhslim_control();
  virtual ~ljhslim_control() {;}
  void process_args(int argc, char * const argv[]);
  void usage() const;
  void version() const;
  void handle_one_file(const char *fname);

 protected:
  void usage_printoptions() const;
  void set_defaults();
  virtual void compress_one_file(const char *fname);
};

#endif // LJHSLIM_H guard
