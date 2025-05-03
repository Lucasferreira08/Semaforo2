#ifndef INIT_CONFIG_H
#define INIT_CONFIG_H

#define OUT_PIN 7

#define BUZZER_PIN 21
#define BUZZER_FREQUENCY 100

#define BUTTON_A 5   // Botão A para alternar modos

#define LED_RED     13  // Pino para LED vermelho do RGB
#define LED_GREEN   11  // Pino para LED verde do RGB
#define LED_BLUE    12  // Pino para LED azul do RGB

#include "pio_matrix.pio.h"
#include "ssd1306.h"

uint pio_init(PIO pio);

void display_init(ssd1306_t *ssd);

void pwm_init_config();

void button_init();

void leds_init();

#endif