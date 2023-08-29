/* 
 * File:   led.h
 * Author: Mohamed Sameh
 * Description:
 * This header file defines the interface for controlling LEDs based on a provided LED configuration. 
 * The LED configuration includes the port, pin, and initial status (ON or OFF) of the LED. The functions defined here allow initializing an LED, turning it on, turning it off, and toggling its state
 * 
 * Created on August 22, 2023, 11:29 PM
 */

#ifndef LED_H
#define	LED_H

/* Section : Includes */
#include "../..//MCAL/GPIO/gpio.h"
#include "led_config.h"

/* Section : Macro Declarations */


/* Section : Macro Functions Declarations */


/* Section : Data Types Declarations  */
typedef enum
{
    LED_OFF=0,
    LED_ON       
}led_status;

typedef struct
{
    uint8 port : 4;
    uint8 pin : 3;
    uint8 led_status : 1;
}led_t;

/**
 * @brief Initializes the assigned pin to be OUTPUT 
 * 
 * @param led A pointer to the led configuration structure.
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The operation was successful.
 *         - E_NOT_OK: An error occurred during the operation.
 */
Std_ReturnType led_init(const led_t *led);

/**
 * @brief Turns on the LED based on the provided LED configuration.
 * 
 * @param led led A pointer to the LED configuration structure.
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The operation was successful.
 *         - E_NOT_OK: An error occurred during the operation.
 */
Std_ReturnType led_turn_on(const led_t *led);

/**
 * @brief Turns on the LED based off the provided LED configuration.
 * 
 * @param led led A pointer to the LED configuration structure.
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The operation was successful.
 *         - E_NOT_OK: An error occurred during the operation.
 */
Std_ReturnType led_turn_off(const led_t *led);

/**
 * @brief Toggles the LED state based on the provided LED configuration.
 * 
 * @param led A pointer to the LED configuration structure.
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The operation was successful.
 *         - E_NOT_OK: An error occurred during the operation.
 */
Std_ReturnType led_toggle(const led_t *led);

#endif	/* LED_H */

