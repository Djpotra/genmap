#ifndef _CSR_H_
#define _CSR_H_

#include "linalg.h"

//------------------------------------------------------------------------------
// CSRMatrix structure
struct genmap_csr {
  int *ia;
  int *ja;
  int *va;
};
typedef struct genmap_csr CSRMatrix;

//------------------------------------------------------------------------------
// CSRMatrix vector multiplication
void csr_matrix_vector_multiply(Vector *y, CSRMatrix *A, Vector *x);
//------------------------------------------------------------------------------

#endif
