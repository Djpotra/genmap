#include <stdio.h>

#include "linalg.h"
#include "csr.h"

void test_1() {
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

  CSRMatrix A = { .ia = ia, .ja = ja, .va = va };
  Vector x = { .size = 5, .vv = vx };
  Vector y = { .size = 5, .vv = vy };

  csr_matrix_vector_multiply(&y, &A, &x);

}

int main() {
  test_1();

  return 0;
}
