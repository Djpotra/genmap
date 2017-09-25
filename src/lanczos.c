#include "csr.h"
#include "linalg.h"

//------------------------------------------------------------------------------
void lanczos(Vector *alpha, Vector *beta, CSRMatrix *A) {
  /* Asserts:
       - #Rows of A == size of alpha == size of beta - 1
  */
  assert(A->nrows == alpha->size);
  assert(A->nrows + 1 == beta->size);

  int n = A->nrows;
  Vector q0, q1, u;

  // Set q_0 and beta_0 to zero (beta uses 1-indexing, alpha uses 0)
  zeros_vector(&q0, n);
  beta->vv[0] = 0.;

  // Set q1 to normalize q0
  random_vector(&q1, n);
  mult_scalar_add_vector(&q1, 0., &q1, 1./norm_vector(&q1, 2));

  for (int k = 0; k < n; k++) {
    csr_matrix_vector_multiply(&u, A, &q1);

    alpha->vv[k] = dot_vector(&q1, &u);

    mult_scalar_add_vector(&u, 1.0, &q0, beta ->vv[k]);
    mult_scalar_add_vector(&u, 1.0, &q1, alpha->vv[k]);

    beta->vv[k + 1] = norm_vector(&u, 2);
    if (beta->vv[k + 1] < 1e-12) // if zero
      return;

    copy_vector(&q0, &q1);
    mult_scalar_add_vector(&q1, 0., &u, 1./beta->vv[k + 1]);
  }
}
