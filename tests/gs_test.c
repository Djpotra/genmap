#include "mpiwrapper.h"
#include "test.h"

//------------------------------------------------------------------------------
int32 test_1() {
  slong id;
  id = 1;

  return id;
}
//------------------------------------------------------------------------------
#ifdef MPI
int32 test_2() {
  struct comm c;

  comm_init(&c, MPI_COMM_WORLD);

  comm_free(&c);

  return 1;
}
//------------------------------------------------------------------------------
int32 test_3() {
  slong npts, glo_num;
  struct comm c;

  comm_init(&c, MPI_COMM_WORLD);

  gs_setup(&glo_num, npts, &c, 0, gs_crystal_router, 1);

  comm_free(&c);

  return 1;
}
//------------------------------------------------------------------------------
#endif
int32 main() {
  struct comm c;
  init_genmap(&c, NULL, NULL);

  run_test(&test_1,"gs_link1");
#ifdef MPI
  run_test(&test_2,"gs_comm1");
  run_test(&test_3,"gs_comm2");
#endif

  finalize_genmap(&c);

  return 0;
}
//------------------------------------------------------------------------------
