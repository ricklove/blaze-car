#include "run.h"
#include <simple2d.h>
#include <math.h>
#include "draw_pixels.h"
#include "image_data.h"

static void drawLine(float x1, float y1, float x2, float y2, char r_255, char g_255, char b_255)
{
  float r = r_255 * 1.0f / 255;
  float g = g_255 * 1.0f / 255;
  float b = b_255 * 1.0f / 255;

  S2D_DrawLine(x1, y1, x2, y2, 10, r, g, b, 1, r, g, b, 1, r, g, b, 1, r, g, b, 1);
}

static S2D_Text *info;

static int frame = 0;
static BYTE *rawImageData;
static BYTE *pixels_a;
static BYTE *pixels_b;
static BYTE *pixels_c;
static BYTE *pixels_d;

static void setup()
{
  info = S2D_CreateText("resources/vera.ttf", "", 11);
  if (info == NULL)
  {
    printf("ERROR: Text was not created (is font file missing?)\n");
  }

  info->x = 10;
  info->y = 10;

  int imageSize = imageWidth * imageHeight;
  pixels_a = malloc(sizeof(BYTE) * 4 * imageSize);
  pixels_b = malloc(sizeof(BYTE) * 4 * imageSize);
  pixels_c = malloc(sizeof(BYTE) * 4 * imageSize);
  pixels_d = malloc(sizeof(BYTE) * 4 * imageSize);

  for (int x = 0; x < imageWidth; x++)
  {
    for (int y = 0; y < imageHeight; y++)
    {
      // Zero Out RGB
      pixels_a[(x + y * imageWidth) * 4 + 0] = 0;
      pixels_b[(x + y * imageWidth) * 4 + 0] = 0;
      pixels_c[(x + y * imageWidth) * 4 + 0] = 0;
      pixels_d[(x + y * imageWidth) * 4 + 0] = 0;
      pixels_a[(x + y * imageWidth) * 4 + 1] = 0;
      pixels_b[(x + y * imageWidth) * 4 + 1] = 0;
      pixels_c[(x + y * imageWidth) * 4 + 1] = 0;
      pixels_d[(x + y * imageWidth) * 4 + 1] = 0;
      pixels_a[(x + y * imageWidth) * 4 + 2] = 0;
      pixels_b[(x + y * imageWidth) * 4 + 2] = 0;
      pixels_c[(x + y * imageWidth) * 4 + 2] = 0;
      pixels_d[(x + y * imageWidth) * 4 + 2] = 0;

      // Alpha = 100%
      pixels_a[(x + y * imageWidth) * 4 + 3] = 255;
      pixels_b[(x + y * imageWidth) * 4 + 3] = 255;
      pixels_c[(x + y * imageWidth) * 4 + 3] = 255;
      pixels_d[(x + y * imageWidth) * 4 + 3] = 255;
    }
  }
}

static void update()
{
  imageDataModule.load_image_index(frame);

  for (int x = 0; x < imageWidth; x++)
  {
    for (int y = 0; y < imageHeight; y++)
    {
      int xRaw = x * 2;
      int yRaw = y * 2;

      // BGgR Bayer => RGB Values
      pixels_a[(x + y * imageWidth) * 4 + 0] = rawImageData[xRaw + yRaw * rawWidth + 0];
      pixels_b[(x + y * imageWidth) * 4 + 1] = rawImageData[xRaw + yRaw * rawWidth + 1];
      pixels_c[(x + y * imageWidth) * 4 + 0] = rawImageData[xRaw + yRaw * rawWidth + rawWidth + 0];
      pixels_c[(x + y * imageWidth) * 4 + 1] = rawImageData[xRaw + yRaw * rawWidth + rawWidth + 0];
      pixels_d[(x + y * imageWidth) * 4 + 2] = rawImageData[xRaw + yRaw * rawWidth + rawWidth + 1];
    }
  }

  S2D_SetText(info, "frame = %d", frame);
  frame++;
}

static void render()
{
  // S2D_DrawTriangle(
  //     320, 50, 1, 0, 0, 1,
  //     540, 430, 0, 1, 0, 1,
  //     100, 430, 0, 0, 1, 1);

  // drawLine(fmod(a_x, 640), fmod(a_y, 480), fmod(a_x, 640) + 50, fmod(a_y, 480) + 50, a_r % 255, 255, 255);
  S2D_DrawText(info);

  int scale = 2;
  debugging_draw_pixels(0, imageHeight * 0 * scale, imageWidth, imageHeight, pixels_a, scale);
  debugging_draw_pixels(0, imageHeight * 1 * scale, imageWidth, imageHeight, pixels_b, scale);
  debugging_draw_pixels(0, imageHeight * 2 * scale, imageWidth, imageHeight, pixels_c, scale);
  debugging_draw_pixels(0, imageHeight * 3 * scale, imageWidth, imageHeight, pixels_d, scale);
}

void debugging_run(char *imageDataFilePath)
{
  imageDataModule.load_file(imageDataFilePath);
  imageDataModule.load_image_index(0);
  rawImageData = imageDataModule.get_image_data_buffer();

  setup();

  S2D_Window *window = S2D_CreateWindow(
      "Hello Triangle", 1280, 800, update, render, 0);

  S2D_Show(window);
}