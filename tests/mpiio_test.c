#include "io.h"
#include "test.h"
#include "mpiwrapper.h"

#define FILESIZE (1024 * 1024)
//------------------------------------------------------------------------------
int test_1()
{
  char *buf, rank, nprocs, nints, bufsize;

  file_open("nbrhd/nbrhd.map.bin", MPI_MODE_RDONLY);
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
