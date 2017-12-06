#include "io.h"
#include "mpiwrapper.h"
//------------------------------------------------------------------------------

#ifdef MPI
void readmap_mpi(struct comm *c, int64 **header, int64 **glo_num,
                                        int64** element_id, char* name)
{
  MPI_File fh;
  MPI_Offset offset;
  MPI_Status st;

  MPI_File_open(MPI_COMM_WORLD, name, MPI_MODE_RDONLY, MPI_INFO_NULL, &fh);

  MPI_File_seek(fh, 0, MPI_SEEK_SET);
  MPI_File_get_size(fh, &offset);

  *header = malloc(sizeof(int64)*(MAP_HEADER_SIZE + 2));
  MPI_File_read(fh, *header, MAP_HEADER_SIZE, MPI_LONG, &st);

  // nc = npts/nel
  int64 nc  = (*header)[NPTS]/(*header)[NEL];
  int64 chunk_size =  (*header)[NEL]/ c->np;
  int64 start = c->id*chunk_size*(nc + 1);
  if (c->id == c->np - 1) chunk_size = (*header)[NEL] - c->id*chunk_size;

  *glo_num    = malloc(sizeof(int64)*chunk_size*nc);
  *element_id = malloc(sizeof(int64)*chunk_size   );

  MPI_File_seek(fh, (MAP_HEADER_SIZE + start)*sizeof(uint64), MPI_SEEK_SET);
  for (int64 i = 0; i < chunk_size; i++) {
    MPI_File_read(fh, *element_id + i,  1, MPI_LONG, &st);
    MPI_File_read(fh, *glo_num + i*nc, nc, MPI_LONG, &st);
  }
  (*header)[MYCHUNK] = chunk_size;
  (*header)[NC] = nc;

  MPI_File_close(&fh);
#ifdef DEBUG
  printf("MPI_SEEK_CUR: %ld\n", MPI_SEEK_CUR);
  printf("Size of file in parallel: %ld\n", offset);
  printf("Total number to be read: %ld\n", numbers);
  printf("My chunk_size: %ld\n", chunk_size);
  printf("My start: %ld\n", start);
  printf("MPI_SEEK_CUR: %ld\n", MPI_SEEK_CUR);
#endif
}
#endif
//------------------------------------------------------------------------------

void readmap_serial(int64 **header, int64 **glo_num, int64** element_id,
                                                                    char* name)
{
  FILE *fp;

  fp = fopen(name, "rb");
  if (fp == NULL) {
    printf("Unable to open the file.\n");
    exit(1);
  }

  *header = malloc(sizeof(int64)*MAP_HEADER_SIZE);

  // nel, nactive, depth, d2, npts, nrank, noutflow
  fread(*header, sizeof(int64), MAP_HEADER_SIZE, fp);

  // nc = npts/nel
  int64 nc  = (*header)[NPTS]/(*header)[NEL];

  *glo_num = malloc(sizeof(int64)*(*header)[NPTS]);
  *element_id = malloc(sizeof(int64)*(*header)[NEL]);

  int64 count = 0;
  for (int64 i = 0; i < (*header)[NEL]; i++) {
    fread(*element_id + i , sizeof(int64),  1, fp);
    fread(*glo_num + count, sizeof(int64), nc, fp);
    count += nc;
  }

  fclose(fp);
#ifdef DEBUG
  printf("Size of file in serial: %d\n", size);
  fseek(fp, 0, SEEK_END);
  int32 size = ftell(fp);
  fseek(fp, 0, SEEK_SET);
#endif
}
//------------------------------------------------------------------------------

void readmap(struct comm *c, int64 **header, int64 **glo_num,
                                        int64** element_id, char* name)
{
#ifdef MPI
  readmap_mpi   (c, header, glo_num, element_id, name);
#else
  readmap_serial(header, glo_num, element_id, name);
#endif
}
//------------------------------------------------------------------------------
