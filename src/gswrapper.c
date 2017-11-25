#include "gswrapper.h"

//------------------------------------------------------------------------------
int gop_setup = 0;
struct gs_data *gop_handle;
struct comm gop_comm;

//------------------------------------------------------------------------------
void gop_init() {
#ifdef MPI
  comm_init(&gop_comm, MPI_COMM_WORLD);
#else
  comm_init(&gop_comm, 0);
#endif

  long gop_id = 1;

  gop_handle = gs_setup(&gop_id, 1, &gop_comm, 0, gs_auto, 0); 
}

//------------------------------------------------------------------------------
void gop(void *u, gs_dom dom, gs_op op, unsigned transpose) {
  if (gop_setup == 0) {
    gop_init();
    gop_setup = 1;
  } 

  gs(u, dom, op, transpose, gop_handle, NULL); 
}

//------------------------------------------------------------------------------
void gop_finalize() {
  comm_free(&gop_comm);

  gs_free(gop_handle);
}
//------------------------------------------------------------------------------
