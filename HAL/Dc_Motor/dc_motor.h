/* 
 * File:   dc_motor.h
 * Author: Mohamed Sameh
 *
 * Created on August 28, 2023, 4:26 AM
 */

#ifndef DC_MOTOR_H
#define	DC_MOTOR_H

/* -------------- Includes -------------- */
#include "dc_motor_cfg.h"
#include "../..//MCAL/GPIO/gpio.h"

/* -------------- Macro Declarations ------------- */
#define DC_MOTOR_ON_STATUS         0x01U
#define DC_MOTOR_OFF_STATUS        0x00U

#define DC_MOTOR_PIN1        0
#define DC_MOTOR_PIN2        1

/* -------------- Macro Functions Declarations --------------*/

/* -------------- Data Types Declarations --------------  */
typedef struct
{
    uint8 dc_motor_port : 4;
    uint8 dc_motor_pin : 3;
    uint8 dc_motor_status : 1;
}dc_motor_pin_t;

typedef struct
{
    dc_motor_pin_t dc_motor[2];
}dc_motor_t;

/* -------------- Functions Declarations --------------*/
/**
 * @brief Initializes the DC motor pins.
 * 
 * @param dc_motor Pointer to the configuration structure for the DC motor.
 * @return Std_ReturnType Status indicating the success or failure of the operation.
 *         - E_OK: The operation was successful.
 *         - E_NOT_OK: An error occurred during the operation.
 */
Std_ReturnType dc_motor_init(const dc_motor_t *dc_motor);

/**
 * @brief Moves the DC motor to the right.
 * 
 * @param dc_motor Pointer to the configuration structure for the DC motor.
 * @return Std_ReturnType Status indicating the success or failure of the operation.
 *         - E_OK: The operation was successful.
 *         - E_NOT_OK: An error occurred during the operation.
 */
Std_ReturnType dc_motor_move_right(const dc_motor_t *dc_motor);

/**
 * @brief Moves the DC motor to the left.
 * 
 * @param dc_motor Pointer to the configuration structure for the DC motor.
 * @return Std_ReturnType Status indicating the success or failure of the operation.
 *         - E_OK: The operation was successful.
 *         - E_NOT_OK: An error occurred during the operation.
 */
Std_ReturnType dc_motor_move_left(const dc_motor_t *dc_motor);

/**
 * @brief Stops the DC motor.
 * 
 * @param dc_motor Pointer to the configuration structure for the DC motor.
 * @return Std_ReturnType Status indicating the success or failure of the operation.
 *         - E_OK: The operation was successful.
 *         - E_NOT_OK: An error occurred during the operation.
 */
Std_ReturnType dc_motor_stop(const dc_motor_t *dc_motor);

#endif	/* DC_MOTOR_H */

