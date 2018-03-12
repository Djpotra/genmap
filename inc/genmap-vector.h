#ifndef _GENMAP_VECTOR_H_
#define _GENMAP_VECTOR_H_

#include <genmap.h>

//------------------------------------------------------------------------------
// Genmap_Vector
//
struct GenmapVector_private {
  GenmapInt32 size;
  GenmapScalar *vv;
};

//------------------------------------------------------------------------------
// Vector operations
//
void CreateVector(GenmapVector x, int32 size);

void DeleteVector(GenmapVector x);

int32 VectorsEqual(GenmapVector x, GenmapVector y, double tol);

void RandomVector(GenmapVector x, int32 size, int32 seed);

void OnesVector(GenmapVector x, int32 size);

void ZerosVector(GenmapVector x, int32 size);

double norm_vector(Vector *x, int32 p);

void AxpbyVector(GenmapVector z, GenmapVector x, double alpha, \
                      Vector *y, double beta);

void ScaleVector(GenmapVector y, GenmapVector x,  double alpha);

double DotVector(GenmapVector x, GenmapVector y);

void CopyVector(GenmapVector x, GenmapVector y);

void PrintVector(GenmapVector x);

//TODO: get rid of this !!
void   mult_scalar_add_vector(Vector *y, double alpha, Vector *x, \
                              double beta);
//------------------------------------------------------------------------------
// Linear solve

void SymTridiagSolve(GenmapVector x, GenmapVector b, GenmapVector alpha, Vector beta);
//------------------------------------------------------------------------------

#endif
