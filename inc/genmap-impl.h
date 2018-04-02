#ifndef _GENMAP_IMPL_H_
#define _GENMAP_IMPL_H_

#include <genmap.h>

#include <stddef.h>
#include <stdlib.h>
#include <assert.h>
//
// GenmapComm
//
struct GenmapComm_private {
  struct comm gsComm;
  struct gs_data *gsHandle;
  GenmapScalar *laplacianWeights;
};
// Functions to return size and rank of GenmapComm
GenmapInt GenmapNp_private(GenmapComm c);
GenmapInt GenmapId_private(GenmapComm c);
//
// Genmap_Handle
//
struct GenmapHandle_private {
  // Data members
  GenmapComm global;
  GenmapComm local;
  // Function members
  GenmapInt (*Np)(GenmapComm c);
  GenmapInt (*Id)(GenmapComm c);
};
//
// Genmap_Vector
//
struct GenmapVector_private {
  GenmapInt size;
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
  GenmapInt nel;
  GenmapInt nactive;
  GenmapInt depth;
  GenmapInt d2;
  GenmapInt npts;
  GenmapInt nrank;
  GenmapInt noutflow;
  GenmapInt nc;
  GenmapInt lelt;
};
//
// Genmap Element
//
struct GenmapElement_private {
  GenmapScalar fiedler;
  GenmapInt globalId;
  GenmapInt nc;
  GenmapInt vertices[8];
};
//
// Malloc and Free
//
#define GenmapMalloc(n, p) *(p) = malloc(sizeof(**(p))*(n))

#endif
