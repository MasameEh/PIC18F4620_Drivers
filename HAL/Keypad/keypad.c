/* 
 * File:   keypad.c
 * Author: Mohamed Sameh
 * Description: This file contains the implementation of functions to interface with a keypad.
 * Created on August 29, 2023, 8:20 PM
 */

#include "keypad.h"

/* Define the values corresponding to each key on the keypad */
static const uint8 keypad_values[KEYPAD_ROWS_NUM][KEYPAD_COLUMNS_NUM] = {
                                                                {'7','8','9','/'},         
                                                                {'4','5','6','*'},         
                                                                {'1','2','3','-'},         
                                                                {'#','0','=','+'}        
                                                                };

/**
 * @brief Initializes the keypad by configuring its rows and columns OUTPUT and INPUT pins, respectively.
 * @note must connect the Columns pins to ground
 * @param keypad A pointer to the keypad configuration structure.
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The operation was successful.
 *         - E_NOT_OK: An error occurred during the operation.
 */
Std_ReturnType keypad_init(const keypad_t *keypad)
{
    Std_ReturnType ret = E_OK;
    uint8 rows_counter = ZERO_INIT, columns_counter = ZERO_INIT;

    if(NULL == keypad)
    {
        ret = E_NOT_OK;
    }
    else
    {   
        // Initialize rows as input pins and columns as output pins
        for (rows_counter = ZERO_INIT; rows_counter < KEYPAD_ROWS_NUM; rows_counter++)
        {
            ret = gpio_pin_initialize(&(keypad->keypad_rows_pins[rows_counter]));
        }
        for (columns_counter = ZERO_INIT; columns_counter < KEYPAD_COLUMNS_NUM; columns_counter++)
        {
            ret = gpio_pin_set_direction(&(keypad->keypad_columns_pins[columns_counter]));
        }
    }
    return ret;
}

/**
 * @brief Reads the value of the pressed key on the keypad.
 * 
 * @param keypad A pointer to the keypad configuration structure.
 * @param value Pointer to the variable where the pressed key's value will be stored.
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The operation was successful.
 *         - E_NOT_OK: An error occurred during the operation.
 */
Std_ReturnType keypad_get_value(const keypad_t *keypad, uint8 *value)
{
    Std_ReturnType ret = E_OK;
    uint8 rows_counter = ZERO_INIT, columns_counter = ZERO_INIT, l_counter = ZERO_INIT;
    logic_t column_logic = GPIO_LOW;

    if(NULL == keypad || NULL == value)
    {
        ret = E_NOT_OK;
    }
    else
    {
        for (rows_counter = ZERO_INIT; rows_counter < KEYPAD_ROWS_NUM; rows_counter++)
        {
            // Activate one row at a time and read the corresponding column
            for (l_counter = ZERO_INIT; l_counter < KEYPAD_ROWS_NUM; l_counter++)
            {
                ret = gpio_pin_write(&(keypad->keypad_rows_pins[l_counter]), GPIO_LOW);
            }
            ret = gpio_pin_write(&(keypad->keypad_rows_pins[rows_counter]), GPIO_HIGH);
            __delay_ms(10); // Delay for debouncing
            for (columns_counter = ZERO_INIT; columns_counter < KEYPAD_COLUMNS_NUM; columns_counter++)
            {
                ret = gpio_pin_read(&(keypad->keypad_columns_pins[columns_counter]), &column_logic);
                if (GPIO_HIGH == column_logic)
                {
                    // Determine the pressed key's value based on row and column
                    *value = keypad_values[rows_counter][columns_counter];
                }
                
            }
            
        }
        
    }
    return ret;
}