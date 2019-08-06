#ifndef UTILS_H_
#define UTILS_H_

#include <stdbool.h>

int clampIndex(int i, int length);

int absI(int a);
int maxI(int a, int b);
int minI(int a, int b);
int aveI(int a, int b);
int clampI(int a, int vMin, int vMax);
int clampPositiveI(int a);
int randRangeI(int min, int max);

float absF(float a);
float maxF(float a, float b);
float minF(float a, float b);
float aveF(float a, float b);
float clampF(float a, float vMin, float vMax);
float clampPositiveF(float a);
float randRangeF(float min, float max);


#define COUNT_PRED(ITEMS, LEN, PREDICATE, COUNTER) \
  COUNTER = 0;                                     \
  for (int __i = 0; __i < LEN; __i++)              \
  {                                                \
    if (PREDICATE(ITEMS[__i]))                     \
    {                                              \
      COUNTER++;                                   \
    }                                              \
  }

#define COUNT(ITEMS, LEN, ITEMVAR, PREDICATE_EXPRESSION, COUNTERVAR) \
  COUNTERVAR = 0;                                                    \
  for (int __i = 0; __i < LEN; __i++)                                \
  {                                                                  \
    ITEMVAR = ITEMS[__i];                                            \
    if (PREDICATE_EXPRESSION)                                        \
    {                                                                \
      COUNTERVAR++;                                                  \
    }                                                                \
  }

#define FOREACH(ITEMS, LEN, ITEMVAR, EXPRESSION) \
  for (int __i = 0; __i < LEN; __i++)            \
  {                                              \
    ITEMVAR = ITEMS[__i];                        \
    EXPRESSION                                   \
  }

#define FOREACH_WHERE(ITEMS, LEN, ITEMVAR, PREDICATE_EXPRESSION, EXPRESSION) \
  for (int __i = 0; __i < LEN; __i++)                                        \
  {                                                                          \
    ITEMVAR = ITEMS[__i];                                                    \
    if (!(PREDICATE_EXPRESSION))                                             \
    {                                                                        \
      continue;                                                              \
    }                                                                        \
    EXPRESSION                                                               \
  }

#endif