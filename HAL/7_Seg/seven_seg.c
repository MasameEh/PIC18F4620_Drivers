/* 
 * File:   7_seg.c
 * Author: Mohamed Sameh
 * 
 * Description:
 * This source file implements the functions for interfacing with a 7-segment display using a configuration structure
 * and GPIO pins. The functions allow initializing the display and writing numbers to it.
 * 
 *
 * Created on August 29, 2023, 12:41 AM
 */
#include "seven_seg.h"

/**
 * @brief Initializes the seven segment pins to be OUTPUT and turns the pins OFF or on (Start at 0 or 9)
 * 
 * @param seg Pointer to the configuration structure for the seven segment.
 * @return Std_ReturnType Status indicating the success or failure of the operation.
 *         - E_OK: The operation was successful.
 *         - E_NOT_OK: An error occurred during the operation.
 */
Std_ReturnType seven_seg_init(const seg_t *seg)
{
    Std_ReturnType ret = E_OK;

    if (NULL == seg)
    {
        ret = E_NOT_OK;
    }
    else
    {
        ret = gpio_pin_intialize(&(seg->seg_pins[SEGMENT_PIN0]));
        ret = gpio_pin_intialize(&(seg->seg_pins[SEGMENT_PIN1]));
        ret = gpio_pin_intialize(&(seg->seg_pins[SEGMENT_PIN2]));
        ret = gpio_pin_intialize(&(seg->seg_pins[SEGMENT_PIN3]));
    }
    

    return ret;
}

/**
 * @brief Writes a number to a 7-segment display.
 * 
 * @param seg A pointer to the 7-segment configuration structure.
 * @param number The number (0 to 9) to be displayed.
 * @return Std_ReturnType Status indicating the success or failure of the operation.
 *         - E_OK: The operation was successful.
 *         - E_NOT_OK: An error occurred during the operation.
 */
Std_ReturnType seven_seg_write_number(const seg_t *seg, uint8 number)
{
    Std_ReturnType ret = E_OK;

    if (NULL == seg || number > 9)
    {
        ret = E_NOT_OK;
    }
    else
    {
        ret = gpio_pin_write(&(seg->seg_pins[SEGMENT_PIN0]), number & 0x01);
        ret = gpio_pin_write(&(seg->seg_pins[SEGMENT_PIN1]), number>>1 & 0x01);
        ret = gpio_pin_write(&(seg->seg_pins[SEGMENT_PIN2]), number>>2 & 0x01);
        ret = gpio_pin_write(&(seg->seg_pins[SEGMENT_PIN3]), number>>3 & 0x01);
    }
   
    return ret;
}