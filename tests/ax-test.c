#include <genmap-impl.h>

#include <stdio.h>
#ifdef MPI
#include <mpi.h>
#endif
//
// Test IO
//
int TestAx1(GenmapHandle h) {
  char *name = "mesh/box2D_2.bin";

  GenmapRead_private(h, name);

  GenmapVector weights, u, v;
  GenmapCreateVector(&weights, h->header->lelt);
  GenmapCreateOnesVector(&u, h->header->lelt);
  GenmapCreateVector(&v, h->header->lelt);

  h->AxInit(h, h->global, weights);
  for(GenmapInt i = 0; i < h->header->lelt; i++) {
    printf("rank = %d, weight[%d] = %lf\n", h->Id(h->global), i, weights->data[i]);
  }
  h->Ax(h, h->global, u, weights, v);
  for(GenmapInt i = 0; i < h->header->lelt; i++) {
    printf("rank = %d, v[%d] = %lf\n", h->Id(h->global), i, v->data[i]);
  }

  GenmapDestroyVector(weights);
  GenmapDestroyVector(u);
  GenmapDestroyVector(v);

  return 0;
}

int main(int argc, char **argv) {
#ifdef MPI
  MPI_Init(&argc, &argv);
#else
  int MPI_COMM_WORLD = 0;
#endif

  GenmapHandle h;
  GenmapInit(&h, MPI_COMM_WORLD);
  TestAx1(h);
  GenmapFinalize(h);

#ifdef MPI
  MPI_Finalize();
#endif
}
