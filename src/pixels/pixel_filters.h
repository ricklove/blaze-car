#pragma once

#include "../constants.h"
#include "../types.h"

typedef BYTE (*PixelFilter)(BYTE *rawImage, int xRaw, int yRaw);

typedef struct PixelFilters
{
  int count;
  PixelFilter *items;
} PixelFilters;

typedef struct
{
  PixelFilters (*getFilters)();

} PixelFiltersModule;

extern const PixelFiltersModule pixelFiltersModule;
