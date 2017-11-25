#include "io.h"
#include "test.h"
#include "mpiwrapper.h"

//------------------------------------------------------------------------------
int test_1() {
  long *glo_num, *header;

  readmap(&header, &glo_num, "nbrhd/nbrhd.map.bin");

  printf("npts = %ld\n", header[NPTS]);

  for (long i = 0; i < header[NPTS]; i++) {
    printf("%ld\n", glo_num[i]);
  }

  return 1;
}
//------------------------------------------------------------------------------
int main() {
  run_test(&test_1,"map_1");

  return 0;
}
