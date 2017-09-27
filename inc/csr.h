#ifndef _CSR_H_
#define _CSR_H_

#include "linalg.h"

//------------------------------------------------------------------------------
// CSRMatrix structure
struct genmap_csr {
  int nrows;
  int *ia;
  int *ja;
  double *va;
};
typedef struct genmap_csr CSRMatrix;

//------------------------------------------------------------------------------
// CSRMatrix vector multiplication
void csr_matrix_vector_multiply(Vector *y, CSRMatrix *A, Vector *x);

//void csr_init(CSRMatrix *A, ....

void lanczos (Vector *alpha, Vector *beta, CSRMatrix *A, Vector *init);

void lanczos2(Vector *alpha, Vector *beta, CSRMatrix *A, Vector *init);
//------------------------------------------------------------------------------

#endif
