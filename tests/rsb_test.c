#include <math.h>

#include "mpiwrapper.h"
#include "io.h"
#include "laplacian.h"
#include "lanczos.h"
#include "eig.h"

//------------------------------------------------------------------------------
int32 main(int32 argc, char** argv)
{
  struct comm global, partn;

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
  comm_init(&partn, MPI_COMM_PARTITION);
  struct gs_data *parth; gop_init(&parth, &partn);

  int32 nc = header[NC];
  int32 lelt = header[MYCHUNK];

  Vector init, ones, alpha, beta, *q;

  // Remove components of 1-vetor from random vector
  random_vector(&init, lelt, id);
  ones_vector(&ones, lelt);

  double lsum = dot_vector(&init, &ones);
  gop(&lsum, parth, gs_double, gs_add, 0);
  lsum /= header[NEL];

  z_axpby_vector(&init, &init, 1.0, &ones, -lsum);

  int32 iter = 10;
  zeros_vector(&alpha, iter);
  zeros_vector(&beta, iter - 1);

  lanczos(&alpha, &beta, &q, &partn, glo_num, &init, nc, lelt, iter);

  Vector eVector, init1;
  int32 n = alpha.size;

  random_vector(&init1, n, partitionId);
  create_vector(&eVector, n);
  invpower(&eVector, &alpha, &beta, &init1, iter);

#ifdef DEBUG
  printf("eVector = [");
  for (int32 i = 0; i < eVector.size; i++) {
    printf("%lf, ", eVector.vv[i]);
  }
  printf("]\n");
#endif

  Vector lnczs; create_vector(&lnczs, lelt);
  for (int32 i = 0; i < lelt; i++) {
    lnczs.vv[i] = 0.0;
    for (int32 j = 0; j < n; j++) {
      lnczs.vv[i] += q[j].vv[i]*eVector.vv[i];
    }
  }

  printf("lnczs, %d = [", global.id);
  for (int32 i = 0; i < lnczs.size; i++) {
    printf("%lf, ", lnczs.vv[i]);
  }
  printf("], %d\n", global.id);

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
