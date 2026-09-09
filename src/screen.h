#ifndef SCREEN_H
#define SCREEN_H

#include <stdint.h>
#include "st7735.h"

#define SCREEN_WIDTH ST7735_HEIGHT
#define SCREEN_HEIGHT ST7735_WIDTH

void screen_clear(void);
void screen_pixel(int x, int y, uint16_t color);
void screen_present(void);

#endif
