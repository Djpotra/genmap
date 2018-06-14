#ifndef _GENMAP_FORTRAN_READER_
#define _GENMAP_FORTRAN_READER_

#include <genmap-impl.h>

#include <stddef.h>
#include <stdlib.h>
//
// GenmapComm
//
int GenmapCreateComm_fortran(GenmapComm *c, GenmapCommExternal ce);
int GenmapDestroyComm_fortran(GenmapComm c);

// Functions to return size and rank of GenmapComm
int GenmapNp_fortran(GenmapComm c);
int GenmapId_fortran(GenmapComm c);

// Functions to do Laplacian
int GenmapAxInit_fortran(GenmapHandle h, GenmapComm c,
                         GenmapVector weights);
int GenmapAx_fortran(GenmapHandle h, GenmapComm c, GenmapVector u,
                     GenmapVector weights, GenmapVector v);

// Functions to do global operations
int GenmapGop_fortran(GenmapComm c, GenmapScalar *v, GenmapInt size,
                      GenmapInt op);
//
// File I/O
//
// GenmapHeader: Create, Destroy
int GenmapCreateHeader_fortran(GenmapHeader *h);
int GenmapDestroyHeader_fortran(GenmapHeader h);

// GenmapElements: Create, Destroy
int GenmapCreateElements_fortran(GenmapElements *e);
int GenmapDestroyElements_fortran(GenmapElements e);
GenmapElements GenmapGetElements_fortran(GenmapHandle h);

// Function to read from FILE
int GenmapRead_fortran(GenmapHandle h, char *name);
//
// GenmapHandle
//
// GenmapHandle: Create, Destroy
int GenmapCreateHandle_fortran(GenmapHandle h);
int GenmapDestroyHandle_fortran(GenmapHandle h);
#endif
