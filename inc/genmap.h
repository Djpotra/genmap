#ifndef _GENMAP_H_
#define _GENMAP_H_

#include <gslib.h>

//------------------------------------------------------------------------------
// Genmap types
//
typedef int GenmapInt32;
typedef unsigned int GenmapUInt32;
typedef long GenmapInt64;
typedef unsigned long GenmapUInt64;

typedef GenmapInt32 GenmapInt;

typedef double GenmapScalar;

typedef struct GenmapComm_private *GenmapComm;

typedef struct GenmapHandle_private *GenmapHandle;

typedef struct GenmapVector_private *GenmapVector;

typedef struct GenmapElement_private *GenmapElement;

typedef struct GenmapHeader_private *GenmapHeader;
//
// Genmap tolerances
//
#define GENMAP_TOL 1e-12
//
// Genmap Init, Finalize, etc.
//
int GenmapInit(GenmapHandle *h, int argc, char **argv);
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
                      GenmapVector *y, GenmapScalar beta);

int GenmapVectorsEqual(GenmapVector x, GenmapVector y, GenmapScalar tol);
int GenmapCopyVector(GenmapVector x, GenmapVector y);
GenmapScalar GenmapDotVector(GenmapVector x, GenmapVector y);
GenmapScalar GenmapNormVector(GenmapVector x, GenmapInt p);

int GenmapPrintVector(GenmapVector x);
int GenmapDestroyVector(GenmapVector x);
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

int GenmapInvPowerIter(GenmapVector eVector, GenmapVector alpha,
                       GenmapVector beta, GenmapVector init, GenmapInt iter);
//
// Genmap I/O
//
int GenmapRead(GenmapHandle h, GenmapElement *elements, GenmapHeader mapheader,
               char *name);
//
// Compute Laplacain times vector
//
#endif
