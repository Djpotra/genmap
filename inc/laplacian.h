#ifndef _LAPLACIAN_H_
#define _LAPLACIAN_H_

#include "linalg.h"
#include "gswrapper.h"
#include "dtypes.h"

extern int32 rsb_setup;

void ax_init(struct gs_data **gsh, double **weights, struct comm *c, \
             int32 npts, int32 nelt, int32 *glo_num);

void ax(Vector *v, Vector *u, struct gs_data *gsh, double *weights, int32 nc);

#endif
