#ifndef _IO_H_
#define _IO_H_

#include <stdio.h>

//------------------------------------------------------------------------------
#define MAP_HEADER_SIZE 7

#define NEL      0
#define NACTIVE  1
#define DEPTH    2
#define D2       3
#define NPTS     4
#define NRANK    5
#define NOUTFLOW 6
//------------------------------------------------------------------------------
#ifdef MPI
void readmap_mpi   (long **header, long **glo_num, char* name);
#endif

void readmap_serial(long **header, long **glo_num, char* name);

void readmap(long **header, long **glo_num, char* name);
//------------------------------------------------------------------------------

#endif
