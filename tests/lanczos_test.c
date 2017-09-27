#include "linalg.h"
#include "csr.h"
#include "test.h"

#include <stdio.h>
//------------------------------------------------------------------------------
int test_1() {
  int         n = 5;
  double va[13] = {1, 2,          \
                   2, 2, 1,       \
                      1, 3, 2,    \
                         2, 4, 1, \
                            1, 5};
  int ja[13]   = {0, 1,          \
                  0, 1, 2,       \
                     1, 2, 3,    \
                        2, 3, 4, \
                           3, 4};
  int ia[6]    = {0, 2, 5, 8, 11, 13};
  CSRMatrix A = { .nrows = n, .ia = ia, .ja = ja, .va = va };

  Vector alpha, beta, init;
  zeros_vector(&alpha, n); zeros_vector(&beta, n - 1);
  ones_vector(&init, n);

  lanczos(&alpha, &beta, &A, &init);

  printf("\nalpha: "); print_vector(&alpha);
  printf("\nbeta: "); print_vector(&beta);

  return 1;
}
//------------------------------------------------------------------------------
int test_1_2() {
  int         n = 5;
  double va[13] = {1, 2,          \
                   2, 2, 1,       \
                      1, 3, 2,    \
                         2, 4, 1, \
                            1, 5};
  int ja[13]   = {0, 1,          \
                  0, 1, 2,       \
                     1, 2, 3,    \
                        2, 3, 4, \
                           3, 4};
  int ia[6]    = {0, 2, 5, 8, 11, 13};
  CSRMatrix A = { .nrows = n, .ia = ia, .ja = ja, .va = va };

  Vector alpha, beta, init;
  zeros_vector(&alpha, n); zeros_vector(&beta, n - 1);
  ones_vector(&init, n);

  lanczos2(&alpha, &beta, &A, &init);

  printf("\nalpha: "); print_vector(&alpha);
  printf("\nbeta: "); print_vector(&beta);

  return 1;
}
//------------------------------------------------------------------------------
int test_2() {
  int         n = 5;
  double va[13] = {1, 3,          \
                   3, 2, 3,       \
                      3, 3, 3,    \
                         3, 4, 3, \
                            3, 5};
  int ja[13]   = {0, 1,          \
                  0, 1, 2,       \
                     1, 2, 3,    \
                        2, 3, 4, \
                           3, 4};
  int ia[6]    = {0, 2, 5, 8, 11, 13};
  CSRMatrix A = { .nrows = n, .ia = ia, .ja = ja, .va = va };

  Vector alpha, beta, init;
  zeros_vector(&alpha, n); zeros_vector(&beta, n - 1);
  ones_vector(&init, n);

  lanczos(&alpha, &beta, &A, &init);

  printf("\nalpha: "); print_vector(&alpha);
  printf("\nbeta: "); print_vector(&beta);

  return 1;
}
//------------------------------------------------------------------------------
int test_2_2() {
  int         n = 5;
  double va[13] = {1, 3,          \
                   3, 2, 3,       \
                      3, 3, 3,    \
                         3, 4, 3, \
                            3, 5};
  int ja[13]   = {0, 1,          \
                  0, 1, 2,       \
                     1, 2, 3,    \
                        2, 3, 4, \
                           3, 4};
  int ia[6]    = {0, 2, 5, 8, 11, 13};
  CSRMatrix A = { .nrows = n, .ia = ia, .ja = ja, .va = va };

  Vector alpha, beta, init;
  zeros_vector(&alpha, n); zeros_vector(&beta, n - 1);
  ones_vector(&init, n);

  lanczos2(&alpha, &beta, &A, &init);

  printf("\nalpha: "); print_vector(&alpha);
  printf("\nbeta: "); print_vector(&beta);

  return 1;
}
//------------------------------------------------------------------------------
int main() {
  run_test(&test_1, "lnczs1");
  run_test(&test_1_2, "lnczs1_2");
  run_test(&test_2, "lnczs2");
  run_test(&test_2_2, "lnczs2_2");

  return 0;
}
//------------------------------------------------------------------------------
