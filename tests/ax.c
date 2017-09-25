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
  double vans[5] = {3.0, 4.0, 4.0, 4.0, 3.0};

  CSRMatrix A = { .ia = ia, .ja = ja, .va = va };
  Vector x = { .size = 5, .vv = vx };
  Vector y = { .size = 5, .vv = vy };
  Vector answer = {.size = 5, .vv = vans };

  csr_matrix_vector_multiply(&y, &A, &x);

  int equal = is_vectors_equal(&y, &answer, 1e-12);

  assert(equal == 1);
}

int main() {
  test_1();

  return 0;
}
