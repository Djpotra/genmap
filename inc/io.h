#ifndef _IO_H_
#define _IO_H

#include <stdio.h>

#include "gswrapper.h"
//------------------------------------------------------------------------------
long file_open(char *name, int mode);

long file_read(long fh);

long file_write(long fh);

long file_close(long fh);

void readmap(int nid, long *npts, long *nelt, long **glo_num, char* name);
//------------------------------------------------------------------------------

#endif
