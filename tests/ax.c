#include "linalg.h"
#include "csr.h"
#include "test.h"

//------------------------------------------------------------------------------
int test_1() {
  double va[13] = {2, 1,          \
                   1, 2, 1,       \
                      1, 2, 1,    \
                         1, 2, 1, \
                            2, 1};
  int ja[13]   = {0, 1,          \
                  0, 1, 2,       \
                     1, 2, 3,    \
                        2, 3, 4, \
                           3, 4};
  int ia[6]    = {0, 2, 5, 8, 11, 13};

  double vx[5] = {1.0, 1.0, 1.0, 1.0, 1.0};
  double vy[5];
  double vans[5] = {3.0, 4.0, 4.0, 4.0, 3.0};

  CSRMatrix A = { .nrows = 5, .ia = ia, .ja = ja, .va = va };
  Vector x = { .size = 5, .vv = vx };
  Vector y = { .size = 5, .vv = vy };
  Vector answer = {.size = 5, .vv = vans };

  csr_matrix_vector_multiply(&y, &A, &x);

  int equal = is_vectors_equal(&y, &answer, 1e-12);

  return equal;
}
//------------------------------------------------------------------------------
int test_2() {
  double va[13] = {2, 1,          \
                   1, 2, 1,       \
                      1, 2, 1,    \
                         1, 2, 1, \
                            2, 1};
  int ja[13]   = {0, 1,          \
                  0, 1, 2,       \
                     1, 2, 3,    \
                        2, 3, 4, \
                           3, 4};
  int ia[6]    = {0, 2, 5, 8, 11, 13};

  double vx[5] = {0.0, 0.0, 0.0, 0.0, 0.0};
  double vy[5];
  double vans[5] = {0.0, 0.0, 0.0, 0.0, 0.0};

  CSRMatrix A = { .nrows = 5, .ia = ia, .ja = ja, .va = va };
  Vector x = { .size = 5, .vv = vx };
  Vector y = { .size = 5, .vv = vy };
  Vector answer = {.size = 5, .vv = vans };

  csr_matrix_vector_multiply(&y, &A, &x);

  int equal = is_vectors_equal(&y, &answer, 1e-12);

  return equal;
}
//------------------------------------------------------------------------------
int main() {
  run_test(&test_1, "ax_1");
  run_test(&test_2, "ax_2");

  return 0;
}
//------------------------------------------------------------------------------
