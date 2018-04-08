#include <genmap-impl.h>

#ifdef MPI
#include <mpi.h>
#endif

void TestLanczos1(GenmapHandle h) {
  GenmapRead(h, "mesh/box2D_2.bin");

  GenmapInt iter = 100;
  GenmapVector initVec, alphaVec, betaVec;
  GenmapCreateVector(&initVec, h->header->lelt);
  for(GenmapInt i = 0; i < h->header->lelt; i++) {
    initVec->data[i] = i + 1;
  }
  GenmapCreateVector(&alphaVec, iter);
  GenmapCreateVector(&betaVec, iter - 1);

  GenmapLanczos(h, h->global, initVec, alphaVec, betaVec, iter);

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
