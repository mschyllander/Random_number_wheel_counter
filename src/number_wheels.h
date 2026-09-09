#ifndef NUMBER_WHEELS_H
#define NUMBER_WHEELS_H

#include <stdbool.h>
#include <stdint.h>

typedef void (*wheels_pixel_fn)(int x, int y, uint16_t color);

void number_wheels_init(int width, int height);
void number_wheels_start(uint32_t number, int64_t now);
void number_wheels_draw(wheels_pixel_fn pixel, int64_t now);
bool number_wheels_busy(int64_t now);

#endif
