#pragma once

#include "../constants.h"
#include "../types.h"

typedef struct
{
  void (*load_file)(char *filePath);
  int (*get_image_count)();
  void (*load_image_index)(int index);
  // Read the loaded raw image data
  BYTE *(*get_image_data_buffer)();
} ImageDataModule;

extern const ImageDataModule imageDataModule;
