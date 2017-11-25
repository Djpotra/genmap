#include "io.h"
#include "mpiwrapper.h"

#include <stdio.h>
//------------------------------------------------------------------------------
#ifdef MPI
void readmap_mpi(long **header, long **glo_num, char* name)
{
  MPI_File fh;
  MPI_Offset offset;
  MPI_Status st;

  MPI_File_open(MPI_COMM_WORLD, name, MPI_MODE_RDONLY, MPI_INFO_NULL, &fh);

  MPI_File_seek(fh, 0, MPI_SEEK_SET);
  MPI_File_get_size(fh, &offset);
  printf("Size of file in parallel: %d\n", offset);

  *header = malloc(sizeof(long)*MAP_HEADER_SIZE);
  long *header_val = *header;
  MPI_File_read(fh, header_val, MAP_HEADER_SIZE, MPI_LONG, &st);

//  MPI_File_seek(fh, rank*chunk_size, fh);
//  MPI_File_read(fh, *header, MAP_HEADER_SIZE, MPI_LONG, &st);

  MPI_File_close(&fh);
}
#endif
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
#ifdef MPI
  readmap_mpi   (header, glo_num, name);
#else
  readmap_serial(header, glo_num, name);
#endif
}
//------------------------------------------------------------------------------
