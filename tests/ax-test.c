#include <genmap-impl.h>

#ifdef MPI
#include <mpi.h>
#endif
//
// Test IO
//
int TestAx1(GenmapHandle h) {
  char *name = "nbrhd/nbrhd.map.bin";

  GenmapRead_private(h, name);

  GenmapVector weights, u, v;
  GenmapCreateVector(&weights, h->header->lelt);

  h->AxInit(h, h->global, weights);

  GenmapDestroyVector(weights);

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
