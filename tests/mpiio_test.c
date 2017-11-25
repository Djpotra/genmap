#include "io.h"
#include "test.h"

#include <mpi.h>
//------------------------------------------------------------------------------
int test_1()
{
  file_open("nbrhd/nbrhd.map", MPI_MODE_RDONLY);
  return 1;
}
//------------------------------------------------------------------------------
int main(int argc, char **argv)
{
  MPI_Init(&argc, &argv);

  int np, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &np  );
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  run_test(&test_1, "fopen_1");

  MPI_Finalize();
}
//------------------------------------------------------------------------------
