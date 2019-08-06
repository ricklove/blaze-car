#include "image_data.h"
#include <stdio.h>
#include <stdlib.h>

static char *_filePath;
static FILE *_file;
static long int _fileSize;
static BYTE *_imageDataBuffer;
static void load_file(char *filePath)
{
  _filePath = filePath;
  _file = fopen(_filePath, "r");
  if (_file == NULL)
  {
    printf("ERROR: No file found '%s'\n", _filePath);
  }

  fseek(_file, 0, SEEK_END);
  _fileSize = ftell(_file);
  if (_fileSize == 0)
  {
    printf("ERROR: File has 0 size '%s'\n", _filePath);
  }

  if (_imageDataBuffer == NULL)
  {
    _imageDataBuffer = malloc(sizeof(unsigned char) * rawImageSize);
  }
}

static int get_image_count()
{
  return _fileSize / rawImageSize;
}

static void load_image_index(int index)
{
  long int pos = index * rawImageSize % _fileSize;
  fseek(_file, pos, SEEK_SET);

  long int actualPos = ftell(_file);
  if (actualPos != pos)
  {
    printf("ERROR: File did not seek to correct image expectedPos=%ld actualPos=%ld\n", pos, actualPos);
  }
  fread(_imageDataBuffer, sizeof(BYTE), rawImageSize, _file);
}
static BYTE *get_image_data_buffer()
{
  return _imageDataBuffer;
}

const ImageDataModule imageDataModule = (ImageDataModule){
    .load_file = load_file,
    .get_image_count = get_image_count,
    .load_image_index = load_image_index,
    .get_image_data_buffer = get_image_data_buffer,
};
