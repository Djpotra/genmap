#ifndef _GSWRAPPER_H_
#define _GSWRAPPER_H_

#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#include "dtypes.h"

#include "c99.h"
#include "types.h"
#include "name.h"
#include "fail.h"
#include "mem.h"
#include "gs_defs.h"
#include "comm.h"
#include "gs.h"

//------------------------------------------------------------------------------
void init_genmap(struct comm *c, int32 argc, char **argv);

void finalize_genmap(struct comm *c);

//------------------------------------------------------------------------------
void gop_init(struct gs_data **gsh, struct comm *c);

void gop(void *u, struct gs_data *goph, gs_dom dom, gs_op op,
         unsigned transpose);

//------------------------------------------------------------------------------
#endif
