#include "linalg.h"
#include "csr.h"
#include "lanczos.h"
#include "test.h"

#include <stdio.h>
//------------------------------------------------------------------------------
int32 test_1() {
  int32         n = 5;
  double va[13] = {1, 2,          \
                   2, 2, 1,       \
                   1, 3, 2,    \
                   2, 4, 1, \
                   1, 5
                  };
  int32 ja[13]   = {0, 1,          \
                    0, 1, 2,       \
                    1, 2, 3,    \
                    2, 3, 4, \
                    3, 4
                   };
  int32 ia[6]    = {0, 2, 5, 8, 11, 13};
  CSRMatrix A = { .nrows = n, .ia = ia, .ja = ja, .va = va };

  Vector alpha, beta, init;
  zeros_vector(&alpha, n); zeros_vector(&beta, n - 1);
  ones_vector(&init, n);

  lanczos_serial(&alpha, &beta, &A, &init);

  printf("\nalpha: "); print_vector(&alpha);
  printf("\nbeta: "); print_vector(&beta);

  return 1;
}
//------------------------------------------------------------------------------
int32 test_1_2() {
  int32         n = 5;
  double va[13] = {1, 2,          \
                   2, 2, 1,       \
                   1, 3, 2,    \
                   2, 4, 1, \
                   1, 5
                  };
  int32 ja[13]   = {0, 1,          \
                    0, 1, 2,       \
                    1, 2, 3,    \
                    2, 3, 4, \
                    3, 4
                   };
  int32 ia[6]    = {0, 2, 5, 8, 11, 13};
  CSRMatrix A = { .nrows = n, .ia = ia, .ja = ja, .va = va };

  Vector alpha, beta, init;
  zeros_vector(&alpha, n); zeros_vector(&beta, n - 1);
  ones_vector(&init, n);

  lanczos_serial2(&alpha, &beta, &A, &init);

  printf("\nalpha: "); print_vector(&alpha);
  printf("\nbeta: "); print_vector(&beta);

  return 1;
}
//------------------------------------------------------------------------------
int32 test_2() {
  int32         n = 5;
  double va[13] = {1, 3,          \
                   3, 2, 3,       \
                   3, 3, 3,    \
                   3, 4, 3, \
                   3, 5
                  };
  int32 ja[13]   = {0, 1,          \
                    0, 1, 2,       \
                    1, 2, 3,    \
                    2, 3, 4, \
                    3, 4
                   };
  int32 ia[6]    = {0, 2, 5, 8, 11, 13};
  CSRMatrix A = { .nrows = n, .ia = ia, .ja = ja, .va = va };

  Vector alpha, beta, init;
  zeros_vector(&alpha, n); zeros_vector(&beta, n - 1);
  ones_vector(&init, n);

  lanczos_serial(&alpha, &beta, &A, &init);

  printf("\nalpha: "); print_vector(&alpha);
  printf("\nbeta: "); print_vector(&beta);

  return 1;
}
//------------------------------------------------------------------------------
int32 test_2_2() {
  int32         n = 5;
  double va[13] = {1, 3,          \
                   3, 2, 3,       \
                   3, 3, 3,    \
                   3, 4, 3, \
                   3, 5
                  };
  int32 ja[13]   = {0, 1,          \
                    0, 1, 2,       \
                    1, 2, 3,    \
                    2, 3, 4, \
                    3, 4
                   };
  int32 ia[6]    = {0, 2, 5, 8, 11, 13};
  CSRMatrix A = { .nrows = n, .ia = ia, .ja = ja, .va = va };

  Vector alpha, beta, init;
  zeros_vector(&alpha, n); zeros_vector(&beta, n - 1);
  ones_vector(&init, n);

  lanczos_serial2(&alpha, &beta, &A, &init);

  printf("\nalpha: "); print_vector(&alpha);
  printf("\nbeta: "); print_vector(&beta);

  return 1;
}
//------------------------------------------------------------------------------
int32 test_3() {
  int32         n = 4;
  double va[12] = { 2, -1, -1      \
                    -1,  2, -1,     \
                    -1, -1,  3, -1, \
                    -1,  1
                  };
  int32 ja[12]   = {0, 1, 2,      \
                    0, 1, 2,      \
                    0, 1, 2, 3,   \
                    2, 3,
                   };
  int32 ia[5]    = {0, 3, 6, 10, 12};
  CSRMatrix A = { .nrows = n, .ia = ia, .ja = ja, .va = va };

  Vector alpha, beta, init;
  zeros_vector(&alpha, n); zeros_vector(&beta, n - 1);
  ones_vector(&init, n);

  lanczos_serial(&alpha, &beta, &A, &init);

  printf("\nalpha: "); print_vector(&alpha);
  printf("\nbeta: "); print_vector(&beta);

  return 1;
}
//------------------------------------------------------------------------------
int32 test_3_2() {
  int32         n = 4;
  double va[12] = { 2, -1, -1      \
                    -1,  2, -1,     \
                    -1, -1,  3, -1, \
                    -1,  1
                  };
  int32 ja[12]   = {0, 1, 2,      \
                    0, 1, 2,      \
                    0, 1, 2, 3,   \
                    2, 3,
                   };
  int32 ia[5]    = {0, 3, 6, 10, 12};
  CSRMatrix A = { .nrows = n, .ia = ia, .ja = ja, .va = va };

  Vector alpha, beta, init;
  zeros_vector(&alpha, n); zeros_vector(&beta, n - 1);
  ones_vector(&init, n);

  lanczos_serial2(&alpha, &beta, &A, &init);

  printf("\nalpha: "); print_vector(&alpha);
  printf("\nbeta: "); print_vector(&beta);

  return 1;
}
//------------------------------------------------------------------------------
int32 test_4() {
  int32         n = 3;
  double va[9] = { 1, 2, 3,     \
                   2, 4, 5,     \
                   3, 5, 6
                 };
  int32 ja[9]   = {0, 1, 2,      \
                   0, 1, 2,      \
                   0, 1, 2
                  };
  int32 ia[4]    = {0, 3, 6, 9};
  CSRMatrix A = { .nrows = n, .ia = ia, .ja = ja, .va = va };

  Vector alpha, beta, init;
  zeros_vector(&alpha, n); zeros_vector(&beta, n - 1);
  ones_vector(&init, n);

  lanczos_serial(&alpha, &beta, &A, &init);

  printf("\nalpha: "); print_vector(&alpha);
  printf("\nbeta: "); print_vector(&beta);

  return 1;
}
//------------------------------------------------------------------------------
int32 test_5() {
  int32 n = 6;
  double va[20] = {2,   -1,               -1,       \
                   -1,    3,   -1,         -1,       \
                   -1,    2,   -1,             \
                   -1,    3,   -1,   -1, \
                   -1,   -1,         -1,    3,       \
                   -1,          1
                  };
  int32    ja[20] = {0,    1,                4,       \
                     0,    1,    2,          4,       \
                     1,    2,    3,             \
                     2,    3,    4,    5, \
                     0,    1,          3,    4,       \
                     3,          5
                    };
  int32 ia[7]     = {0, 3, 7, 10, 14, 18, 20};
  CSRMatrix A = { .nrows = n, .ia = ia, .ja = ja, .va = va };

  Vector alpha, beta, init;
  zeros_vector(&alpha, n); zeros_vector(&beta, n - 1);
  ones_vector(&init, n);

  lanczos_serial(&alpha, &beta, &A, &init);

  printf("\nalpha: "); print_vector(&alpha);
  printf("\nbeta: "); print_vector(&beta);

  return 1;
}
//------------------------------------------------------------------------------
int32 main() {
  run_test(&test_1,   "lnczs1");
  run_test(&test_1_2, "lnczs1_2");
  run_test(&test_2,   "lnczs2");
  run_test(&test_2_2, "lnczs2_2");
  run_test(&test_3,   "lnczs3");
  run_test(&test_3_2, "lnczs3_2");
  run_test(&test_4,   "lnczs4");
  run_test(&test_5,   "lnczs5");

  return 0;
}
//------------------------------------------------------------------------------
