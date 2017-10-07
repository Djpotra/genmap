#include <stdio.h>
#include <mpi.h>

#include "gswrapper.h"
#include "laplacian.h"
#include "linalg.h"

int rsb_setup = 0;
//------------------------------------------------------------------------------
void ax_setup(struct gs_data *gsh, long **weights, struct comm *c, \
                            long npts, long nelt, long *glo_num)
{
  *weights = malloc(sizeof(long)*nelt);
  for (long i = 0; i < nelt; i++) {
    *(*weights + i) = 1;
  }

  printf("Thilina1\n");
  gsh = gs_setup(glo_num, npts, c, 0, gs_crystal_router, 0);

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
