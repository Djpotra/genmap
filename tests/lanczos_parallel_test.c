#include "test.h"
#include "linalg.h"
#include "lanczos.h"
#include "mpiwrapper.h"
#include "io.h"

//------------------------------------------------------------------------------
int32 main(int32 argc, char **argv) {
  int32 *glo_num, *header, *elem_id;
  int32 nc, lelt, nel;
  Vector init, alpha, beta, *q;
  struct comm c;

  // Initialize genmap
  init_genmap(&c, argc, argv);

  // Read the .map file
  readmap(&c, &header, &glo_num, &elem_id, "nbrhd/nbrhd.map.bin");

  // Element distribution after reading the .map file
  int32 rank = c.id;

  nc = header[NC];
  lelt = header[LELT];
  nel = header[NEL];

  // Setup variables for lanczos
  int32 iter = 100;
  create_vector (&init, lelt);
  for (int32 i = 0; i < lelt; i++) {
    init.vv[i] = i;
  }

  zeros_vector(&alpha, iter);
  zeros_vector(&beta, iter - 1);

  // Do lanczos
  lanczos(&alpha, &beta, &q, &c, glo_num, &init, nc, nel, lelt, iter);

  // Print alpha and beta
  if (rank == 0) {
    printf("beta = [");
    for (int32 i = 0; i < beta.size; i++) {
      printf("%.17g, ", beta.vv[i]);
    }
    printf("]\n");
    printf("alpha= [");
    for (int32 i = 0; i < alpha.size; i++) {
      printf("%.17g, ", alpha.vv[i]);
    }
    printf("]\n");
  }

  // Free data structures
  delete_vector(&alpha); delete_vector(&beta); delete_vector(&init);
  free(glo_num); free(elem_id); free(header);

  finalize_genmap(&c);
  return 0;
}
//------------------------------------------------------------------------------
