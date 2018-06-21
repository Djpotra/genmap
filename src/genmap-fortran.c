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
void fGenmapInit(int *handle, MPI_Fint *comm, int *err) {
  MPI_Comm ccomm = MPI_Comm_f2c(*comm);
#else
void fGenmapInit(int *handle, int *comm, int *err) {
  int ccomm = 1;
#endif
  if(GenmapHandleCount == GenmapHandleMax) {
    GenmapHandleMax += GenmapHandleMax / 2 + 1;
    GenmapRealloc(GenmapHandleMax, &GenmapHandleDict);
  }

  *err = GenmapInit(&GenmapHandleDict[GenmapHandleCount], ccomm,
                    "fortran", 0);
  if(*err == 0) {
    *handle = GenmapHandleCount++;
    GenmapHandleActive++;
  }
}

#define fGenmapSet FORTRAN_NAME(genmapset,GENMAPSET)
void fGenmapSet(int *handle, int *nelements, int *ndim, int *nvertices,
                int *nedges, int *vertices, int *err) {

  GenmapHandle h = GenmapHandleDict[*handle];
  *err = GenmapCreateHeader(&h->header);
  if(*err == 0) {
    h->header->nel      = *nelements;
    h->header->nactive  = 0;
    h->header->depth    = 0;
    h->header->d2       = 0;
    h->header->npts     = (*nvertices) * (*nelements);
    h->header->nrank    = 0;
    h->header->noutflow = 0;
    h->header->nv       = *nvertices;
    h->header->ne       = *nedges;
    h->header->ndim     = *ndim;
  }

  GenmapRead(h, vertices);
  *err = 0;
}

#define fGenmapFinalize FORTRAN_NAME(genmapfinalize,GENMAPFINALIZE)
void fGenmapFinalize(int *handle, int *err) {

  *err = GenmapFinalize(GenmapHandleDict[*handle]);
  if(*err == 0) {
    GenmapHandleActive--;
    if(GenmapHandleActive == 0) {
      GenmapFree(GenmapHandleDict);
      GenmapHandleCount = 0;
      GenmapHandleMax = 0;
    }
  }

  *err = 0;
}
