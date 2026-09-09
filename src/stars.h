#ifndef STARS_H
#define STARS_H

#include <stdint.h>

/* Funktion som kan rita en pixel. */
typedef void (*stars_pixel_fn)(int x, int y, uint16_t color);

void stars_init(int width, int height);
void stars_update_and_draw(stars_pixel_fn draw_pixel);

#endif