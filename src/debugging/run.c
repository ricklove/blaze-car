#include "run.h"
#include <simple2d.h>
#include <math.h>
#include "draw_pixels.h"

static void drawLine(float x1, float y1, float x2, float y2, char r_255, char g_255, char b_255)
{
  float r = r_255 * 1.0f / 255;
  float g = g_255 * 1.0f / 255;
  float b = b_255 * 1.0f / 255;

  S2D_DrawLine(x1, y1, x2, y2, 10, r, g, b, 1, r, g, b, 1, r, g, b, 1, r, g, b, 1);
}

static float a_x = 0.0f;
static float a_y = 0.0f;
static int a_r = 0;
static S2D_Text *text;
static int frames = 0;
static unsigned char pixels[4 * 100];

static void setup()
{
  text = S2D_CreateText("resources/vera.ttf", "", 11);
  if (text == NULL)
  {
    printf("ERROR: Text was not created (is font file missing?)\n");
  }

  text->x = 10;
  text->y = 10;
}

static void update()
{
  a_x++;
  a_y += 0.5f;
  a_r += 3;

  for (int i = 0; i < 100; i++)
  {
    // rgba
    pixels[4 * i + 0] = a_r;
    pixels[4 * i + 1] = 255;
    pixels[4 * i + 2] = i;
    pixels[4 * i + 3] = 255;
  }

  S2D_SetText(text, "frames = %d", frames);
  frames++;
}

static void render()
{
  S2D_DrawTriangle(
      320, 50, 1, 0, 0, 1,
      540, 430, 0, 1, 0, 1,
      100, 430, 0, 0, 1, 1);

  drawLine(fmod(a_x, 640), fmod(a_y, 480), fmod(a_x, 640) + 50, fmod(a_y, 480) + 50, a_r % 255, 255, 255);
  S2D_DrawText(text);

  draw_pixels(50, 50, 10, 10, pixels);
}

void run_debug()
{
  setup();

  S2D_Window *window = S2D_CreateWindow(
      "Hello Triangle", 1280, 800, update, render, 0);

  S2D_Show(window);
}