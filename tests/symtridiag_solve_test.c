#include "mpiwrapper.h"
#include "test.h"
#include "linalg.h"

#include <stdio.h>
//------------------------------------------------------------------------------
int test_1()
{
  int32 n = 3;
  Vector alpha, beta, b, x;

  ones_vector (&alpha, n   );
  zeros_vector(&beta, n - 1);
  ones_vector (&b, n       );

  symtridiag_solve(&x, &b, &alpha, &beta);

  for (int32 i = 0; i < n; i++) {
    printf("%lf, ", x.vv[i]);
  }
  printf("\n");

  return 1;
}

//------------------------------------------------------------------------------
int test_2()
{
  int32 n = 4;
  Vector alpha, beta, b, x;

  ones_vector(&alpha, n   ); scale_vector(&alpha, &alpha, 2.0);
  ones_vector(&beta, n - 1); scale_vector(&beta, &beta, -1.0 );
  ones_vector(&b, n       );

  symtridiag_solve(&x, &b, &alpha, &beta);

  for (int32 i = 0; i < n; i++) {
    printf("%lf, ", x.vv[i]);
  }
  printf("\n");

  Vector correct; create_vector(&correct, n);
  correct.vv[0]=2.0, correct.vv[1]=3.0, correct.vv[2]=3.0, correct.vv[3]=2.0;

  return vectors_equal(&correct, &x, 10e-8);
}

//------------------------------------------------------------------------------
int test_3()
{
  int32 n = 4;
  Vector alpha, beta, b, x;

  create_vector(&alpha, n);
  alpha.vv[0]=2.0,alpha.vv[1]=4.0,alpha.vv[2]=3.0,alpha.vv[3]=5.0;

  ones_vector(&beta, n - 1),beta.vv[0]=1.0,beta.vv[1]=-1.0,beta.vv[2]=7.0;
  ones_vector(&b, n),b.vv[0]=3.0,b.vv[1]=4.0,b.vv[2]=9.0,b.vv[3]=12.0;

  symtridiag_solve(&x, &b, &alpha, &beta);

  for (int32 i = 0; i < n; i++) {
    printf("%lf, ", x.vv[i]);
  }
  printf("\n");

  Vector correct; create_vector(&correct, n);
  correct.vv[0]=1.0, correct.vv[1]=1.0, correct.vv[2]=1.0, correct.vv[3]=1.0;

  return vectors_equal(&correct, &x, 10e-8);
}

//------------------------------------------------------------------------------
int32 main(int32 argc, char** argv)
{
  run_test(&test_1, "symtridiag_solve_1");
  run_test(&test_2, "symtridiag_solve_2");
  run_test(&test_3, "symtridiag_solve_3");

  return 0;
}

//------------------------------------------------------------------------------
