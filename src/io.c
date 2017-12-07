#include "io.h"
#include "mpiwrapper.h"
//------------------------------------------------------------------------------

#ifdef MPI
void readmap_mpi(struct comm *c, int32 **header, int32 **glo_num,
                                        int32** element_id, char* name)
{
  MPI_File fh;
  MPI_Offset offset;
  MPI_Status st;

  MPI_File_open(MPI_COMM_WORLD, name, MPI_MODE_RDONLY, MPI_INFO_NULL, &fh);

  MPI_File_seek(fh, 0, MPI_SEEK_SET);
  MPI_File_get_size(fh, &offset);

  *header = malloc(sizeof(int32)*(MAP_HEADER_SIZE + 2));
  MPI_File_read(fh, *header, MAP_HEADER_SIZE, MPI_LONG, &st);

  // nc = npts/nel
  int32 nc  = (*header)[NPTS]/(*header)[NEL];
  int32 chunk_size =  (*header)[NEL]/ c->np;
  int32 start = c->id*chunk_size*(nc + 1);
  if (c->id == c->np - 1) chunk_size = (*header)[NEL] - c->id*chunk_size;

  *glo_num    = malloc(sizeof(int32)*chunk_size*nc);
  *element_id = malloc(sizeof(int32)*chunk_size   );

  MPI_File_seek(fh, (MAP_HEADER_SIZE + start)*sizeof(uint32), MPI_SEEK_SET);
  for (int32 i = 0; i < chunk_size; i++) {
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

void readmap_serial(int32 **header, int32 **glo_num, int32** element_id,
                                                                    char* name)
{
  int32 result;

  FILE *fp;

  fp = fopen(name, "rb");
  if (fp == NULL) {
    printf("Unable to open the file.\n");
    exit(1);
  }

  *header = malloc(sizeof(int32)*MAP_HEADER_SIZE);

  // nel, nactive, depth, d2, npts, nrank, noutflow
  result = fread(*header, sizeof(int32), MAP_HEADER_SIZE, fp);

  // nc = npts/nel
  int32 nc  = (*header)[NPTS]/(*header)[NEL];

  *glo_num = malloc(sizeof(int32)*(*header)[NPTS]);
  *element_id = malloc(sizeof(int32)*(*header)[NEL]);

  int32 count = 0;
  for (int32 i = 0; i < (*header)[NEL]; i++) {
    result += fread(*element_id + i , sizeof(int32),  1, fp);
    result += fread(*glo_num + count, sizeof(int32), nc, fp);
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

void readmap(struct comm *c, int32 **header, int32 **glo_num,
                                        int32** element_id, char* name)
{
#ifdef MPI
  readmap_mpi   (c, header, glo_num, element_id, name);
#else
  readmap_serial(header, glo_num, element_id, name);
#endif
}
//------------------------------------------------------------------------------
