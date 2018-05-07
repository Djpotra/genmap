#include <genmap-impl.h>

#ifdef MPI
#include <mpi.h>
#endif
#include <stdio.h>

void TestLanczos1(GenmapHandle h) {
  GenmapRead(h, "mesh/box2D_2.bin");

  GenmapInt iter = 10;
  GenmapInt lelt = h->header->lelt;
  GenmapVector initVec, alphaVec, betaVec;
  GenmapInt offset = h->Id(h->global) * h->header->lelt;

  GenmapCreateVector(&initVec, h->header->lelt);
  for(GenmapInt i = 0;  i < lelt; i++) {
    initVec->data[i] = offset + i + 1;
  }

  GenmapCreateVector(&alphaVec, iter);
  GenmapCreateVector(&betaVec, iter - 1);
  GenmapVector *q = NULL;
  GenmapLanczos(h, h->global, initVec, iter, &q, alphaVec, betaVec);
  iter = alphaVec->size;

  // Make sure that q-vectors are orthogonal
  for(GenmapInt i = 0; i < iter; i++) {
    for(GenmapInt j = i + 1; j < iter; j++) {
      GenmapScalar dot = GenmapDotVector(q[i], q[j]);
      h->Gop(h->global, &dot);
      assert(fabs(dot - 0) < GENMAP_TOL);
    }
  }

  GenmapVector evLanczos, evTriDiag, evInit;
  GenmapCreateVector(&evTriDiag, iter);
  GenmapCreateVector(&evInit, iter);
  for(GenmapInt i = 0; i < iter; i++) {
    evInit->data[i] = i + 1;
  }

  GenmapPowerIter(evTriDiag, alphaVec, betaVec, evInit, iter * 30);

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
  h->Gop(h->global, &lNorm);
  GenmapScaleVector(evLanczos, evLanczos, 1. / sqrt(lNorm));
#ifdef DEBUG
  printf("proc : %d (lanczos power) ", h->Id(h->global));
  GenmapPrintVector(evLanczos);
  printf("\n");
#endif

  GenmapInt n = 8;
  GenmapVector answer;
  GenmapScalar d[8] = {-0.1913417161825456, -0.1913417161825448, 0.4619397662556435,
                       0.4619397662556433, -0.4619397662556433, -0.4619397662556433,
                       0.1913417161825447, 0.1913417161825447
                      };
  GenmapCreateVector(&answer, n);
  GenmapSetVector(answer, d);

  GenmapInt start = h->Id(h->global) * lelt;
  for(GenmapInt i = start; i < start + lelt; i++) {
    assert(fabs(answer->data[i] - evLanczos->data[i - start]) <
           GENMAP_SP_TOL);
  }

  for(GenmapInt i = 0; i < iter; i++) {
    GenmapDestroyVector(q[i]);
  }
  GenmapFree(q);

  GenmapDestroyVector(initVec);
  GenmapDestroyVector(alphaVec);
  GenmapDestroyVector(betaVec);
  GenmapDestroyVector(evLanczos);
  GenmapDestroyVector(evTriDiag);
  GenmapDestroyVector(evInit);
}

void TestLanczos2(GenmapHandle h) {
  GenmapRead(h, "mesh/box2D_2.bin");

  GenmapInt iter = 10;
  GenmapInt lelt = h->header->lelt;
  GenmapVector initVec, alphaVec, betaVec;
  GenmapInt offset = h->Id(h->global) * h->header->lelt;

  GenmapCreateVector(&initVec, h->header->lelt);
  for(GenmapInt i = 0;  i < lelt; i++) {
    initVec->data[i] = offset + i + 1;
  }

  GenmapCreateVector(&alphaVec, iter);
  GenmapCreateVector(&betaVec, iter - 1);
  GenmapVector *q = NULL;
  GenmapLanczos(h, h->global, initVec, iter, &q, alphaVec, betaVec);
  iter = alphaVec->size;

  // Make sure that q-vectors are orthogonal
  for(GenmapInt i = 0; i < iter; i++) {
    for(GenmapInt j = i + 1; j < iter; j++) {
      GenmapScalar dot = GenmapDotVector(q[i], q[j]);
      h->Gop(h->global, &dot);
      assert(fabs(dot - 0) < GENMAP_TOL);
    }
  }

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
  h->Gop(h->global, &lNorm);
  GenmapScaleVector(evLanczos, evLanczos, 1. / sqrt(lNorm));
#ifdef DEBUG
  printf("proc : %d (lanczos invpower) ", h->Id(h->global));
  GenmapPrintVector(evLanczos);
  printf("\n");
#endif

  GenmapInt n = 8;
  GenmapVector answer;
  // Numpy gives the negative of this vector
  GenmapScalar d[8] = {0.3535533905932737, 0.3535533905932736, 0.3535533905932737,
                       0.3535533905932737, 0.3535533905932737, 0.3535533905932737,
                       0.353553390593274,  0.353553390593274
                      };
  GenmapCreateVector(&answer, n);
  GenmapSetVector(answer, d);

  GenmapInt start = h->Id(h->global) * lelt;
  for(GenmapInt i = start; i < start + lelt; i++) {
    assert(fabs(answer->data[i] - evLanczos->data[i - start]) <
           GENMAP_SP_TOL);
  }

  for(GenmapInt i = 0; i < iter; i++) {
    GenmapDestroyVector(q[i]);
  }
  GenmapFree(q);

  GenmapDestroyVector(initVec);
  GenmapDestroyVector(alphaVec);
  GenmapDestroyVector(betaVec);
  GenmapDestroyVector(evLanczos);
  GenmapDestroyVector(evTriDiag);
  GenmapDestroyVector(evInit);
}

void TestLanczos3(GenmapHandle h) {
  GenmapRead(h, "mesh/box2D_2.bin");

  GenmapInt iter = 10;
  GenmapInt lelt = h->header->lelt;
  GenmapVector initVec, alphaVec, betaVec;

  GenmapCreateVector(&initVec, h->header->lelt);
  GenmapScalar sum = 0.0;
  for(GenmapInt i = 0; i < lelt; i++) {
    initVec->data[i] = h->elements[i].globalId;
    sum += initVec->data[i];
  }

  h->Gop(h->global, &sum);

  for(GenmapInt i = 0;  i < lelt; i++) {
    initVec->data[i] -= sum / h->header->nel;
  }

  GenmapCreateVector(&alphaVec, iter);
  GenmapCreateVector(&betaVec, iter - 1);
  GenmapVector *q = NULL;
  GenmapLanczos(h, h->global, initVec, iter, &q, alphaVec, betaVec);
  iter = alphaVec->size;

  // Make sure that q-vectors are orthogonal
  for(GenmapInt i = 0; i < iter; i++) {
    for(GenmapInt j = i + 1; j < iter; j++) {
      GenmapScalar dot = GenmapDotVector(q[i], q[j]);
      h->Gop(h->global, &dot);
      assert(fabs(dot - 0) < GENMAP_TOL);
    }
  }

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
  h->Gop(h->global, &lNorm);
  GenmapScaleVector(evLanczos, evLanczos, 1. / sqrt(lNorm));
#ifdef DEBUG
  printf("proc : %d (lanczos fiedler) ", h->Id(h->global));
  GenmapPrintVector(evLanczos);
  printf("\n");
#endif

  GenmapInt n = 8;
  GenmapVector answer;
  GenmapScalar d[8] = {-0.4619397662556433, -0.4619397662556434, -0.1913417161825449,
                       -0.1913417161825449,  0.1913417161825448,  0.1913417161825448,
                       0.4619397662556433,  0.4619397662556433
                      };
  GenmapCreateVector(&answer, n);
  GenmapSetVector(answer, d);

  GenmapInt start = h->Id(h->global) * lelt;
  for(GenmapInt i = start; i < start + lelt; i++) {
    assert(fabs(answer->data[i] - evLanczos->data[i - start]) <
           GENMAP_SP_TOL);
  }

  for(GenmapInt i = 0; i < iter; i++) {
    GenmapDestroyVector(q[i]);
  }
  GenmapFree(q);

  GenmapDestroyVector(initVec);
  GenmapDestroyVector(alphaVec);
  GenmapDestroyVector(betaVec);
  GenmapDestroyVector(evLanczos);
  GenmapDestroyVector(evTriDiag);
  GenmapDestroyVector(evInit);
}

int main(int argc, char **argv) {
#ifdef MPI
  MPI_Init(&argc, &argv);
#else
  int MPI_COMM_WORLD = 0;
#endif

  GenmapHandle h;
  GenmapInit(&h, MPI_COMM_WORLD, "default");

  TestLanczos1(h);
  TestLanczos2(h);
  TestLanczos3(h);

  GenmapFinalize(h);

#ifdef MPI
  MPI_Finalize();
#endif
  return 0;
}
