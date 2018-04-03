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
int GenmapNp_private(GenmapComm c);

int GenmapId_private(GenmapComm c);

int GenmapAx_private(GenmapVector v, GenmapVector u, GenmapHandle h,
                     GenmapComm c);
int GenmapAxInit_private(GenmapVector v, GenmapVector u, GenmapHandle h,
                         GenmapComm c);
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
struct GenmapElements_private {
  GenmapScalar *fiedler;
  GenmapInt *globalId;
  GenmapInt *vertices;
};
//
// Genmap_Handle
//
struct GenmapHandle_private {
  // Data members
  GenmapComm global;
  GenmapComm local;
  GenmapHeader header;
  GenmapElements elements;
  // Function members
  GenmapInt(*Np)(GenmapComm c);
  GenmapInt(*Id)(GenmapComm c);
  GenmapInt(*Ax)(GenmapVector v, GenmapVector u, GenmapComm c);
};
//
// GenmapHandle_private: Create, Destroy
//
int GenmapHandleInit_private(GenmapHandle *h);
int GenmapDestroyHandle_private(GenmapHandle h);
//
// Genmap_Vector
//
struct GenmapVector_private {
  GenmapInt size;
  GenmapScalar *data;
};
//
// Malloc and Free
//
#define GenmapMalloc(n, p) *(p) = malloc(sizeof(**(p))*(n))

#endif
