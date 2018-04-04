#include <genmap-impl.h>

#ifdef MPI
#include <mpi.h>
#endif
//
// Test Ax
//
int TestAx1(GenmapHandle h) {
  char *name = "mesh/box2D_2.bin";

  GenmapRead_private(h, name);

  GenmapVector weights, u, v, answer;

  GenmapCreateVector(&weights, h->header->lelt);
  GenmapCreateOnesVector(&u, h->header->lelt);
  GenmapCreateZerosVector(&answer, h->header->lelt);
  GenmapCreateVector(&v, h->header->lelt);

  h->AxInit(h, h->global, weights);
  h->Ax(h, h->global, u, weights, v);

  assert(GenmapVectorsEqual(v, answer, GENMAP_TOL) == 1);

  GenmapDestroyVector(weights);
  GenmapDestroyVector(u);
  GenmapDestroyVector(v);
  GenmapDestroyVector(answer);

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
