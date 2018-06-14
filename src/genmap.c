#define _POSIX_C_SOURCE 200112
#include <genmap-impl.h>
#include <genmap-default-reader.h>

#include <stdlib.h>
#include <stdarg.h>
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
  if(GenmapNumReaders >= sizeof(GenmapReaders) / sizeof(
        GenmapReaders[0])) {
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
    for(size_t j = 0; reader[j]
        && (reader[j] == registeredReader[j]); j++) {
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
  GenmapCreateComm(*h, &(*h)->local, ce);
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

  GenmapDestroyHeader(h, h->header);

  array_free(&(h->elementArray));

  GenmapFree(h);

  return 0;
}
//
// GenmapMalloc, Realloc, Calloc and Free
//
int GenmapMallocArray(size_t n, size_t unit, void *p) {
  int ierr = posix_memalign((void **)p, GENMAP_ALIGN, n*unit);
  if(ierr)
    printf("GenmapMallocArray Failed: %s:%d\n",__FILE__,__LINE__);
  return ierr;
}

int GenmapCallocArray(size_t n, size_t unit, void *p) {
  int ierr = 0;
  *(void **)p = calloc(n, unit);
  if(n && unit && !*(void **)p) {
    ierr = 1;
    printf("GenmapCallocArray Failed: %s:%d\n",__FILE__,__LINE__);
  }
  return ierr;
}

int GenmapReallocArray(size_t n, size_t unit, void *p) {
  int ierr=0;
  *(void **)p = realloc(*(void **)p, n*unit);
  if(n && unit && !*(void **)p) {
    ierr = 1;
    printf("GenmapReallocArray Failed: %s:%d\n",__FILE__,__LINE__);
  }
  return ierr;
}

int GenmapFree(void *p) {
  free(p);
  p = NULL;
  return 0;
}
