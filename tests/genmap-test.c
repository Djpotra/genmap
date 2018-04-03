#include <genmap.h>

#ifdef MPI
#include <mpi.h>
#endif
//
// Test GenmapInit, GenmpaFinalize, etc.
//
int main(int argc, char **argv) {
#ifdef MPI
  MPI_Init(&argc, &argv);
#else
  int MPI_COMM_WORLD = 0;
#endif

  GenmapHandle h;
  GenmapInit(&h, MPI_COMM_WORLD);
  GenmapFinalize(h);

#ifdef MPI
  MPI_Finalize();
#endif
}
