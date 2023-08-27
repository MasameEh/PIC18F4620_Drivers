/* 
 * File:   button.h
 * Author: Mohamed Sameh
 *
 * Created on August 27, 2023, 1:54 AM
 */

#include "button.h"

/**
 * @brief Initializes the assigned pin to be INPUT.
 * 
 * @param btn A pointer to the button configuration structure.
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The operation was successful.
 *         - E_NOT_OK: An error occurred during the operation.
 */
Std_ReturnType button_init(const button_t *btn)
{
    Std_ReturnType ret = E_OK;

    if(NULL == btn)
    {
        ret = E_NOT_OK;
    }
    else
    {
        gpio_pin_set_direction(&btn->pin);
    }
    return ret;
}

/**
 * @brief Reads the state of the button
 * 
 * @param btn A pointer to the button configuration structure.
 * @param state Retrieves the button state
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The operation was successful.
 *         - E_NOT_OK: An error occurred during the operation.
 */
Std_ReturnType button_read_state(const button_t *btn, button_state_t *state)
{
    Std_ReturnType ret = E_OK;
    logic_t logic = GPIO_LOW;

    if(NULL == btn || NULL == state)
    {
        ret = E_NOT_OK;
    }
    else
    {   
        gpio_pin_read(&btn->pin, &logic);
        if(BUTTON_ACTIVE_HIGH == btn->button_connection)
        {
            if (GPIO_HIGH == logic)
            {
                *state = BUTTON_PRESSED;
            }
            else
            {
                *state = BUTTON_RELEASED;
            }
            
        }
        else if(BUTTON_ACTIVE_LOW == btn->button_connection)
        {   
            if (GPIO_HIGH == logic)
            {
                *state = BUTTON_RELEASED;
            }
            else
            {
                *state = BUTTON_PRESSED;
            }

        }else{}
        
    }

    return ret;

}