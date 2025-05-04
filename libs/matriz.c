#include "matriz.h"

// Número de LEDs na matriz (5x5)
#define NUM_PIXELS 25

// Luz vermelha (parte superior do semáforo)
static double desenho_vermelho[25] = {0.0, 0.0, 0.0, 0.0, 0.0,
                              0.0, 1.0, 1.0, 1.0, 0.0, 
                              0.0, 0.0, 0.0, 0.0, 0.0,
                              0.0, 0.0, 0.0, 0.0, 0.0,
                              0.0, 0.0, 0.0, 0.0, 0.0};

// Luz amarela (parte central do semáforo)
static double desenho_amarelo[25] = {0.0, 0.0, 0.0, 0.0, 0.0,
                             0.0, 0.0, 0.0, 0.0, 0.0, 
                             0.0, 1.0, 1.0, 1.0, 0.0,
                             0.0, 0.0, 0.0, 0.0, 0.0,
                             0.0, 0.0, 0.0, 0.0, 0.0};

// Luz verde (parte inferior do semáforo)
static double desenho_verde[25] =   {0.0, 0.0, 0.0, 0.0, 0.0,
                             0.0, 0.0, 0.0, 0.0, 0.0, 
                             0.0, 0.0, 0.0, 0.0, 0.0,
                             0.0, 1.0, 1.0, 1.0, 0.0,
                             0.0, 0.0, 0.0, 0.0, 0.0};

// Estrutura do semáforo completo (para visualização)
static double desenho_semaforo[25] = {0.0, 0.0, 0.0, 0.0, 0.0,
                              0.0, 0.0, 0.0, 0.0, 0.0, 
                              0.0, 0.0, 0.0, 0.0, 0.0,
                              0.0, 0.0, 0.0, 0.0, 0.0,
                              0.0, 0.0, 0.0, 0.0, 0.0};

// Rotina para definir a intensidade das cores do LED (RGB)
uint32_t matrix_rgb(double b, double r, double g)
{
  unsigned char R, G, B;
  R = r * 255; // Converte a intensidade de vermelho para 8 bits
  G = g * 255; // Converte a intensidade de verde para 8 bits
  B = b * 255; // Converte a intensidade de azul para 8 bits
  return (G << 24) | (R << 16) | (B << 8); // Retorna o valor RGB no formato 32 bits
}

void desenho_pio(double *desenho, uint32_t valor_led, PIO pio, uint sm, double r, double g, double b){
    for (int16_t i = 0; i < NUM_PIXELS; i++) {
        // Define a cor do LED com base no vetor de desenho
        if (desenho[24-i]>0.000) 
        {
            valor_led = matrix_rgb(b, r, g);
        }
        else
        {
            valor_led = matrix_rgb(0.0, 0.0, 0.0);
        }
        pio_sm_put_blocking(pio, sm, valor_led); // Envia o valor para a matriz de LEDs via PIO
    }
}

// Função para exibir o semáforo na matriz de LEDs
void mostrar_semaforo(EstadoSemaforo estado, PIO pio, uint sm)
{
    uint32_t valor_led;
    
    switch (estado) {
        case VERMELHO: // Vermelho
            desenho_pio(desenho_vermelho, valor_led, pio, sm, 0.1, 0.0, 0.0); // Vermelho (r=1.0)
            set_vermelho();
            break;
        case AMARELO: // Amarelo
            desenho_pio(desenho_amarelo, valor_led, pio, sm, 0.05, 0.05, 0.0); // Amarelo (r=1.0, g=1.0)
            set_amarelo();
            break;
        case VERDE: // Verde
            desenho_pio(desenho_verde, valor_led, pio, sm, 0.0, 0.1, 0.0); // Verde (g=1.0)
            set_verde();
            break;
        case CINZA: // Estrutura completa do semáforo (para visualização)
            desenho_pio(desenho_semaforo, valor_led, pio, sm, 0.02, 0.02, 0.02); // Cinza claro
            set_cinza();
            break;
    }
}