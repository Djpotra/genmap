#include "io.h"
#include "mpiwrapper.h"

//------------------------------------------------------------------------------
#ifdef MPI
void readmap_mpi(struct comm *c, long **header, long **glo_num, char* name)
{
  MPI_File fh;
  MPI_Offset offset;
  MPI_Status st;

  MPI_File_open(MPI_COMM_WORLD, name, MPI_MODE_RDONLY, MPI_INFO_NULL, &fh);

  MPI_File_seek(fh, 0, MPI_SEEK_SET);
  MPI_File_get_size(fh, &offset);
#ifdef DEBUG
  printf("MPI_SEEK_CUR: %ld\n", MPI_SEEK_CUR);
  printf("Size of file in parallel: %ld\n", offset);
#endif

  *header = malloc(sizeof(long)*(MAP_HEADER_SIZE + 1));
  MPI_File_read(fh, *header, MAP_HEADER_SIZE, MPI_LONG, &st);

  long numbers = offset / sizeof(long); numbers -= MAP_HEADER_SIZE;
  long chunk_size = numbers / c->np;
  long start = c->id*chunk_size;
  if (c->id == c->np - 1) chunk_size = numbers - start;
#ifdef DEBUG
  printf("Total number to be read: %ld\n", numbers);
  printf("My chunk_size: %ld\n", chunk_size);
  printf("My start: %lld\n", start);
#endif

  *glo_num = malloc(sizeof(long)*chunk_size);
#ifdef DEBUG
  printf("MPI_SEEK_CUR: %ld\n", MPI_SEEK_CUR);
#endif
  MPI_File_seek(fh, (MAP_HEADER_SIZE + start)*sizeof(long), MPI_SEEK_SET);
  MPI_File_read(fh, *glo_num, chunk_size, MPI_LONG, &st);

  (*header)[MYCHUNK] = chunk_size;

  MPI_File_close(&fh);
}
#endif
//------------------------------------------------------------------------------
void readmap_serial(struct comm *c, long **header, long **glo_num, char* name)
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
  jnk = fread(header_val, sizeof(long), MAP_HEADER_SIZE, fp);
#ifdef DEBUG
  printf("Size of file in serial: %d\n", size);
#endif

  // nc = npts/nel
  nc  = header_val[NPTS]/header_val[NEL];

  *glo_num = malloc(sizeof(long)*header_val[NPTS]);

  long count = 0;
  for (long i = 0; i < header_val[NEL]; i++) {
    jnk = fread(&jnk, sizeof(long), 1, fp);
    jnk = fread(*glo_num + count, sizeof(long), nc, fp);
    count += nc;
  }

  fclose(fp);
}
//------------------------------------------------------------------------------
void readmap(struct comm *c, long **header, long **glo_num, char* name)
{
#ifdef MPI
  readmap_mpi   (c, header, glo_num, name);
#else
  readmap_serial(c, header, glo_num, name);
#endif
}
//------------------------------------------------------------------------------
