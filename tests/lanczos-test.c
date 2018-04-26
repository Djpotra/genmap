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

  GenmapPowerIter(evTriDiag, alphaVec, betaVec, evInit, iter * 20);

  // Multiply tri-diagonal matrix by [q1, q2, ...q_{iter}]
  GenmapCreateZerosVector(&evLanczos, lelt);
  for(GenmapInt i = 0; i < lelt; i++) {
    for(GenmapInt j = 0; j < iter; j++) {
      evLanczos->data[i] += q[j]->data[i] * evTriDiag->data[j];
    }
  }

  GenmapScalar lNorm = GenmapNormVector(evLanczos, 2);
  lNorm = lNorm*lNorm;
  h->Gop(h->global, &lNorm);
  GenmapScaleVector(evLanczos, evLanczos, 1. / sqrt(lNorm));
#ifdef DEBUG
  printf("proc : %d (lanczos power) ", h->Id(h->global));
  GenmapPrintVector(evLanczos);
  printf("\n");
#endif

  for(GenmapInt i = 0; i < iter; i++) {
    GenmapDestroyVector(q[i]);
  }
  free(q);

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

  GenmapInvPowerIter(evTriDiag, alphaVec, betaVec, evInit, iter * 10);

  // Multiply tri-diagonal matrix by [q1, q2, ...q_{iter}]
  GenmapCreateZerosVector(&evLanczos, lelt);
  for(GenmapInt i = 0; i < lelt; i++) {
    for(GenmapInt j = 0; j < iter; j++) {
      evLanczos->data[i] += q[j]->data[i] * evTriDiag->data[j];
    }
  }

  GenmapScalar lNorm = GenmapNormVector(evLanczos, 2);
  lNorm = lNorm*lNorm;
  h->Gop(h->global, &lNorm);
  GenmapScaleVector(evLanczos, evLanczos, 1. / sqrt(lNorm));
#ifdef DEBUG
  printf("proc : %d (lanczos invpower) ", h->Id(h->global));
  GenmapPrintVector(evLanczos);
  printf("\n");
#endif

  for(GenmapInt i = 0; i < iter; i++) {
    GenmapDestroyVector(q[i]);
  }
  free(q);

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
    initVec->data[i] = h->elements->globalId[i];
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

  GenmapInvPowerIter(evTriDiag, alphaVec, betaVec, evInit, iter * 10);

  // Multiply tri-diagonal matrix by [q1, q2, ...q_{iter}]
  GenmapCreateZerosVector(&evLanczos, lelt);
  for(GenmapInt i = 0; i < lelt; i++) {
    for(GenmapInt j = 0; j < iter; j++) {
      evLanczos->data[i] += q[j]->data[i] * evTriDiag->data[j];
    }
  }

  GenmapScalar lNorm = GenmapNormVector(evLanczos, 2);
  lNorm = lNorm*lNorm;
  h->Gop(h->global, &lNorm);
#ifdef DEBUG
  printf("proc : %d (lanczos fiedler) ", h->Id(h->global));
  GenmapPrintVector(evLanczos);
  printf("\n");
#endif

  for(GenmapInt i = 0; i < iter; i++) {
    GenmapDestroyVector(q[i]);
  }
  free(q);

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
