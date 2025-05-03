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


#include "global_manage.h"
#include "matriz.h"
#include "init_config.h"

#include "FreeRTOS.h"
#include "task.h"

void vTrafficLightTask()
{
    PIO pio = pio0;

    uint sm = pio_init(pio);

    while (1) {
        if (get_dia_est()==DIA)
        {
            mostrar_semaforo(VERDE, pio, sm);
            printf("Sinal: Verde\n");
            vTaskDelay(pdMS_TO_TICKS(5000));
        
            mostrar_semaforo(AMARELO, pio, sm);
            printf("Sinal: Amarelo\n");
            vTaskDelay(pdMS_TO_TICKS(2000));

            mostrar_semaforo(VERMELHO, pio, sm);
            printf("Sinal: Vermelho\n");
            vTaskDelay(pdMS_TO_TICKS(5000));
        }
        else 
        {
            mostrar_semaforo(CINZA, pio, sm);
            vTaskDelay(pdMS_TO_TICKS(1000));
        }
    }
}

void vLedsTask() 
{
    leds_init();

    while (true)
    {
        if (get_dia_est()==NOITE) 
        {
            gpio_put(LED_RED, true);
            gpio_put(LED_GREEN, true);
            vTaskDelay(pdMS_TO_TICKS(2000));

            gpio_put(LED_RED, false);
            gpio_put(LED_GREEN, false);
            vTaskDelay(pdMS_TO_TICKS(2000));
        }
    }
}

void vBuzzerTask()
{
    pwm_init_config();

    while (true)
    {
        if (get_dia_est()==NOITE) 
        {
            printf("Buzzer: night mode\n");
            pwm_set_gpio_level(BUZZER_PIN, 2048);
            vTaskDelay(pdMS_TO_TICKS(200));  

            pwm_set_gpio_level(BUZZER_PIN, 0);
            vTaskDelay(pdMS_TO_TICKS(1800)); 
        }
        else 
        {
            printf("Buzzer: day mode\n");
            if (get_cor_sem()==VERDE) 
            {
                pwm_set_gpio_level(BUZZER_PIN, 2048);
                vTaskDelay(pdMS_TO_TICKS(200));  // 200ms ligado

                pwm_set_gpio_level(BUZZER_PIN, 0);
                vTaskDelay(pdMS_TO_TICKS(200)); // 1800ms desligado (total 2s)
            }
            else if (get_cor_sem()==AMARELO)
            {
                for (int i = 0; i < 4; i++)  // 4 beeps rápidos
                {
                    pwm_set_gpio_level(BUZZER_PIN, 2048);
                    vTaskDelay(pdMS_TO_TICKS(100));  // 100ms ligado

                    pwm_set_gpio_level(BUZZER_PIN, 0);
                    vTaskDelay(pdMS_TO_TICKS(100));  // 100ms desligado
                }
                vTaskDelay(pdMS_TO_TICKS(600));  // Pausa no final (total 1200ms)
            }
            else if (get_cor_sem()==VERMELHO)
            {
                pwm_set_gpio_level(BUZZER_PIN, 2048);
                vTaskDelay(pdMS_TO_TICKS(500));  // 500ms ligado

                pwm_set_gpio_level(BUZZER_PIN, 0);
                vTaskDelay(pdMS_TO_TICKS(1500)); // 1.5s desligado
            }
        }
    }
}

void vButtonTask()
{
    button_init();

    bool lastButtonState = true;

    while (true)
    {
        bool currentButtonState = gpio_get(BUTTON_A);
        
        if (lastButtonState && !currentButtonState)
        {

            if (get_dia_est()==NOITE) set_dia();
            else set_noite();
        }
        
        lastButtonState = currentButtonState;
        vTaskDelay(pdMS_TO_TICKS(500));  // Polling a cada 20ms
    }
}

void vDisplayTask()
{
    ssd1306_t ssd;

    display_init(&ssd);

    while (true)
    {
        if (get_cor_sem()==VERMELHO) {desenhar_vermelho(&ssd);printf("Display: Vermelho\n");}
        else if (get_cor_sem()==AMARELO) {desenhar_amarelo(&ssd);printf("Display: Amarelo\n");}
        else if (get_cor_sem()==VERDE) {desenhar_verde(&ssd);printf("Display: Verde\n");}
        else {desenhar_amarelo(&ssd);printf("Display: Amarelo\n");}
        vTaskDelay(pdMS_TO_TICKS(500));  // Atualiza a cada segundo
    }
}

int main()
{
    stdio_init_all();
    printf("Sistema de Semáforo Inteligente Iniciado\n");

    set_dia();
    set_verde();
    
    xTaskCreate(vTrafficLightTask, "Traffic Light Task", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL);
    xTaskCreate(vDisplayTask, "Display Task", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL);
    xTaskCreate(vBuzzerTask, "Buzzer Task", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL);
    xTaskCreate(vButtonTask, "Button Task", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL);  // Prioridade maior para melhor resposta
    xTaskCreate(vLedsTask, "Leds Task", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL);  // Prioridade maior para melhor resposta
    
    vTaskStartScheduler();
    
    panic_unsupported();
}