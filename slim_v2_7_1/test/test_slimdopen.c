
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "slimlib.h"

int main(int argc, char *argv[]) {
  
  const char SLIM_FILENAME[]="/tmp/fake_test_data.bin.slm";
  const char OUT_FILENAME[]="/tmp/fake_test_data.bin.raw";

  FILE *fp = fopen(SLIM_FILENAME, "rb");
  if (fp == NULL)
    return 1;

  int fd = fileno(fp);

  SLIMFILE *sp = slimdopen(fd, "r");
  FILE *fout = fopen(OUT_FILENAME, "wb");
  const size_t NDATA=8;
  int32_t data[NDATA];
  while (1) {
    int nread = slimread(data, sizeof(*data), NDATA, sp);
    if (nread < NDATA)
      break;
    fwrite(data, sizeof(*data), NDATA, fout);
  }

  slimclose(sp);
  fclose(fp);
  fclose(fout);
  return 0;
}
