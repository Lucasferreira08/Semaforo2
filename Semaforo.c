#include <stdbool.h>         // Define o tipo bool (true/false)
#include <stdio.h>           // Funções de E/S padrão (printf, etc.)
#include <stdlib.h>          // Funções utilitárias (malloc, free, etc.)
#include <math.h>            // Funções matemáticas (não usada aqui, mas incluída)
#include "pico/stdlib.h"     // SDK base da Raspberry Pi Pico (gpio, timer, etc.)
#include "hardware/clocks.h" // Controle de clocks do RP2040
#include "pico/bootrom.h"    // Funções do bootloader (panic_unsupported)
#include "pio_matrix.pio.h"  // Programa PIO customizado para matriz de LEDs
#include "hardware/pio.h"    // API de PIO do RP2040
#include "hardware/pwm.h"    // API de PWM para controle de buzzer e LEDs

#include "global_manage.h"   // Funções de gerenciamento de estado global (dia/noite, cor do semáforo)
#include "matriz.h"          // Funções para desenhar semáforo na matriz de LEDs via PIO
#include "init_config.h"     // Configuração inicial de pinos, PWM, display, botões

#include "FreeRTOS.h"        // Kernel FreeRTOS
#include "task.h"            // API de criação e controle de tarefas FreeRTOS


// Tarefa que controla o semáforo na matriz de LEDs via PIO
void vTrafficLightTask()
{
    PIO pio = pio0;            // Escolhe a instância PIO 0
    uint sm = pio_init(pio);   // Inicializa o state machine e retorna qual SM foi usado

    while (1) {
        // Se estiver no modo dia, cicla pelas cores do semáforo
        if (get_dia_est() == DIA) {
            mostrar_semaforo(VERDE, pio, sm);   // Desenha verde na matriz
            printf("Sinal: Verde\n");
            vTaskDelay(pdMS_TO_TICKS(5000));    // Aguarda 5 segundos

            if (get_dia_est() == NOITE) continue;  // Se mudou para noite durante a espera, reinicia

            mostrar_semaforo(AMARELO, pio, sm); // Desenha amarelo
            printf("Sinal: Amarelo\n");
            vTaskDelay(pdMS_TO_TICKS(2000));    // Aguarda 2 segundos

            if (get_dia_est() == NOITE) continue;

            mostrar_semaforo(VERMELHO, pio, sm);// Desenha vermelho
            printf("Sinal: Vermelho\n");
            vTaskDelay(pdMS_TO_TICKS(5000));    // Aguarda 5 segundos
        }
        else {
            // Se estiver no modo noite, mantém semáforo apagado (cinza)
            mostrar_semaforo(CINZA, pio, sm);
            vTaskDelay(pdMS_TO_TICKS(1000));    // Atualiza a cada 1 segundo
        }
    }
}

// Tarefa que faz piscar dois LEDs onboard no modo noite
void vLedsTask() 
{
    leds_init();  // Configura pinos dos LEDs

    while (true)
    {
        if (get_dia_est() == NOITE) 
        {
            // Liga ambos os LEDs por 2s
            gpio_put(LED_RED, true);
            gpio_put(LED_GREEN, true);
            vTaskDelay(pdMS_TO_TICKS(2000));

            // Desliga por 2s
            gpio_put(LED_RED, false);
            gpio_put(LED_GREEN, false);
            vTaskDelay(pdMS_TO_TICKS(2000));
        }
        else {
            // No modo dia, não faz nada (loop rápido)
            vTaskDelay(pdMS_TO_TICKS(100));
        }
    }
}

// Tarefa que controla o buzzer em diferentes modos e cores
void vBuzzerTask()
{
    pwm_init_config();  // Inicializa PWM para o buzzer

    while (true)
    {
        if (get_dia_est() == NOITE) 
        {
            // Noite: som intermitente fixo (200ms on, 1800ms off)
            printf("Buzzer: night mode\n");
            pwm_set_gpio_level(BUZZER_PIN, 2048);
            vTaskDelay(pdMS_TO_TICKS(200));  
            pwm_set_gpio_level(BUZZER_PIN, 0);
            vTaskDelay(pdMS_TO_TICKS(1800)); 
        }
        else 
        {
            // Dia: comportamento varia conforme cor do semáforo
            printf("Buzzer: day mode\n");

            if (get_cor_sem() == VERDE) 
            {
                // Verde: bip único (200ms on, 200ms off)
                pwm_set_gpio_level(BUZZER_PIN, 2048);
                vTaskDelay(pdMS_TO_TICKS(200));
                pwm_set_gpio_level(BUZZER_PIN, 0);
                vTaskDelay(pdMS_TO_TICKS(200));
            }
            else if (get_cor_sem() == AMARELO)
            {
                // Amarelo: 4 bips rápidos (100ms on/off) e pausa final
                for (int i = 0; i < 4; i++)
                {
                    pwm_set_gpio_level(BUZZER_PIN, 2048);
                    vTaskDelay(pdMS_TO_TICKS(100));
                    pwm_set_gpio_level(BUZZER_PIN, 0);
                    vTaskDelay(pdMS_TO_TICKS(100));
                }
                vTaskDelay(pdMS_TO_TICKS(600));  // Pausa de 600ms
            }
            else if (get_cor_sem() == VERMELHO)
            {
                // Vermelho: bip longo (500ms on, 1500ms off)
                pwm_set_gpio_level(BUZZER_PIN, 2048);
                vTaskDelay(pdMS_TO_TICKS(500));
                pwm_set_gpio_level(BUZZER_PIN, 0);
                vTaskDelay(pdMS_TO_TICKS(1500));
            }
        }
    }
}

// Tarefa que atualiza o display OLED SSD1306 conforme cor atual do semáforo
void vDisplayTask()
{
    ssd1306_t ssd;
    display_init(&ssd);  // Inicializa comunicação I2C e SSD1306

    while (true)
    {
        // Desenha a cor correspondente e exibe no console
        if (get_cor_sem() == VERMELHO) {
            desenhar_vermelho(&ssd);
            printf("Display: Vermelho\n");
        }
        else if (get_cor_sem() == AMARELO) {
            desenhar_amarelo(&ssd);
            printf("Display: Amarelo\n");
        }
        else if (get_cor_sem() == VERDE) {
            desenhar_verde(&ssd);
            printf("Display: Verde\n");
        }
        else {
            // Caso inesperado, desenha amarelo por segurança
            desenhar_amarelo(&ssd);
            printf("Display: Amarelo (fallback)\n");
        }
        vTaskDelay(pdMS_TO_TICKS(500));  // Atualiza a cada 500ms
    }
}

// Tarefa que monitora um botão para alternar entre dia e noite
void vButtonTask()
{
    button_init();           // Configura pino do botão
    bool lastButtonState = true;  // Estado anterior (pull-up)

    while (true)
    {
        bool currentButtonState = gpio_get(BUTTON_A);  // Leitura do botão
        
        // Detecta borda de descida (pressionado)
        if (lastButtonState && !currentButtonState)
        {
            if (get_dia_est() == NOITE)
                set_dia();     // Alterna para dia
            else
                set_noite();   // Alterna para noite
        }
        
        lastButtonState = currentButtonState;
        vTaskDelay(pdMS_TO_TICKS(20));  // Polling a cada 20ms para debouncing
    }
}

int main()
{
    stdio_init_all();  // Inicializa UART/USB para printf
    printf("Sistema de Semáforo Inteligente Iniciado\n");

    set_dia();    // Inicia no modo dia
    set_verde();  // Define cor inicial do semáforo como verde
    
    // Cria as tarefas do FreeRTOS
    xTaskCreate(vTrafficLightTask, "Traffic Light Task", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL);
    xTaskCreate(vDisplayTask,      "Display Task",       configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL);
    xTaskCreate(vBuzzerTask,       "Buzzer Task",        configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL);
    xTaskCreate(vButtonTask,       "Button Task",        configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);
    xTaskCreate(vLedsTask,         "Leds Task",          configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);
    
    // Inicia o scheduler do FreeRTOS
    vTaskStartScheduler();
    
    // Se voltar aqui, o bootrom exibe mensagem de erro
    panic_unsupported();
}
