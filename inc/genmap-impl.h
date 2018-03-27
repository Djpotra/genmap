#ifndef _GENMAP_IMPL_H_
#define _GENMAP_IMPL_H_

#include <genmap.h>

#include <stddef.h>
#include <stdlib.h>
#include <assert.h>
//
// Genmap_Handle
//
struct GenmapHandle_private {
  struct comm c;
  struct gs_data *gsh;
};
//
// Genmap_Vector
//
struct GenmapVector_private {
  GenmapInt32 size;
  GenmapScalar *data;
};
//
// Malloc and Free
//
#define GenmapMalloc(n, p) *(p) = malloc(sizeof(**(p))*(n))

#endif
