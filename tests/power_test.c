#include <stdio.h>

#include "mpiwrapper.h"
#include "test.h"
#include "eig.h"

//------------------------------------------------------------------------------
int32 test_1() {
  int32 n = 3;
  Vector alpha, beta, eVector, init;

  create_vector(&alpha, n    ); ones_vector(&alpha, n);
  create_vector(&init , n    ); ones_vector(&init , n);
  create_vector(&beta , n - 1); beta.vv[0] = 2.0; beta.vv[1] = 3.0;

  create_vector(&eVector, n);

  printf("(%lf, %lf, %lf)\n", alpha.vv[0], alpha.vv[1], alpha.vv[2]);

  power(&eVector, &alpha, &beta, &init, 10);

  printf("(%lf, %lf, %lf)\n", eVector.vv[0], eVector.vv[1], eVector.vv[2]);

  return 0;
}

//------------------------------------------------------------------------------
int32 main() {
  test_1();

  return 0;
}

//------------------------------------------------------------------------------
