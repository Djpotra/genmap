#include <genmap-impl.h>
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
  (*h)->local = (*h)->global;
  comm_init(&(*h)->global->gsComm, MPI_COMM_WORLD);
  (*h)->Id = GenmapId_private;
  (*h)->Np = GenmapNp_private;

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
