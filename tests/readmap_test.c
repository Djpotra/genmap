#include "io.h"
#include "test.h"

//------------------------------------------------------------------------------
int32 test_1(struct comm *c) {
  struct element *elements;
  struct header mapheader;

  readmap(c, &elements, &mapheader, "nbrhd/nbrhd.map.bin");

  printf("npts = %d\n", mapheader.npts);
  printf("lelt = %d\n", mapheader.lelt);

  for(int32 i = 0; i < MAP_HEADER_SIZE; i++) {
    printf("%d\n", elements[i].globalId);
  }

  return 1;
}
//------------------------------------------------------------------------------
int32 main(int32 argc, char **argv) {
  struct comm c;
  init_genmap(&c, argc, argv);

  run_test_mpi(&test_1, &c, "map_1");

  finalize_genmap(&c);

  return 0;
}
