#include "io.h"
#include "test.h"
#include "mpiwrapper.h"

//------------------------------------------------------------------------------
int32 test_1(struct comm *c) {
  char *name = "nbrhd/nbrhd.map.bin";
  struct element *elements; struct header mapheader;
  readmap(c, &elements, &mapheader, name);

  return 1;
}
//------------------------------------------------------------------------------
int32 test_2(struct comm *c) {
  char *name = "nbrhd/nbrhd.map.bin";
  struct element *elements; struct header mapheader;
  readmap(c, &elements, &mapheader, name);

  for (int32 i = 0; i < 1; i++) {
    printf("%d ", mapheader.nel);
    printf("%d ", mapheader.nactive);
    printf("%d ", mapheader.depth);
    printf("%d ", mapheader.d2);
    printf("%d ", mapheader.npts);
    printf("%d ", mapheader.nrank);
    printf("%d ", mapheader.noutflow);
    printf("%d ", mapheader.nc);
    printf("%d\n", mapheader.lelt);
  }
  printf("\n");

  int32 i = 0;
  while (i < mapheader.nc * mapheader.lelt) {
    printf("%d ", elements[i].globalId);
    i++;
    if (i % mapheader.nc == 0) printf("\n");
  }

  return 1;
}
//------------------------------------------------------------------------------
int32 main(int32 argc, char **argv) {

  struct comm c;
  init_genmap(&c, argc, argv);

  run_test_mpi(&test_1, &c, "readmap_1");
  run_test_mpi(&test_2, &c, "readmap_2");

  finalize_genmap(&c);
}
//------------------------------------------------------------------------------
