#include <math.h>
#include <assert.h>

#include "eig.h"

//------------------------------------------------------------------------------
void power(Vector *eVector, Vector *alpha, Vector *beta, Vector *init,
           int32 iter) {
  assert(alpha->size == beta->size + 1);
  assert(alpha->size == eVector->size);

  int32 n = alpha->size;

  Vector x, y;
  create_vector(&x, n);
  create_vector(&y, n);
  copy_vector(&x, init);

  if(n == 1) {
    eVector->vv[0] = alpha->vv[0];
    return;
  } else {
    for(int32 j = 0; j < iter; j++) {
      // y = Ax
      y.vv[0] = alpha->vv[0] * x.vv[0] + beta->vv[0] * x.vv[1];
      for(int32 i = 1; i < n - 1; i++) {
        y.vv[i] = beta->vv[i - 1] * x.vv[i - 1] + alpha->vv[i] * x.vv[i] +
                  beta->vv[i] * x.vv[i + 1];
      }
      y.vv[n - 1] = beta->vv[n - 2] * x.vv[n - 2] + alpha->vv[n - 1] * x.vv[n - 1];

      // calculate 2-norm(y)
      scale_vector(&y, &y, 1.0 / norm_vector(&y, 2));

      copy_vector(&x, &y);
    }
  }

  copy_vector(eVector, &y);
}

//------------------------------------------------------------------------------
void invpower(Vector *eVector, Vector *alpha, Vector *beta, Vector *init,
              int32 iter) {
  assert(alpha->size == beta->size + 1);
  assert(alpha->size == eVector->size);

  int32 n = alpha->size;

  Vector x, y;
  create_vector(&x, n);
  create_vector(&y, n);
  copy_vector(&x, init);

  if(n == 1) {
    eVector->vv[0] = alpha->vv[0];
    return;
  } else {
    for(int32 j = 0; j < iter; j++) {
      // Ay = x
      symtridiag_solve(&y, &x, alpha, beta);

      // calculate 2-norm(y) and scale y by that amount
      scale_vector(&y, &y, 1.0 / norm_vector(&y, 2));

      copy_vector(&x, &y);
    }
  }

  copy_vector(eVector, &y);
}

//------------------------------------------------------------------------------
