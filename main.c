#include <stdio.h>
#include <math.h>
#include "driver/ledc.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define LEDC_TIMER              LEDC_TIMER_0
#define LEDC_MODE               LEDC_LOW_SPEED_MODE
#define LEDC_OUTPUT_IO          25 // Define the output GPIO
#define LEDC_CHANNEL            LEDC_CHANNEL_0
#define LEDC_DUTY_RES           LEDC_TIMER_10_BIT // Set duty resolution to 13 bits
#define LEDC_DUTY               0 // Set duty to 50%. (2 ** 13) * 50% = 4096
#define LEDC_FREQUENCY          10000 // Frequency in Hertz. Set frequency at 4 kHz
#define RES_PWM                 10
#define PIN_PUL                 18

esp_err_t initPwn(void);
esp_err_t controlPwm(uint32_t duty);
esp_err_t intGpio(int);
esp_err_t lecturaPulsador(int);

void app_main(void)
{
    initPwn();

    while (1)
    {
        uint32_t duty=0;
        int var=100;

        for(duty=0 ; duty <pow(2,RES_PWM)-1 ; duty = duty+var)
        {
            controlPwm(duty);
            vTaskDelay(pdMS_TO_TICKS(1000));
            printf("duty %ld\n" , duty);
        }
    }
    
}

esp_err_t initPwn(void)
{
    ledc_timer_config_t ledc_timer = {
        .speed_mode       = LEDC_MODE,
        .timer_num        = LEDC_TIMER,
        .duty_resolution  = LEDC_DUTY_RES,
        .freq_hz          = LEDC_FREQUENCY,  // Set output frequency at 4 kHz
        .clk_cfg          = LEDC_AUTO_CLK
    };
    ledc_timer_config(&ledc_timer);

    ledc_channel_config_t ledc_channel = {
        .speed_mode     = LEDC_MODE,
        .channel        = LEDC_CHANNEL,
        .timer_sel      = LEDC_TIMER,
        .intr_type      = LEDC_INTR_DISABLE,
        .gpio_num       = LEDC_OUTPUT_IO,
        .duty           = 0, // Set duty to 0%
        .hpoint         = 0
    };
    ledc_channel_config(&ledc_channel);

    return ESP_OK;
}

esp_err_t controlPwm(uint32_t duty)
{
    ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, duty);
    ledc_update_duty(LEDC_MODE, LEDC_CHANNEL);
    return ESP_OK;
}

esp_err_t intGpio(int pin)
{
    gpio_set_direction(PIN_PUL, GPIO_MODE_INPUT );
    gpio_set_pull_mode(PIN_PUL, GPIO_PULLDOWN_ONLY);
    return ESP_OK;
}

int lecturaPulsador(int pin)
{
   return gpio_get_level(pin);
}
