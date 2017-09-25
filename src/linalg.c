#include <time.h>
#include <stdlib.h>

#include "linalg.h"
#include "random.h"

int genmap_srand_initialized = 0;
//------------------------------------------------------------------------------
void create_vector(Vector *x, int size) {
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
  /* Asserts:
       - size > 0
  */
  assert(size > 0);

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
  /* Asserts:
       - size > 0
  */
  assert(size > 0);

  create_vector(x, size);

  for (int i = 0; i < size; i++) {
    x->vv[i] = 1.;
  }
}
//------------------------------------------------------------------------------
void zeros_vector(Vector *x, int size) {
  /* Asserts:
       - size > 0
  */
  assert(size > 0);

  create_vector(x, size);

  for (int i = 0; i < size; i++) {
    x->vv[i] = 0.;
  }
}
//------------------------------------------------------------------------------
