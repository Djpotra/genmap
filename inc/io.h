#ifndef _IO_H_
#define _IO_H_

//------------------------------------------------------------------------------
long file_open(char *name, int mode);

long file_read(long fh);

long file_write(long fh);

long file_close(long fh);

void readmap_dev(int nid, long *npts, long *nelt, long **glo_num, char* name);
//TODO: Get rid of this one
void readmap(long *npts, long *nelt, long **glo_num, char* name);
//------------------------------------------------------------------------------

#endif
