#ifndef _GENMAP_H_
#define _GENMAP_H_

#include <gslib.h>
//
// Genmap types
//
typedef int GenmapInt32;
typedef unsigned int GenmapUInt32;
typedef long GenmapInt64;
typedef unsigned long GenmapUInt64;

typedef GenmapInt32 GenmapInt;
typedef GenmapInt64 GenmapLong;

typedef double GenmapScalar;

typedef struct GenmapComm_private *GenmapComm;

typedef struct GenmapHandle_private *GenmapHandle;

typedef struct GenmapVector_private *GenmapVector;

typedef struct GenmapElements_private *GenmapElements;

typedef struct GenmapHeader_private *GenmapHeader;
//
// Genmap tolerances
//
#define GENMAP_TOL 1e-12
#define GENMAP_SP_TOL 1e-8
//
// Genmap Readers
//
#define GENMAP_READER_LEN 256
#define GENMAP_MAX_READERS 32

int GenmapRegisterReader(char *name, int (*Create)(GenmapHandle h));
//
// GenmapCommExternal
//
#ifdef MPI
typedef MPI_Comm GenmapCommExternal;
#else
typedef int GenmapCommExternal;
#endif
//
// GenmapComm
//
int GenmapCreateComm(GenmapHandle h, GenmapComm *c, GenmapCommExternal ce);
int GenmapDestroyComm(GenmapHandle h, GenmapComm c);
// Functions to return size and rank of GenmapComm
int GenmapNp(GenmapComm c);
int GenmapId(GenmapComm c);
// Functions to do global operations
int GenmapGop(GenmapHandle h, GenmapComm c, GenmapScalar *v);
//
// File I/O
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
// GenmapHeader: Create, Destroy
int GenmapCreateHeader(GenmapHandle h, GenmapHeader *header);
int GenmapDestroyHeader(GenmapHandle h, GenmapHeader header);
// GenmapElements: Create, Destroy
int GenmapCreateElements(GenmapHandle h, GenmapElements *e);
int GenmapDestroyElements(GenmapHandle h, GenmapElements e);
// Function to read from FILE
int GenmapRead(GenmapHandle h, char *name);
//
// GenmapHandle
//
// GenmapHandle: Create, Destroy
int GenmapCreateHandle(GenmapHandle *h);
int GenmapDestroyHandle(GenmapHandle h);
//
// Genmap: Init, Finalize
//
int GenmapInit(GenmapHandle *h, GenmapCommExternal ce, char *reader);
int GenmapFinalize(GenmapHandle h);
//
// GenmapVector operations
//
int GenmapCreateVector(GenmapVector *x, GenmapInt size);
int GenmapSetVector(GenmapVector x, GenmapScalar *array);
int GenmapGetVector(GenmapVector x, GenmapScalar *array);

int GenmapCreateRandomVector(GenmapVector *x, GenmapInt size,
                             GenmapInt seed);
int GenmapCreateOnesVector(GenmapVector *x, GenmapInt size);
int GenmapCreateZerosVector(GenmapVector *x, GenmapInt size);

int GenmapScaleVector(GenmapVector y, GenmapVector x, GenmapScalar alpha);
int GenmapAxpbyVector(GenmapVector z, GenmapVector x, GenmapScalar alpha,
                      GenmapVector y, GenmapScalar beta);

int GenmapVectorsEqual(GenmapVector x, GenmapVector y, GenmapScalar tol);
int GenmapCopyVector(GenmapVector x, GenmapVector y);
GenmapScalar GenmapDotVector(GenmapVector x, GenmapVector y);
GenmapScalar GenmapNormVector(GenmapVector x, GenmapInt p);

int GenmapPrintVector(GenmapVector x);
int GenmapDestroyVector(GenmapVector x);
// Functions to do Laplacian
int GenmapAxInit(GenmapHandle h, GenmapComm c, GenmapVector weights);

int GenmapAx(GenmapHandle h, GenmapComm c, GenmapVector u,
             GenmapVector weights, GenmapVector v);
void GenmapLanczos(GenmapHandle h, GenmapComm c, GenmapVector init,
                   GenmapInt iter, GenmapVector **q, GenmapVector alpha,
                   GenmapVector beta);
void GenmapRSB(GenmapHandle h);
//
// Linear solve
//
int GenmapSymTriDiagSolve(GenmapVector x, GenmapVector b, GenmapVector alpha,
                          GenmapVector beta);
//
// Power and inverse power iterations
//
int GenmapPowerIter(GenmapVector eVector, GenmapVector alpha,
                    GenmapVector beta, GenmapVector init, GenmapInt iter);

int GenmapPowerIterNew(GenmapVector eVector, void (*Ax)(GenmapVector ax,
                       GenmapVector x, void* data), void *data,
                       GenmapVector init, GenmapInt iter);

int GenmapInvPowerIter(GenmapVector eVector, GenmapVector alpha,
                       GenmapVector beta, GenmapVector init, GenmapInt iter);
//
// Memory Allocation and Free
//
int GenmapFree(void *p);
#endif
