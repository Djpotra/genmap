#include "io.h"
#include "mpiwrapper.h"

#include <stdio.h>
#include <stdlib.h>
//------------------------------------------------------------------------------
long file_open(char *header, char *data, char* name, int mode)
{
#ifdef MPI
  MPI_File fh;
  MPI_Offset offset;

  MPI_File_open(MPI_COMM_WORLD, name, mode, MPI_INFO_NULL, &fh);

  int file_size = MPI_File_get_size(fh, &offset);
  printf("File size = %d\n", offset);

  MPI_File_seek(fh, 0, MPI_SEEK_SET);

  MPI_File_close(&fh);
#else
  printf("Serial !!!!\n");
#endif
  return 1;
}
//------------------------------------------------------------------------------
void readmap_mpi(int nid, long *npts, long *nel, long **glo_num,
            char* name)
{
}
//------------------------------------------------------------------------------
void readmap_serial(long **header, long **glo_num, char* name)
{
  FILE *fp;
  long nc, jnk;

  fp = fopen(name, "rb");
  if (fp == NULL) {
    printf("Unable to open the file.\n");
    exit(1);
  }

  fseek(fp, 0, SEEK_END);
  int size = ftell(fp);
  fseek(fp, 0, SEEK_SET);

  *header = malloc(sizeof(long)*MAP_HEADER_SIZE);
  long *header_val = *header;

  // nel, nactive, depth, d2, npts, nrank, noutflow
  fread(header_val, sizeof(long), MAP_HEADER_SIZE, fp);

  printf("Size of file in serial: %d\n", size);

  // nc = npts/nel
  nc  = header_val[NPTS]/header_val[NEL];

  *glo_num = malloc(sizeof(long)*header_val[NPTS]);

  long count = 0;
  for (long i = 0; i < header_val[NEL]; i++) {
    fread(&jnk, sizeof(long), 1, fp);
    fread(*glo_num + count, sizeof(long), nc, fp);
    count += nc;
  }

  fclose(fp);
}
//------------------------------------------------------------------------------
void readmap(long **header, long **glo_num, char* name)
{
  int nid = 0;
  long npts, nelt;
#ifdef MPI
  readmap_mpi(nid, npts, nelt, glo_num, name);
#else
  readmap_serial(header, glo_num, name);
#endif
}
//------------------------------------------------------------------------------
