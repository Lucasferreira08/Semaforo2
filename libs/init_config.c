#include "init_config.h"

uint pio_init(PIO pio)
{
    set_sys_clock_khz(128000, false); // Configura o clock do sistema para 128 MHz

    uint offset = pio_add_program(pio, &pio_matrix_program);

    uint sm = pio_claim_unused_sm(pio, true);

    pio_matrix_program_init(pio, sm, offset, OUT_PIN);

    return sm;
}

void display_init(ssd1306_t *ssd)
{
    i2c_init(I2C_PORT, 400 * 1000);
 
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);                    // Set the GPIO pin function to I2C
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);                    // Set the GPIO pin function to I2C
    gpio_pull_up(I2C_SDA);                                        // Pull up the data line
    gpio_pull_up(I2C_SCL);                                        // Pull up the clock line

    ssd1306_init(ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT); // Inicializa o display
    ssd1306_config(ssd);                                         // Configura o display
    ssd1306_send_data(ssd);                                      // Envia os dados para o display

    // Limpa o display. O display inicia com todos os pixels apagados.
    ssd1306_fill(ssd, false);
    ssd1306_send_data(ssd);
}

// gpio_init(LED_GREEN);
    // gpio_set_dir(LED_GREEN, GPIO_OUT);
    // gpio_init(LED_BLUE);
    // gpio_set_dir(LED_BLUE, GPIO_OUT);
    // gpio_init(LED_RED);
    // gpio_set_dir(LED_RED, GPIO_OUT);