#ifndef _CSR_H_
#define _CSR_H_

#include "linalg.h"
#include "gswrapper.h"
#include "laplacian.h"
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

void lanczos(Vector *alpha, Vector *beta, \
  struct gs_data *gsh, double *weights, int nc, Vector *init, int iter);

void lanczos_serial (Vector *alpha, Vector *beta, CSRMatrix *A, Vector *init);

void lanczos_serial2(Vector *alpha, Vector *beta, CSRMatrix *A, Vector *init);
//------------------------------------------------------------------------------

#endif
