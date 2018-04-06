#include <genmap-impl.h>

//TODO: Get rid of this
#include <stdio.h>
//
// Genmap Readers (FEM meshes, .map files, etc.)
//
static struct {
  char name[GENMAP_READER_LEN];
  int (*Create)(GenmapHandle *h);
} GenmapReaders[GENMAP_MAX_READERS];

static size_t GenmapNumReaders = 0;

int GenmapRegisterReader(char *name, int (*Create)(GenmapHandle h)) {
  printf("Thilina0 is here");
  if(GenmapNumReaders >= sizeof(GenmapReaders) / sizeof(GenmapReaders[0])) {
    //TODO: GenmapError
    printf("Error: Too many readers.\n");
  }

  strncpy(GenmapReaders[GenmapNumReaders].name, name, GENMAP_READER_LEN);
  GenmapReaders[GenmapNumReaders].Create = Create;
  GenmapNumReaders++;

  return 0;
}
//
// GenmapInit
//
int GenmapInit(GenmapHandle *h, GenmapCommExternal ce, char *reader) {
  char *registeredReader;
  int matchLen = 0, matchIdx;

  for(int i = 0; i < GenmapNumReaders; i++) {
    printf("Thilina0 %d\n", i);
    registeredReader = GenmapReaders[i].name;
    for(int j = 0; reader[j] && (reader[j] == registeredReader[j]); j++) {
      printf("Thilina1 %d\n", i);
      if(j > matchLen) {
        matchLen = j;
        matchIdx = i;
      }
    }
  }

  if (!matchLen) {
    //TODO: GenmapError
    printf("MatchLen = %d\n", matchLen);
    printf("Error: Reader not found.\n");
  }

  GenmapCreateHandle(h);

  GenmapCreateComm(&(*h)->global, ce);

  (*h)->local = NULL;

  GenmapCreateElements(&(*h)->elements);
  GenmapCreateHeader(&(*h)->header);

  return 0;
}
//
// GenmapFinalize
//
int GenmapFinalize(GenmapHandle h) {
  if(h->global)
    GenmapDestroyComm(h->global);
  if(h->local)
    GenmapDestroyComm(h->local);
  GenmapDestroyElements(h->elements);
  GenmapDestroyHeader(h->header);
  GenmapDestroyHandle(h);

  return 0;
}
//
// TODO Malloc and Free
//
int GenmapMallocArray();
int GenmapCallocArray();
int GenmapReallocArray();
int GenmapFree();
