#include <genmap-impl.h>

// Urgh ugly
#include <default.h>

//TODO: Get rid of this
#include <stdio.h>
//
// Genmap Readers (FEM meshes, .map files, etc.)
//
static struct {
  char name[GENMAP_READER_LEN];
  int (*Create)(GenmapHandle h);
} GenmapReaders[GENMAP_MAX_READERS];

static size_t GenmapNumReaders = 0;
static size_t GenmapReadersRegistered = 0;

int GenmapRegisterReader(char *name, int (*Create)(GenmapHandle h)) {
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
// GenmapRegister
//
int GenmapRegister() {
  return GenmapRegisterReader("default", GenmapCreateHandle_default);
}
//
// GenmapInit
//
int GenmapInit(GenmapHandle *h, GenmapCommExternal ce, char *reader) {
  // TODO: Make this to use __attribute__((constructor))
  // Needs -fPIC in gslib :(
  if(!GenmapReadersRegistered) {
    GenmapRegister();
    GenmapReadersRegistered = 1;
  }

  char *registeredReader;
  size_t matchLen = 0, matchIdx = 0;

  for(size_t i = 0; i < GenmapNumReaders; i++) {
    registeredReader = GenmapReaders[i].name;
    for(size_t j = 0; reader[j] && (reader[j] == registeredReader[j]); j++) {
      if(j > matchLen) {
        matchLen = j;
        matchIdx = i;
      }
    }
  }

  if(!matchLen) {
    //TODO: GenmapError
    printf("Error: Reader not found.\n");
  }

  GenmapMalloc(1, h);
  (*h)->Create = GenmapReaders[matchIdx].Create;
  (*h)->Create(*h);

  GenmapCreateComm(*h, &(*h)->global, ce);
  GenmapCreateElements(*h, &(*h)->elements);
  GenmapCreateHeader(*h, &(*h)->header);

  return 0;
}
//
// GenmapFinalize
//
int GenmapFinalize(GenmapHandle h) {
  if(h->global)
    GenmapDestroyComm(h, h->global);
  if(h->local)
    GenmapDestroyComm(h, h->local);
  GenmapDestroyElements(h, h->elements);
  GenmapDestroyHeader(h, h->header);

  GenmapFree(h);

  return 0;
}
//
// TODO Malloc and Free
//
int GenmapMallocArray();
int GenmapCallocArray();
int GenmapReallocArray();

int GenmapFree(void *p) {
  free(p);
  p = NULL;
  return 0;
}
