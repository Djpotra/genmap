#include "io.h"
#include "test.h"

//------------------------------------------------------------------------------
int32 test_1(struct comm *c) {
  int32 *glo_num, *header, *elem_id;

  readmap(c, &header, &glo_num, &elem_id, "nbrhd/nbrhd.map.bin");

  printf("npts = %d\n", header[NPTS]);
  printf("mychunk = %d\n", header[MYCHUNK]);

  for (int32 i = 0; i < MAP_HEADER_SIZE; i++) {
    printf("%d\n", glo_num[i]);
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
