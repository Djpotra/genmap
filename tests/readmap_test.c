#include "io.h"
#include "test.h"

//------------------------------------------------------------------------------
int test_1() {
  int npts, glo_num;

  readmap(&npts, &glo_num, "nbrhd.map");

  printf("npts = %d\n", npts);

  return 1;
}
//------------------------------------------------------------------------------
int main() {
  run_test(&test_1,"map_1");

  return 0;
}
