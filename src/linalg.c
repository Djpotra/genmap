#include <time.h>
#include <stdlib.h>

#include "linalg.h"

//------------------------------------------------------------------------------
void create_vector(Vector *x, int size) {
  x = (Vector *) malloc(sizeof(Vector));
  x->size = size;
  x->vv = (double *) malloc(sizeof(double)*size);
}
//------------------------------------------------------------------------------
void delete_vector(Vector *x, int size) {
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
}
//------------------------------------------------------------------------------
