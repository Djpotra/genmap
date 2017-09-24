#ifndef _CSR_H_
#define _CSR_H_

struct csr {
  int *ia;
  int *ja;
  int *va;
};

typedef struct csr CSRMatrix;

#endif
