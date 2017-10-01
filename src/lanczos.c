#include "csr.h"
#include "linalg.h"
#include "genmap.h"

#include <stdio.h>
#include <float.h>
//------------------------------------------------------------------------------
void lanczos(Vector *alpha, Vector *beta, CSRMatrix *A, Vector *init) {
  /* Asserts:
       - #Rows of A == size of alpha  == size of beta + 1 = size of init
  */
  assert(A->nrows == alpha->size);
  assert(A->nrows == init->size);
  assert(A->nrows == beta->size + 1);

  int n = A->nrows;
  double norm_q1, b = 0.;
  Vector q0, q1, u;

  // Set q_0 and beta_0 to zero (both uses 0-indexing)
  zeros_vector(&q0, n);
  beta->vv[0] = 0.;

  // Create vector u
  create_vector(&u, n);

  // Set q1 to normalized initial vector
  create_vector(&q1,    n);
  copy_vector  (&q1, init);
  norm_q1 = norm_vector(&q1, 2);
  mult_scalar_add_vector(&q1, 0., &q1, 1./norm_q1);

  for (int k = 0; k < n; k++) {
    csr_matrix_vector_multiply(&u, A, &q1);

    alpha->vv[k] = dot_vector(&q1, &u);

    mult_scalar_add_vector(&u, 1., &q0, -b);
    mult_scalar_add_vector(&u, 1., &q1, -alpha->vv[k]);

    beta->vv[k] = norm_vector(&u, 2);
    b = beta->vv[k];

    copy_vector(&q0, &q1);

    if (beta->vv[k] < DBL_EPSILON) {
      return;
    }

    mult_scalar_add_vector(&q1, 0., &u, 1./beta->vv[k]);
  }
}
//------------------------------------------------------------------------------
void lanczos2(Vector *alpha, Vector *beta, CSRMatrix *A, Vector *init) {
  /* Asserts:
       - #Rows of A == size of alpha  == size of beta + 1 = size of init
  */
  assert(A->nrows == alpha->size);
  assert(A->nrows == beta->size + 1);
  assert(A->nrows == init->size);

  int i = 0, n = A->nrows;
  double b1 = 1.;
  Vector v, r, v1, p;

  create_vector(&v, n); create_vector(&r, n); create_vector(&p, n);
  copy_vector(&v, init);
  mult_scalar_add_vector(&v, 0., &v, 1./norm_vector(&v, 2));
  copy_vector(&r, &v);
  zeros_vector(&v1, n);

  while (i < n - 1) {
    mult_scalar_add_vector(&v, 0., &r, 1./b1);

    csr_matrix_vector_multiply(&p, A, &v);

    alpha->vv[i] = dot_vector(&p, &v);

    copy_vector(&r, &p);
    mult_scalar_add_vector(&r, 1., &v, -alpha->vv[i]);
    mult_scalar_add_vector(&r, 1., &v1, -b1);

    b1 = norm_vector(&r, 2);
    beta->vv[i] = b1;

    copy_vector(&v1, &v);
    i = i + 1;
  }

  mult_scalar_add_vector(&v, 0., &r, 1./b1);
  csr_matrix_vector_multiply(&p, A, &v);
  alpha->vv[i] = dot_vector(&p, &v);
}
//------------------------------------------------------------------------------
