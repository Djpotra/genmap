#include "test.h"
#include "gswrapper.h"
#include "laplacian.h"
#include "linalg.h"
#include "io.h"

//------------------------------------------------------------------------------
int test_1() {
  Vector v, u;

  struct comm c;
  struct gs_data *gsh;

  long npts, nelt, *glo_num;

  readmap(&npts, &nelt, &glo_num, "nbrhd.map");

//  ax(&v, &u);

  return 1;
}
//------------------------------------------------------------------------------
int main() {
  run_test(&test_1, "lapla1");

  return 0;
}
//------------------------------------------------------------------------------
