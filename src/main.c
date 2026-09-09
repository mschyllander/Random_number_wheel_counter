#include <zephyr/kernel.h>
#include <zephyr/random/random.h>
#include <zephyr/sys/printk.h>
#include <stdint.h>
#include "screen.h"
#include "matrix.h"
#include "st7735.h"
#include "number_wheels.h"

#define HOLD_TIME_MS 2500

int main(void)
{
    st7735_init();
    matrix_init(SCREEN_WIDTH, SCREEN_HEIGHT);
    number_wheels_init(SCREEN_WIDTH, SCREEN_HEIGHT);

    int64_t next_spin = 0;
    bool was_spinning = false;

    while (1) {
        int64_t now = k_uptime_get();
        bool busy = number_wheels_busy(now);

        if (was_spinning && !busy) {
            next_spin = now + HOLD_TIME_MS;
        }

        if (!busy && now >= next_spin) {
            uint32_t rnd = sys_rand32_get() % 1000000U;
            number_wheels_start(rnd, now);
            printk("New target: %06u\n", (unsigned int)rnd);
            busy = true;
        }
        was_spinning = busy;

        screen_clear();
        matrix_update_and_draw(screen_pixel);
        number_wheels_draw(screen_pixel, now);
        screen_present();
        /* Bildöverföringen tar redan tid; håll extra väntan kort för jämnare rörelse. */
        k_msleep(8);
    }
    return 0;
}

