#ifndef _GENMAP_FORTRAN_READER_
#define _GENMAP_FORTRAN_READER_

#include <genmap-impl.h>

#include <stddef.h>
#include <stdlib.h>
//
// GenmapHandle
//
int GenmapCreateHandle_fortran(GenmapHandle h);
//
// File I/O
//
int GenmapRead_fortran(GenmapHandle h, void *data);
#endif
