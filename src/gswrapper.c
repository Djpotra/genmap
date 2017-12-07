#include <stdio.h>

#include "gswrapper.h"

//------------------------------------------------------------------------------
int32 gop_setup = 0;
struct gs_data *gop_handle;

//------------------------------------------------------------------------------
void init_genmap(struct comm *c, int32 argc, char **argv)
{
#ifdef MPI
  MPI_Init(&argc, &argv);
#else
  int32 MPI_COMM_WORLD = 0;
#endif
  comm_init(c, MPI_COMM_WORLD);
}

void finalize_genmap(struct comm *c)
{
  comm_free(c);

#ifdef MPI
  MPI_Finalize();
#endif
}

//------------------------------------------------------------------------------
void gop_init(struct comm *c) {
  const int32 gop_id = 1;

  gop_handle = gs_setup(&gop_id, 1, c, 0, gs_auto, 0);

  gop_setup = 1;
}

//------------------------------------------------------------------------------
void gop(void *u, gs_dom dom, gs_op op, unsigned transpose) {
  if (gop_setup == 0) {
    printf("gop_init must be called before gop\n");
    return;
  }

  gs(u, dom, op, transpose, gop_handle, NULL);
}

//------------------------------------------------------------------------------
void gop_finalize() {
  gs_free(gop_handle);
}
//------------------------------------------------------------------------------
