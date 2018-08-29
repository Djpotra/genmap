#include <genmap-impl.h>

#ifdef GENMAP_MPI
#include <mpi.h>
#endif
#include <stdio.h>
//
// Test IO
//
int TestIO1(GenmapHandle h) {
  char *name = "mesh/cubeHexE64.msh";

  GenmapRead(h, name);

  return 0;
}

int main(int argc, char **argv) {
#ifdef GENMAP_MPI
  MPI_Init(&argc, &argv);
#else
  int MPI_COMM_WORLD = 0;
#endif

  GenmapHandle h;
  GenmapInit(&h, MPI_COMM_WORLD, "gmsh", 0);
  TestIO1(h);
  GenmapRSB(h);
  GenmapWrite(h, "cubeHexE64");
  GenmapFinalize(h);

#ifdef GENMAP_MPI
  MPI_Finalize();
#endif
}
