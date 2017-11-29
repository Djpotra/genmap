#include "mpiwrapper.h"
#include "test.h"

//------------------------------------------------------------------------------
#ifdef MPI
int32 test_1() {
  struct comm c;

  comm_init(&c, MPI_COMM_WORLD);

  comm_free(&c);

  return 1;
}
//------------------------------------------------------------------------------
int32 test_2() {
  int64 npts, glo_num;
  npts = 16;

  struct comm c;
  comm_init(&c, MPI_COMM_WORLD);

  gs_setup(&glo_num, npts, &c, 0, gs_crystal_router, 1);

  comm_free(&c);

  return 1;
}
//------------------------------------------------------------------------------
int32 main(int32 argc, char** argv) {
  struct comm c;
  init_genmap(&c, argc, argv);

  run_test(&test_1,"gs_comm1");
  run_test(&test_2,"gs_comm2");

  finalize_genmap(&c);

  return 0;
}
#endif
//------------------------------------------------------------------------------
