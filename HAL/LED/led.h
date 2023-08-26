/* 
 * File:   led.h
 * Author: Mohamed Sameh
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

/* Section : Functions Declarations */
Std_ReturnType led_init(const led_t *led);

Std_ReturnType led_turn_on(const led_t *led);

Std_ReturnType led_turn_off(const led_t *led);

Std_ReturnType led_toggle(const led_t *led);

#endif	/* LED_H */

