#include "csr.h"
#include "linalg.h"

#include <stdio.h>
//------------------------------------------------------------------------------
void lanczos(Vector *alpha, Vector *beta, CSRMatrix *A, Vector *init) {
  /* Asserts:
       - #Rows of A == size of alpha == size of beta - 1
  */
  // TODO: Change lanczos API to accept a random vector
  assert(A->nrows == alpha->size);
  assert(A->nrows == init->size);
  assert(A->nrows + 1 == beta->size);

  int n = A->nrows;
  Vector q0, q1, u;

  // Set q_0 and beta_0 to zero (beta uses 1-indexing, alpha uses 0)
  zeros_vector(&q0, n);
  beta->vv[0] = 0.;
  // Set u to zeros vector
  zeros_vector(&u, n);

  // Set q1 to normalized initi vector
  copy_vector(&q1, init);
  mult_scalar_add_vector(&q1, 0., &q1, 1./norm_vector(&q1, 2));

  for (int k = 0; k < n; k++) {
    csr_matrix_vector_multiply(&u, A, &q1);

    alpha->vv[k] = dot_vector(&q1, &u);

    mult_scalar_add_vector(&u, 1.0, &q0, -beta ->vv[k]);
    mult_scalar_add_vector(&u, 1.0, &q1, -alpha->vv[k]);

    beta->vv[k + 1] = norm_vector(&u, 2);
    if (beta->vv[k + 1] < 1e-17) // if zero
      return;

    copy_vector(&q0, &q1);
    mult_scalar_add_vector(&q1, 0., &u, 1./beta->vv[k + 1]);
  }
}
//------------------------------------------------------------------------------
void lanczos_ggavl(Vector *alpha, Vector *beta, CSRMatrix *A, Vector *init) {
  /* Asserts:
       - #Rows of A == size of alpha == size of beta - 1
  */
  // TODO: Change lanczos API to accept a random vector
  assert(A->nrows == alpha->size);
  assert(A->nrows == init->size);
  assert(A->nrows + 1 == beta->size);

  int n = A->nrows;
  double t;

  Vector v, w, u;

  // Set v and beta_0 to initial values (beta uses 1-indexing, alpha uses 0)
  zeros_vector(&v, n);
  beta->vv[0] = 1.;
  // Set w to zeros vector
  ones_vector(&w, n);
  mult_scalar_add_vector(&w, 0, &w, 1./norm_vector(&w, 2));

  for (int k = 0; beta->vv[k] > 3e-16; k++) {
    if (k) {
      for (int i = 0; i < n; i++) {
        t = w->vv[i]; w->vv[i] = v->vv[i]/beta->vv[k];
        v->vv[i] = -beta->[k]*t;
      }
    }

    csr_matrix_vector_multiply(&u, A, &w);
    mult_scalar_add_vector(&v, 1, &u, 1);

    alpha->vv[k] = dot_vector(&w, &v);
    mult_scalar_add_vector(&v, 1, &w, alpha->vv[k]);
    beta->vv[k + 1] = norm_vector(&v, 2);
  }
}
//------------------------------------------------------------------------------
