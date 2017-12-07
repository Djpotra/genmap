#ifndef _LANCZOS_H_
#define _LANCZOS_H_

#include "csr.h"
#include "laplacian.h"
#include "linalg.h"
#include "mpiwrapper.h"
#include "dtypes.h"

//------------------------------------------------------------------------------
// Lanczos iteration

void lanczos(Vector *alpha, Vector *beta, struct comm *c, int32 *glo_num,
            Vector *init, int32 nc, int32 lelt, int32 iter);

void lanczos_serial (Vector *alpha, Vector *beta, CSRMatrix *A, Vector *init);

void lanczos_serial2(Vector *alpha, Vector *beta, CSRMatrix *A, Vector *init);
//------------------------------------------------------------------------------

#endif
