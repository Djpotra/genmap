#include "csr.h"
#include "linalg.h"
#include "gswrapper.h"
#include "laplacian.h"
#include "io.h"

#include <stdio.h>
#include <float.h>
#include <mpi.h>
#include <math.h>
//------------------------------------------------------------------------------
void lanczos(Vector *alpha, Vector *beta,
  struct gs_data *gsh, double *weights, int nc, Vector *init, int iter) {

  assert(alpha->size == iter);
  assert(alpha->size == beta->size + 1);

  int n = init->size;
  double norm_q1, b = 0.;
  Vector q0, q1, u;

  // Set q_0 and beta_0 to zero (both uses 0-indexing)
  zeros_vector(&q0, n);
  beta->vv[0] = 0.;

  // Create vector u
  create_vector(&u, n);
  // Set q1 to normalized initial vector
  create_vector(&q1,     n);

  double sum = 0.;
  int nglobal= n;
  for (int i = 0; i < n; i++) {
    sum += init->vv[i];
  }
  gop(&sum, gs_double, gs_add, 0);
  gop(&nglobal, gs_int, gs_add, 0);
  for (int i = 0; i < n; i++) {
    init->vv[i] -= sum/nglobal;
  }

  copy_vector(&q1, init);
  norm_q1 = dot_vector(&q1, &q1);
  gop(&norm_q1, gs_double, gs_add, 0);
  norm_q1 = sqrt(norm_q1);

  scale_vector(&q1, &q1, 1./norm_q1);

  for (int k = 0; k < iter; k++) {
    // Multiplication by the laplacian
    ax(&u, &q1, gsh, weights, nc);

    alpha->vv[k] = dot_vector(&q1, &u);
    gop(&alpha->vv[k], gs_double, gs_add, 0);

    z_axpby_vector(&u, &u, 1., &q0, -b);
    z_axpby_vector(&u, &u, 1., &q1, -alpha->vv[k]);

    // This should be a global operation
    b = dot_vector(&u, &u);
    gop(&b, gs_double, gs_add, 0);
    b = sqrt(b);
    if (k < iter - 1) {
      beta->vv[k] = b;
    }

    copy_vector(&q0, &q1);

    if (beta->vv[k] < DBL_EPSILON) {
      beta->size = k;
      alpha->size = k + 1;
      return;
    }

    scale_vector(&q1, &u, 1./beta->vv[k]);
  }
}

//------------------------------------------------------------------------------
void lanczos_serial(Vector *alpha, Vector *beta, CSRMatrix *A, Vector *init) {
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

    b = norm_vector(&u, 2);
    if (k < n - 1) {
      beta->vv[k] = b;
    }

    copy_vector(&q0, &q1);

//    if (beta->vv[k] < DBL_EPSILON) {
//      return;
//    }

    mult_scalar_add_vector(&q1, 0., &u, 1./beta->vv[k]);
  }
}

//------------------------------------------------------------------------------
void lanczos_serial2(Vector *alpha, Vector *beta, CSRMatrix *A, Vector *init) {
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
