#include "csr.h"
#include "linalg.h"

#include <stdio.h>
//------------------------------------------------------------------------------
void lanczos(Vector *alpha, Vector *beta, CSRMatrix *A, Vector *init) {
  /* Asserts:
       - #Rows of A == size of alpha - 1 == size of beta - 1 = size of init
  */
  assert(A->nrows + 1 == alpha->size);
  assert(A->nrows == init->size);
  assert(A->nrows + 1 == beta->size);

  int n = A->nrows;
  double norm_q1;
  Vector q0, q1, u;

  // Set q_0 and beta_0 to zero (both uses 1-indexing)
  zeros_vector(&q0, n);
  beta->vv[0] = 0.;

  // Create vector u
  create_vector(&u, n);

  // Set q1 to normalized initial vector
  create_vector(&q1,    n);
  copy_vector  (&q1, init);
  norm_q1 = norm_vector(&q1, 2);
  mult_scalar_add_vector(&q1, 0., &q1, 1./norm_q1);

  for (int k = 1; k <= n; k++) {
    csr_matrix_vector_multiply(&u, A, &q1);

    alpha->vv[k] = dot_vector(&q1, &u);

    mult_scalar_add_vector(&u, 1.0, &q0, -beta ->vv[k - 1]);
    mult_scalar_add_vector(&u, 1.0, &q1, -alpha->vv[k]);

    beta->vv[k] = norm_vector(&u, 2);

    copy_vector(&q0, &q1);
    mult_scalar_add_vector(&q1, 0., &u, 1./beta->vv[k]);
  }
}
//------------------------------------------------------------------------------
