#include "io.h"
#include "test.h"
#include "mpiwrapper.h"

//------------------------------------------------------------------------------
int test_1(struct comm *c)
{
  char *name = "nbrhd/nbrhd.map.bin";
  long *header, *glo_num;

  readmap(c, &header, &glo_num, name);

  return 1;
}
//------------------------------------------------------------------------------
int main(int argc, char **argv)
{

  struct comm c;
  init_genmap(&c, argc, argv);

  run_test_mpi(&test_1, &c, "fopen_1");

  finalize_genmap(&c);
}
//------------------------------------------------------------------------------
