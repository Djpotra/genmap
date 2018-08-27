#include <genmap-impl.h>
#include <genmap-io.h>

#define GENMAP_GMSH_HEX 5
#define GENMAP_GMSH_QUAD 3
//
// GenmapHandle: Create
//
int GenmapCreateHandle_gmsh(GenmapHandle h) {
  h->Read = GenmapRead_gmsh;

  return 0;
}
//
// Do File I/O in serial
//
int GenmapRead_gmsh(GenmapHandle h, void *data) {
  char *fileName = (char *) data;
  char *status;
  char buf[BUFSIZ];

  GenmapInt NglobalElements, NlocalElements;

  int rank, size;
  MPI_Comm_rank(h->global->gsComm.c, &rank);
  MPI_Comm_size(h->global->gsComm.c, &size);

  FILE *fp = fopen(fileName, "r");
  if(fp == NULL) {
    printf("GenmapRead_gmsh:%s:%d can't open file.\n", __FILE__, __LINE__);
  }

  do {
    status = fgets(buf, BUFSIZ, fp);
  } while(!strstr(buf, "$Elements"));

  status = fgets(buf, BUFSIZ, fp);
  sscanf(buf, "%d", &NglobalElements);

  struct array globalElements = null_array;
  array_reserve(struct GenmapElement_private, &globalElements,
                NglobalElements);
  globalElements.n = NglobalElements;
  GenmapElements elements = (GenmapElements)(globalElements.ptr);
  printf("NglobalElements = %d\n", NglobalElements);

  GenmapInt count = 0;

  for(GenmapInt i = 0; i < NglobalElements; i++) {
    status = fgets(buf, BUFSIZ, fp);
    GenmapInt elementId, elementType;
    GenmapInt v1, v2, v3, v4, v5, v6, v7, v8;

    sscanf(buf, "%d %d", &elementId, &elementType);
    if(elementType == GENMAP_GMSH_HEX) {
      sscanf(buf, "%*d %*d %*d %*d %*d %d %d %d %d %d %d %d %d",
             &v1, &v2, &v3, &v4, &v5, &v6, &v7, &v8);
      elements[count].globalId = elementId;
      elements[count].vertices[0] = v1;
      elements[count].vertices[1] = v2;
      elements[count].vertices[2] = v3;
      elements[count].vertices[3] = v4;
      elements[count].vertices[4] = v5;
      elements[count].vertices[5] = v6;
      elements[count].vertices[6] = v7;
      elements[count].vertices[7] = v8;
      count++;
    }
  }

  NlocalElements = count / size;
  int nrem = count - NlocalElements * size;
  if(rank < nrem) NlocalElements++;

  GenmapInt out[2][1], bufs[2][1];
  comm_scan(out, &(h->global->gsComm), gs_int, gs_add, &NlocalElements, 1,
            bufs);
  h->header->start = out[0][0];
  GenmapInt start = out[0][0];

  h->header->nel = count;
  h->header->ndim = 3;
  h->header->nv = 8;
  h->header->ne = 12;
  h->header->lelt = NlocalElements;

  array_reserve(struct GenmapElement_private, &(h->elementArray),
                NlocalElements);
  h->elementArray.n = NlocalElements;
  GenmapElements localElements = GenmapGetElements(h);
  for(GenmapInt i = start, count = 0; count < NlocalElements;
      i++, count++) {
    localElements[count] = elements[i];
  }

  fclose(fp);

  array_free(&globalElements);

  return 0;
}
