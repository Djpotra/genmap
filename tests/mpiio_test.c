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
int test_2(struct comm *c)
{
  char *name = "nbrhd/nbrhd.map.bin";
  long *header, *glo_num;

  readmap(c, &header, &glo_num, name);
  for (int i = 0; i < MAP_HEADER_SIZE; i++)
  {
    if (c->id == 0) {
      printf("%ld\n", header[i]);
    }
  }

  return 1;
}
//------------------------------------------------------------------------------
int main(int argc, char **argv)
{

  struct comm c;
  init_genmap(&c, argc, argv);

  run_test_mpi(&test_1, &c, "readmap_1");
  run_test_mpi(&test_2, &c, "readmap_2");

  finalize_genmap(&c);
}
//------------------------------------------------------------------------------
