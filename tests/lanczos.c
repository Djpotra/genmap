#include "linalg.h"
#include "csr.h"
#include "test.h"

#include <stdio.h>
//------------------------------------------------------------------------------
int test_1() {
  int         n = 5;
  double va[13] = {2, 1,          \
                   1, 2, 1,       \
                      1, 2, 1,    \
                         1, 2, 1, \
                            1, 2};
  int ja[13]   = {0, 1,          \
                  0, 1, 2,       \
                     1, 2, 3,    \
                        2, 3, 4, \
                           3, 4};
  int ia[6]    = {0, 2, 5, 8, 11, 13};
  CSRMatrix A = { .nrows = n, .ia = ia, .ja = ja, .va = va };

  Vector alpha, beta;
  zeros_vector(&alpha, n); zeros_vector(&beta, n + 1);

  lanczos(&alpha, &beta, &A);

  for (int i = 0; i < n; i++) {
    printf("%1.5f -- %1.5f\n", alpha.vv[i], beta.vv[i + 1]);
  }

  return 1;
}

int main() {
  run_test(&test_1, "lanczos1");

  return 0;
}
