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
  struct comm globalComm;
  struct comm localComm;
  struct gs_data *globalHandle;
  struct gs_data *localHandle;
  GenmapScalar *globalWeights;
  GenmapScalar *localWeights;
};
//
// Genmap_Vector
//
struct GenmapVector_private {
  GenmapInt32 size;
  GenmapScalar *data;
};
//
// Genmap File I/O
//
#define GENMAP_HEADER_SIZE 7
#define GENMAP_NEL      0
#define GENMAP_NACTIVE  1
#define GENMAP_DEPTH    2
#define GENMAP_D2       3
#define GENMAP_NPTS     4
#define GENMAP_NRANK    5
#define GENMAP_NOUTFLOW 6
#define GENMAP_NC       7
#define GENMAP_LELT     8
//
// Genmap Input File header
//
struct GenmapHeader_private {
  GenmapInt32 nel;
  GenmapInt32 nactive;
  GenmapInt32 depth;
  GenmapInt32 d2;
  GenmapInt32 npts;
  GenmapInt32 nrank;
  GenmapInt32 noutflow;
  GenmapInt32 nc;
  GenmapInt32 lelt;
};
//
// Genmap Element
//
struct GenmapElement_private {
  GenmapScalar fiedler;
  GenmapInt32 globalId;
  GenmapInt32 nc;
  GenmapInt32 vertices[8];
};
//
// Malloc and Free
//
#define GenmapMalloc(n, p) *(p) = malloc(sizeof(**(p))*(n))

#endif
