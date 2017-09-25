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

void create_vector(Vector *x, int size);

void delete_vector(Vector *x, int size);

int is_vectors_equal(Vector *x, Vector *y, double tol);

void random_vector(Vector *x, int size);

//------------------------------------------------------------------------------

#endif
