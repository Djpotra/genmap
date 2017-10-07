#include <stdio.h>
#include <mpi.h>

#include "gswrapper.h"
#include "laplacian.h"
#include "linalg.h"

int rsb_setup = 0;
//------------------------------------------------------------------------------
void ax_setup(struct gs_data *gsh, long **weights, long npts, long nelt, long *glo_num)
{
  struct comm c;
  comm_init(&c, MPI_COMM_WORLD);

  *weights = malloc(sizeof(long)*nelt);
  for (long i = 0; i < nelt; i++) {
    *(*weights + i) = 1;
  }

  gsh = gs_setup(glo_num, npts, &c, 0, gs_crystal_router, 0);

  rsb_setup = 1;
}
//------------------------------------------------------------------------------
void ax(Vector *v, Vector *u, struct gs_data *gsh) {
  if (rsb_setup == 0) {
    fprintf(stderr, "Need to call ax_setup before this routine.");
    return;
  }


}
//------------------------------------------------------------------------------
