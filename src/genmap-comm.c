#include <genmap-impl.h>

int GenmapCreateComm(GenmapComm *c, GenmapCommExternal ce) {
  GenmapMalloc(1, c);
  comm_init(&(*c)->gsComm, ce);
  (*c)->verticesHandle = NULL;
  (*c)->edgesHandle = NULL;
  (*c)->facesHandle = NULL;
  (*c)->laplacianWeights = NULL;

  return 0;
}

int GenmapDestroyComm(GenmapComm c) {
  if(c->verticesHandle)
    gs_free(c->verticesHandle);
  if(c->edgesHandle)
    gs_free(c->edgesHandle);
  if(c->facesHandle)
    gs_free(c->facesHandle);
  if(c->laplacianWeights)
    GenmapFree(c->laplacianWeights);
  comm_free(&c->gsComm);
  GenmapFree(c);

  return 0;
}

int GenmapNp(GenmapComm c) {
  return c->gsComm.np;
}

int GenmapId(GenmapComm c) {
  return c->gsComm.id;
}

int GenmapAx(GenmapHandle h, GenmapComm c, GenmapVector u,
             GenmapVector weights, GenmapVector v) {
  assert(u->size == v->size);

  GenmapInt lelt = u->size;
  GenmapInt nv = h->header->nv;

  GenmapScalar *ucv;
  GenmapMalloc(nv * lelt, &ucv);

  for(GenmapInt i = 0; i < lelt; i++)
    for(GenmapInt j = 0; j < nv; j++)
      ucv[nv * i + j] = u->data[i];

  gs(ucv, gs_double, gs_add, 0, c->verticesHandle, NULL);

  for(GenmapInt i = 0; i < lelt; i++) {
    v->data[i] = weights->data[i] * u->data[i];
    for(GenmapInt j = 0; j < nv; j ++) {
      v->data[i] += ucv[nv * i + j];
    }
  }

  for(GenmapInt i = 0; i < lelt; i++)
    for(GenmapInt j = 0; j < nv; j++)
      ucv[nv * i + j] = u->data[i];

  gs(ucv, gs_double, gs_add, 0, c->edgesHandle, NULL);

  for(GenmapInt i = 0; i < lelt; i++) {
    for(GenmapInt j = 0; j < nv; j ++) {
      v->data[i] -= ucv[nv * i + j];
    }
  }

  GenmapFree(ucv);

  return 0;
}

int GenmapAxInit(GenmapHandle h, GenmapComm c,
                 GenmapVector weights) {
  GenmapInt lelt = h->header->lelt;
  GenmapInt nv = h->header->nv;
  GenmapInt numPoints = nv * lelt;

  GenmapInt *vertices, *edges;
  GenmapMalloc(numPoints, &vertices);
  // For 2D only, for 3D this is different
  GenmapMalloc(numPoints, &edges);

  GenmapElements elements = GenmapGetElements(h);
  for(GenmapInt i = 0; i < lelt; i++) {
    for(int j = 0; j < nv; j++) {
      vertices[i * nv + j] = elements[i].vertices[j];
      edges[i * nv + j] = elements[i].edges[j];
    }
  }

  if(c->verticesHandle)
    gs_free(c->verticesHandle);
  if(c->edgesHandle)
    gs_free(c->edgesHandle);
  c->verticesHandle = gs_setup(vertices, numPoints, &c->gsComm, 0,
                               gs_auto, 0);
  c->edgesHandle = gs_setup(edges, numPoints, &c->gsComm, 0,
                            gs_auto, 0);

  GenmapScalar *u;
  GenmapMalloc(numPoints, &u);

  for(GenmapInt i = 0; i < lelt; i++)
    for(GenmapInt j = 0; j < nv; j++)
      u[nv * i + j] = 1.;

  gs(u, gs_double, gs_add, 0, c->verticesHandle, NULL);

  assert(weights->size == lelt);

  for(GenmapInt i = 0; i < lelt; i++) {
    weights->data[i] = 0.;
    for(GenmapInt j = 0; j < nv; j++) {
      weights->data[i] += u[nv * i + j];
    }
  }

#ifdef DEBUG
  printf("proc: %d 1-weights: ", h->Id(h->global));
  GenmapPrintVector(weights);
  printf("\n");
#endif

  for(GenmapInt i = 0; i < lelt; i++)
    for(GenmapInt j = 0; j < nv; j++)
      u[nv * i + j] = 1.;

  gs(u, gs_double, gs_add, 0, c->edgesHandle, NULL);

  for(GenmapInt i = 0; i < lelt; i++) {
    for(GenmapInt j = 0; j < nv; j++) {
      weights->data[i] -= u[nv * i + j];
    }
    weights->data[i] *= -1;
  }

#ifdef DEBUG
  printf("proc: %d 2-weights: ", h->Id(h->global));
  GenmapPrintVector(weights);
  printf("\n");
#endif

  GenmapFree(u);
  GenmapFree(vertices);
  GenmapFree(edges);

  return 0;
}

int GenmapGop(GenmapComm c, GenmapScalar *v, GenmapInt size,
              GenmapInt op) {
#ifdef MPI
  if(op == 0) {
    MPI_Allreduce(MPI_IN_PLACE, v, size, MPI_DOUBLE, MPI_SUM, c->gsComm.c);
  }
#endif

  return 0;
}
