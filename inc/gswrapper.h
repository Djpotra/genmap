#ifndef _GSWRAPPER_H
#define _GSWRAPPER_H

#define MPI
//#define PREFIX gslib

// For using functions in comm.h
#include "test.h"
#include "types.h"
#include "name.h"
#include "gs_defs.h"
#include "gs_local.h"
#include "tensor.h"
#include "fail.h"
#include "comm.h"

// For gssetup
#include "c99.h"
#include "mem.h"
#include "crystal.h"
#include "sarray_transfer.h"
#include "sarray_sort.h"
#include "sort.h"
#include "comm.h"
#include "gs_local.h"
#include "gs_defs.h"
#include "types.h"
#include "fail.h"
#include "name.h"
#include "gs.h"

extern int gop_setup;
extern struct gs_data *gop_handle;
extern struct comm gop_comm;

void gop_init();

void gop(void *u, gs_dom dom, gs_op op, unsigned transpose);

void gop_finalize();

#endif
