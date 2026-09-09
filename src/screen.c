#include "screen.h"
#include <string.h>

/* Store RGB565 high byte first, ready for the synchronous SPI transfer.
 * Only this module owns the image and landscape coordinate conversion. */
static uint8_t framebuffer[ST7735_WIDTH * ST7735_HEIGHT * 2];

void screen_clear(void)
{
    memset(framebuffer, 0, sizeof(framebuffer));
}

void screen_pixel(int x, int y, uint16_t color)
{
    if (x < 0 || x >= SCREEN_WIDTH || y < 0 || y >= SCREEN_HEIGHT) {
        return;
    }
    int physical_x = ST7735_WIDTH - 1 - y;
    int index = 2 * (x * ST7735_WIDTH + physical_x);
    framebuffer[index] = (uint8_t)(color >> 8);
    framebuffer[index + 1] = (uint8_t)color;
}

void screen_present(void)
{
    st7735_draw_frame_bytes(framebuffer);
}
