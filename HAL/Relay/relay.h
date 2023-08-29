/* 
 * File:   relay.h
 * Author: Mohamed Sameh
  * This header file provides the interface and data structures for controlling a relay based on a provided relay configuration.
 * Created on August 27, 2023, 8:58 PM
 */

#ifndef RELAY_H
#define	RELAY_H

/* -------------- Includes -------------- */
#include "../..//MCAL/GPIO/gpio.h"
#include "relay_cfg.h"

/* -------------- Macro Declarations ------------- */
#define RELAY_ON_STATUS         0x01U
#define RELAY_OFF_STATUS        0x00U

/* -------------- Macro Functions Declarations --------------*/

/* -------------- Data Types Declarations --------------  */
typedef struct
{
    uint8 relay_port : 4;
    uint8 relay_pin : 3;
    uint8 relay_status : 1;
} relay_t;
/* -------------- Functions Declarations --------------*/

/**
 * @brief Initializes the assigned pin to be OUTPUT and turns the relay on or off.
 * 
 * @param relay A pointer to the relay configuration structure.
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The operation was successful.
 *         - E_NOT_OK: An error occurred during the operation.
 */
Std_ReturnType relay_init(const relay_t *relay);

/**
 * @brief Turns on the relay
 * 
 * @param relay A pointer to the relay configuration structure.
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The operation was successful.
 *         - E_NOT_OK: An error occurred during the operation.
 */
Std_ReturnType relay_turn_on(const relay_t *relay);

/**
 * @brief Turns off the relay
 * 
 * @param relay A pointer to the relay configuration structure.
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The operation was successful.
 *         - E_NOT_OK: An error occurred during the operation.
 */
Std_ReturnType relay_turn_off(const relay_t *relay);

#endif	/* RELAY_H */

