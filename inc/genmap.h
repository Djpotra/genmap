#ifndef _GENMAP_H_
#define _GENMAP_H_

#include <gslib.h>

#ifdef GENMAP_MPI
#include <mpi.h>
#endif
//
// Genmap Operators
//
#define GENMAP_SUM 0
#define GENAMP_MAX 1
#define GENMAP_MIN 2
#define GENMAP_MUL 3
//
// Genmap Memory Align
//
#define GENMAP_ALIGN 64
//
// Genmap types
//
typedef int GenmapInt32;
typedef unsigned int GenmapUInt32;
typedef long GenmapInt64;
typedef unsigned long GenmapUInt64;

typedef GenmapInt32 GenmapInt;
typedef GenmapInt64 GenmapLong;
typedef GenmapUInt32 GenmapUInt;
typedef GenmapUInt64 GenmapULong;

typedef double GenmapScalar;

typedef struct GenmapComm_private *GenmapComm;

typedef struct GenmapHandle_private *GenmapHandle;

typedef struct GenmapVector_private *GenmapVector;

typedef struct GenmapElement_private *GenmapElements;

typedef struct GenmapHeader_private *GenmapHeader;
//
// Genmap Debug routines
//
#ifdef GENMAP_DEBUG
#  define dbgfl printf("%s:%d\n",__FILE__,__LINE__);
#else
#  define dbgfl ;
#endif
//
// Genmap tolerances
//
#define GENMAP_SP_TOL 1e-08
#define GENMAP_DP_TOL 1e-12
#define GENMAP_TOL GENMAP_DP_TOL
//
// Genmap Readers
//
#define GENMAP_READER_LEN 256
#define GENMAP_MAX_READERS 32

int GenmapRegisterReader(char *name, int (*Create)(GenmapHandle h));
//
// GenmapCommExternal
//
#ifdef GENMAP_MPI
typedef MPI_Comm GenmapCommExternal;
#else
typedef int GenmapCommExternal;
#endif
//
// GenmapComm
//
int GenmapCreateComm(GenmapComm *c, GenmapCommExternal ce);
int GenmapDestroyComm(GenmapComm c);
// Functions to return size and rank of GenmapComm
int GenmapNp(GenmapComm c);
int GenmapId(GenmapComm c);
// Functions to do global operations
int GenmapGop(GenmapComm c, GenmapScalar *v, GenmapInt size,
              GenmapInt op);
#define GENMAP_SUM 0
#define GENMAP_MAX 1
#define GENMAP_MIN 2
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
//
// Get Elements
//
GenmapElements GenmapGetElements(GenmapHandle h);
// Function to read from FILE
int GenmapRead(GenmapHandle h, void *data);
//
// Genmap: Init, Finalize
//
int GenmapInit(GenmapHandle *h, GenmapCommExternal ce, char *reader,
               int exactAx);
int GenmapFinalize(GenmapHandle h);
// GenmapMalloc, Realloc, Calloc and Free
int GenmapMallocArray(size_t n, size_t unit, void *p);
int GenmapCallocArray(size_t n, size_t unit, void *p);
int GenmapReallocArray(size_t n, size_t unit, void *p);
int GenmapFree(void *p);
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

int GenmapScaleVector(GenmapVector y, GenmapVector x,
                      GenmapScalar alpha);
int GenmapAxpbyVector(GenmapVector z, GenmapVector x,
                      GenmapScalar alpha,
                      GenmapVector y, GenmapScalar beta);

int GenmapVectorsEqual(GenmapVector x, GenmapVector y,
                       GenmapScalar tol);
int GenmapCopyVector(GenmapVector x, GenmapVector y);
GenmapScalar GenmapDotVector(GenmapVector x, GenmapVector y);
GenmapScalar GenmapNormVector(GenmapVector x, GenmapInt p);

int GenmapPrintVector(GenmapVector x);
int GenmapDestroyVector(GenmapVector x);
// Functions to do Laplacian
int GenmapAxInit(GenmapHandle h, GenmapComm c, GenmapVector weights);

int GenmapAx(GenmapHandle h, GenmapComm c, GenmapVector u,
             GenmapVector weights, GenmapVector v);

int GenmapAxInit_exact(GenmapHandle h, GenmapComm c,
                       GenmapVector weights);

int GenmapAx_exact(GenmapHandle h, GenmapComm c, GenmapVector u,
                   GenmapVector weights, GenmapVector v);
void GenmapLanczos(GenmapHandle h, GenmapComm c, GenmapVector init,
                   GenmapInt iter, GenmapVector **q, GenmapVector alpha,
                   GenmapVector beta);
void GenmapRSB(GenmapHandle h);
//
// Linear solve
//
int GenmapSymTriDiagSolve(GenmapVector x, GenmapVector b,
                          GenmapVector alpha,
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
#endif
