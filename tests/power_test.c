#include <stdio.h>

#include "mpiwrapper.h"
#include "test.h"
#include "eig.h"

//------------------------------------------------------------------------------
int32 test_1() {
  int32 n = 3;
  Vector alpha, beta, eVector, init;

  create_vector(&alpha, n    ); ones_vector(&alpha, n);
  create_vector(&beta , n - 1); beta.vv[0] = 2.0; beta.vv[1] = 3.0;

  create_vector(&init , n    ); ones_vector(&init , n);
  create_vector(&eVector, n);

  power(&eVector, &alpha, &beta, &init, 15);

  printf("(%lf, %lf, %lf)\n", eVector.vv[0], eVector.vv[1], eVector.vv[2]);

  return 1;
}

//------------------------------------------------------------------------------
int32 test_2() {
  int32 n = 4;
  Vector alpha, beta, eVector, init;

  create_vector(&alpha, n    );
  for (int i = 0; i < n; i++) alpha.vv[i] = (double) i + 2.0;
  create_vector(&beta , n - 1); ones_vector(&beta , n - 1);

  create_vector(&init , n    ); ones_vector(&init , n    );
  create_vector(&eVector, n);

  power(&eVector, &alpha, &beta, &init, 15);

  printf("(%lf, %lf, %lf, %lf)\n",
                eVector.vv[0], eVector.vv[1], eVector.vv[2], eVector.vv[3]);

  return 1;
}

//------------------------------------------------------------------------------
int32 test_3() {
  int32 n = 3;
  Vector alpha, beta, eVector, init;

  create_vector(&alpha, n    ); ones_vector(&alpha, n);
  create_vector(&beta , n - 1); beta.vv[0] = 2.0; beta.vv[1] = 3.0;

  create_vector(&init , n    ); ones_vector(&init , n);
  create_vector(&eVector, n);

  invpower(&eVector, &alpha, &beta, &init, 15);

  printf("(%lf, %lf, %lf)\n", eVector.vv[0], eVector.vv[1], eVector.vv[2]);

  return 1;
}

//------------------------------------------------------------------------------
int32 test_4() {
  int32 n = 4;
  Vector alpha, beta, eVector, init;

  create_vector(&alpha, n    );
  for (int32 i = 0; i < n; i++) alpha.vv[i] = (double) i + 2.0;
  create_vector(&beta , n - 1); ones_vector(&beta , n - 1);

  ones_vector  (&init, n  );
  create_vector(&eVector, n);

  invpower(&eVector, &alpha, &beta, &init, 15);

  printf("(%lf, %lf, %lf, %lf)\n",
                eVector.vv[0], eVector.vv[1], eVector.vv[2], eVector.vv[3]);

  return 1;
}

//------------------------------------------------------------------------------
int32 main() {
  run_test(&test_1, "power_iteration_1");
  run_test(&test_2, "power_iteration_2");
  run_test(&test_3, "invpower_iteration_1");
  run_test(&test_4, "invpower_iteration_2");

  return 0;
}

//------------------------------------------------------------------------------
