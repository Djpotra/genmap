#include "mpiwrapper.h"
#include "test.h"
#include "linalg.h"

//------------------------------------------------------------------------------
int test_1()
{
  int32 n = 3;
  Vector alpha, beta, b, x;

  ones_vector (&alpha, n   );
  zeros_vector(&beta, n - 1);
//  zeros_vector(&x, n       );
  ones_vector (&b, n       );

  symtridiag_solve(&x, &b, &alpha, &beta); 

  for (int32 i = 0; i < n; i++) {
    printf("%lf, ", x.vv[i]);
  }
  printf("\n");

  return 0;
}

//------------------------------------------------------------------------------
int32 main(int32 argc, char** argv)
{
  run_test(&test_1, "symtridiag_solve");

  return 0;
}

//------------------------------------------------------------------------------
