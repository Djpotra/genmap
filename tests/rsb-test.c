#include <genmap-impl.h>

#ifdef MPI
#include <mpi.h>
#endif
#include <stdio.h>

void TestRSB1(GenmapHandle h) {
  GenmapRead(h, "mesh/box2D_2.bin");

  GenmapRSB(h);
  buffer buf = null_array;
  for(GenmapInt i = 0; i < h->header->lelt; i++) {
    h->elements[i].fiedler *= -1;
  }
  sarray_sort(struct GenmapElement_private, h->elements, h->header->lelt,
              fiedler, 3, &buf);
  for(GenmapInt i = 0; i < h->header->lelt; i++) {
    printf("proc = %d id = %d fiedler = %lf\n", h->Id(h->global),
           h->elements[i].globalId, h->elements[i].fiedler);
  }
//  GenmapInt iter = 10;
//  GenmapInt lelt = h->header->lelt;
//  GenmapVector initVec, alphaVec, betaVec;
//  GenmapInt offset = h->Id(h->global) * h->header->lelt;
//
//  GenmapCreateVector(&initVec, h->header->lelt);
//  for(GenmapInt i = 0;  i < lelt; i++) {
//    initVec->data[i] = offset + i + 1;
//  }
//
//  GenmapCreateVector(&alphaVec, iter);
//  GenmapCreateVector(&betaVec, iter - 1);
//  GenmapVector *q = NULL;
//  GenmapLanczos(h, h->global, initVec, iter, &q, alphaVec, betaVec);
//  iter = alphaVec->size;
//
//  // Make sure that q-vectors are orthogonal
//  for(GenmapInt i = 0; i < iter; i++) {
//    for(GenmapInt j = i + 1; j < iter; j++) {
//      GenmapScalar dot = GenmapDotVector(q[i], q[j]);
//      h->Gop(h->global, &dot);
//      assert(fabs(dot - 0) < GENMAP_TOL);
//    }
//  }
//
//  GenmapVector evLanczos, evTriDiag, evInit;
//  GenmapCreateVector(&evTriDiag, iter);
//  GenmapCreateVector(&evInit, iter);
//  for(GenmapInt i = 0; i < iter; i++) {
//    evInit->data[i] = i + 1;
//  }
//
//  GenmapPowerIter(evTriDiag, alphaVec, betaVec, evInit, iter * 30);
//
//  // Multiply tri-diagonal matrix by [q1, q2, ...q_{iter}]
//  GenmapCreateZerosVector(&evLanczos, lelt);
//  for(GenmapInt i = 0; i < lelt; i++) {
//    for(GenmapInt j = 0; j < iter; j++) {
//      evLanczos->data[i] += q[j]->data[i] * evTriDiag->data[j];
//    }
//  }
//
//  GenmapScalar lNorm = 0;
//  for(GenmapInt i = 0; i < lelt; i++) {
//    lNorm += evLanczos->data[i] * evLanczos->data[i];
//  }
//  h->Gop(h->global, &lNorm);
//  GenmapScaleVector(evLanczos, evLanczos, 1. / sqrt(lNorm));

//  for(GenmapInt i = 0; i < iter; i++) {
//    GenmapDestroyVector(q[i]);
//  }
//  GenmapFree(q);
//
//  GenmapDestroyVector(initVec);
//  GenmapDestroyVector(alphaVec);
//  GenmapDestroyVector(betaVec);
//  GenmapDestroyVector(evLanczos);
//  GenmapDestroyVector(evTriDiag);
//  GenmapDestroyVector(evInit);
}
int main(int argc, char **argv) {
  GenmapCommExternal local;
#ifdef MPI
  int rank, np;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &np);
  MPI_Comm_split(MPI_COMM_WORLD, rank % 2, rank, &local);
#else
  local = 0;
  int MPI_COMM_WORLD = 0;
#endif

  GenmapHandle h;
  GenmapInit(&h, MPI_COMM_WORLD, "default");
  GenmapCreateComm(h, &h->local, local);

  TestRSB1(h);

  GenmapFinalize(h);

#ifdef MPI
  MPI_Finalize();
#endif
  return 0;
}
