#include <time.h>
#include <stdlib.h>

#include "linalg.h"
#include "random.h"

int genmap_srand_initialized = 0;
//------------------------------------------------------------------------------
void create_vector(Vector *x, int size) {
  /* Asserts:
       - size > 0
  */
  assert(size > 0);

  x = (Vector *) malloc(sizeof(Vector));
  x->size = size;
  x->vv = (double *) malloc(sizeof(double)*size);
}
//------------------------------------------------------------------------------
void delete_vector(Vector *x) {
  if (x->vv) {
    free(x->vv);
    x->vv = NULL;
  }

  if (x) {
    free(x);
    x = NULL;
  }
}
//------------------------------------------------------------------------------
int is_vectors_equal(Vector *x, Vector *y, double tol) {
  /* Asserts:
       - size of y == size of x
  */
  assert(x->size == y->size);

  int  equal = 1;

  int      n = x->size;
  for (int i = 0; i < n; i++) {
    if (fabs(x->vv[i] - y->vv[i]) > tol) {
      equal = 0;
      break;
    }
  }

  return equal;
}
//------------------------------------------------------------------------------
void random_vector(Vector *x, int size) {
  create_vector(x, size);

  if (!genmap_srand_initialized) {
    srand(time(NULL));
    genmap_srand_initialized = 1;
  }

  for (int i = 0; i < size; i++) {
    x->vv[i] = (double) rand()/RAND_MAX*2. - 1.;
  }
}
//------------------------------------------------------------------------------
void ones_vector(Vector *x, int size) {
  create_vector(x, size);

  for (int i = 0; i < size; i++) {
    x->vv[i] = 1.;
  }
}
//------------------------------------------------------------------------------
void zeros_vector(Vector *x, int size) {
  create_vector(x, size);

  for (int i = 0; i < size; i++) {
    x->vv[i] = 0.;
  }
}
//------------------------------------------------------------------------------
double norm_vector(Vector *x, int p) {
  double sum = 0.;

  int n = x->size;
  if (p == 1) {
    for (int i = 0; i < n; i++) {
      sum += fabs(x->vv[i]);
    }
  } else if (p == 2) {
    for (int i = 0; i < n; i++) {
      sum += x->vv[i]*x->vv[i];
    }
  }

  return sum;
}
//------------------------------------------------------------------------------
void mult_scalar_add_vector(Vector *y, double alpha, Vector *x, \
                                                        double beta) {
  /* Asserts:
       - size y = size x
  */
  assert(y->size == x->size);

  int n = x->size;
  for (int i = 0; i < n; i++) {
    y->vv[i] = alpha*y->vv[i] + beta*x->vv[i];
  }
}
//------------------------------------------------------------------------------
double dot_vector(Vector *x, Vector *y) {
  /* Asserts:
       - size y = size x
  */
  assert(y->size == x->size);

  double dot = 0.;

  int n = x->size;
  for (int i = 0; i < n; i++) {
    dot += y->vv[i]*x->vv[i];
  }

  return dot;
}
//------------------------------------------------------------------------------
void copy_vector(Vector *x, Vector *y) {
  /* Asserts:
       - size y = size x
  */
  assert(y->size == x->size);

  int n = x->size;
  for (int i = 0; i < n; i++) {
    x->vv[i] =y->vv[i];
  }
}
//------------------------------------------------------------------------------
