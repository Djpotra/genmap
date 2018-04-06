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
// GenmapElements
struct GenmapElements_private {
  GenmapScalar *fiedler;
  GenmapInt *globalId;
  GenmapInt *vertices;
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

  GenmapElements elements;
  int (*CreateElements)(GenmapElements *e);
  int (*DestroyElements)(GenmapElements e);

  GenmapInt(*Np)(GenmapComm c);
  GenmapInt(*Id)(GenmapComm c);

  int (*Ax)(GenmapHandle h, GenmapComm c, GenmapVector u,
            GenmapVector weights, GenmapVector v);
  int (*AxInit)(GenmapHandle h, GenmapComm c, GenmapVector weights);

  int (*Gop)(GenmapComm c, GenmapScalar *v);

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
// Malloc and Free
//
#define GenmapMalloc(n, p) *(p) = malloc(sizeof(**(p))*(n))
#endif
