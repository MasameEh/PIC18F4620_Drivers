/* 
 * File:   button.h
 * Author: Mohamed Sameh
 *
 * Created on August 27, 2023, 1:54 AM
 */

#ifndef BUTTON_H
#define	BUTTON_H

/* Section : Includes */
#include "../..//MCAL/GPIO/gpio.h"
#include "button_cfg.h"
/* Section : Macro Declarations */


/* Section : Macro Functions Declarations */

/* Section : Data Types Declarations  */
typedef enum
{
    BUTTON_PRESSED,
    BUTTON_RELEASED        
}button_state_t;

typedef enum
{
    BUTTON_ACTIVE_HIGH,
    BUTTON_ACTIVE_LOW
}button_active_t ;

typedef struct
{
    pin_config_t pin;
    button_state_t state;
    button_active_t button_connection;
}button_t;

/* Section : Functions Declarations */

/**
 * @brief Initializes the assigned pin to be INPUT.
 * 
 * @param btn A pointer to the button configuration structure.
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The operation was successful.
 *         - E_NOT_OK: An error occurred during the operation.
 */
Std_ReturnType button_init(const button_t *btn);

/**
 * @brief Reads the state of the button
 * 
 * @param btn A pointer to the button configuration structure.
 * @param state Retrieves the button state
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The operation was successful.
 *         - E_NOT_OK: An error occurred during the operation.
 */
Std_ReturnType button_read_state(const button_t *btn, button_state_t *state);

#endif	/* BUTTON_H */