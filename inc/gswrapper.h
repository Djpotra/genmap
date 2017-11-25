#ifndef _GSWRAPPER_H_
#define _GSWRAPPER_H_

#include <stdlib.h>
#include <stddef.h>

#include "types.h"
#include "name.h"
#include "fail.h"
#include "comm.h"
#include "gs_defs.h"

//------------------------------------------------------------------------------
typedef enum {gs_auto, gs_pairwise, gs_crystal_router, gs_all_reduce} gs_method;

//------------------------------------------------------------------------------
extern int gop_setup;
extern struct gs_data *gop_handle;
extern struct comm gop_comm;

void gop_init();

void gop(void *u, gs_dom dom, gs_op op, unsigned transpose);

void gop_finalize();

#endif
