#include "test.h"
#include "linalg.h"
#include "lanczos.h"
#include "mpiwrapper.h"
#include "io.h"

//------------------------------------------------------------------------------
int32 main(int32 argc, char **argv) {
  // Initialize genmap
  struct comm c;
  init_genmap(&c, argc, argv);

  // Read the .map file
  struct element* elements; struct header mapheader;
  readmap(&c, &elements, &mapheader, "nbrhd/nbrhd.map.bin");

  // Element distribution after reading the .map file
  int32 rank = c.id;
  int32 lelt; lelt = mapheader.lelt;

  // Setup variables for lanczos
  Vector init;
  int32 iter = 100;
  create_vector(&init, lelt);
  for(int32 i = 0; i < lelt; i++) {
    init.vv[i] = i;
  }

  // Do lanczos
  Vector alpha, beta, *q; zeros_vector(&alpha, iter);
  zeros_vector(&beta, iter - 1);

  lanczos(&alpha, &beta, &q, &c, &mapheader, elements, &init, iter);

  // Print alpha and beta
  if(rank == 0) {
    printf("beta = [");
    for(int32 i = 0; i < beta.size; i++) {
      printf("%.17g, ", beta.vv[i]);
    }
    printf("]\n");
    printf("alpha= [");
    for(int32 i = 0; i < alpha.size; i++) {
      printf("%.17g, ", alpha.vv[i]);
    }
    printf("]\n");
  }

  // Free data structures
  delete_vector(&alpha); delete_vector(&beta); delete_vector(&init);

  finalize_genmap(&c);
  return 0;
}
//------------------------------------------------------------------------------
