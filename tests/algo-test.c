#include <genmap-impl.h>

#include <stdio.h>
//
// Test Algorithms
//
// Linear solve
//
void TestSymTriDiagSolve1() {
  GenmapVector x, a, b, rhs, answer;

  GenmapCreateVector(&x, 3);
  GenmapCreateVector(&a, 3);
  GenmapCreateVector(&b, 2);
  GenmapCreateVector(&rhs, 3);
  GenmapCreateVector(&answer, 3);

  GenmapScalar da[3] = {1., 1., 1.};
  GenmapScalar db[2] = {0., 0.};
  GenmapScalar drhs[3] = {1., 2., 3.};
  GenmapScalar danswer[3] = {1., 2., 3.};

  GenmapSetVector(a, da);
  GenmapSetVector(b, db);
  GenmapSetVector(rhs, drhs);
  GenmapSetVector(answer, danswer);

  GenmapSymTriDiagSolve(x, rhs, a, b);

  assert(GenmapVectorsEqual(x, answer, GENMAP_TOL));

  GenmapDestroyVector(x);
  GenmapDestroyVector(a);
  GenmapDestroyVector(b);
  GenmapDestroyVector(rhs);
  GenmapDestroyVector(answer);
}

void TestSymTriDiagSolve2() {
  GenmapVector x, a, b, rhs, answer;

  GenmapCreateVector(&x, 3);
  GenmapCreateVector(&a, 3);
  GenmapCreateVector(&b, 2);
  GenmapCreateVector(&rhs, 3);
  GenmapCreateVector(&answer, 3);

  GenmapScalar da[3] = {1., 1., 1.};
  GenmapScalar db[2] = {2., 3.};
  GenmapScalar drhs[3] = {1., 2., 3.};
  GenmapScalar danswer[3] = {-0.5, 0.75, 0.75};

  GenmapSetVector(a, da);
  GenmapSetVector(b, db);
  GenmapSetVector(rhs, drhs);
  GenmapSetVector(answer, danswer);

  GenmapSymTriDiagSolve(x, rhs, a, b);

  assert(GenmapVectorsEqual(x, answer, GENMAP_TOL));

  GenmapDestroyVector(x);
  GenmapDestroyVector(a);
  GenmapDestroyVector(b);
  GenmapDestroyVector(rhs);
  GenmapDestroyVector(answer);
}

void TestSymTriDiagSolve3() {
  GenmapVector x, a, b, rhs, answer;

  GenmapCreateVector(&x, 3);
  GenmapCreateVector(&a, 3);
  GenmapCreateVector(&b, 2);
  GenmapCreateVector(&rhs, 3);
  GenmapCreateVector(&answer, 3);

  GenmapScalar da[3] = {1., 1., 1.};
  GenmapScalar db[2] = {1., 1.};
  GenmapScalar drhs[3] = {1., 2., 3.};
  GenmapScalar danswer[3] = {-1., 2., 1.};

  GenmapSetVector(a, da);
  GenmapSetVector(b, db);
  GenmapSetVector(rhs, drhs);
  GenmapSetVector(answer, danswer);

  GenmapSymTriDiagSolve(x, rhs, a, b);

  assert(GenmapVectorsEqual(x, answer, GENMAP_TOL));
  GenmapPrintVector(x);

  GenmapDestroyVector(x);
  GenmapDestroyVector(a);
  GenmapDestroyVector(b);
  GenmapDestroyVector(rhs);
  GenmapDestroyVector(answer);
}
//
// Power and inverse power Iteration
//
void TestPowerIter1() {
  GenmapInt n = 3;
  GenmapVector alpha, beta, eVector, init;

  GenmapCreateOnesVector(&alpha, n);
  GenmapCreateOnesVector(&init, n);
  GenmapCreateVector(&eVector, n);
  GenmapCreateVector(&beta, n - 1);
  beta->data[0] = 2.0; beta->data[1] = 3.0;

  GenmapPowerIter(eVector, alpha, beta, init, 100);

  GenmapScalar eValCorrect = 4.60555127546399;
  GenmapScalar eVal = 0;
  for(GenmapInt i = 0; i < n; i++) {
    if(abs(eVector->data[i]) > eVal)
      eVal = eVector->data[i];
  }

  assert(abs(eVal - eValCorrect) < GENMAP_TOL);

  GenmapScaleVector(eVector, eVector, 1.0 / GenmapNormVector(eVector, 2));

  GenmapVector eVectorCorrect;
  GenmapScalar d[3] = {3.922322702763679e-01, 7.071067811865475e-01, \
                       5.883484054145522e-01
                      };
  GenmapCreateVector(&eVectorCorrect, n);
  GenmapSetVector(eVectorCorrect, d);

  assert(GenmapVectorsEqual(eVector, eVectorCorrect, GENMAP_TOL));

  GenmapDestroyVector(alpha);
  GenmapDestroyVector(beta);
  GenmapDestroyVector(eVector);
  GenmapDestroyVector(init);
  GenmapDestroyVector(eVectorCorrect);
}

void TestPowerIter2() {
  GenmapInt n = 4;
  GenmapVector alpha, beta, eVector, init;

  GenmapCreateVector(&alpha, n);
  for(GenmapInt i = 0; i < n; i++)
    alpha->data[i] = (double) i + 2.0;
  GenmapCreateOnesVector(&beta, n - 1);
  GenmapCreateOnesVector(&init, n);
  GenmapCreateVector(&eVector, n);

  GenmapPowerIter(eVector, alpha, beta, init, 100);

  GenmapScalar eValCorrect = 5.745281240174139;
  GenmapScalar eVal = 0;
  for(GenmapInt i = 0; i < n; i++) {
    if(abs(eVector->data[i]) > eVal)
      eVal = eVector->data[i];
  }

  assert(abs(eVal - eValCorrect) < GENMAP_TOL);

  GenmapScaleVector(eVector, eVector, 1.0 / GenmapNormVector(eVector, 2));

  GenmapVector eVectorCorrect;
  GenmapScalar d[4] = {0.062465125787784, 0.233949463778106, 0.57979194827105, \
                       0.777950546743373
                      };
  GenmapCreateVector(&eVectorCorrect, n);
  GenmapSetVector(eVectorCorrect, d);

  assert(GenmapVectorsEqual(eVector, eVectorCorrect, GENMAP_TOL));

  GenmapDestroyVector(alpha);
  GenmapDestroyVector(beta);
  GenmapDestroyVector(eVector);
  GenmapDestroyVector(init);
  GenmapDestroyVector(eVectorCorrect);
}

void TestInvPowerIter1() {
  GenmapInt n = 3;
  GenmapVector alpha, beta, eVector, init;

  GenmapCreateOnesVector(&alpha, n);
  GenmapCreateVector(&beta, n - 1);
  beta->data[0] = 2.0; beta->data[1] = 3.0;
  GenmapCreateOnesVector(&init, n);
  GenmapCreateVector(&eVector, n);

  GenmapInvPowerIter(eVector, alpha, beta, init, 100);

  GenmapScalar eValCorrect = 1.0;
  GenmapScalar eVal = 0;
  for(GenmapInt i = 0; i < n; i++) {
    if(abs(eVector->data[i]) > eVal)
      eVal = eVector->data[i];
  }

  assert(abs(eVal - eValCorrect) < GENMAP_TOL);

  GenmapScaleVector(eVector, eVector, 1.0 / GenmapNormVector(eVector, 2));

  GenmapVector eVectorCorrect;
  GenmapScalar d[4] = {8.320502943378435e-01,  -1.110223024625157e-16, \
                       -5.547001962252294e-01
                      };
  GenmapCreateVector(&eVectorCorrect, n);
  GenmapSetVector(eVectorCorrect, d);

  assert(GenmapVectorsEqual(eVector, eVectorCorrect, GENMAP_TOL));

  GenmapDestroyVector(alpha);
  GenmapDestroyVector(beta);
  GenmapDestroyVector(eVector);
  GenmapDestroyVector(init);
  GenmapDestroyVector(eVectorCorrect);
}

void TestInvPowerIter2() {
  GenmapInt n = 4;
  GenmapVector alpha, beta, eVector, init;

  GenmapCreateVector(&alpha, n);
  for(GenmapInt i = 0; i < n; i++)
    alpha->data[i] = (double) i + 2.0;
  GenmapCreateOnesVector(&beta, n - 1);

  GenmapCreateOnesVector(&init, n);
  GenmapCreateVector(&eVector, n);

  GenmapInvPowerIter(eVector, alpha, beta, init, 100);

  GenmapScalar eValCorrect = 0.796991351383626;
  GenmapScalar eVal = 0;
  for(GenmapInt i = 0; i < n; i++) {
    if(abs(eVector->data[i]) > eVal)
      eVal = eVector->data[i];
  }

  assert(abs(eVal - eValCorrect) < GENMAP_TOL);

  GenmapScaleVector(eVector, eVector, 1.0 / GenmapNormVector(eVector, 2));

  GenmapVector eVectorCorrect;
  GenmapScalar d[4] = {0.777950546743372, -0.579791948271051, \
                       0.233949463778106, -0.062465125787784
                      };
  GenmapCreateVector(&eVectorCorrect, n);
  GenmapSetVector(eVectorCorrect, d);

  assert(GenmapVectorsEqual(eVector, eVectorCorrect, GENMAP_TOL));

  GenmapDestroyVector(alpha);
  GenmapDestroyVector(beta);
  GenmapDestroyVector(eVector);
  GenmapDestroyVector(init);
}

int main() {
  TestSymTriDiagSolve1();
  TestSymTriDiagSolve2();
  // TODO Fix GenmapSymTriDiag to pass the following test
  //TestSymTriDiagSolve3();

  TestPowerIter1();
  TestPowerIter2();
  TestInvPowerIter1();
  TestInvPowerIter2();

  return 0;
}
