#include <genmap-impl.h>
//
// GenmapHandle_private: Create, Destroy
//
int GenmapHandleInit_private(GenmapHandle *h) {
  return 0;
}

int GenmapDestroyHandle_private(GenmapHandle h) {
  return 0;
}
//
// GenmapInit
//
int GenmapInit(GenmapHandle *h, int argc, char **argv) {
#ifdef MPI
  MPI_Init(&argc, &argv);
#else
  int MPI_COMM_WORLD = 0;
#endif
  GenmapMalloc(1, h);
  GenmapMalloc(1, &(*h)->global);
  GenmapMalloc(1, &(*h)->local);
  GenmapMalloc(1, &(*h)->elements);
  GenmapMalloc(1, &(*h)->header);
  comm_init(&(*h)->global->gsComm, MPI_COMM_WORLD);
  (*h)->local = (*h)->global;
  (*h)->Id = GenmapId_private;
  (*h)->Np = GenmapNp_private;
  (*h)->Ax = GenmapAx_private;
  return 0;
}
//
// GenmapFinalize
//
int GenmapFinalize(GenmapHandle h) {
  comm_free(&h->global->gsComm);
  // TODO Replace with gsfree
  free(h->local);
  if(h->global != h->local)
    free(h->global);
  free(h->elements);
  free(h->header);
  free(h);
  h = NULL;
#ifdef MPI
  MPI_Finalize();
#endif

  return 0;
}
//
// TODO Malloc and Free
//
int GenmapMallocArray();
int GenmapCallocArray();
int GenmapReallocArray();
int GenmapFree();
