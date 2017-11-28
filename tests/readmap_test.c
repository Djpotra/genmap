#include "io.h"
#include "test.h"

//------------------------------------------------------------------------------
int test_1(struct comm *c) {
  long *glo_num, *header;

  readmap(c, &header, &glo_num, "nbrhd/nbrhd.map.bin");

  printf("npts = %ld\n", header[NPTS]);
  printf("mychunk = %ld\n", header[MYCHUNK]);

  for (long i = 0; i < MAP_HEADER_SIZE; i++) {
    printf("%ld\n", glo_num[i]);
  }

  return 1;
}
//------------------------------------------------------------------------------
int main(int argc, char **argv) {
  struct comm c;
  init_genmap(&c, argc, argv);

  run_test_mpi(&test_1, &c, "map_1");

  finalize_genmap(&c);

  return 0;
}
