#ifndef _IO_H_
#define _IO_H_

//------------------------------------------------------------------------------
long file_open(char *name, int mode);

long file_read(long fh);

long file_write(long fh);

long file_close(long fh);

void readmap_mpi(int nid, long *npts, long *nelt, long **glo_num, char* name);

void readmap_serial(long *npts, long *nelt, long **glo_num, char* name);

void readmap(long *npts, long *nelt, long **glo_num, char* name);
//------------------------------------------------------------------------------

#endif
