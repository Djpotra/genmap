#include "mpiwrapper.h"
#include "io.h"

//------------------------------------------------------------------------------
int32 main(int32 argc, char** argv)
{
  struct comm global, partition;

  init_genmap(&global, argc, argv);

  char *name = "nbrhd/nbrhd.map.bin";
  int32 *header, *glo_num, *elem_id;
  double *weights = NULL;

  readmap(&global, &header, &glo_num, &elem_id, name);

#ifdef MPI
  int32 partitions = 2;
  int32 id = global.id;
  int32 partitionId = id/partitions;

  MPI_Comm MPI_COMM_PARTITION;
  MPI_Comm_split(MPI_COMM_WORLD, partitionId, id, &MPI_COMM_PARTITION);

  comm_init(&partition, MPI_COMM_PARTITION);
  struct gs_data *gsh;
//  ax_setup(&gsh, &weights, &partition, lpts, lelt, glo_num);

#endif

#ifdef DEBUG
  for (int32 i = 0; i < HEADER_SIZE; i++)
  {
    printf("%d ", header[i]);
  }
  printf("\n");

  int32 i = 0;
  while (i < header[NC]*header[MYCHUNK])
  {
    printf("%d ", glo_num[i]);
    i++;
    if (i%header[NC] == 0) printf("\n");
  }
#endif

  finalize_genmap(&global);


  return 0;
}
