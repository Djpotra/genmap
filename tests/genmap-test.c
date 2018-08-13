#include <genmap.h>

#ifdef GENMAP_MPI
#include <mpi.h>
#endif
//
// Test GenmapInit, GenmpaFinalize, etc.
//
int main(int argc, char **argv) {
#ifdef GENMAP_MPI
  MPI_Init(&argc, &argv);
#else
  int MPI_COMM_WORLD = 0;
#endif

  GenmapHandle h;
  GenmapInit(&h, MPI_COMM_WORLD, "default", 1);
  GenmapFinalize(h);

#ifdef GENMAP_MPI
  MPI_Finalize();
#endif
}
