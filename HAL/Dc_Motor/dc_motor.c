/* 
 * File:   dc_motor.c
 * Author: Mohamed Sameh
 * Description:
 * This source file contains the implementation of functions for controlling a DC motor. 
 * The functions allow initializing the motor, moving it right, moving it left, and stopping it based on the provided motor configuration.
 * 
 * Created on August 28, 2023, 4:26 AM
 */

#include "dc_motor.h"

/**
 * @brief Initializes the DC motor pins to be OUTPUT and turns the motor OFF or on.
 * 
 * @param dc_motor Pointer to the configuration structure for the DC motor.
 * @return Std_ReturnType Status indicating the success or failure of the operation.
 *         - E_OK: The operation was successful.
 *         - E_NOT_OK: An error occurred during the operation.
 */
Std_ReturnType dc_motor_init(const dc_motor_t *dc_motor)
{
    Std_ReturnType ret = E_OK;

    if(NULL == dc_motor)
    {
        ret = E_NOT_OK;
    }
    else
    {
        pin_config_t pin1 = 
        {
            .port = dc_motor->dc_motor[DC_MOTOR_PIN1].dc_motor_port,
            .pin_num = dc_motor->dc_motor[DC_MOTOR_PIN1].dc_motor_pin,
            .direction = GPIO_DIRECTION_OUTPUT,
            .logic = dc_motor->dc_motor[DC_MOTOR_PIN1].dc_motor_status
        };
        ret = gpio_pin_initialize(&pin1);

        pin_config_t pin2 = 
        {
            .port = dc_motor->dc_motor[DC_MOTOR_PIN2].dc_motor_port,    
            .pin_num = dc_motor->dc_motor[DC_MOTOR_PIN2].dc_motor_pin,
            .direction = GPIO_DIRECTION_OUTPUT,
            .logic = dc_motor->dc_motor[DC_MOTOR_PIN2].dc_motor_status
        };
        ret = gpio_pin_initialize(&pin2);
    }
    return ret;
}

/**
 * @brief Moves the DC motor to the right.
 * 
 * @param dc_motor Pointer to the configuration structure for the DC motor.
 * @return Std_ReturnType Status indicating the success or failure of the operation.
 *         - E_OK: The operation was successful.
 *         - E_NOT_OK: An error occurred during the operation.
 */
Std_ReturnType dc_motor_move_right(const dc_motor_t *dc_motor)
{
    Std_ReturnType ret = E_OK;

    if(NULL == dc_motor)
    {
        ret = E_NOT_OK;
    }
    else
    {
        pin_config_t pin1 = 
        {
            .port = dc_motor->dc_motor[DC_MOTOR_PIN1].dc_motor_port,
            .pin_num = dc_motor->dc_motor[DC_MOTOR_PIN1].dc_motor_pin,
            .direction = GPIO_DIRECTION_OUTPUT,
            .logic = dc_motor->dc_motor[DC_MOTOR_PIN1].dc_motor_status
        };

        pin_config_t pin2 = 
        {
            .port = dc_motor->dc_motor[DC_MOTOR_PIN2].dc_motor_port,    
            .pin_num = dc_motor->dc_motor[DC_MOTOR_PIN2].dc_motor_pin,
            .direction = GPIO_DIRECTION_OUTPUT,
            .logic = dc_motor->dc_motor[DC_MOTOR_PIN2].dc_motor_status
        };
        ret = gpio_pin_write(&pin1, GPIO_HIGH);
        ret = gpio_pin_write(&pin2, GPIO_LOW);
    }
    return ret;
}

/**
 * @brief Moves the DC motor to the left.
 * 
 * @param dc_motor Pointer to the configuration structure for the DC motor.
 * @return Std_ReturnType Status indicating the success or failure of the operation.
 *         - E_OK: The operation was successful.
 *         - E_NOT_OK: An error occurred during the operation.
 */
Std_ReturnType dc_motor_move_left(const dc_motor_t *dc_motor)
{
    Std_ReturnType ret = E_OK;

    if(NULL == dc_motor)
    {
        ret = E_NOT_OK;
    }
    else
    {
        pin_config_t pin1 = 
        {
            .port = dc_motor->dc_motor[DC_MOTOR_PIN1].dc_motor_port,
            .pin_num = dc_motor->dc_motor[DC_MOTOR_PIN1].dc_motor_pin,
            .direction = GPIO_DIRECTION_OUTPUT,
            .logic = dc_motor->dc_motor[DC_MOTOR_PIN1].dc_motor_status
        };

        pin_config_t pin2 = 
        {
            .port = dc_motor->dc_motor[DC_MOTOR_PIN2].dc_motor_port,    
            .pin_num = dc_motor->dc_motor[DC_MOTOR_PIN2].dc_motor_pin,
            .direction = GPIO_DIRECTION_OUTPUT,
            .logic = dc_motor->dc_motor[DC_MOTOR_PIN2].dc_motor_status
        };
        ret = gpio_pin_write(&pin1, GPIO_LOW);
        ret = gpio_pin_write(&pin2, GPIO_HIGH);
    }
    return ret;
}

/**
 * @brief Stops the DC motor.
 * 
 * @param dc_motor Pointer to the configuration structure for the DC motor.
 * @return Std_ReturnType Status indicating the success or failure of the operation.
 *         - E_OK: The operation was successful.
 *         - E_NOT_OK: An error occurred during the operation.
 */
Std_ReturnType dc_motor_stop(const dc_motor_t *dc_motor)
{
    Std_ReturnType ret = E_OK;

    if(NULL == dc_motor)
    {
        ret = E_NOT_OK;
    }
    else
    {
        pin_config_t pin1 = 
        {
            .port = dc_motor->dc_motor[DC_MOTOR_PIN1].dc_motor_port,
            .pin_num = dc_motor->dc_motor[DC_MOTOR_PIN1].dc_motor_pin,
            .direction = GPIO_DIRECTION_OUTPUT,
            .logic = dc_motor->dc_motor[DC_MOTOR_PIN1].dc_motor_status
        };

        pin_config_t pin2 = 
        {
            .port = dc_motor->dc_motor[DC_MOTOR_PIN2].dc_motor_port,    
            .pin_num = dc_motor->dc_motor[DC_MOTOR_PIN2].dc_motor_pin,
            .direction = GPIO_DIRECTION_OUTPUT,
            .logic = dc_motor->dc_motor[DC_MOTOR_PIN2].dc_motor_status
        };

        ret = gpio_pin_write(&pin1, GPIO_LOW);
        ret = gpio_pin_write(&pin2, GPIO_LOW);
    }
    return ret;    
}