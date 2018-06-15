#ifndef _GENMAP_DEFAULT_READER_H_
#define _GENMAP_DEFAULT_READER_H_

#include <genmap-impl.h>

#include <stddef.h>
#include <stdlib.h>
//
// GenmapHandle
//
int GenmapCreateHandle_default(GenmapHandle h);
//
// File I/O
//
int GenmapRead_default(GenmapHandle h, char *name);
#endif
