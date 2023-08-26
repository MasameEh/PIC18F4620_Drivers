/* 
 * File:   application.c
 * Author: Mohamed Sameh
 *
 * Created on August 22, 2023, 10:49 PM
 */

#include "application.h"

led_t led_1 = 
{
    .port = PORTC_INDEX,
    .pin = GPIO_PIN0,
    .led_status = LED_OFF
};

led_t led_2 = 
{
    .port = PORTC_INDEX,
    .pin = GPIO_PIN1,
    .led_status = LED_OFF
};

led_t led_3 = 
{
    .port = PORTC_INDEX,
    .pin = GPIO_PIN2,
    .led_status = LED_OFF
};

pin_config_t btn = 
{
    .port = PORTB_INDEX,
    .pin_num = GPIO_PIN0,
    .direction = GPIO_DIRECTION_INPUT,
    .logic = GPIO_LOW
};

uint8 d;
uint8 l;
logic_t btn_status;
Std_ReturnType ret = E_NOT_OK;

int main() {
    
    app_intialize();
    
    while(1)
    {
        ret = led_toggle(&led_1);
        ret = led_toggle(&led_2);
        ret = led_toggle(&led_3);
        __delay_ms(500);
        // ret = led_turn_off(&led_1);
        // ret = led_turn_off(&led_2);
        // ret = led_turn_off(&led_3);
        // __delay_ms(5000);
    }
    
    
    return (EXIT_SUCCESS);
}

void app_intialize(void)
{
    ret = led_init(&led_1);
    ret = led_init(&led_2);
    ret = led_init(&led_3);
}