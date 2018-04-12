#include <genmap-impl.h>

#ifdef MPI
#include <mpi.h>
#endif
#include <stdio.h>

struct AxStruct {
  GenmapHandle h;
  GenmapVector weights;
};

void Ax(GenmapVector ax, GenmapVector x, void *data) {
  struct AxStruct *axData_ = (struct AxStruct *)data;
  struct AxStruct axData = *axData_;
  GenmapAx(axData.h, axData.h->global, x, axData.weights, ax);
}

void TestLanczos1(GenmapHandle h) {
  GenmapRead(h, "mesh/box2D_2.bin");

  GenmapInt iter = 500;
  GenmapInt lelt = h->header->lelt;
  GenmapVector initVec, alphaVec, betaVec;
  GenmapCreateVector(&initVec, h->header->lelt);
  GenmapInt i = 100*h->Id(h->global);
  for(GenmapInt j = i;  j < i + lelt; j++) {
    initVec->data[j-i] = j;
  }
  GenmapCreateVector(&alphaVec, iter);
  GenmapCreateVector(&betaVec, iter - 1);

  GenmapVector *q = NULL;
  GenmapLanczos(h, h->global, initVec, iter, &q, alphaVec, betaVec);
  // Make sure that q-vectors are orthogonal
  for(GenmapInt i = 0; i < iter; i++) {
    for(GenmapInt j = i + 1; j < iter; j++) {
      GenmapScalar dot = GenmapDotVector(q[i], q[j]);
      h->Gop(h->global, &dot);
      assert(abs(dot - 0) < GENMAP_TOL);
    }
  }

  GenmapVector evLanczos, evTriDiag, evInit;
  GenmapCreateVector(&evTriDiag, iter);
  GenmapCreateVector(&evInit, iter);
  for(GenmapInt i = 0; i < iter; i++) {
    evInit->data[i] = i + 1;
  }

  GenmapPowerIter(evTriDiag, alphaVec, betaVec, evInit, iter);
  if(h->Id(h->global) == 0) {
//    printf("alpha :  ");
//    GenmapPrintVector(alphaVec);
//    printf("\nbeta :  ");
//    GenmapPrintVector(betaVec);
//    printf("\nevTriDiag :  ");
//    GenmapPrintVector(evTriDiag);
  }

  // Multiply tri-diagonal matrix by [q1, q2, ...q_{iter}]
  GenmapCreateZerosVector(&evLanczos, lelt);
  for(GenmapInt i = 0; i < lelt; i++) {
    for(GenmapInt j = 0; j < iter; j++) {
      evLanczos->data[i] += q[j]->data[i] * evTriDiag->data[j];
    }
  }

  GenmapVector evOriginal, weights, evInit1;
  GenmapCreateVector(&weights, lelt);

  struct AxStruct axData;
  GenmapAxInit(h, h->global, weights);
  axData.h = h;
  axData.weights = weights;

  GenmapCreateVector(&evOriginal, lelt);
  GenmapCreateVector(&evInit1, lelt);
  for(GenmapInt i = 0; i < lelt; i++) {
    evInit1->data[i] = i;
  }

  GenmapPowerIterNew(evOriginal, Ax, &axData, evInit1, iter);

  printf("proc : %d (lanczos) ", h->Id(h->global));
  GenmapPrintVector(evLanczos);
  printf("\nproc : %d (original) ", h->Id(h->global));
  GenmapPrintVector(evOriginal);
  printf("\n");
  for(GenmapInt i = 0; i < lelt; i++) {
    printf("ratio: %lf\n",evLanczos->data[i]/evOriginal->data[i]);
  }

//  assert(GenmapVectorsEqual(evOriginal, evLanczos, GENMAP_TOL) == 1);
  for(GenmapInt i = 0; i < 10; i++) {
    GenmapDestroyVector(q[i]);
  }
  free(q);

  GenmapDestroyVector(initVec);
  GenmapDestroyVector(alphaVec);
  GenmapDestroyVector(betaVec);
  GenmapDestroyVector(evLanczos);
  GenmapDestroyVector(evInit);
  GenmapDestroyVector(evOriginal);
  GenmapDestroyVector(weights);
  GenmapDestroyVector(evInit1);
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

  GenmapFinalize(h);

#ifdef MPI
  MPI_Finalize();
#endif
  return 0;
}
