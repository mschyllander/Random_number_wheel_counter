#include "stars.h"
#include <zephyr/random/random.h>

#define STAR_COUNT 40

struct star {
    int x;
    int y;
    int speed;
    uint16_t color;
};

static struct star stars[STAR_COUNT];

static int screen_width;
static int screen_height;

void stars_init(int width, int height)
{
    screen_width = width;
    screen_height = height;

    for (int i = 0; i < STAR_COUNT; i++) {
        stars[i].x = sys_rand32_get() % screen_width;
        stars[i].y = sys_rand32_get() % screen_height;

        /* Tre lager med olika hastighet och ljusstyrka. */
        stars[i].speed = 1 + (i % 3);

        switch (stars[i].speed) {
        case 1:
            stars[i].color = 0x4208; /* Mörkgrå */
            break;

        case 2:
            stars[i].color = 0x8410; /* Grå */
            break;

        default:
            stars[i].color = 0xFFFF; /* Vit */
            break;
        }
    }
}

void stars_update_and_draw(stars_pixel_fn draw_pixel)
{
    for (int i = 0; i < STAR_COUNT; i++) {
        struct star *s = &stars[i];

        /* Minska x för att flytta åt vänster. */
        s->x -= s->speed;

        /* Börja om på höger sida. */
        if (s->x < 0) {
            s->x += screen_width;
            s->y = sys_rand32_get() % screen_height;
        }

        draw_pixel(s->x, s->y, s->color);
    }
}