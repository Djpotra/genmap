#ifndef _GENMAP_IMPL_H_
#define _GENMAP_IMPL_H_

#include <genmap.h>

#include <stddef.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

#ifdef GENMAP_DEBUG
#include <stdio.h>
#endif
//
// GenmapComm
//
struct GenmapComm_private {
  struct comm gsComm;
  struct gs_data *verticesHandle;
  struct gs_data *edgesHandle;
  struct gs_data *facesHandle;
  GenmapScalar *laplacianWeights;
};
//
// File I/O
//
// Genmap Input File header
struct GenmapHeader_private {
  GenmapInt nel;
  GenmapInt npts;
  GenmapInt nv;
  GenmapInt ne;
  GenmapInt ndim;
  GenmapInt lelt;
  GenmapInt start;
  GenmapInt Nnodes;
};
//
// GenmapHeader: Create, Destroy
//
int GenmapCreateHeader(GenmapHeader *h);
int GenmapDestroyHeader(GenmapHeader h);
// GenmapElements
struct GenmapElement_private {
  GenmapScalar fiedler;
  GenmapInt globalId;
  GenmapInt vertices[8];
  GenmapInt edges[12];
  GenmapInt faces[6];
  GenmapUInt proc;
  GenmapScalar x[8];
  GenmapScalar y[8];
  GenmapScalar z[8];
};
//
// GenmapElements: Create, Destroy
//
int GenmapCreateElements(GenmapElements *e);
int GenmapDestroyElements(GenmapElements e);
GenmapElements GenmapGetElements_default(GenmapHandle h);
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

  int (*Read)(GenmapHandle h, void *data);

  int (*Write)(GenmapHandle h, char *fileNameBase);

  GenmapInt exactAx;
};
// GenmapHandle
int GenmapCreateHandle(GenmapHandle h, int exactAx);
int GenmapDestroyHandle(GenmapHandle h);
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
