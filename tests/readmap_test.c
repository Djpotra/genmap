#include "io.h"
#include "test.h"
#include "gswrapper.h"

//------------------------------------------------------------------------------
int test_1() {
  long npts, nelt, *glo_num;

  readmap(&npts, &nelt, &glo_num, "nbrhd/nbrhd.map");

  printf("npts = %ld\n", npts);

  for (long i = 0; i < npts; i++) {
    printf("%ld\n", glo_num[i]);
  }

  return 1;
}
//------------------------------------------------------------------------------
int main() {
  run_test(&test_1,"map_1");

  return 0;
}
