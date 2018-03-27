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
  comm_init(&(*h)->globalComm, MPI_COMM_WORLD);

  return 0;
}
//
// GenmapFinalize
//
int GenmapFinalize(GenmapHandle h) {
  comm_free(&h->globalComm);
  // TODO Replace with gsfree
  h->globalHandle = NULL;
  //gs_free(h->gsh);
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
