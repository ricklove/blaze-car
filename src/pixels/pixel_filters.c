#include "pixel_filters.h"
#include "../constants.h"
#include "../utils.h"
#include <stdlib.h>

static BYTE pixel_average(BYTE *rawImage, int xRaw, int yRaw)
{
  static int a_count = 0;
  static int b_count = 0;
  static int c_count = 0;
  static int d_count = 0;
  static int a_total = 0;
  static int b_total = 0;
  static int c_total = 0;
  static int d_total = 0;

  const int POOL_SIZE = 100000;
  if (a_count > POOL_SIZE)
  {
    a_count /= 2;
    a_total /= 2;
  }
  if (b_count > POOL_SIZE)
  {
    b_count /= 2;
    b_total /= 2;
  }
  if (c_count > POOL_SIZE)
  {
    c_count /= 2;
    c_total /= 2;
  }
  if (d_count > POOL_SIZE)
  {
    d_count /= 2;
    d_total /= 2;
  }

  int xMod = xRaw % 2;
  int yMod = yRaw % 2;
  BYTE val = rawImage[xRaw + yRaw * rawWidth];

  // Don't use ave
  if (yRaw < yRawMin)
  {
    return val;
  }

  if (xMod == 0 && yMod == 0)
  {
    a_count++;
    a_total += val;
    return a_total / a_count;
  }
  else if (xMod == 1 && yMod == 0)
  {
    b_count++;
    b_total += val;
    return b_total / b_count;
  }
  else if (xMod == 0 && yMod == 1)
  {
    c_count++;
    c_total += val;
    return c_total / c_count;
  }
  else //if (xMod == 1 && yMod == 1)
  {
    d_count++;
    d_total += val;
    return d_total / d_count;
  }
}

static BYTE diff_ave_mid(BYTE *rawImage, int xRaw, int yRaw)
{
  BYTE val = rawImage[xRaw + yRaw * rawWidth];
  BYTE ave = pixel_average(rawImage, xRaw, yRaw);
  return clampI(val - ave + 128, 0, 255);
}

static BYTE diff_ave_abs(BYTE *rawImage, int xRaw, int yRaw)
{
  BYTE val = rawImage[xRaw + yRaw * rawWidth];
  BYTE ave = pixel_average(rawImage, xRaw, yRaw);
  return absI(val - ave);
}

static BYTE pixel_histogram_count(BYTE *rawImage, int xRaw, int yRaw)
{
#define quantize 256

  static bool isFirst = true;
  static int count_total[4];
  static int count_used[4];
  static int hist[4][quantize];

  if (isFirst)
  {
    isFirst = false;
    for (int i = 0; i < 4; i++)
    {
      count_total[i] = 0;
      count_used[i] = 0;
      for (int j = 0; j < quantize; j++)
      {
        hist[i][j] = 0;
      }
    }
  }

  BYTE val = rawImage[xRaw + yRaw * rawWidth];
  val /= 256 / quantize;

  // Skip if out of range
  if (yRaw < yRawMin)
  {
    return val;
  }

  int xMod = xRaw % 2;
  int yMod = yRaw % 2;
  int i = xMod + yMod * 2;

  int h = hist[i][val] + 1;
  int c_used = count_used[i];
  int c_total = count_total[i] + 1;

  if (h == 1)
  {
    c_used++;
  }
  c_total++;

  const int MAX_SIZE = 0x10000000;
  if (c_total < MAX_SIZE)
  {
    count_used[i] = c_used;
    count_total[i] = c_total;
    hist[i][val] = h;
  }

  float avePerBucket = c_total / c_used;
  float r = h * 16.0f / avePerBucket;

  return clampI(r, 0, 255);
}

static BYTE pixel_histogram_blur(BYTE *rawImage, int xRaw, int yRaw)
{
#define quantize 256

  static bool isFirst = true;
  static int count_total[4];
  static int count_used[4];
  static int hist[4][quantize];

  if (isFirst)
  {
    isFirst = false;
    for (int i = 0; i < 4; i++)
    {
      count_total[i] = 0;
      count_used[i] = 0;
      for (int j = 0; j < quantize; j++)
      {
        hist[i][j] = 0;
      }
    }
  }

  // Skip if out of range or near border
  if (yRaw < yRawMin || yRaw >= rawHeight - 2 || xRaw >= rawWidth - 2 || xRaw <= 1)
  {
    return 255;
  }

  int val_a = rawImage[xRaw + yRaw * rawWidth] / (256 / quantize);
  int val_b = rawImage[xRaw + (yRaw + 2) * rawWidth] / (256 / quantize);
  int val_c = rawImage[xRaw - 2 + yRaw * rawWidth] / (256 / quantize);
  int val_d = rawImage[xRaw + 2 + yRaw * rawWidth] / (256 / quantize);
  int val = val_a * 0.5f + (val_b + val_c + val_d) * 0.5f;

  int xMod = xRaw % 2;
  int yMod = yRaw % 2;
  int i = xMod + yMod * 2;

  int h = hist[i][val] + 1;
  int c_used = count_used[i];
  int c_total = count_total[i] + 1;

  if (h == 1)
  {
    c_used++;
  }
  c_total++;

  const int MAX_SIZE = 0x10000000;
  if (c_total < MAX_SIZE)
  {
    count_used[i] = c_used;
    count_total[i] = c_total;
    hist[i][val] = h;
  }

  float avePerBucket = c_total / c_used;
  float r = h * 16.0f / avePerBucket;

  return clampI(r, 0, 255);
}

static BYTE pixel_histogram_combo(BYTE *rawImage, int xRaw, int yRaw)
{
#define quantize 0x10
#define quantize4 0x10000

  static bool isFirst = true;
  static int count_total = 0;
  static int count_used = 0;
  static int hist[quantize4];

  if (isFirst)
  {
    isFirst = false;
    for (int j = 0; j < quantize4; j++)
    {
      hist[j] = 0;
    }
  }

  // Skip if out of range or near right or bottom border
  if (yRaw < yRawMin || yRaw >= rawHeight - 1 || xRaw >= rawWidth - 1)
  {
    return 255;
  }

  BYTE val_a = rawImage[xRaw + 0 + (yRaw + 0) * rawWidth] / (256 / quantize);
  BYTE val_b = rawImage[xRaw + 1 + (yRaw + 0) * rawWidth] / (256 / quantize);
  BYTE val_c = rawImage[xRaw + 0 + (yRaw + 1) * rawWidth] / (256 / quantize);
  BYTE val_d = rawImage[xRaw + 1 + (yRaw + 1) * rawWidth] / (256 / quantize);
  int val = val_a * val_b * val_c * val_d;

  int xMod = xRaw % 2;
  int yMod = yRaw % 2;
  int i = xMod + yMod * 2;

  int h = hist[val] + 1;
  int c_used = count_used;
  int c_total = count_total + 1;

  if (h == 1)
  {
    c_used++;
  }
  c_total++;

  const int MAX_SIZE = 0x10000000;
  if (c_total < MAX_SIZE)
  {
    count_used = c_used;
    count_total = c_total;
    hist[val] = h;
  }

  float avePerBucket = c_total / c_used;
  float r = h * 16.0f / avePerBucket;

  return clampI(r, 0, 255);
}

PixelFilters getFilters()
{
  static PixelFilters filters;

  if (filters.items == NULL)
  {
    filters.count = 3;
    filters.items = malloc(sizeof(PixelFilter) * filters.count);
    // filters.items[0] = pixel_average;
    filters.items[0] = diff_ave_mid;
    //filters.items[2] = diff_ave_abs;
    filters.items[1] = pixel_histogram_count;
    // filters.items[2] = pixel_histogram_combo;
    filters.items[2] = pixel_histogram_blur;
  }

  return filters;
}

const PixelFiltersModule pixelFiltersModule = (PixelFiltersModule){
    .getFilters = getFilters,
};
