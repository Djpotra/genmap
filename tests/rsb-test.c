#include <genmap-impl.h>

#ifdef MPI
#include <mpi.h>
#endif
#include <stdio.h>

void TestRSB1(GenmapHandle h) {
  GenmapRead(h, "mesh/box2D_2.bin");

  GenmapRSB(h);
}

int main(int argc, char **argv) {
#ifdef MPI
  int rank, np;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &np);
#else
  int MPI_COMM_WORLD = 0;
#endif

  GenmapHandle h;
  GenmapInit(&h, MPI_COMM_WORLD, "default");
  // Initialize local communicator from MPI_COMM_WORLD as
  // well. We can do this inside RSB routine as well
  GenmapCreateComm(h, &h->local, MPI_COMM_WORLD);

  TestRSB1(h);

  GenmapFinalize(h);

#ifdef MPI
  MPI_Finalize();
#endif
  return 0;
}
