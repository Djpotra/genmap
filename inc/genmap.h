#ifndef _GENMAP_H_
#define _GENMAP_H_
#endif

//------------------------------------------------------------------------------
// Genmap types
//
typedef int GenmapInt32;
typedef unsigned int GenmapUInt32;

typedef long GenmapInt64;
typedef unsigned long GenmapUInt64;

typedef double GenmapScalar;

typedef struct GenmapVector_private *GenmapVector;

//------------------------------------------------------------------------------
// GenmapVector operations
//
void CreateVector(GenmapVector x, GenmapInt32 size);

void DeleteVector(GenmapVector x);

GenmapInt32 VectorsEqual(GenmapVector x, GenmapVector y, GenmapScalar tol);

void RandomVector(GenmapVector x, GenmapInt32 size, GenmapInt32 seed);

void OnesVector(GenmapVector x, GenmapInt32 size);

void ZerosVector(GenmapVector x, GenmapInt32 size);

GenmapScalar NormVector(GenmapVector *x, GenmapInt32 p);

void AxpbyVector(GenmapVector z, GenmapVector x, GenmapScalar alpha, \
                 GenmapVector *y, GenmapScalar beta);

void ScaleVector(GenmapVector y, GenmapVector x, GenmapScalar alpha);

GenmapScalar DotVector(GenmapVector x, GenmapVector y);

void CopyVector(GenmapVector x, GenmapVector y);

void PrintVector(GenmapVector x);
//------------------------------------------------------------------------------
