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
  comm_init(&(*h)->c, MPI_COMM_WORLD);
}
//
// GenmapFinalize
//
int GenmapFinalize(GenmapHandle h) {
  comm_free(&h->c);
  // TODO Replace with free
  h->gsh = NULL;
//  gs_free(h->gsh);
  free(h);
  h = NULL;
#ifdef MPI
  MPI_Finalize();
#endif
}
//
// TODO Malloc and Free
//
int GenmapMallocArray();
int GenmapCallocArray();
int GenmapReallocArray();
int GenmapFree();
