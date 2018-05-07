#ifndef _GENMAP_DEFAULT_H_
#define _GENMAP_DEFAULT_H_

#include <genmap-impl.h>

#include <stddef.h>
#include <stdlib.h>
//
// GenmapComm
//
int GenmapCreateComm_default(GenmapComm *c, GenmapCommExternal ce);

int GenmapDestroyComm_default(GenmapComm c);

// Functions to return size and rank of GenmapComm
int GenmapNp_default(GenmapComm c);

int GenmapId_default(GenmapComm c);
// Functions to do Laplacian
int GenmapAxInit_default(GenmapHandle h, GenmapComm c,
                         GenmapVector weights);

int GenmapAx_default(GenmapHandle h, GenmapComm c, GenmapVector u,
                     GenmapVector weights, GenmapVector v);
// Functions to do global operations
int GenmapGop_default(GenmapComm c, GenmapScalar *v, GenmapInt size,
                      GenmapInt op);
//
// File I/O
//
// GenmapHeader: Create, Destroy
int GenmapCreateHeader_default(GenmapHeader *h);

int GenmapDestroyHeader_default(GenmapHeader h);
// GenmapElements: Create, Destroy
int GenmapCreateElements_default(GenmapElements *e);

int GenmapDestroyElements_default(GenmapElements e);
// Function to read from FILE
int GenmapRead_default(GenmapHandle h, char *name);
//
// GenmapHandle
//
// GenmapHandle: Create, Destroy
int GenmapCreateHandle_default(GenmapHandle h);

int GenmapDestroyHandle_default(GenmapHandle h);
#endif
