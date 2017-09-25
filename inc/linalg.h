#ifndef _LINALG_H_
#define _LINALG_H_

#include <stdlib.h>
#include <assert.h>
#include <math.h>

//------------------------------------------------------------------------------
struct genmap_vector {
  int size;
  double *vv;
};
typedef struct genmap_vector Vector;

int is_vectors_equal(Vector *x, Vector *y, double tol);

//------------------------------------------------------------------------------

#endif
