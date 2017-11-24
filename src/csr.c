#include "csr.h"

#include <assert.h>
//------------------------------------------------------------------------------
void csr_matrix_vector_multiply(Vector *y, CSRMatrix *A, Vector *x) {
  /* Asserts:
       - #Rows of A == size of y == size of x
  */
  assert(A->nrows == y->size);
  assert(A->nrows == x->size);

  int    *ia = A->ia;
  int    *ja = A->ja;
  double *va = A->va;
  double *vx = x->vv;
  double *vy = y->vv;
  int      n = A->nrows;

  for (int i=0; i<n; i++) {
    vy[i] = 0.0;
    for (int j = ia[i]; j < ia[i + 1]; j++) {
      vy[i] += va[j]*vx[ja[j]];
    }
  }
}
//------------------------------------------------------------------------------
