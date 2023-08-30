/* 
 * File:   keypad.h
 * Author: Mohamed Sameh
 * Description: This header file provides the interface and data structures for interfacing with a keypad.
 * Created on August 29, 2023, 8:20 PM
 */

#ifndef KEYPAD_H
#define	KEYPAD_H

/* -------------- Includes -------------- */
#include "keypad_cfg.h"
#include "../..//MCAL/GPIO/gpio.h"

/* -------------- Macro Declarations ------------- */
#define KEYPAD_ROWS_NUM    		4
#define KEYPAD_COLUMNS_NUM 		4

/* -------------- Macro Functions Declarations --------------*/

/* -------------- Data Types Declarations --------------  */
typedef struct
{
    pin_config_t keypad_rows_pins[KEYPAD_ROWS_NUM];     // Array of pin configurations for keypad rows
    pin_config_t keypad_columns_pins[KEYPAD_COLUMNS_NUM];  // Array of pin configurations for keypad columns
} keypad_t;  // Structure to hold the keypad configuration
 


/* -------------- Functions Declarations --------------*/

/**
 * @brief Initializes the keypad by configuring its rows and columns as input and output pins, respectively.
 * 
 * @param keypad A pointer to the keypad configuration structure.
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The operation was successful.
 *         - E_NOT_OK: An error occurred during the operation.
 */
Std_ReturnType keypad_init(const keypad_t *keypad);

/**
 * @brief Reads the value of the pressed key on the keypad.
 * 
 * @param keypad A pointer to the keypad configuration structure.
 * @param value Pointer to the variable where the pressed key's value will be stored.
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The operation was successful.
 *         - E_NOT_OK: An error occurred during the operation.
 */
Std_ReturnType keypad_get_value(const keypad_t *keypad, uint8 *value);

#endif	/* KEYPAD_H */

