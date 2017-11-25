#ifndef _IO_H_
#define _IO_H_

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
long file_open(char *header, char *data, char *name, int mode);

void readmap_mpi(int nid, long *npts, long *nelt, long **glo_num, char* name);

void readmap_serial(long **header, long **glo_num, char* name);

void readmap(long **header, long **glo_num, char* name);
//------------------------------------------------------------------------------

#endif
