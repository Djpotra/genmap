#include "mpiwrapper.h"
#include "io.h"
#include "laplacian.h"
#include "lanczos.h"

//------------------------------------------------------------------------------
int32 main(int32 argc, char** argv)
{
  struct comm global, partition;

  init_genmap(&global, argc, argv);

  char *name = "nbrhd/nbrhd.map.bin";
  int32 *header, *glo_num, *elem_id;

  readmap(&global, &header, &glo_num, &elem_id, name);

#ifdef MPI
  int32 partitions = 2;
  int32 id = global.id;
  int32 partitionId = id/partitions;

  MPI_Comm MPI_COMM_PARTITION;
  MPI_Comm_split(MPI_COMM_WORLD, partitionId, id, &MPI_COMM_PARTITION);
  comm_init(&partition, MPI_COMM_PARTITION);

  struct gs_data *gsh;
  double *weights = NULL;
  int32 nc = header[NC];
  int32 lelt = header[MYCHUNK];
  int32 lpts = nc*lelt;

  ax_setup(&gsh, &weights, &partition, lpts, lelt, glo_num);

  Vector init, alpha, beta;
  random_vector(&init, lelt, id);

  int32 iter = 10;
  zeros_vector(&alpha, iter);
  zeros_vector(&beta, iter - 1 );

//  lanczos(&alpha, &beta, gsh, weights, nc, &init, iter);

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
