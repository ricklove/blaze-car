#include "utils.h"
#include <stdlib.h>

int clampIndex(int i, int length) { return i < 0 ? 0 : i > length - 1 ? length - 1 : i; }

int absI(int a) { return a > 0 ? a : -a; } // abs is defined in math.h
int maxI(int a, int b) { return a > b ? a : b; }
int minI(int a, int b) { return a < b ? a : b; }
int aveI(int a, int b) { return (a + b) / 2; }
int clampI(int a, int vMin, int vMax) { return a < vMin ? vMin : a > vMax ? vMax : a; }
int clampPositiveI(int a) { return a > 0 ? a : 0; }
int randRangeI(int min, int max)
{
  return (int)randRangeF(min, max);
}

float absF(float a) { return a > 0 ? a : -a; }
float maxF(float a, float b) { return a > b ? a : b; }
float minF(float a, float b) { return a < b ? a : b; }
float aveF(float a, float b) { return (a + b) / 2; }
float clampF(float a, float vMin, float vMax) { return a < vMin ? vMin : a > vMax ? vMax : a; }
float clampPositiveF(float a) { return a > 0 ? a : 0; }
float randRangeF(float min, float max)
{
  float ratio = rand() * 1.0f / RAND_MAX;
  return min + ratio * (max - min);
}