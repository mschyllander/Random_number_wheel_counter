#include "digit_font.h"
#include "matrix.h"
#include <zephyr/random/random.h>

#define MAX_COLUMNS 20
#define COLUMN_SPACING 8
#define CHAR_HEIGHT 8
#define TRAIL_LENGTH 8

struct rain_column {
    int x;
    int y;
    int speed;
    uint8_t digits[TRAIL_LENGTH];
};

static struct rain_column columns[MAX_COLUMNS];
static int column_count;
static int screen_height;

/* Små siffror, 5 × 7 pixlar. */


static void randomize_digits(struct rain_column *column)
{
    for (int i = 0; i < TRAIL_LENGTH; i++) {
        column->digits[i] = sys_rand32_get() % 10U;
    }
}

void matrix_init(int width, int height)
{
    screen_height = height;

    column_count = width / COLUMN_SPACING;

    if (column_count > MAX_COLUMNS) {
        column_count = MAX_COLUMNS;
    }

    if (width <= 0 || height <= 0) {
        column_count = 0;
        return;
    }

    for (int i = 0; i < column_count; i++) {
        columns[i].x = i * COLUMN_SPACING + 1;

        /* Sprid ut starten ovanför och inne på skärmen. */
        columns[i].y =
            (int)(sys_rand32_get() % (uint32_t)(height * 2))
            - height;

        columns[i].speed = 1 + sys_rand32_get() % 3U;

        randomize_digits(&columns[i]);
    }
}

static void draw_digit(matrix_pixel_fn draw_pixel,
                       int x, int y,
                       uint8_t digit, uint16_t color)
{
    for (int col = 0; col < 5; col++) {
        uint8_t bits = digit_font[digit][col];

        for (int row = 0; row < 7; row++) {
            if (bits & (1U << row)) {
                draw_pixel(x + col, y + row, color);
            }
        }
    }
}

void matrix_update_and_draw(matrix_pixel_fn draw_pixel)
{
    for (int i = 0; i < column_count; i++) {
        struct rain_column *column = &columns[i];

        column->y += column->speed;

        /* Starta om när hela svansen har lämnat bilden. */
        if (column->y >=
            screen_height + TRAIL_LENGTH * CHAR_HEIGHT) {
            column->y =
                -CHAR_HEIGHT - (int)(sys_rand32_get() % 64U);

            column->speed = 1 + sys_rand32_get() % 3U;
            randomize_digits(column);
        }

        /* Ändra ett tecken ibland för lite liv i regnet. */
        if (sys_rand32_get() % 12U == 0) {
            int index = sys_rand32_get() % TRAIL_LENGTH;
            column->digits[index] = sys_rand32_get() % 10U;
        }

        for (int j = 0; j < TRAIL_LENGTH; j++) {
            int y = column->y - j * CHAR_HEIGHT;
            if (y >= screen_height || y + 7 <= 0) {
                continue;
            }
            uint16_t color;

            if (j == 0) {
                /* Ljust grönt huvud. */
                color = 006400; // old value: 0xBFF7
            } else {
                /* Grönt med avtagande ljusstyrka, RGB565. */
                uint16_t green =
                    63U * (TRAIL_LENGTH - j) / TRAIL_LENGTH;

                color = (uint16_t)(green << 5);
            }

            draw_digit(draw_pixel,
                       column->x, y,
                       column->digits[j], color);
        }
    }
}
