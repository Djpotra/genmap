#include <genmap-impl.h>

int GenmapCreateComm_default(GenmapComm *c, GenmapCommExternal ce) {
  GenmapMalloc(1, c);
  comm_init(&(*c)->gsComm, ce);
  (*c)->verticesHandle = NULL;
  (*c)->edgesHandle = NULL;
  (*c)->laplacianWeights = NULL;

  return 0;
}

int GenmapDestroyComm_default(GenmapComm c) {
  comm_free(&c->gsComm);

  if(c->verticesHandle)
    gs_free(c->verticesHandle);
  if(c->verticesHandle)
    gs_free(c->edgesHandle);
  if(c->laplacianWeights)
    free(c->laplacianWeights);
  free(c);
  c = NULL;

  return 0;
}

int GenmapNp_default(GenmapComm c) {
  return c->gsComm.np;
}

int GenmapId_default(GenmapComm c) {
  return c->gsComm.id;
}

int GenmapAx_default(GenmapHandle h, GenmapComm c, GenmapVector u,
                     GenmapVector weights, GenmapVector v) {
  assert(u->size == v->size);

  GenmapInt lelt = u->size;
  GenmapInt nc = h->header->nc;

  GenmapScalar *ucv;
  GenmapMalloc(nc * lelt, &ucv);

  for(GenmapInt i = 0; i < lelt; i++)
    for(GenmapInt j = 0; j < nc; j++)
      ucv[nc * i + j] = u->data[i];

  gs(ucv, gs_double, gs_add, 0, c->verticesHandle, NULL);

  for(GenmapInt i = 0; i < lelt; i++) {
    v->data[i] = weights->data[i] * u->data[i];
    for(GenmapInt j = 0; j < nc; j ++) {
      v->data[i] += ucv[nc * i + j];
    }
  }

  for(GenmapInt i = 0; i < lelt; i++)
    for(GenmapInt j = 0; j < nc; j++)
      ucv[nc * i + j] = u->data[i];

  gs(ucv, gs_double, gs_add, 0, c->edgesHandle, NULL);

  for(GenmapInt i = 0; i < lelt; i++) {
    for(GenmapInt j = 0; j < nc; j ++) {
      v->data[i] -= ucv[nc * i + j];
    }
  }

  free(ucv);

  return 0;
}

int GenmapAxInit_default(GenmapHandle h, GenmapComm c, GenmapVector weights) {
  GenmapInt lelt = h->header->lelt;
  GenmapInt nc = h->header->nc;
  GenmapInt numPoints = nc * lelt;

  c->verticesHandle = gs_setup(h->elements->vertices, numPoints, &c->gsComm, 0,
		         gs_auto, 0);
  c->edgesHandle = gs_setup(h->elements->edges, numPoints, &c->gsComm, 0,
		         gs_auto, 0);

  GenmapScalar *u;
  GenmapMalloc(numPoints, &u);

  for(GenmapInt i = 0; i < lelt; i++)
    for(GenmapInt j = 0; j < nc; j++)
      u[nc * i + j] = 1.;

  gs(u, gs_double, gs_add, 0, c->verticesHandle, NULL);

  assert(weights->size == lelt);

  for(GenmapInt i = 0; i < lelt; i++) {
    weights->data[i] = 0.;
    for(GenmapInt j = 0; j < nc; j++) {
      weights->data[i] += u[nc * i + j];
    }
  }

#ifdef DEBUG
  printf("proc: %d 1-weights: ", h->Id(h->global));
  GenmapPrintVector(weights);
  printf("\n");
#endif

  for(GenmapInt i = 0; i < lelt; i++)
    for(GenmapInt j = 0; j < nc; j++)
      u[nc * i + j] = 1.;

  gs(u, gs_double, gs_add, 0, c->edgesHandle, NULL);

  for(GenmapInt i = 0; i < lelt; i++) {
    for(GenmapInt j = 0; j < nc; j++) {
      weights->data[i] -= u[nc * i + j];
    }
    weights->data[i] *= -1;
  }

#ifdef DEBUG
  printf("proc: %d 2-weights: ", h->Id(h->global));
  GenmapPrintVector(weights);
  printf("\n");
#endif

  free(u);

  return 0;
}

int GenmapGop_default(GenmapComm c, GenmapScalar *v) {
#ifdef MPI
  GenmapScalar u;
  MPI_Allreduce(v, &u, 1, MPI_DOUBLE, MPI_SUM, c->gsComm.c);
  *v = u;
#endif

  return 0;
}
