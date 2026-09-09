#include "digit_font.h"
#include "number_wheels.h"

#define WHEEL_COUNT 6
#define SCALE 4
#define PITCH 36
#define CELL_WIDTH 25
#define WINDOW_HEIGHT 64
#define SPIN_MS 1400
#define STOP_GAP_MS 220
#define UNIT 256

struct wheel {
    int digit;
    int steps;
};

static struct wheel wheels[WHEEL_COUNT];
static int origin_x;
static int center_y;
static int64_t started;
static bool spinning;



static uint16_t gray(int level)
{
    return (uint16_t)(((level >> 3) << 11) |
                      ((level >> 2) << 5) | (level >> 3));
}

void number_wheels_init(int width, int height)
{
    origin_x = (width - WHEEL_COUNT * CELL_WIDTH) / 2;
    center_y = height / 2;
    spinning = false;
    for (int i = 0; i < WHEEL_COUNT; i++) {
        wheels[i].digit = 0;
        wheels[i].steps = 0;
    }
}

bool number_wheels_busy(int64_t now)
{
    return spinning &&
           now - started < SPIN_MS + (WHEEL_COUNT - 1) * STOP_GAP_MS;
}

/* Start only after the preceding spin has finished. */
void number_wheels_start(uint32_t number, int64_t now)
{
    if (number_wheels_busy(now)) {
        return;
    }

    for (int i = WHEEL_COUNT - 1; i >= 0; i--) {
        struct wheel *w = &wheels[i];
        w->digit = (w->digit + w->steps) % 10;
        int target = number % 10U;
        number /= 10U;

        /* Full turns followed by the distance to the target digit. */
        w->steps = 20 + i * 10 + (target - w->digit + 10) % 10;
    }
    started = now;
    spinning = true;
}

void number_wheels_draw(wheels_pixel_fn pixel, int64_t now)
{
    int top = center_y - WINDOW_HEIGHT / 2;
    int bottom = top + WINDOW_HEIGHT;

    for (int i = 0; i < WHEEL_COUNT; i++) {
        struct wheel *w = &wheels[i];
        int left = origin_x + i * CELL_WIDTH;
        int position = w->digit * PITCH * UNIT;

        if (spinning) {
            int duration = SPIN_MS + i * STOP_GAP_MS;
            int64_t elapsed = now - started;
            if (elapsed < 0) {
                elapsed = 0;
            }
            if (elapsed > duration) {
                elapsed = duration;
            }

            /* Cubic easing: quick start, gradual stop. Integer arithmetic. */
            int64_t remaining = (duration - elapsed) * 1024 / duration;
            int64_t ease = 1024 -
                remaining * remaining * remaining / (1024 * 1024);
            position += (int)((int64_t)w->steps * PITCH * UNIT *
                              ease / 1024);
        }

        int base = position / (PITCH * UNIT);
        int offset = (position / UNIT) % PITCH;

        /* Neighbouring digits are clipped at the wheel's window. */
        for (int neighbour = -1; neighbour <= 2; neighbour++) {
            int digit = (base + neighbour + 10) % 10;
            int digit_y = center_y - 7 * SCALE / 2 +
                          neighbour * PITCH - offset;

            for (int col = 0; col < 5; col++) {
                for (int row = 0; row < 7; row++) {
                    if (!(digit_font[digit][col] & (1U << row))) {
                        continue;
                    }
                    for (int dy = 0; dy < SCALE; dy++) {
                        int y = digit_y + row * SCALE + dy;
                        if (y <= top || y >= bottom - 1) {
                            continue;
                        }
                        int distance = y - center_y;
                        if (distance < 0) {
                            distance = -distance;
                        }
                        int brightness = distance <= 14 ? 255 :
                                         255 - (distance - 14) * 12;
                        for (int dx = 0; dx < SCALE; dx++) {
                            pixel(left + 2 + col * SCALE + dx, y,
                                  gray(brightness));
                        }
                    }
                }
            }
        }
    }
}

