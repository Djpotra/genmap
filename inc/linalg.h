#ifndef _LINALG_H_
#define _LINALG_H_

#include "dtypes.h"

//------------------------------------------------------------------------------
struct genmap_vector {
  int32 size;
  double *vv;
};
typedef struct genmap_vector Vector;

void   create_vector(Vector *x, int32 size);

void   delete_vector(Vector *x);

int32    vectors_equal(Vector *x, Vector *y, double tol);

void   random_vector(Vector *x, int32 size, int32 seed);

void   ones_vector(Vector *x, int32 size);

void   zeros_vector(Vector *x, int32 size);

double norm_vector(Vector *x, int32 p);

void   z_axpby_vector(Vector *z, Vector *x, double alpha, \
                                             Vector *y, double beta);

void   scale_vector(Vector *y, Vector *x,  double alpha);

double dot_vector(Vector *x, Vector *y);

void   copy_vector(Vector *x, Vector *y);

void   print_vector(Vector *x);

//TODO: get rid of this !!
void   mult_scalar_add_vector(Vector *y, double alpha, Vector *x, \
                                                        double beta);
//------------------------------------------------------------------------------

#endif
