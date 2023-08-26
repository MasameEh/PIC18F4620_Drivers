/* 
 * File:   led.c
 * Author: Mohamed Sameh
 *
 * Created on August 22, 2023, 11:29 PM
 */


#include "led.h"

/**
 * @brief 
 * 
 * @param led 
 * @return Std_ReturnType 
 */
Std_ReturnType led_init(const led_t *led)
{
    Std_ReturnType ret = E_OK;
    
    if(NULL == led)
    {
        ret = E_NOT_OK;
    }
    else
    {
        pin_config_t pin = 
        {
            .port = led->port,
            .pin_num = led->pin,
            .direction = GPIO_DIRECTION_OUTPUT,
            .logic = led->led_status
        };
        gpio_pin_intialize(&pin);
    }

    return ret;
}

/**
 * @brief 
 * 
 * @param led 
 * @return Std_ReturnType 
 */
Std_ReturnType led_turn_on(const led_t *led)
{
    Std_ReturnType ret = E_OK;

    if(NULL == led)
    {
        ret = E_NOT_OK;
    }
    else
    {
        pin_config_t pin = 
        {
            .port = led->port,
            .pin_num = led->pin,
            .direction = GPIO_DIRECTION_OUTPUT,
            .logic = led->led_status
        };
        gpio_pin_write(&pin, GPIO_HIGH);
    }

    return ret;
}

/**
 * @brief 
 * 
 * @param led 
 * @return Std_ReturnType 
 */
Std_ReturnType led_turn_off(const led_t *led)
{
    Std_ReturnType ret = E_OK;

    if(NULL == led)
    {
        ret = E_NOT_OK;
    }
    else
    {
        pin_config_t pin = 
        {
            .port = led->port,
            .pin_num = led->pin,
            .direction = GPIO_DIRECTION_OUTPUT,
            .logic = led->led_status
        };
        gpio_pin_write(&pin, GPIO_LOW);
    }

    return ret;
}

/**
 * @brief 
 * 
 * @param led 
 * @return Std_ReturnType 
 */
Std_ReturnType led_toggle(const led_t *led)
{
    Std_ReturnType ret = E_OK;

    if(NULL == led)
    {
        ret = E_NOT_OK;
    }
    else
    {
        pin_config_t pin = 
        {
            .port = led->port,
            .pin_num = led->pin,
            .direction = GPIO_DIRECTION_OUTPUT,
            .logic = led->led_status
        };
        gpio_pin_toggle(&pin);
    }

    return ret;
}