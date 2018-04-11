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

  GenmapInt iter = 100;
  GenmapInt lelt = h->header->lelt;
  GenmapVector initVec, alphaVec, betaVec;
  GenmapCreateVector(&initVec, h->header->lelt);
  for(GenmapInt i = 0; i < lelt; i++) {
    initVec->data[i] = i + 1;
  }
  GenmapCreateVector(&alphaVec, iter);
  GenmapCreateVector(&betaVec, iter - 1);

  GenmapVector *q = NULL;
  GenmapLanczos(h, h->global, initVec, iter, q, alphaVec, betaVec);

  GenmapVector evLanczos, evInit;
  GenmapCreateVector(&evLanczos, iter);
  GenmapCreateVector(&evInit, iter);
  for(GenmapInt i = 0; i < iter; i++) {
    evInit->data[i] = i + 1;
  }

  GenmapPowerIter(evLanczos, alphaVec, betaVec, evInit, iter);

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

  GenmapPowerIterNew(evOriginal, Ax, &axData, evInit1, 1);

//  if(h->Id(h->global) == 0) {
//    GenmapPrintVector(evLanczos);
//    printf("\n");
//  }
//  printf("%d\n", h->Id(h->global));
//  GenmapPrintVector(evOriginal);
//  printf("\n");

//  assert(GenmapVectorsEqual(evOriginal, evLanczos, GENMAP_TOL) == 1);

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
