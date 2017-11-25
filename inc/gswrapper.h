#ifndef _GSWRAPPER_H_
#define _GSWRAPPER_H_

#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#include "c99.h"
#include "types.h"
#include "name.h"
#include "fail.h"
#include "mem.h"
#include "comm.h"
#include "gs_defs.h"
#include "gs.h"

//------------------------------------------------------------------------------
extern int gop_setup;
extern struct gs_data *gop_handle;
extern struct comm gop_comm;

//------------------------------------------------------------------------------
void init_genmap(struct comm *c, int argc, char **argv);

void finalize_genmap(struct comm *c);

//------------------------------------------------------------------------------
void gop_init();

void gop(void *u, gs_dom dom, gs_op op, unsigned transpose);

void gop_finalize();

//------------------------------------------------------------------------------
#endif
