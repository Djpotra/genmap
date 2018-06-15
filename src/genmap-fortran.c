#include <genmap.h>
#include <genmap-impl.h>
#include <genmap-fortran-name.h>

#include <stdlib.h>
#include <string.h>

static GenmapHandle *GenmapHandleDict;
static int GenmapHandleCount = 0;
static int GenmapHandleActive = 0;
static int GenmapHandleMax = 0;

#define fGenmapInit FORTRAN_NAME(genmapinit,GENMAPINIT)
#ifdef MPI
void fGenmapInit(int *handle, MPI_Fint *comm, int *nelements,
                 int *nvertices, int *facedata, int *traversal, int *err) {
  MPI_Comm ccomm = MPI_Comm_f2c(*comm);
#else
void fGenmapInit(int *handle, int *comm, int *nelements, int *nvertices,
                 int *facedata, int *traversal, int *err) {
  int ccomm = 1;
#endif
  if(GenmapHandleCount == GenmapHandleMax) {
    GenmapHandleMax += GenmapHandleMax / 2 + 1;
    GenmapRealloc(GenmapHandleMax, &GenmapHandleDict);
  }

  *err = GenmapInit(&GenmapHandleDict[GenmapHandleCount], ccomm,
                    "default");
  if(*err == 0) {
    *handle = GenmapHandleCount++;
    GenmapHandleActive++;
  }
}
