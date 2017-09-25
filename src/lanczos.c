#include "csr.h"

//------------------------------------------------------------------------------
void lanczos(Vector *alpha, Vector *beta, CSRMatrix *A) {
  /* Asserts:
       - #Rows of A == size of alpha == size of beta - 1
  */
  assert(A->nrows == alpha->size);
  assert(A->nrows - 1 == beta->size);

  int n = A->nrows;
  Vector q0, q1;


}
