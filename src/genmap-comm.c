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
//    printf("v->data[%d]=%lf\n",i,v->data[i]);
    for(GenmapInt j = 0; j < nv; j ++) {
      v->data[i] += ucv[nv * i + j];
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

  GenmapInt *vertices;
  GenmapMalloc(numPoints, &vertices);

  GenmapElements elements = GenmapGetElements(h);
  for(GenmapInt i = 0; i < lelt; i++) {
    for(int j = 0; j < nv; j++) {
      vertices[i * nv + j] = elements[i].vertices[j];
    }
  }

  if(c->verticesHandle)
    gs_free(c->verticesHandle);

  c->verticesHandle = gs_setup(vertices, numPoints, &c->gsComm, 0,
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
//    weights->data[i] -= nv;
  }

  for(GenmapInt i = 0; i < lelt; i++) {
    weights->data[i] *= -1;
  }

#ifdef GENMAP_DEBUG
  printf("proc: %d 2-weights: ", h->Id(h->global));
  GenmapPrintVector(weights);
  printf("\n");
#endif

  GenmapFree(u);
  GenmapFree(vertices);

  return 0;
}

int GenmapAx_exact(GenmapHandle h, GenmapComm c, GenmapVector u,
                   GenmapVector weights, GenmapVector v) {
  assert(u->size == v->size);

  GenmapInt lelt = u->size;
  GenmapInt ne = h->header->ne;
  GenmapInt nv = h->header->nv;
  GenmapInt nf = h->header->nf;
  GenmapInt nDim = h->header->ndim;
  if(nDim == 2) nf = 0;

  GenmapScalar *ucv;
  GenmapMalloc(ne * lelt, &ucv);

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
    for(GenmapInt j = 0; j < ne; j++)
      ucv[ne * i + j] = u->data[i];

  gs(ucv, gs_double, gs_add, 0, c->edgesHandle, NULL);

  for(GenmapInt i = 0; i < lelt; i++) {
    for(GenmapInt j = 0; j < ne; j ++) {
      v->data[i] -= ucv[ne * i + j];
    }
  }

  if(nDim == 3) {
    for(GenmapInt i = 0; i < lelt; i++)
      for(GenmapInt j = 0; j < nf; j++)
        ucv[nf * i + j] = u->data[i];

    gs(ucv, gs_double, gs_add, 0, c->facesHandle, NULL);

    for(GenmapInt i = 0; i < lelt; i++) {
      for(GenmapInt j = 0; j < nf; j ++) {
        v->data[i] -= ucv[nf * i + j];
      }
    }
  }

  GenmapFree(ucv);

  return 0;
}

int GenmapAxInit_exact(GenmapHandle h, GenmapComm c,
                       GenmapVector weights) {
  GenmapInt lelt = h->header->lelt;
  GenmapInt nv = h->header->nv;
  GenmapInt ne = h->header->ne;
  GenmapInt nDim = h->header->ndim;

  GenmapInt nf = ne + 2 - nv;
  if(nDim == 2) nf = 0;

  GenmapInt numPoints = nv * lelt;
  GenmapInt numEdges = ne * lelt;
  GenmapInt numFaces = nf * lelt;

  GenmapInt *vertices, *edges, *faces;
  GenmapMalloc(numPoints, &vertices);
  GenmapMalloc(numEdges, &edges);
  if(nDim == 3) GenmapMalloc(numFaces, &faces);

  GenmapElements elements = GenmapGetElements(h);
  for(GenmapInt i = 0; i < lelt; i++) {
    for(int j = 0; j < nv; j++) {
      vertices[i * nv + j] = elements[i].vertices[j];
    }
    for(int j = 0; j < ne; j++) {
      edges[i * ne + j] = elements[i].edges[j];
    }
    for(int j = 0; j < nf; j++) {
      faces[i * nf + j] = elements[i].faces[j];
    }
  }

  if(c->verticesHandle)
    gs_free(c->verticesHandle);
  if(c->edgesHandle)
    gs_free(c->edgesHandle);
  if(c->facesHandle)
    gs_free(c->facesHandle);

  c->verticesHandle = gs_setup(vertices, numPoints, &c->gsComm, 0,
                               gs_auto, 0);
  c->edgesHandle = gs_setup(edges, numEdges, &c->gsComm, 0,
                            gs_auto, 0);
  if(nDim == 3)
    c->facesHandle = gs_setup(faces, numFaces, &c->gsComm, 0,
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
//    weights->data[i] -= nv;
  }

  GenmapRealloc(numEdges, &u);
  for(GenmapInt i = 0; i < lelt; i++)
    for(GenmapInt j = 0; j < ne; j++)
      u[ne * i + j] = 2.;

  gs(u, gs_double, gs_add, 0, c->edgesHandle, NULL);

  for(GenmapInt i = 0; i < lelt; i++) {
    for(GenmapInt j = 0; j < ne; j++) {
      weights->data[i] -= u[ne * i + j];
    }
//    weights->data[i] += ne;
  }

  if(nDim == 3) {
    GenmapRealloc(numFaces, &u);
    for(GenmapInt i = 0; i < lelt; i++)
      for(GenmapInt j = 0; j < nf; j++)
        u[nf * i + j] = 4.;

    gs(u, gs_double, gs_add, 0, c->facesHandle, NULL);

    for(GenmapInt i = 0; i < lelt; i++) {
      for(GenmapInt j = 0; j < nf; j++) {
        weights->data[i] += u[nf * i + j];
      }
//      weights->data[i] -= nf;
    }
  }

  for(GenmapInt i = 0; i < lelt; i++) {
    weights->data[i] *= -1;
  }

#ifdef GENMAP_DEBUG
  printf("proc: %d 2-weights: ", h->Id(h->global));
  GenmapPrintVector(weights);
  printf("\n");
#endif

  GenmapFree(u);
  GenmapFree(vertices);
  GenmapFree(edges);
  if(nDim == 3) GenmapFree(faces);

  return 0;
}

int GenmapGop(GenmapComm c, GenmapScalar *v, GenmapInt size,
              GenmapInt op) {
#ifdef GENMAP_MPI
  if(op == GENMAP_SUM) {
    MPI_Allreduce(MPI_IN_PLACE, v, size, MPI_DOUBLE, MPI_SUM, c->gsComm.c);
  } else if(op == GENMAP_MAX) {
    MPI_Allreduce(MPI_IN_PLACE, v, size, MPI_DOUBLE, MPI_MAX, c->gsComm.c);
  } else if(op == GENMAP_MIN) {
    MPI_Allreduce(MPI_IN_PLACE, v, size, MPI_DOUBLE, MPI_MIN, c->gsComm.c);
  }
#endif

  return 0;
}
