#ifndef INIT_CONFIG_H
#define INIT_CONFIG_H

#define OUT_PIN 7

#include "pio_matrix.pio.h"
#include "ssd1306.h"

uint pio_init(PIO pio);
void display_init(ssd1306_t *ssd);

#endif