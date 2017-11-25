#include "io.h"
#include "test.h"
#include "mpiwrapper.h"

//------------------------------------------------------------------------------
int test_1()
{
  char *name = "nbrhd/nbrhd.map.bin";
  long *header, *glo_num;

  readmap(&header, &glo_num, name);

  return 1;
}
//------------------------------------------------------------------------------
int main(int argc, char **argv)
{
#ifdef MPI
  MPI_Init(&argc, &argv);

  int np, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &np  );
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  run_test(&test_1, "fopen_1");

  MPI_Finalize();
#endif
}
//------------------------------------------------------------------------------
