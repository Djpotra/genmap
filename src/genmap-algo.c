#include <genmap-impl.h>

#include <math.h>
#include <assert.h>
//------------------------------------------------------------------------------
// Power and inverse power iterations
//
void GenmapPowerIter(GenmapVector eVector, GenmapVector alpha,
                     GenmapVector beta, GenmapVector init, GenmapInt32 iter) {
  assert(alpha->size == beta->size + 1);
  assert(alpha->size == eVector->size);

  GenmapInt32 n = alpha->size;

  GenmapVector x, y;
  GenmapCreateVector(&x, n);
  GenmapCreateVector(&y, n);
  GenmapCopyVector(x, init);

  if(n == 1) {
    eVector->data[0] = alpha->data[0];
    return;
  } else {
    for(GenmapInt32 j = 0; j < iter; j++) {
      // y = Ax
      y->data[0] = alpha->data[0] * x->data[0] + beta->data[0] * x->data[1];
      for(GenmapInt32 i = 1; i < n - 1; i++) {
        y->data[i] = beta->data[i - 1] * x->data[i - 1] + alpha->data[i] * x->data[i] +
                     beta->data[i] * x->data[i + 1];
      }
      y->data[n - 1] = beta->data[n - 2] * x->data[n - 2] + alpha->data[n - 1] *
                       x->data[n - 1];

      // calculate 2-norm(y)
      GenmapScaleVector(y, y, 1.0 / GenmapNormVector(y, 2));

      GenmapCopyVector(x, y);
    }
  }

  GenmapCopyVector(eVector, y);
}

//void GenmapInvPowerIter(GenmapVector eVector, GenmapVector alpha,
//                        GenmapVector beta, GenmapVector init, GenmapInt32 iter) {
//  assert(alpha->size == beta->size + 1);
//  assert(alpha->size == eVector->size);
//
//  int32 n = alpha->size;
//
//  Vector x, y;
//  create_vector(&x, n);
//  create_vector(&y, n);
//  copy_vector(&x, init);
//
//  if(n == 1) {
//    eVector->vv[0] = alpha->vv[0];
//    return;
//  } else {
//    for(int32 j = 0; j < iter; j++) {
//      // Ay = x
//      symtridiag_solve(&y, &x, alpha, beta);
//
//      // calculate 2-norm(y) and scale y by that amount
//      scale_vector(&y, &y, 1.0 / norm_vector(&y, 2));
//
//      copy_vector(&x, &y);
//    }
//  }
//
//  copy_vector(eVector, &y);
//}

//------------------------------------------------------------------------------
