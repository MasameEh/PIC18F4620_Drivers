/* 
 * File:   7_seg.h
 *
 * Author: Mohamed Sameh
 * 
 * Description:
 * This header file provides the interface and data structures for controlling a 7-segment display using GPIO pins.
 * The functions defined in this file allow initializing the display and writing numbers to it. The 7-segment display
 * can be either common anode or common cathode type, and the corresponding pin configurations are specified using
 * the provided data structures.
 * 
 * Created on August 29, 2023, 12:41 AM
 */

#ifndef SEVEN_SEG_H
#define	SEVEN_SEG_H

/* -------------- Includes -------------- */
#include "seven_seg_cfg.h"
#include "../..//MCAL/GPIO/gpio.h"

/* -------------- Macro Declarations ------------- */
#define SEGMENT_PIN0      0
#define SEGMENT_PIN1      1
#define SEGMENT_PIN2      2
#define SEGMENT_PIN3      3
/* -------------- Macro Functions Declarations --------------*/

/* -------------- Data Types Declarations --------------  */
typedef enum
{
    SEG_COMMON_ANODE,
    SEG_COMMON_CATHODE   
}seg_type_t;

typedef struct 
{
    pin_config_t seg_pins[4];
    seg_type_t seg_type;
}seg_t;


/* -------------- Functions Declarations --------------*/
/**
 * @brief Initializes the seven segment pins to be OUTPUT and turns the pins OFF or on (Start at 0 or 9)
 * 
 * @param seg Pointer to the configuration structure for the seven segment.
 * @return Std_ReturnType Status indicating the success or failure of the operation.
 *         - E_OK: The operation was successful.
 *         - E_NOT_OK: An error occurred during the operation.
 */
Std_ReturnType seven_seg_init(const seg_t *seg);

/**
 * @brief Writes a number to a 7-segment display.
 * 
 * @param seg A pointer to the 7-segment configuration structure.
 * @param number The number (0 to 9) to be displayed.
 * @return Std_ReturnType Status indicating the success or failure of the operation.
 *         - E_OK: The operation was successful.
 *         - E_NOT_OK: An error occurred during the operation.
 */
Std_ReturnType seven_seg_write_number(const seg_t *seg, uint8 number);

#endif	/* 7_SEG_H */

