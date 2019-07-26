#include "draw_pixels.h"
#include <simple2d.h>

void draw_pixels(int x, int y, int width, int height, unsigned char *pixels_rgba)
{
  static S2D_Image *img;

  if (img != NULL)
  {
    SDL_FreeSurface(img->surface);
    S2D_GL_FreeTexture(&img->texture_id);
    free(img);
  }

  img = (S2D_Image *)malloc(sizeof(S2D_Image));

  int depth = 32;
  int pitch = 4 * width;
  // unsigned int rmask = 0x000000ff;
  // unsigned int gmask = 0x0000ff00;
  // unsigned int bmask = 0x00ff0000;
  //  // Defaults
  unsigned int rmask = 0;
  unsigned int bmask = 0;
  unsigned int gmask = 0;
  unsigned int amask = 0xff000000;

  SDL_Surface *surface = SDL_CreateRGBSurfaceFrom(pixels_rgba, width, height, depth, pitch, rmask, gmask, bmask, amask);
  img->surface = surface;

  // Initialize values
  // img->path = path;
  img->x = x;
  img->y = y;
  img->color.r = 1.f;
  img->color.g = 1.f;
  img->color.b = 1.f;
  img->color.a = 1.f;
  img->orig_width = img->surface->w;
  img->orig_height = img->surface->h;
  img->width = img->orig_width;
  img->height = img->orig_height;
  img->rotate = 0;
  img->rx = 0;
  img->ry = 0;
  img->texture_id = 0;
  img->format = GL_RGBA;

  S2D_GL_CreateTexture(&img->texture_id, img->format,
                       img->orig_width, img->orig_height,
                       img->surface->pixels, GL_NEAREST);

  S2D_GL_DrawImage(img);
}