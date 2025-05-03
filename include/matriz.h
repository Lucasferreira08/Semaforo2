#ifndef MATRIZ_H  // Se INIT_CONFIG_H não estiver definido
#define MATRIZ_H  // Defina INIT_CONFIG_H

#include "pio_matrix.pio.h"

#include "global_manage.h"

// Initializes the keypad
uint32_t matrix_rgb(double b, double r, double g);

// Checks if a specific key is pressed
void desenho_pio(double *desenho, uint32_t valor_led, PIO pio, uint sm, double r, double g, double b);

void mostrar_semaforo(EstadoSemaforo estado, PIO pio, uint sm);

#endif