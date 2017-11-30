#include "linalg.h"
#include "random.h"

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>

int32 genmap_srand_initialized = 0;
//------------------------------------------------------------------------------
void create_vector(Vector *x, int32 size) {
  /* Asserts:
       - size > 0
  */
  assert(size > 0);

  x->size = size;
  x->vv = NULL;
  x->vv = (double *) malloc(sizeof(double)*size);
  if (x->vv == NULL) {
    printf("malloc failed in %s:%d", __FILE__, __LINE__);
  }
}
//------------------------------------------------------------------------------
void delete_vector(Vector *x) {
  if (x->vv) {
    free(x->vv);
    x->vv = NULL;
  }

  if (x) {
    x = NULL;
  }
}
//------------------------------------------------------------------------------
int32 vectors_equal(Vector *x, Vector *y, double tol) {
  /* Asserts:
       - size of y == size of x
  */
  assert(x->size == y->size);

  int32  equal = 1;

  int32      n = x->size;
  for (int32 i = 0; i < n; i++) {
    if (fabs(x->vv[i] - y->vv[i]) > tol) {
      equal = 0;
      break;
    }
  }

  return equal;
}
//------------------------------------------------------------------------------
void random_vector(Vector *x, int32 size) {
  create_vector(x, size);

  if (!genmap_srand_initialized) {
    srand(time(NULL));
    genmap_srand_initialized = 1;
  }

  for (int32 i = 0; i < size; i++) {
    x->vv[i] = (double) rand()/RAND_MAX*2. - 1.;
  }
}
//------------------------------------------------------------------------------
void parallel_random_vector(Vector *x, int32 size, int32 seed) {
  create_vector(x, size);

  if (!genmap_srand_initialized) {
    srand(time(NULL) + seed);
    genmap_srand_initialized = 1;
  }

  for (int32 i = 0; i < size; i++) {
    x->vv[i] = (double) rand()/RAND_MAX*2. - 1.;
  }
}
//------------------------------------------------------------------------------
void ones_vector(Vector *x, int32 size) {
  create_vector(x, size);

  for (int32 i = 0; i < size; i++) {
    x->vv[i] = 1.;
  }
}
//------------------------------------------------------------------------------
void zeros_vector(Vector *x, int32 size) {
  create_vector(x, size);

  for (int32 i = 0; i < size; i++) {
    x->vv[i] = 0.;
  }
}
//------------------------------------------------------------------------------
double norm_vector(Vector *x, int32 p) {
  assert(x->size > 0);

  int32 n = x->size;
  double norm = 0.;

  if (p == 1) {
    for (int32 i = 0; i < n; i++) {
      norm += fabs(x->vv[i]);
    }
  } else if (p == 2) {
    for (int32 i = 0; i < n; i++) {
      norm += x->vv[i]*x->vv[i];
    }
    norm = sqrt(norm);
  } else if (p == -1) {
    norm = fabs(x->vv[0]);

    for (int32 i = 1; i < n; i++) {
      if (fabs(x->vv[i]) > norm) norm = fabs(x->vv[i]);
    }
  }

  return norm;
}
//------------------------------------------------------------------------------
void mult_scalar_add_vector(Vector *y, double alpha, Vector *x, \
                                                        double beta) {
  /* Asserts:
       - size y = size x
  */
  assert(y->size == x->size);

  int32 n = x->size;
  for (int32 i = 0; i < n; i++) {
    y->vv[i] = alpha*y->vv[i] + beta*x->vv[i];
  }
}
//------------------------------------------------------------------------------
void z_axpby_vector(Vector *z, Vector *x, double alpha, \
                                             Vector *y, double beta) {
  /* asserts:
       - size z = size x = size y
  */
  assert(z->size == x->size);
  assert(z->size == y->size);

  int32 n = x->size;
  for (int32 i = 0; i < n; i++) {
    z->vv[i] = alpha*x->vv[i] + beta*y->vv[i];
  }
}
//------------------------------------------------------------------------------
void scale_vector(Vector *y, Vector *x,  double alpha) {
  /* asserts:
       - size x = size y
  */
  assert(x->size == y->size);

  int32 n = x->size;
  for (int32 i = 0; i < n; i++) {
    y->vv[i] = alpha*(x->vv[i]);
  }
}
//------------------------------------------------------------------------------
double dot_vector(Vector *x, Vector *y) {
  /* Asserts:
       - size y = size x
  */
  assert(y->size == x->size);

  double dot = 0.;

  int32 n = x->size;
  for (int32 i = 0; i < n; i++) {
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

  int32 n = x->size;
  for (int32 i = 0; i < n; i++) {
    x->vv[i] = y->vv[i];
  }
}
//------------------------------------------------------------------------------
void print_vector(Vector *x) {
  /* Asserts:
       - size y > 0
  */
  printf("(%f", x->vv[0]);
  for (int32 i = 1; i < x->size - 1; i++) {
    printf(", %f", x->vv[i]);
  }

  if (x->size > 1) {
    printf(", %f)", x->vv[x->size - 1]);
  } else {
    printf(")");
  }
}
//------------------------------------------------------------------------------
