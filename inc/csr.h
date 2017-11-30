#ifndef _CSR_H_
#define _CSR_H_

#include "linalg.h"
#include "dtypes.h"

//------------------------------------------------------------------------------
// CSRMatrix structure
struct genmap_csr {
  int32 nrows;
  int32 *ia;
  int32 *ja;
  double *va;
};
typedef struct genmap_csr CSRMatrix;

//------------------------------------------------------------------------------
// CSRMatrix vector multiplication
void csr_matrix_vector_multiply(Vector *y, CSRMatrix *A, Vector *x);

//------------------------------------------------------------------------------

#endif
