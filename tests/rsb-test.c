#include <genmap-impl.h>

#ifdef MPI
#include <mpi.h>
#endif
#include <stdio.h>

int main(int argc, char **argv) {
#ifdef MPI
  MPI_Init(&argc, &argv);
#else
  int MPI_COMM_WORLD = 0;
#endif

  GenmapHandle h;
  GenmapInit(&h, MPI_COMM_WORLD, "default");

  GenmapFinalize(h);

#ifdef MPI
  MPI_Finalize();
#endif
  return 0;
}
