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
// GenmapComm: Create, Destroy
int GenmapCreateComm_private(GenmapComm *c, GenmapCommExternal ce);

int GenmapDestroyComm_private(GenmapComm c);

// Functions to return size and rank of GenmapComm
int GenmapNp_private(GenmapComm c);

int GenmapId_private(GenmapComm c);
// Functions to do Laplacian
int GenmapAxInit_private(GenmapHandle h, GenmapComm c, GenmapVector weights);

int GenmapAx_private(GenmapHandle h, GenmapComm c, GenmapVector u,
                     GenmapVector weights, GenmapVector v);
//
// File I/O
//
// Genmap Input File header
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
// GenmapHeader: Create, Destroy
int GenmapCreateHeader_private(GenmapHeader *h);

int GenmapDestroyHeader_private(GenmapHeader h);
// Genmap Element
struct GenmapElements_private {
  GenmapScalar *fiedler;
  GenmapInt *globalId;
  GenmapInt *vertices;
};
// GenmapElements: Create, Destroy
int GenmapCreateElements_private(GenmapElements *e);

int GenmapDestroyElements_private(GenmapElements e);
// Function to read from FILE
int GenmapRead_private(GenmapHandle h, char *name);
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
  GenmapInt(*Ax)(GenmapHandle h, GenmapComm c, GenmapVector u,
                 GenmapVector weights, GenmapVector v);
  GenmapInt(*AxInit)(GenmapHandle h, GenmapComm c, GenmapVector weights);
};
// GenmapHandle: Create, Destroy
int GenmapCreateHandle_private(GenmapHandle *h);

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
