#include "csr.h"

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
  int      n = x->size;

  for (int i=0; i<n; i++) {
    y[i] = 0.0;
    for (int j = ia[i]; j < ia[i+1]; j++) {
      y[i] += va[j]*x[ja[j]];
    }
  }
}
//------------------------------------------------------------------------------
