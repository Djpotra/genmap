#include <stdio.h>

#include "linalg.h"
#include "csr.h"

void test_1() {
  double *va = {2, 1,          \
                1, 2, 1,       \
                   1, 2, 1,    \
                      1, 2, 1, \
                         2, 1};
  int *ja    = {0, 1,          \
                0, 1, 2,       \
                   1, 2, 3,    \
                      2, 3, 4, \
                         3, 4};
  int *ia = {0, 2, 5, 8, 11, 13};

  CSRMatrix A; A->va = va; A->ja = ja, A->ia = ia;
  Vector x; x->size = 5; x->vv = {1.0, 1.0, 1.0, 1.0, 1.0};
}

int main() {
  test_1();

  return 0;
}
