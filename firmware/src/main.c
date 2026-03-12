/* SPDX-License-Identifier: MIT-0 | Copyright (c) 2026 bareware */

#include "mod-mcu/mcu_stm32g0x0.h"

void delay_clk3(uint32_t num) {
    __asm volatile (
        ".syntax unified \n"
        "1: subs %[num], #1 \n"
        "   bne 1b \n"
        : [num] "+r" (num)
    );
}

// 16MHz / 1000 / 3 cycles per iteration
#define delay_ms(x) delay_clk3((x) * 5333)

void main(void) {
    rcc_gpio_clk_enable(rcc_gpio_b);
    gpio_setup_output(gpio_b, gpio_output_push_pull, gpio_speed_low, gpio_pin_0);

    while (1) {
        gpio_set(gpio_b, gpio_pin_0);
        delay_ms(500);
        gpio_clear(gpio_b, gpio_pin_0);
        delay_ms(500);
    }
}

