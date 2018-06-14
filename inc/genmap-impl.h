#ifndef _GENMAP_IMPL_H_
#define _GENMAP_IMPL_H_

#include <genmap.h>

#include <stddef.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

#ifdef DEBUG
#include <stdio.h>
#endif
//
// GenmapComm
//
struct GenmapComm_private {
  struct comm gsComm;
  struct gs_data *verticesHandle;
  struct gs_data *edgesHandle;
  GenmapScalar *laplacianWeights;
};
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
  GenmapInt start;
};
// GenmapElements
struct GenmapElement_private {
  GenmapScalar fiedler;
  GenmapInt globalId;
  GenmapInt vertices[8];
  GenmapInt edges[12];
  GenmapUInt proc;
};
//
// Genmap_Handle
//
struct GenmapHandle_private {
  int (*Create)(GenmapHandle h);
  int (*Destroy)(GenmapHandle h);

  GenmapComm global;
  GenmapComm local;
  int (*CreateComm)(GenmapComm *c, GenmapCommExternal ce);
  int (*DestroyComm)(GenmapComm c);

  GenmapHeader header;
  int (*CreateHeader)(GenmapHeader *h);
  int (*DestroyHeader)(GenmapHeader h);

  struct array elementArray;
  GenmapElements(*GetElements)(GenmapHandle h);
  int (*CreateElements)(GenmapElements *e);
  int (*DestroyElements)(GenmapElements e);

  GenmapInt(*Np)(GenmapComm c);
  GenmapInt(*Id)(GenmapComm c);

  int (*Ax)(GenmapHandle h, GenmapComm c, GenmapVector u,
            GenmapVector weights, GenmapVector v);
  int (*AxInit)(GenmapHandle h, GenmapComm c, GenmapVector weights);

  int (*Gop)(GenmapComm c, GenmapScalar *v, GenmapInt size, GenmapInt op);

  int (*Read)(GenmapHandle h, char *name);
};
//
// Genmap_Vector
//
struct GenmapVector_private {
  GenmapInt size;
  GenmapScalar *data;
};
//
// Memory management routines
//
#define GenmapMalloc(n, p) GenmapMallocArray ((n), sizeof(**(p)), p)
#define GenmapCalloc(n, p) GenmapCallocArray ((n), sizeof(**(p)), p)
#define GenmapRealloc(n, p) GenmapReallocArray((n), sizeof(**(p)), p)

#endif
