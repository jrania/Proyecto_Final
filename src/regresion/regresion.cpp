#include "Arduino.h"
#include "regresion.h"
#include "../pines/pines.h"
#include "../comunicacionSerie/comunicacionSerie.h"

float Sx, Sy, Sxx, Syy, Sxy, N;

void constantesRegresion(float *m, float *b, int x[], int sizeX, unsigned long y[], int sizeY) {

  calculoSumas(x, sizeX, y, sizeY);

  *m = terminoConstante();
  *b = terminoOrdenada(*m);
}

void calculoSumas(int x[], int sizeX, unsigned long y[], int sizeY) {
  int i = 0;
  Sx = 0;
  Sy = 0;
  Sxx = 0;
  Syy = 0;
  Sxy = 0;
  N = 0;

  for (i = 0; i < sizeX; i++) {
    if (y[i] != 0) {
      N = N + 1;
      Sx = Sx + x[i];
      Sy = Sy + y[i];
      Sxx = Sxx + x[i] * x[i];
      Syy = Syy + y[i] * y[i];
      Sxy = Sxy + x[i] * y[i];
    }
  }
}

float terminoConstante() {
  float temp = (N * Sxy - Sx * Sy) / (N * Sxx - Sx * Sx);
  return temp;
}

float terminoOrdenada(float b) {
  float temp = Sy / N - b * Sx / N;

  return temp;
}