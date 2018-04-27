#include <math.h>

#include "mpiwrapper.h"
#include "io.h"
#include "laplacian.h"
#include "lanczos.h"
#include "eig.h"

//------------------------------------------------------------------------------
int comp_element(const void *a, const void *b) {
  struct element aae = *((struct element *) a);
  struct element bbe = *((struct element *) b);

  double aa = aae.fiedler;
  double bb = bbe.fiedler;

  return aa > bb ? 1 : ((aa < bb) ? -1 : 0);
}

//------------------------------------------------------------------------------
void scatter_by_max(struct element *elements, int32 lelt, struct comm *c) {

  int32 id, np; np = c->np; id = c->id;
  MPI_Comm global = c->c;

  // ElementType
  MPI_Datatype ElementType;

  int32 blocklen[2] = {1, 10};
  MPI_Aint offset[2], extent;
  offset[0] = 0; MPI_Type_extent(MPI_DOUBLE, &extent); offset[1] = extent;
  MPI_Datatype types[2] = {MPI_DOUBLE, MPI_INT};

  MPI_Type_create_struct(2, blocklen, offset, types, &ElementType);
  MPI_Type_commit(&ElementType);

  struct element *recv_data = NULL;
  struct element e = {elements[lelt - 1].fiedler, lelt, 4, {0}};
  if(id == 0) {
    recv_data = malloc(sizeof(struct element) * np);
  }

  MPI_Gather(&e, 1, ElementType, recv_data, 1, ElementType, 0, global);

  if(id == 0) {
    for(int32 i = 0; i < np; i++) {
      printf("(%lf, %d) ", recv_data[i].fiedler, recv_data[i].globalId);
    }
    printf("\n");
  }
}

//------------------------------------------------------------------------------
void parallel_sort(struct element *local, int32 lelt, struct comm *c) {
  int32 id, np; np = c->np; id = c->id;
  int32 partner, left, right;
  int32 llelt, rlelt, recvlelt;

  left = id - 1; llelt = 0;
  right = id + 1; rlelt = 0;
  if(id % 2 == 0) {
    if(left >= 0)
      comm_send(c, &lelt, sizeof(int32), left, 0);
    if(right < np)
      comm_send(c, &lelt, sizeof(int32), right, 0);
    if(left >= 0)
      comm_recv(c, &llelt, sizeof(int32), left, 0);
    if(right < np)
      comm_recv(c, &rlelt, sizeof(int32), right, 0);
  } else {
    if(right < np)
      comm_recv(c, &rlelt, sizeof(int32), right, 0);
    if(left >= 0)
      comm_recv(c, &llelt, sizeof(int32), left, 0);
    if(right < np)
      comm_send(c, &lelt, sizeof(int32), right, 0);
    if(left >= 0)
      comm_send(c, &lelt, sizeof(int32), left, 0);
  }

  int32 maxlelt = llelt > rlelt ? llelt : rlelt;

  struct element *other = malloc(sizeof(struct element) * maxlelt);

  for(int32 i = 0; i < np; i++) {
    qsort(local, lelt, sizeof(struct element), comp_element);

    // Even phase
    if(i % 2 == 0) {
      if(id % 2 == 0) {
        partner = id + 1;
        recvlelt = rlelt;
      } else {
        partner = id - 1;
        recvlelt = llelt;
      }
    } else {
      if(id % 2 == 0) {
        partner = id - 1;
        recvlelt = llelt;
      } else {
        partner = id + 1;
        recvlelt = rlelt;
      }
    }

    // Move on if partner is not valid
    if(partner < 0 || partner > np - 1) {
      continue;
    }

    if(id % 2 == 0) {
      comm_send(c, local, sizeof(struct element)*lelt, partner, 1);
      comm_recv(c, other, sizeof(struct element)*recvlelt, partner, 2);
    } else {
      comm_recv(c, other, sizeof(struct element)*recvlelt, partner, 1);
      comm_send(c, local, sizeof(struct element)*lelt, partner, 2);
    }

    struct element tmp;

    if(id < partner) {  // keep smaller part
      int32 locali = lelt - 1;
      int32 otheri = 0;

      while(locali > -1 && otheri < recvlelt) {
        if(local[locali].fiedler < other[otheri].fiedler)
          break;
        else {
          tmp = local[locali];
          local[locali] = other[otheri];
          other[otheri] = tmp;
          locali--; otheri++;
        }
      }
    } else { // keep larger part
      int32 locali = 0;
      int32 otheri = recvlelt - 1;

      while(locali < lelt  && otheri > -1) {
        if(local[locali].fiedler > other[otheri].fiedler)
          break;
        else {
          tmp = local[locali];
          local[locali] = other[otheri];
          other[otheri] = tmp;
          locali++; otheri--;
        }
      }
    }
  }
}

//------------------------------------------------------------------------------
int32 main(int32 argc, char** argv) {
  // Global and local communicators
  struct comm global, partn;
  struct gs_data *partn_h, *global_h;

  init_genmap(&global, argc, argv); gop_init(&global_h, &global);

  char *name = "nbrhd/nbrhd.map.bin";
  struct element *elements; struct header mapheader;
  readmap(&global, &elements, &mapheader, name);

#ifdef MPI
  MPI_Comm mpi_partn, mpi_global;
  MPI_Comm_dup(MPI_COMM_WORLD, &mpi_global);
#endif

  // read NC value and number of elements local to the processor
  int32 lelt = mapheader.lelt;

  // Data structures needed to find Fiedler vector
  Vector fiedler; create_vector(&fiedler, lelt);

  int32 exsum, buf;
  // Set number of partitions
  int32 partitions = 2;
  // Set global id
  int32 global_id = global.id;
  // Find the partition size
  int32 partition_size = (int32) ceil(global.np / (1.0 * partitions));
  // Find the partition id
  int32 partn_id = global_id / partition_size;

  for(int32 i = 0; i < 2; i++) {
    MPI_Comm_split(mpi_global, partn_id, global_id, &mpi_partn);
    comm_init(&partn, mpi_partn); gop_init(&partn_h, &partn);

    // Vectors required to do Lanczos
    Vector init, ones, alpha, beta, *q;

    // Remove components of 1-vetor from random vector
    random_vector(&init, lelt, global_id);
    ones_vector(&ones, lelt);

    double partn_sum = dot_vector(&init, &ones);
    gop(&partn_sum, partn_h, gs_double, gs_add, 0);
    int32  partn_nel = lelt;
    gop(&partn_nel, partn_h, gs_int, gs_add, 0);

    partn_sum /= sqrt(partn_nel);
    z_axpby_vector(&init, &init, 1.0, &ones, -partn_sum);

    // Run lanczos in the partition
    int32 iter = 10;
    zeros_vector(&alpha, iter);
    zeros_vector(&beta, iter - 1);

    lanczos(&alpha, &beta, &q, &partn, &mapheader, elements, &init, iter);
#if 0
#ifdef DEBUG
    printf("alpha = [");
    for(int32 i = 0; i < iter; i++) {
      printf("%lf, ", alpha.vv[i]);
    }
    printf("]\n");
#endif
#endif

    // Run inverse power iteration in each processor in the partition
    int32 n = alpha.size;
    Vector eVector, init1;
    random_vector(&init1, n, global_id);
    create_vector(&eVector, n);

    invpower(&eVector, &alpha, &beta, &init1, iter);

    // find the local fiedler vector
    double partn_max = 0.0;
    for(int32 i = 0; i < lelt; i++) {
      fiedler.vv[i] = 0.0;
      for(int32 j = 0; j < n; j++) {
        fiedler.vv[i] += q[j].vv[i] * eVector.vv[j];
      }
      fiedler.vv[i] = ffabs(fiedler.vv[i]);
      if(partn_max < fiedler.vv[i]) {
        partn_max = fiedler.vv[i];
      }
    }
    gop(&partn_max, partn_h, gs_double, gs_max, 0);
    for(int32 i = 0; i < lelt; i++) {
      fiedler.vv[i] = fiedler.vv[i] / partn_max;
    }

    // find the median of the global fiedler vector in parallel
    for(int32 i = 0; i < lelt; i++) {
      elements[i].fiedler = fiedler.vv[i];
    }

    parallel_sort(elements, lelt, &global);

    comm_scan(&exsum, &partn, gs_int, gs_add, &lelt, 1, &buf);

    int32 medianPos = (partn_nel + 1) / 2;
    if(exsum + lelt < medianPos) {
      partn_id = 0;
    } else {
      partn_id = 1;
    }

    MPI_Comm_dup(mpi_partn, &mpi_global);
    comm_init(&global, mpi_global); gop_init(&global_h, &global);
    global_id = global.id;
  }

#ifdef DEBUG
  printf("%d: %d\n", global_id, exsum);

  for(int32 i = 0; i < 1; i++) {
    printf("%d ", mapheader.nel);
    printf("%d ", mapheader.nactive);
    printf("%d ", mapheader.depth);
    printf("%d ", mapheader.d2);
    printf("%d ", mapheader.npts);
    printf("%d ", mapheader.nrank);
    printf("%d ", mapheader.noutflow);
    printf("%d ", mapheader.nc);
    printf("%d\n", mapheader.lelt);
  }
  printf("\n");

  int32 i = 0;
  while(i < mapheader.lelt) {
    printf("%d ", elements[i].globalId);
    i++;
  }

  printf("fiedler: %d = [", global.id);
  for(int32 i = 0; i < lelt; i++) {
    printf("%lf, ", fiedler.vv[i]);
  }
  printf("], %d\n", global.id);

  printf("sorted_fiedler: %d = [", global.id);
  for(int32 i = 0; i < lelt; i++) {
    printf("(%lf, %d), ", elements[i].fiedler, elements[i].globalId);
  }
  printf("], %d\n", global.id);

#endif

  finalize_genmap(&global);

  return 0;
}
