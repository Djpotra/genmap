#ifndef _GSLIB_H_
#define _GSLIB_H_

//#define USE_LONG
#ifdef GENMAP_UNDERSCORE
#  define UNDERSCORE
#endif

#ifdef GENMAP_MPI
#  define MPI
#endif

#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>

#include "c99.h"
#include "types.h"
#include "name.h"
#include "fail.h"
#include "types.h"
#include "mem.h"
#include "gs_defs.h"
#include "comm.h"
#include "gs.h"
#include "sort.h"
#include "sarray_sort.h"
#include "crystal.h"
#include "sarray_transfer.h"

#define TYPE_INT    0
#define TYPE_LONG   1
#define TYPE_FLOAT  2
#define TYPE_DOUBLE 3

#endif
