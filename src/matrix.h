#ifndef MATRIX_H
#define MATRIX_H

#include <stdint.h>

typedef void (*matrix_pixel_fn)(int x, int y, uint16_t color);

void matrix_init(int width, int height);
void matrix_update_and_draw(matrix_pixel_fn draw_pixel);

#endif