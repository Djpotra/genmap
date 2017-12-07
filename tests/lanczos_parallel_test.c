#include "test.h"
#include "linalg.h"
#include "lanczos.h"
#include "mpiwrapper.h"
#include "io.h"

//------------------------------------------------------------------------------
int32 main(int32 argc, char **argv) {
  int32 *glo_num, *header, *elem_id;
  int32 nc, lelt;
  Vector init, alpha, beta;
  struct comm c;

  // Initialize genmap
  init_genmap(&c, argc, argv);

  // Read the .map file
  readmap(&c, &header, &glo_num, &elem_id, "nbrhd/nbrhd.map.bin");

  // Element distribution after reading the .map file
  int32 rank = c.id;

  nc = header[NC];
  lelt = header[MYCHUNK];

  // Setup variables for lanczos
  int32 iter = 8;
  ones_vector (&init , lelt    );
  zeros_vector(&alpha, iter    );
  zeros_vector(&beta , iter - 1);

  // Do lanczos
  lanczos(&alpha, &beta, &c, glo_num, &init, nc, lelt, iter);
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

    Vector d, e;
    zeros_vector(&d, iter); zeros_vector(&e, iter);
    copy_vector(&d, &alpha);
    for (int32 i = 0; i < iter - 1; i++) {
      e.vv[i] = beta.vv[i];
    }
  }

  // Free data structures
  delete_vector(&alpha); delete_vector(&beta); delete_vector(&init);
  free(glo_num); free(elem_id); free(header);

  finalize_genmap(&c);
  return 0;
}
//------------------------------------------------------------------------------
