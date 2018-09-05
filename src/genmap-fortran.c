#include <genmap.h>
#include <genmap-impl.h>
#include <genmap-fortran-name.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static GenmapHandle *GenmapHandleDict;
static int GenmapHandleCount = 0;
static int GenmapHandleActive = 0;
static int GenmapHandleMax = 0;

#define fGenmapInit FORTRAN_NAME(genmapinit,GENMAPINIT)
#ifdef GENMAP_MPI
void fGenmapInit(int *handle, MPI_Fint *comm, int *exactAx, int *err) {
  MPI_Comm ccomm = MPI_Comm_f2c(*comm);
#else
void fGenmapInit(int *handle, int *comm, int *exactAx, int *err) {
  int ccomm = 1;
#endif
  if(GenmapHandleCount == GenmapHandleMax) {
    GenmapHandleMax += GenmapHandleMax / 2 + 1;
    GenmapRealloc(GenmapHandleMax, &GenmapHandleDict);
  }

  *err = GenmapInit(&GenmapHandleDict[GenmapHandleCount], ccomm,
                    "fortran", *exactAx);
  if(*err == 0) {
    *handle = GenmapHandleCount++;
    GenmapHandleActive++;
  }

  GenmapHandle h = GenmapHandleDict[*handle];

  printf("%s:%d np:%d\n", __FILE__, __LINE__, h->Np(h->global));
  printf("%s:%d rank:%d\n", __FILE__, __LINE__, h->Id(h->global));
}

#define fGenmapSet FORTRAN_NAME(genmapset,GENMAPSET)
void fGenmapSet(int *handle, int *nelements, int *ndim, int *nvertices,
                int *nedges, int *vertices, int *err) {

#ifdef GENMAP_DEBUG
  for(int i = 0; i < *nelements; i++) {
    for(int j = 0; j < *nvertices; j++) {
//      printf("fvertices:%d\n",vertices[i*(*nvertices)+j]);
    }
  }
#endif

  GenmapHandle h = GenmapHandleDict[*handle];
  *err = GenmapCreateHeader(&h->header);
  h->header->lelt     = *nelements;
  h->header->npts     = (*nvertices) * (*nelements);
  h->header->nv       = *nvertices;
  h->header->ne       = *nedges;
  h->header->ndim     = *ndim;

#ifdef GENMAP_DEBUG
  printf("%s:%d nelements:%d\n", __FILE__, __LINE__, h->header->nel);
  printf("%s:%d ndim:%d\n", __FILE__, __LINE__, h->header->ndim);
  printf("%s:%d nvertices:%d\n", __FILE__, __LINE__, h->header->nv);
  printf("%s:%d nedges:%d\n", __FILE__, __LINE__, h->header->ne);
#endif

  GenmapRead(h, vertices);

#ifdef GENMAP_DEBUG
  GenmapElements elem = GenmapGetElements(h);
  for(int i = 0; i < h->header->nel; i++) {
    printf("%s:%d %d nel:%d\n", __FILE__, __LINE__, h->Id(h->global),
           elem[i].globalId);
    GenmapInt nv = h->header->nv;
    for(int j = 0; j < nv; j++) {
      printf("%d vertices:%d\n", h->Id(h->global), elem[i].vertices[j]);
    }
  }
#endif

  *err = 0;
}

#define fGenmapRunRSB FORTRAN_NAME(genmaprunrsb,GENMAPRUNRSB)
void fGenmapRunRSB(int *handle, int *nelem, int *elem, int *vertices,
                   int *ierr) {
  GenmapHandle h = GenmapHandleDict[*handle];
  GenmapRSB(h);

  GenmapElements elements = GenmapGetElements(h);
  GenmapInt nv = h->header->nv;

  for(int i = 0; i < h->header->lelt; i++) {
    elem[i] = elements[i].globalId;
    for(int j = 0; j < nv; j++) {
      vertices[nv * i + j] = elements[i].vertices[j];
    }
  }

  *nelem = h->header->lelt;
  *ierr = 0;
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
