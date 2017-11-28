#ifndef _IO_H_
#define _IO_H_

#include <stdio.h>

#include "mpiwrapper.h"
#include "dtypes.h"

//------------------------------------------------------------------------------
#define MAP_HEADER_SIZE 7
#define HEADER_SIZE (MAP_HEADER_SIZE + 2)

#define NEL      0
#define NACTIVE  1
#define DEPTH    2
#define D2       3
#define NPTS     4
#define NRANK    5
#define NOUTFLOW 6
#define NC       7
#define MYCHUNK  8
//------------------------------------------------------------------------------
#ifdef MPI
void readmap_mpi   (struct comm *c, int64 **header, int64 **glo_num,
                                        long** element_id, char* name);
#endif
void readmap_serial(struct comm *c, int64 **header, int64 **glo_num,
                                        long** element_id, char* name);
void readmap(struct comm *c, int64 **header, int64 **glo_num,
                                        long** element_id, char* name);
//------------------------------------------------------------------------------

#endif
