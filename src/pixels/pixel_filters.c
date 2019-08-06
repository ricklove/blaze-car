#include "pixel_filters.h"
#include "../constants.h"
#include "../utils.h"
#include <stdlib.h>
#include <math.h>

static BYTE pixel_none(BYTE *rawImage, int xRaw, int yRaw)
{
  return rawImage[xRaw + yRaw * rawWidth];
}

static BYTE pixel_midblur(BYTE *rawImage, int xRaw, int yRaw)
{
  if (yRaw < yRawMin || yRaw >= rawHeight - 2 || xRaw <= 2 || xRaw >= rawWidth - 2)
  {
    return 0;
  }

  BYTE v0 = rawImage[xRaw + yRaw * rawWidth];
  BYTE v1 = rawImage[(xRaw - 2) + (yRaw + 0) * rawWidth];
  BYTE v2 = rawImage[(xRaw + 2) + (yRaw + 0) * rawWidth];
  BYTE v3 = rawImage[(xRaw + 0) + (yRaw - 2) * rawWidth];
  BYTE v4 = rawImage[(xRaw + 0) + (yRaw + 2) * rawWidth];

  int vMin = minI(minI(minI(minI(v0, v1), v2), v3), v4);
  int vMax = maxI(maxI(maxI(maxI(v0, v1), v2), v3), v4);
  int vSum = v0 + v1 + v2 + v3 + v4;
  int vMidAve = (vSum - vMin - vMax) / 3;
  return vMidAve;
}

static BYTE pixel_midblur_cross(BYTE *rawImage, int xRaw, int yRaw)
{
  if (yRaw < yRawMin || yRaw >= rawHeight - 2 || xRaw <= 2 || xRaw >= rawWidth - 2)
  {
    return 0;
  }

  BYTE v0 = rawImage[xRaw + yRaw * rawWidth];
  BYTE v1 = rawImage[(xRaw - 2) + (yRaw - 2) * rawWidth];
  BYTE v2 = rawImage[(xRaw - 2) + (yRaw + 2) * rawWidth];
  BYTE v3 = rawImage[(xRaw + 2) + (yRaw - 2) * rawWidth];
  BYTE v4 = rawImage[(xRaw + 2) + (yRaw + 2) * rawWidth];

  int vMin = minI(minI(minI(minI(v0, v1), v2), v3), v4);
  int vMax = maxI(maxI(maxI(maxI(v0, v1), v2), v3), v4);
  int vSum = v0 + v1 + v2 + v3 + v4;
  int vMidAve = (vSum - vMin - vMax) / 3;
  return vMidAve;
}

static BYTE pixel_midblur_square(BYTE *rawImage, int xRaw, int yRaw)
{
  if (yRaw < yRawMin || yRaw >= rawHeight - 2 || xRaw <= 2 || xRaw >= rawWidth - 2)
  {
    return 0;
  }

  int vMin = 255;
  int vMax = 0;
  int vSum = 0;

  for (int i = -1; i <= 1; i++)
  {
    for (int j = -1; j <= 1; j++)
    {
      int v = rawImage[(xRaw + 2 * i) + (yRaw + 2 * j) * rawWidth];
      vSum += v;
      vMin = minI(vMin, v);
      vMax = maxI(vMax, v);
    }
  }

  int vMidAve = (vSum - vMin - vMax) / 7;
  return vMidAve;
}

static BYTE pixel_medianBlur_square(BYTE *rawImage, int xRaw, int yRaw)
{
  if (yRaw < yRawMin || yRaw >= rawHeight - 2 || xRaw <= 2 || xRaw >= rawWidth - 2)
  {
    return 0;
  }

#define VAL_SWAP(a, b) \
  {                    \
    int t = a;         \
    a = b;             \
    b = t;             \
  }
#define VAL_SORT(a, b) \
  {                    \
    if (a > b)         \
    {                  \
      VAL_SWAP(a, b)   \
    }                  \
  }

  int vMax0 = 0;
  int vMax1 = 0;
  int vMax2 = 0;
  int vMax3 = 0;
  int vMax4 = 0;

  for (int i = -1; i <= 1; i++)
  {
    for (int j = -1; j <= 1; j++)
    {
      int v = rawImage[(xRaw + 2 * i) + (yRaw + 2 * j) * rawWidth];
      VAL_SORT(v, vMax0);
      VAL_SORT(vMax0, vMax1);
      VAL_SORT(vMax1, vMax2);
      VAL_SORT(vMax2, vMax3);
      VAL_SORT(vMax3, vMax4);
    }
  }

  return vMax0;
}

static BYTE pixel_medianBlur_square25(BYTE *rawImage, int xRaw, int yRaw)
{
  if (yRaw < yRawMin || yRaw >= rawHeight - 2 || xRaw <= 2 || xRaw >= rawWidth - 2)
  {
    return 0;
  }

#define VAL_SWAP(a, b) \
  {                    \
    int t = a;         \
    a = b;             \
    b = t;             \
  }
#define VAL_SORT(a, b) \
  {                    \
    if (a > b)         \
    {                  \
      VAL_SWAP(a, b)   \
    }                  \
  }

  int vMax00 = 0;
  int vMax01 = 0;
  int vMax02 = 0;
  int vMax03 = 0;
  int vMax04 = 0;
  int vMax05 = 0;
  int vMax06 = 0;
  int vMax07 = 0;
  int vMax08 = 0;
  int vMax09 = 0;
  int vMax10 = 0;
  int vMax11 = 0;
  int vMax12 = 0;

  for (int i = -2; i <= 2; i++)
  {
    for (int j = -2; j <= 2; j++)
    {
      int v = rawImage[(xRaw + 2 * i) + (yRaw + 2 * j) * rawWidth];
      VAL_SORT(v, vMax00);
      VAL_SORT(vMax00, vMax01);
      VAL_SORT(vMax01, vMax02);
      VAL_SORT(vMax02, vMax03);
      VAL_SORT(vMax03, vMax04);
      VAL_SORT(vMax04, vMax05);
      VAL_SORT(vMax05, vMax06);
      VAL_SORT(vMax06, vMax07);
      VAL_SORT(vMax07, vMax08);
      VAL_SORT(vMax08, vMax09);
      VAL_SORT(vMax09, vMax10);
      VAL_SORT(vMax10, vMax11);
      VAL_SORT(vMax11, vMax12);
    }
  }

  return vMax00;
}

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
  float r = h * 128.0f / avePerBucket;

  return 255 - clampI(r, 0, 255);
}

static BYTE pixel_histogram_blur_simple(BYTE *rawImage, int xRaw, int yRaw)
{
  if (yRaw < yRawMin || yRaw >= rawHeight - 2 || xRaw <= 2 || xRaw >= rawWidth - 2)
  {
    return 0;
  }

  BYTE v0 = pixel_histogram_count(rawImage, xRaw, yRaw);
  BYTE v1 = pixel_histogram_count(rawImage, xRaw, yRaw + 2);
  BYTE v2 = pixel_histogram_count(rawImage, xRaw - 2, yRaw);
  BYTE v3 = pixel_histogram_count(rawImage, xRaw + 2, yRaw);
  return v0 * 0.5f + (v1 + v2 + v3) * 0.5f / 3;
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
  float r = h * 128.0f / avePerBucket;

  return 255 - clampI(r, 0, 255);
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
  float r = h * 128.0f / avePerBucket;

  return 255 - clampI(r, 0, 255);
}

PixelFilters getFilters_edgeDetect()
{
  static PixelFilters filters;

  if (filters.items == NULL)
  {
    const int MAX = 16;
    int i = 0;
    filters.items = malloc(sizeof(PixelFilter) * 16);
    // filters.items[i++] = pixel_none;
    // // // filters.items[i++] = pixel_average;
    // // filters.items[i++] = diff_ave_mid;
    // filters.items[i++] = diff_ave_abs;
    // filters.items[i++] = pixel_histogram_count;
    // // // filters.items[i++] = pixel_histogram_blur;
    // filters.items[i++] = pixel_histogram_blur_simple;
    // // filters.items[i++] = pixel_histogram_combo;

    filters.items[i++] = pixel_medianBlur_square;

    filters.count = i;
  }

  return filters;
}

static BYTE sobel(BYTE *rawImage, int xRaw, int yRaw, PixelFilter filter)
{
  static bool isInit = false;
  static int s1[3][3];
  static int s2[3][3];

  if (!isInit)
  {
    for (int i = 0; i <= 2; i++)
    {
      for (int j = 0; j <= 2; j++)
      {
        s1[i][j] = 0;
        s2[i][j] = 0;
      }
    }

    // Sobel
    s1[0][0] = -1;
    s1[0][1] = -2;
    s1[0][2] = -1;
    s1[2][0] = +1;
    s1[2][1] = +2;
    s1[2][2] = +1;

    s2[0][0] = -1;
    s2[1][0] = -2;
    s2[2][0] = -1;
    s2[0][2] = +1;
    s2[1][2] = +2;
    s2[2][2] = +1;

    // // Scharr
    //     s1[0][0] = -47;
    //     s1[0][1] = -162;
    //     s1[0][2] = -47;
    //     s1[2][0] = +47;
    //     s1[2][1] = +162;
    //     s1[2][2] = +47;

    //     s2[0][0] = -47;
    //     s2[1][0] = -162;
    //     s2[2][0] = -47;
    //     s2[0][2] = +47;
    //     s2[1][2] = +162;
    //     s2[2][2] = +47;
  }

  int g1 = 0;
  int g2 = 0;

  for (int i = 0; i <= 2; i++)
  {
    int x = xRaw + 2 * i - 2;
    for (int j = 0; j <= 2; j++)
    {
      int y = yRaw + 2 * j - 2;
      BYTE v = filter(rawImage, x, y);
      g1 += v * s1[i][j];
      g2 += v * s2[i][j];
    }
  }

  int g = (g1 * g1 + g2 * g2) / 256;
  g = clampI(g, 0, 255);
  //int g = sqrt(g1 * g1 + g2 * g2);
  // int theta = atan(g2 / g1);
  return g;
}

static BYTE pixel_edgeDetect_sobel_combo(BYTE *rawImage, int xRaw, int yRaw)
{
  static bool isInit = false;
  static PixelFilters filters;
  if (!isInit)
  {
    filters = getFilters_edgeDetect();
  }

  if (yRaw < yRawMin || yRaw >= rawHeight - 2 || xRaw <= 2 || xRaw >= rawWidth - 2)
  {
    return 0;
  }

  int gTotal = 0;
  for (int i = 0; i < filters.count; i++)
  {
    gTotal += sobel(rawImage, xRaw, yRaw, filters.items[i]);
  }

  BYTE gAve = gTotal / filters.count;
  return gAve;
}

PixelFilters getFilters()
{
  static PixelFilters filters;

  if (filters.items == NULL)
  {
    const int MAX = 16;
    int i = 0;
    filters.items = malloc(sizeof(PixelFilter) * 16);
    filters.items[i++] = pixel_none;
    filters.items[i++] = pixel_midblur;
    filters.items[i++] = pixel_midblur_cross;
    filters.items[i++] = pixel_midblur_square;
    filters.items[i++] = pixel_medianBlur_square;
    filters.items[i++] = pixel_medianBlur_square25;
    filters.items[i++] = pixel_edgeDetect_sobel_combo;

    // // filters.items[i++] = pixel_average;
    // filters.items[i++] = diff_ave_mid;
    // filters.items[i++] = diff_ave_abs;
    // filters.items[i++] = pixel_histogram_count;
    // // filters.items[i++] = pixel_histogram_blur;
    // filters.items[i++] = pixel_histogram_blur_simple;
    // filters.items[i++] = pixel_histogram_combo;
    // // filters.items[i++] = pixel_edgeDetect_sobel_combo;
    filters.count = i;
  }

  return filters;
}

const PixelFiltersModule pixelFiltersModule = (PixelFiltersModule){
    .getFilters = getFilters,
};
