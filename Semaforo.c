#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/clocks.h"
#include "pico/bootrom.h"
#include "pio_matrix.pio.h"
#include "hardware/pio.h"
#include "hardware/pwm.h"
// #include "hardware/adc.h"
//#include "hardware/i2c.h"


#include "global_manage.h"
#include "matriz.h"
#include "init_config.h"

#include "FreeRTOS.h"
#include "task.h"

#define BUTTON_A    7   // Botão A para alternar modos

#define BUZZER_PIN 21
#define BUZZER_FREQUENCY 100

// Definição dos pinos
#define LED_RED     13  // Pino para LED vermelho do RGB
#define LED_GREEN   11  // Pino para LED verde do RGB
#define LED_BLUE    12  // Pino para LED azul do RGB


// Tarefa para controle do semáforo
void vTrafficLightTask()
{
    PIO pio = pio0;

    uint sm = pio_init(pio);

    while (1) {
        if (get_dia_est()==DIA)
        {
            // Verde
            //gpio_put(LED_GREEN, true);
            mostrar_semaforo(VERDE, pio, sm);
            printf("Sinal: Verde\n");
            //sleep_ms(5000); // Verde aceso por 5 segundos
            //gpio_put(LED_GREEN, false);
            vTaskDelay(pdMS_TO_TICKS(5000));
        
            // Amarelo
            //gpio_put(LED_GREEN, true);
            //gpio_put(LED_BLUE, true);
            mostrar_semaforo(AMARELO, pio, sm);
            printf("Sinal: Amarelo\n");
            //sleep_ms(2000); // Amarelo aceso por 2 segundos
            //gpio_put(LED_GREEN, false);
            //gpio_put(LED_BLUE, false);
            vTaskDelay(pdMS_TO_TICKS(2000));

            // Vermelho
            //gpio_put(LED_RED, true);
            mostrar_semaforo(VERMELHO, pio, sm);
            printf("Sinal: Vermelho\n");
            //sleep_ms(5000); // Vermelho aceso por 5 segundos
            //gpio_put(LED_RED, false);
            vTaskDelay(pdMS_TO_TICKS(5000));
        }
    }
}

// if (get_dia_est()==NOITE) 
        // {
        //     // Amarelo
        //     mostrar_semaforo(AMARELO, pio, sm);
        //     printf("Sinal: Amarelo, night mode\n");
        //     sleep_ms(2000); // Amarelo aceso por 2 segundos
        //     //vTaskDelay(pdMS_TO_TICKS(1000));

        //     // Amarelo
        //     mostrar_semaforo(CINZA, pio, sm);
        //     printf("Sinal: Cinza, night mode\n");
        //     sleep_ms(2000); // Amarelo aceso por 2 segundos
        //     //vTaskDelay(pdMS_TO_TICKS(1000));
        // } else

// void vBuzzerTask()
// {
//     gpio_set_function(BUZZER_PIN, GPIO_FUNC_PWM);

//     uint slice_num = pwm_gpio_to_slice_num(BUZZER_PIN);

//     pwm_config config = pwm_get_default_config();
//     pwm_config_set_clkdiv(&config, clock_get_hz(clk_sys) / (BUZZER_FREQUENCY * 4096)); // Divisor de clock
//     pwm_init(slice_num, &config, true);

//     pwm_set_gpio_level(BUZZER_PIN, 0);

//     // uint slice_num = pwm_gpio_to_slice_num(BUZZER_PIN);

//     while (1)
//     {
//         if (nightMode) 
//         {
//             printf("Buzzer: night mode\n");
//             pwm_set_gpio_level(BUZZER_PIN, 2048);
//             //vTaskDelay(pdMS_TO_TICKS(200));  // 200ms ligado
//             sleep_ms(200); 
//             pwm_set_gpio_level(BUZZER_PIN, 0);
//             //vTaskDelay(pdMS_TO_TICKS(1800)); // 1800ms desligado (total 2s)
//             sleep_ms(1800); // Vermelho aceso por 5 segundos
//         }
//         else 
//         {
//             printf("Buzzer: day mode\n");
//             if (estadoAtual==VERDE) 
//             {
//                 pwm_set_gpio_level(BUZZER_PIN, 2048);
//                 //vTaskDelay(pdMS_TO_TICKS(200));  // 200ms ligado
//                 sleep_ms(200); // Vermelho aceso por 5 segundos
//                 pwm_set_gpio_level(BUZZER_PIN, 0);
//                 //vTaskDelay(pdMS_TO_TICKS(1800)); // 1800ms desligado (total 2s)
//                 sleep_ms(200); // Vermelho aceso por 5 segundos
//             }
//             else if (estadoAtual==AMARELO)
//             {
//                 for (int i = 0; i < 4; i++)  // 4 beeps rápidos
//                 {
//                     pwm_set_gpio_level(BUZZER_PIN, 2048);
//                     //vTaskDelay(pdMS_TO_TICKS(100));  // 100ms ligado
//                     sleep_ms(100); // Vermelho aceso por 5 segundos
//                     pwm_set_gpio_level(BUZZER_PIN, 0);
//                     //vTaskDelay(pdMS_TO_TICKS(100));  // 100ms desligado
//                     sleep_ms(100); // Vermelho aceso por 5 segundos
//                 }
//                 //vTaskDelay(pdMS_TO_TICKS(600));  // Pausa no final (total 1200ms)
//                 sleep_ms(600); // Vermelho aceso por 5 segundos
//             }
//             else if (estadoAtual==VERMELHO)
//             {
//                 pwm_set_gpio_level(BUZZER_PIN, 2048);
//                 //vTaskDelay(pdMS_TO_TICKS(500));  // 500ms ligado
//                 sleep_ms(500); // Vermelho aceso por 5 segundos
//                 pwm_set_gpio_level(BUZZER_PIN, 0);
//                 //vTaskDelay(pdMS_TO_TICKS(1500)); // 1.5s desligado
//                 sleep_ms(1500); // Vermelho aceso por 5 segundos
//             }
//             else
//             {
//                 //vTaskDelay(pdMS_TO_TICKS(500));
//                 sleep_ms(50); // Vermelho aceso por 5 segundos
//             }
//         }
//     }
// }

// // Tarefa para monitorar o botão A
// void vButtonTask()
// {
//     gpio_init(BUTTON_A);
//     gpio_set_dir(BUTTON_A, GPIO_IN);
//     gpio_pull_up(BUTTON_A);

//     while (true)
//     {
//         // Lê o estado atual do botão
//         bool currentButtonState = gpio_get(BUTTON_A);
        
//         // Detecta borda de descida (botão pressionado)
//         if (lastButtonState && !currentButtonState)
//         {
//             // Alterna o modo
//             nightMode = !nightMode;
//             printf("Modo alterado para: %s\n", nightMode ? "Noturno" : "Normal");
            
//             // Pequeno delay para debounce
//             //vTaskDelay(pdMS_TO_TICKS(50));
//             sleep_ms(50); // Vermelho aceso por 5 segundos
//         }
        
//         lastButtonState = currentButtonState;
//         //vTaskDelay(pdMS_TO_TICKS(20));  // Polling a cada 20ms
//         sleep_ms(20); // Vermelho aceso por 5 segundos
//     }
// }

// Tarefa para atualizar o display (simulado com printf)
void vDisplayTask()
{
    ssd1306_t ssd;

    display_init(&ssd);

    while (true)
    {
        if (get_cor_sem()==VERMELHO) {desenhar_vermelho(&ssd);printf("Display: Vermelho\n");}
        else if (get_cor_sem()==AMARELO) {desenhar_amarelo(&ssd);printf("Display: Amarelo\n");}
        else if (get_cor_sem()==VERDE) {desenhar_verde(&ssd);printf("Display: Verde\n");}
        vTaskDelay(pdMS_TO_TICKS(500));  // Atualiza a cada segundo
        //sleep_ms(50); 
    }
}

int main()
{
    // Inicialização da UART para printf
    stdio_init_all();
    printf("Sistema de Semáforo Inteligente Iniciado\n");

    set_dia();
    set_verde();
    
    // Criação das tarefas
    xTaskCreate(vTrafficLightTask, "Traffic Light Task", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL);
    // xTaskCreate(vBuzzerTask, "Buzzer Task", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL);
    // xTaskCreate(vButtonTask, "Button Task", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL);  // Prioridade maior para melhor resposta
    xTaskCreate(vDisplayTask, "Display Task", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL);
    
    // Inicia o escalonador
    vTaskStartScheduler();
    
    // Nunca deve chegar aqui
    panic_unsupported();
}