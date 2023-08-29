/* 
 * File:   relay.c
 * Author: Mohamed Sameh
 * This implementation file provides functions to control a relay using GPIO pins. 
 * It includes initialization, turning on, and turning off the relay functions.
 * Created on August 27, 2023, 8:58 PM
 */

#include "relay.h"

/**
 * @brief Initializes the assigned pin to be OUTPUT.
 * 
 * @param relay A pointer to the relay configuration structure.
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The operation was successful.
 *         - E_NOT_OK: An error occurred during the operation.
 */
Std_ReturnType relay_init(const relay_t *relay)
{
    Std_ReturnType ret = E_OK;

    if(NULL == relay)
    {
        ret = E_NOT_OK;
    }
    else
    {
        pin_config_t pin = 
        {
            .port = relay->relay_port,
            .pin_num = relay->relay_pin,
            .direction = GPIO_DIRECTION_OUTPUT,
            .logic = relay->relay_status
        };
        ret = gpio_pin_intialize(&pin);
    }

    return ret;
}

/**
 * @brief Turns on the relay
 * 
 * @param relay A pointer to the relay configuration structure.
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The operation was successful.
 *         - E_NOT_OK: An error occurred during the operation.
 */
Std_ReturnType relay_turn_on(const relay_t *relay)
{
    Std_ReturnType ret = E_OK;

    if(NULL == relay)
    {
        ret = E_NOT_OK;
    }
    else
    {
        pin_config_t pin = 
        {
            .port = relay->relay_port,
            .pin_num = relay->relay_pin,
            .direction = GPIO_DIRECTION_OUTPUT,
            .logic = relay->relay_status
        };
        ret = gpio_pin_write(&pin, GPIO_HIGH);
    }

    return ret;
}

/**
 * @brief Turns off the relay
 * 
 * @param relay A pointer to the relay configuration structure.
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The operation was successful.
 *         - E_NOT_OK: An error occurred during the operation.
 */
Std_ReturnType relay_turn_off(const relay_t *relay)
{
    Std_ReturnType ret = E_OK;

    if(NULL == relay)
    {
        ret = E_NOT_OK;
    }
    else
    {   
        pin_config_t pin = 
        {
            .port = relay->relay_port,
            .pin_num = relay->relay_pin,
            .direction = GPIO_DIRECTION_OUTPUT,
            .logic = relay->relay_status
        };
        ret = gpio_pin_write(&pin, GPIO_LOW);
    }

    return ret;
}