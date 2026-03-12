/* SPDX-License-Identifier: MIT-0 | Copyright (c) 2026 bareware */

#include <stdint.h>

extern uint32_t _estack;
extern uint32_t _sbss, _ebss, _sdata, _edata, _sidata;

extern void main(void);

typedef void (*isr_handler)(void);

void default_handler(void) {
    while(1) { }
}

[[gnu::weak, gnu::alias("default_handler")]] void nmi_handler(void);
[[gnu::weak, gnu::alias("default_handler")]] void hard_fault_handler(void);
[[gnu::weak, gnu::alias("default_handler")]] void sv_call_handler(void);
[[gnu::weak, gnu::alias("default_handler")]] void pend_sv_handler(void);
[[gnu::weak, gnu::alias("default_handler")]] void sys_tick_handler(void);

[[noreturn]]
void reset_handler(void) {
    uint32_t *src = &_sidata;
    uint32_t *dst = &_sdata;

    // copy .data section to RAM region
    while (dst < &_edata) {
        *dst++ = *src++;
    }

    // memset .bss to zero
    dst = &_sbss;
    while (dst < &_ebss) {
        *dst++ = 0;
    }

    // jump to main
    main();
    while (1) { }
}


[[gnu::section(".isr_vector"), gnu::used]]
const isr_handler isr_vector[] = {
    (isr_handler)&_estack,
    reset_handler,
    nmi_handler,
    hard_fault_handler,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    sv_call_handler,
    0,
    0,
    pend_sv_handler,
    sys_tick_handler,
};


