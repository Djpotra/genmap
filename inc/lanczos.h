#ifndef _LANCZOS_H_
#define _LANCZOS_H_

#include "csr.h"
#include "laplacian.h"
#include "linalg.h"
#include "mpiwrapper.h"

//------------------------------------------------------------------------------
// Lanczos iteration

void lanczos(Vector *alpha, Vector *beta, struct gs_data *gsh,
            double *weights, int nc, Vector *init, int iter);

void lanczos_serial (Vector *alpha, Vector *beta, CSRMatrix *A, Vector *init);

void lanczos_serial2(Vector *alpha, Vector *beta, CSRMatrix *A, Vector *init);
//------------------------------------------------------------------------------

#endif
