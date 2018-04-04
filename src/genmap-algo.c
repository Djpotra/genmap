#include <genmap-impl.h>

#include <math.h>
//
// Algorithms
//
// Power and inverse power iterations
int GenmapPowerIter(GenmapVector eVector, GenmapVector alpha,
                    GenmapVector beta, GenmapVector init, GenmapInt iter) {
  assert(alpha->size == beta->size + 1);
  assert(alpha->size == eVector->size);

  GenmapInt n = alpha->size;

  GenmapVector x, y;
  GenmapCreateVector(&x, n);
  GenmapCreateVector(&y, n);
  GenmapCopyVector(x, init);

  if(n == 1) {
    eVector->data[0] = alpha->data[0];
    return 0;
  } else {
    for(GenmapInt j = 0; j < iter; j++) {
      // y = Ax
      y->data[0] = alpha->data[0] * x->data[0] + beta->data[0] * x->data[1];
      for(GenmapInt i = 1; i < n - 1; i++) {
        y->data[i] = beta->data[i - 1] * x->data[i - 1] + alpha->data[i] * x->data[i] +
                     beta->data[i] * x->data[i + 1];
      }
      y->data[n - 1] = beta->data[n - 2] * x->data[n - 2] + alpha->data[n - 1] *
                       x->data[n - 1];

      // calculate 2-norm(y)
      if(j != iter - 1)
        GenmapScaleVector(y, y, 1.0 / GenmapNormVector(y, -1));

      GenmapCopyVector(x, y);
    }
  }

  GenmapCopyVector(eVector, y);

  GenmapDestroyVector(x);
  GenmapDestroyVector(y);

  return 0;
}

int GenmapInvPowerIter(GenmapVector eVector, GenmapVector alpha,
                       GenmapVector beta, GenmapVector init, GenmapInt iter) {
  assert(alpha->size == beta->size + 1);
  assert(alpha->size == eVector->size);

  GenmapInt n = alpha->size;

  GenmapVector x, y;
  GenmapCreateVector(&x, n);
  GenmapCreateVector(&y, n);

  GenmapCopyVector(x, init);

  if(n == 1) {
    eVector->data[0] = alpha->data[0];
    return 0;
  } else {
    for(GenmapInt j = 0; j < iter; j++) {
      // Ay = x
      GenmapSymTriDiagSolve(y, x, alpha, beta);

      // calculate 2-norm(y) and scale y by that amount
      if(j != iter - 1)
        GenmapScaleVector(y, y, 1.0 / GenmapNormVector(y, 2));

      GenmapCopyVector(x, y);
    }
  }

  GenmapCopyVector(eVector, y);

  GenmapDestroyVector(x);
  GenmapDestroyVector(y);

  return 0;
}
//
// Linear solve for Symmetric Tridiagonal Matrix
//
int GenmapSymTriDiagSolve(GenmapVector x, GenmapVector b, GenmapVector alpha,
                          GenmapVector beta) {
  assert((x->size == b->size) && (x->size == alpha->size));
  assert(alpha->size == beta->size + 1);
  assert(b->size > 0);

  GenmapInt n = b->size;

  GenmapVector diag;
  GenmapCreateVector(&diag, n);
  GenmapCopyVector(diag, alpha);

  GenmapCopyVector(x, b);

  for(GenmapInt i = 0; i < n - 1; i++) {
    GenmapScalar m = (beta->data[i] / diag->data[i]);
    x->data[i + 1] = x->data[i + 1] - m * x->data[i];
    diag->data[i + 1] = diag->data[i + 1] - m * beta->data[i];
  }

  x->data[n - 1] = x->data[n - 1] / diag->data[n - 1];

  for(GenmapInt i = n - 2; i >= 0; i--) {
    x->data[i] = (x->data[i] - beta->data[i] * x->data[i + 1]) / diag->data[i];
  }

  GenmapDestroyVector(diag);
  return 0;
}

void GenmapLanczos(GenmapHandle h, GenmapComm c, GenmapVector alpha,
                   GenmapVector beta, GenmapVector init, GenmapInt iter) {
  assert(alpha->size == iter);
  assert(alpha->size == beta->size + 1);
  assert(init->size == h->header->lelt);

  GenmapVector q, q0, q1, u;
  GenmapScalar normq1, b = 0.;

  GenmapInt lelt = h->header->lelt;

  // Create vector u and q1
  GenmapCreateVector(&q, lelt);
  GenmapCreateVector(&q1, lelt);
  GenmapCopyVector(q1, init);
  GenmapCreateVector(&u, lelt);

  // Set q_0 and beta_0 to zero (both uses 0-indexing)
  GenmapCreateZerosVector(&q0, lelt);
  beta->data[0] = 0.;

  normq1 = GenmapDotVector(q1, q1);
  h->Gop(c, &normq1);
  normq1 = sqrt(normq1);
  GenmapScaleVector(q, q1, 1. / normq1);


  // Store Local Laplacian weights
  GenmapVector weights;
  GenmapCreateVector(&weights, lelt);
  h->AxInit(h, c, weights);

  for(GenmapInt k = 0; k < iter; k++) {
    // Multiplication by the laplacian
    h->Ax(h, c, q1, weights, u);

    alpha->data[k] = GenmapDotVector(q1, u);
    h->Gop(h->local, &alpha->data[k]);

    GenmapAxpbyVector(u, u, 1., q0, -b);
    GenmapAxpbyVector(u, u, 1., q1, -alpha->data[k]);

    b = GenmapDotVector(u, u);
    h->Gop(c, &b);
    b = sqrt(b);

    if(k < iter - 1) {
      beta->data[k] = b;
    }

    GenmapCopyVector(q0, q1);

//    if (abs(beta->vv[k]) < DBL_EPSILON) {
//      beta->size = k;
//      alpha->size = k + 1;
//      return;
//    }

    GenmapScaleVector(q1, u, 1. / beta->data[k]);
  }
}
