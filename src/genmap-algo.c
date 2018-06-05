#include <genmap-impl.h>

#include <math.h>
#include <stdio.h>
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
        y->data[i] = beta->data[i - 1] * x->data[i - 1] + alpha->data[i] *
                     x->data[i] +
                     beta->data[i] * x->data[i + 1];
      }
      y->data[n - 1] = beta->data[n - 2] * x->data[n - 2] + alpha->data[n - 1]
                       *
                       x->data[n - 1];

      // Normalize by inf-norm(y)
      if(j != iter - 1) {
        GenmapScalar norm = GenmapNormVector(y, -1);
        GenmapScaleVector(y, y, 1.0 / norm);
      }

      GenmapCopyVector(x, y);
    }
  }

  GenmapCopyVector(eVector, y);

  GenmapDestroyVector(x);
  GenmapDestroyVector(y);

  return 0;
}

int GenmapPowerIterNew(GenmapVector eVector, void (*Ax)(GenmapVector ax,
                       GenmapVector x, void* data), void *data,
                       GenmapVector init, GenmapInt iter) {
  assert(eVector->size == init->size);
  GenmapInt n = init->size;

  GenmapVector x, y;
  GenmapCreateVector(&x, n);
  GenmapCreateVector(&y, n);
  GenmapCopyVector(x, init);

  for(GenmapInt j = 0; j < iter; j++) {
    // y = Ax
    Ax(y, x, data);
    // Normalize by inf-norm(y)
    if(j != iter - 1)
      GenmapScaleVector(y, y, 1.0 / GenmapNormVector(y, -1));

    GenmapCopyVector(x, y);
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

      // Normalize by inf-norm(y)
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
//
// Linear solve for Symmetric Tridiagonal Matrix
//
int GenmapSymTriDiagSolve(GenmapVector x, GenmapVector b,
                          GenmapVector alpha,
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
    x->data[i] = (x->data[i] - beta->data[i] * x->data[i + 1]) /
                 diag->data[i];
  }

  GenmapDestroyVector(diag);
  return 0;
}
//
//
void GenmapLanczos(GenmapHandle h, GenmapComm c, GenmapVector init,
                   GenmapInt iter, GenmapVector **q, GenmapVector alpha,
                   GenmapVector beta) {
  assert(alpha->size == iter);
  assert(alpha->size == beta->size + 1);
  assert(init->size == h->header->lelt);

  GenmapVector q0, q1, u;
  GenmapScalar normq1, b = 0.;

  GenmapInt lelt = h->header->lelt;

  // Create vector u and q1
  GenmapCreateVector(&q1, lelt);
  GenmapCopyVector(q1, init);
  GenmapCreateVector(&u, lelt);

  // Set q_0 and beta_0 to zero (both uses 0-indexing)
  GenmapCreateZerosVector(&q0, lelt);
  beta->data[0] = 0.;

  // Allocate memory for q-vectors
  if(*q == NULL)
    GenmapMalloc(iter, q);

  normq1 = GenmapDotVector(q1, q1);
  h->Gop(c, &normq1, 1, GENMAP_SUM);
  normq1 = sqrt(normq1);
  GenmapScaleVector(q1, q1, 1. / normq1);

  // Store Local Laplacian weights
  GenmapVector weights;
  GenmapCreateVector(&weights, lelt);
  h->AxInit(h, c, weights);

  for(GenmapInt k = 0; k < iter; k++) {
    // Store q1
    GenmapCreateVector(&(*q)[k], lelt);
    GenmapCopyVector((*q)[k], q1);

    // Multiplication by the laplacian
    h->Ax(h, c, q1, weights, u);

    alpha->data[k] = GenmapDotVector(q1, u);
    h->Gop(c, &alpha->data[k], 1, GENMAP_SUM);

    GenmapAxpbyVector(u, u, 1., q0, -b);
    GenmapAxpbyVector(u, u, 1., q1, -alpha->data[k]);

    b = GenmapDotVector(u, u);
    h->Gop(c, &b, 1, GENMAP_SUM);
    b = sqrt(b);

    if(k < iter - 1) {
      beta->data[k] = b;
    }

    GenmapCopyVector(q0, q1);

    if(fabs(beta->data[k]) < GENMAP_TOL) {
      beta->size = k;
      alpha->size = k + 1;
      GenmapDestroyVector(q0);
      GenmapDestroyVector(q1);
      GenmapDestroyVector(u);
      GenmapDestroyVector(weights);
      return;
    }

    if(k < iter - 1) {
      GenmapScaleVector(q1, u, 1. / beta->data[k]);
    }
  }

  GenmapDestroyVector(q0);
  GenmapDestroyVector(q1);
  GenmapDestroyVector(u);
  GenmapDestroyVector(weights);
}

void GenmapPrimeFactors(GenmapInt n, GenmapInt *primes) {
}

void GenmapRQI(GenmapHandle h, GenmapVector v) {
  // Calculate Lv, v^T(Lv)
}

GenmapInt GetProcessorId(GenmapScalar fiedler, GenmapInt np) {
  GenmapScalar min = -1.0, max = 1.0;
  GenmapScalar range = max - min;
  GenmapInt nbins = np;
  for(GenmapInt id = 0; id < np; id++) {
    GenmapScalar start = min + (range * id) / nbins;
    GenmapScalar end = min + (range * (id + 1)) / nbins;
    if(start <= fiedler && fiedler < end)
      return id;
  }
  return 0;
}

void GenmapFiedler(GenmapHandle h, GenmapComm c, int global) {
  // 1. Do lanczos in local communicator.
  GenmapInt iter = 10;
  GenmapInt lelt = h->header->lelt;
  GenmapVector initVec, alphaVec, betaVec;

  if (lelt == 0) return;

  GenmapCreateVector(&initVec, h->header->lelt);
  GenmapScalar sum = 0.0;
  GenmapElements elements = GenmapGetElements(h);
  for(GenmapInt i = 0;  i < lelt; i++) {
    if(global)
      initVec->data[i] = elements[i].globalId;
    else
      initVec->data[i] = elements[i].fiedler;
    sum += initVec->data[i];
  }

  h->Gop(c, &sum, 1, GENMAP_SUM);

  for(GenmapInt i = 0;  i < lelt; i++) {
    initVec->data[i] -= sum / h->header->nel;
  }

  GenmapCreateVector(&alphaVec, iter);
  GenmapCreateVector(&betaVec, iter - 1);
  GenmapVector *q = NULL;
  GenmapLanczos(h, c, initVec, iter, &q, alphaVec, betaVec);
  iter = alphaVec->size;

  // 2. Do inverse power iteration on local communicator and find
  // local Fiedler vector.
  GenmapVector evLanczos, evTriDiag, evInit;
  GenmapCreateVector(&evTriDiag, iter);
  GenmapCreateVector(&evInit, iter);
  for(GenmapInt i = 0; i < iter; i++) {
    evInit->data[i] = i + 1;
  }

  GenmapInvPowerIter(evTriDiag, alphaVec, betaVec, evInit, iter * 30);

  // Multiply tri-diagonal matrix by [q1, q2, ...q_{iter}]
  GenmapCreateZerosVector(&evLanczos, lelt);
  for(GenmapInt i = 0; i < lelt; i++) {
    for(GenmapInt j = 0; j < iter; j++) {
      evLanczos->data[i] += q[j]->data[i] * evTriDiag->data[j];
    }
  }

  GenmapScalar lNorm = 0;
  for(GenmapInt i = 0; i < lelt; i++) {
    lNorm += evLanczos->data[i] * evLanczos->data[i];
  }

  h->Gop(c, &lNorm, 1, GENMAP_SUM);
  GenmapScaleVector(evLanczos, evLanczos, 1. / sqrt(lNorm));
  for(GenmapInt i = 0; i < lelt; i++) {
    elements[i].fiedler = evLanczos->data[i];
  }

  // n. Destory the data structures
  GenmapDestroyVector(initVec);
  GenmapDestroyVector(alphaVec);
  GenmapDestroyVector(betaVec);
  GenmapDestroyVector(evLanczos);
  GenmapDestroyVector(evTriDiag);
  GenmapDestroyVector(evInit);
  for(GenmapInt i = 0; i < iter; i++) {
    GenmapDestroyVector(q[i]);
  }
  GenmapFree(q);
}

void GenmapRSB(GenmapHandle h) {
  int done = 0;
  // Calculate the global Fiedler vector, local communicator
  // must be initialized using the global communicator
  GenmapFiedler(h, h->local, 1);
  do {
    GenmapInt nbins = h->Np(h->local);
    GenmapInt id = h->Id(h->local);
    printf("Nbins=%d, Id=%d\n",nbins,id);

    GenmapElements elements = GenmapGetElements(h);
    GenmapInt lelt = h->header->lelt;
    for(GenmapElements p = elements, e = p + lelt; p != e; p++) {
      p->proc = GetProcessorId(p->fiedler, nbins);
    }

    struct crystal cr;
    crystal_init(&cr, &(h->local->gsComm));
    sarray_transfer(struct GenmapElement_private, &(h->elementArray), proc,
                    1, &cr);
    elements = GenmapGetElements(h);
    h->header->lelt = h->elementArray.n;

    for(GenmapInt i = 0; i < h->header->lelt; i++) {
      printf("proc = %d id = %d fiedler = %lf\n", h->Id(h->local),
             elements[i].globalId, elements[i].fiedler);
    }

    GenmapCommExternal local;
#ifdef MPI
    int partition = 0;
    if (id >= nbins/2) partition = 1;
    MPI_Comm_split(h->local->gsComm.c, partition, id, &local);
    if (nbins == 1) done = 1;
#else
    local=0
    done=1;
#endif
    GenmapCreateComm(h, &h->local, local);
    GenmapFiedler(h, h->local, 0);
  } while(!done);
}