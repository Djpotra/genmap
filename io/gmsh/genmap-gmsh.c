#include <genmap-impl.h>
#include <genmap-io.h>

#define GENMAP_GMSH_HEX 5
#define GENMAP_GMSH_QUAD 3
//
// GenmapHandle: Create
//
int GenmapCreateHandle_gmsh(GenmapHandle h) {
  h->Read = GenmapRead_gmsh;
  h->Write = GenmapWrite_gmsh;

  return 0;
}
//
// Do File I/O in serial
//
int GenmapRead_gmsh(GenmapHandle h, void *data) {
  char *fileName = (char *) data;
  char *status;
  char buf[BUFSIZ];

  GenmapInt NglobalElements, NlocalElements, NglobalNodes;

  int rank, size;
  MPI_Comm_rank(h->global->gsComm.c, &rank);
  MPI_Comm_size(h->global->gsComm.c, &size);

  FILE *fp = fopen(fileName, "r");
  if(fp == NULL) {
    printf("GenmapRead_gmsh:%s:%d can't open file.\n", __FILE__, __LINE__);
  }

  do {
    status = fgets(buf, BUFSIZ, fp);
  } while(!strstr(buf, "$Nodes"));

  status = fgets(buf, BUFSIZ, fp);
  sscanf(buf, "%d", &NglobalNodes);
  h->header->Nnodes = NglobalNodes;

  GenmapScalar *VX, *VY, *VZ; 
  GenmapCalloc(NglobalNodes, &VX);
  GenmapCalloc(NglobalNodes, &VY);
  GenmapCalloc(NglobalNodes, &VZ);
  for(GenmapInt i = 0; i < NglobalNodes; i++) {
    status = fgets(buf, BUFSIZ, fp);
    sscanf(buf, "%*d %lf %lf %lf", VX+i, VY+i, VZ+i);
  }

  do {
    status = fgets(buf, BUFSIZ, fp);
  } while(!strstr(buf, "$Elements"));

  status = fgets(buf, BUFSIZ, fp);
  sscanf(buf, "%d", &NglobalElements);

  struct array globalElements = null_array;
  array_reserve(struct GenmapElement_private, &globalElements,
                NglobalElements);
  globalElements.n = NglobalElements;
  GenmapElements elements = (GenmapElements)(globalElements.ptr);

  GenmapInt count = 0;

  for(GenmapInt i = 0; i < NglobalElements; i++) {
    status = fgets(buf, BUFSIZ, fp);
    GenmapInt elementId, elementType;
    GenmapInt v1, v2, v3, v4, v5, v6, v7, v8;

    sscanf(buf, "%d %d", &elementId, &elementType);
    if(elementType == GENMAP_GMSH_HEX) {
      sscanf(buf, "%*d %*d %*d %*d %*d %d %d %d %d %d %d %d %d",
             &v1, &v2, &v3, &v4, &v5, &v6, &v7, &v8);
      elements[count].globalId = elementId;
      elements[count].vertices[0] = v1;
      elements[count].vertices[1] = v2;
      elements[count].vertices[2] = v3;
      elements[count].vertices[3] = v4;
      elements[count].vertices[4] = v5;
      elements[count].vertices[5] = v6;
      elements[count].vertices[6] = v7;
      elements[count].vertices[7] = v8;
      for(GenmapInt j = 0; j < 8; j++) {
        elements[count].x[j] = VX[elements[count].vertices[j] - 1];
        elements[count].y[j] = VY[elements[count].vertices[j] - 1];
        elements[count].z[j] = VZ[elements[count].vertices[j] - 1];
      }
      count++;
    }
  }

  NlocalElements = count / size;
  int nrem = count - NlocalElements * size;
  if(rank < nrem) NlocalElements++;

  GenmapInt out[2][1], bufs[2][1];
  comm_scan(out, &(h->global->gsComm), gs_int, gs_add, &NlocalElements, 1,
            bufs);
  h->header->start = out[0][0];
  GenmapInt start = out[0][0];

  h->header->nel = count;
  h->header->ndim = 3;
  h->header->nv = 8;
  h->header->ne = 12;
  h->header->nf = 6;
  h->header->lelt = NlocalElements;

  array_reserve(struct GenmapElement_private, &(h->elementArray),
                NlocalElements);
  h->elementArray.n = NlocalElements;
  GenmapElements localElements = GenmapGetElements(h);
  for(GenmapInt i = start, count = 0; count < NlocalElements;
      i++, count++) {
    localElements[count] = elements[i];
  }

  fclose(fp);

  array_free(&globalElements);

  GenmapFree(VX);
  GenmapFree(VY);
  GenmapFree(VZ);

  return 0;
}

int GenmapWrite_gmsh(GenmapHandle h, char *fileNameBase) {
  FILE *fp;
  char fileName[BUFSIZ]; 

  GenmapInt rank = h->Id(h->global);
  sprintf(fileName, "%s_%04d.vtu", fileNameBase, rank);

  fp = fopen(fileName, "w");

  GenmapInt Nq = 2;
  GenmapInt Np = Nq*Nq*Nq;
  GenmapInt Eloc = (Nq-1)*(Nq-1)*(Nq-1);

  fprintf(fp, "<VTKFile type=\"UnstructuredGrid\" version=\"0.1\" byte_order=\"BigEndian\">\n");
  fprintf(fp, "  <UnstructuredGrid>\n");
  fprintf(fp, "    <Piece NumberOfPoints=\"%d\" NumberOfCells=\"%d\">\n",
    h->header->lelt*Np, h->header->lelt*Eloc);

  fprintf(fp, "      <Points>\n");
  fprintf(fp, "        <DataArray type=\"Float32\" NumberOfComponents=\"3\" Format=\"ascii\">\n");
  GenmapElements elements = GenmapGetElements(h);
  for(GenmapInt i = 0; i < h->header->lelt; i++) {
    for(GenmapInt j = 0; j < Np; j++) {
      fprintf(fp, "          ");
      fprintf(fp, "%lf %lf %lf\n", elements[i].x[j],
		                   elements[i].y[j],
				   elements[i].z[j]);
    }
  }
  fprintf(fp, "        </DataArray>\n");
  fprintf(fp, "      </Points>\n");

  fprintf(fp, "      <PointData Scalars=\"scalars\">\n");
  fprintf(fp, "      </PointData>\n"); 

  fprintf(fp, "      <Cells>\n");
  fprintf(fp, "        <DataArray type=\"Int32\" Name=\"connectivity\" Format=\"ascii\">\n");
  for(GenmapInt e=0;e<h->header->lelt;++e){
    for(int k=0;k<Nq-1;++k){
      for(int j=0;j<Nq-1;++j){
	for(int i=0;i<Nq-1;++i){
	  int b = e*Np + i + j*Nq + k*Nq*Nq;
	  fprintf(fp, 
		  "%d "
		  "%d "
		  "%d "
		  "%d "
		  "%d "
		  "%d "
		  "%d "
		  "%d\n ",
		  b,
		  b+1,
		  b+Nq,
		  b+Nq+1,
		  b+Nq*Nq,
		  b+1+Nq*Nq,
		  b+Nq+Nq*Nq,
		  b+Nq+1+Nq*Nq);
	}
      }
    }
  }
  
  fprintf(fp, "        </DataArray>\n");

  fprintf(fp, "        <DataArray type=\"Int32\" Name=\"offsets\" Format=\"ascii\">\n");
  GenmapInt cnt = 0;
  for(GenmapInt e=0;e<h->header->lelt;++e){
    for(int n=0;n<Eloc;++n){
      cnt += 8;
      fprintf(fp, "       ");
      fprintf(fp, "%d\n", cnt);
    }
  }
  fprintf(fp, "        </DataArray>\n");

  fprintf(fp, "        <DataArray type=\"Int32\" Name=\"types\" Format=\"ascii\">\n");
  for(GenmapInt e=0;e<h->header->lelt;++e){
    for(int n=0;n<Eloc;++n){
      fprintf(fp, "12\n");
    }
  }
  fprintf(fp, "        </DataArray>\n");

  fprintf(fp, "      </Cells>\n");

  fprintf(fp, "    </Piece>\n");
  fprintf(fp, "  </UnstructuredGrid>\n");
  fprintf(fp, "</VTKFile>\n");

  fclose(fp);

  return 0;
}
