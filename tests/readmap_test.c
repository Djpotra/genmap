#include "io.h"
#include "test.h"

//------------------------------------------------------------------------------
int32 test_1(struct comm *c) {
  int64 *glo_num, *header, *elem_id;

  readmap(c, &header, &glo_num, &elem_id, "nbrhd/nbrhd.map.bin");

  printf("npts = %ld\n", header[NPTS]);
  printf("mychunk = %ld\n", header[MYCHUNK]);

  for (int64 i = 0; i < MAP_HEADER_SIZE; i++) {
    printf("%ld\n", glo_num[i]);
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
