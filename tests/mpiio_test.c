#include "io.h"
#include "test.h"
#include "mpiwrapper.h"

//------------------------------------------------------------------------------
int test_1(struct comm *c)
{
  char *name = "nbrhd/nbrhd.map.bin";
  long *header, *glo_num, *elem_id;

  readmap(c, &header, &glo_num, &elem_id, name);

  return 1;
}
//------------------------------------------------------------------------------
int test_2(struct comm *c)
{
  char *name = "nbrhd/nbrhd.map.bin";
  long *header, *glo_num, *elem_id;

  readmap(c, &header, &glo_num, &elem_id, name);

  for (int i = 0; i < HEADER_SIZE; i++)
  {
    printf("%ld ", header[i]);
  }
  printf("\n");

  long i = 0;
  while (i < header[NC]*header[MYCHUNK])
  {
    printf("%ld ", glo_num[i]);
    i++;
    if (i%header[NC] == 0) printf("\n");
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
