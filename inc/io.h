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
#define LELT     8

//------------------------------------------------------------------------------
// Structures used to store the .map files
struct header {
  int32 nel;
  int32 nactive;
  int32 depth;
  int32 d2;
  int32 npts;
  int32 nrank;
  int32 noutflow;
  int32 nc;
  int32 lelt;
};

//------------------------------------------------------------------------------
struct element {
  double fiedler;
  int32 globalId;
  int32 nc;
  int32 vertices[8];
};

//------------------------------------------------------------------------------
#ifdef MPI
void readmap_mpi(struct comm *c, struct element **elements,
                 struct header *mapheader, char* name);
#endif

void readmap_serial(struct element **elements, struct header *mapheader,
                    char* name);

void readmap(struct comm *c, struct element **elements,
             struct header *mapheader, char* name);
//------------------------------------------------------------------------------

#endif
