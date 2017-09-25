#include "linalg.h"

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
