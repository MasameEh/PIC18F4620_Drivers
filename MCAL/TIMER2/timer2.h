/* 
 * File:   timer2.h
 * Author: Mohamed Sameh
 *
 * Created on September 28, 2023, 3:59 AM
 */

#ifndef TIMER2_H
#define	TIMER2_H

/* -------------- Includes ----------------------- */
#include <pic18f4620.h>
#include "../std_types.h"
#include "../interrupt/internal_interrupt.h"

/* -------------- Macro Declarations ------------- */
/* Timer2 Postscaler values  */
#define TIMER2_POSTSCALER_DIV_1     0
#define TIMER2_POSTSCALER_DIV_2     1
#define TIMER2_POSTSCALER_DIV_3     2
#define TIMER2_POSTSCALER_DIV_4     3
#define TIMER2_POSTSCALER_DIV_5     4
#define TIMER2_POSTSCALER_DIV_6     5
#define TIMER2_POSTSCALER_DIV_7     6
#define TIMER2_POSTSCALER_DIV_8     7
#define TIMER2_POSTSCALER_DIV_9     8       
#define TIMER2_POSTSCALER_DIV_10    9
#define TIMER2_POSTSCALER_DIV_11    10
#define TIMER2_POSTSCALER_DIV_12    11
#define TIMER2_POSTSCALER_DIV_13    12
#define TIMER2_POSTSCALER_DIV_14    13
#define TIMER2_POSTSCALER_DIV_15    14  
#define TIMER2_POSTSCALER_DIV_16    15

/* Timer2 Prescaler values  */
#define TIMER2_PRESCALER_DIV_1     0
#define TIMER2_PRESCALER_DIV_4     1
#define TIMER2_PRESCALER_DIV_16    2

/* -------------- Macro Functions Declarations -------------- */
//Timer2 enable or disable.
#define TIMER2_MODULE_ENABLE()   (T2CONbits.TMR2ON = 1)
#define TIMER2_MODULE_DISABLE()  (T2CONbits.TMR2ON = 0)


/* -------------- Data Types Declarations ----------------- */
typedef struct
{
#if TIMER2_INTERRUPT_ENABLE_FEATURE==INTERRUPT_FEATURE_ENABLE
    void (* TMR2_InterruptHandler)(void);
#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE
    interrupt_priority priority;    
#endif
#endif              
    uint8 timer2_preload;               // Value to write as start in TMR2 register.
    uint8 prescaler_val : 2;
    uint8 postscaler_val : 4;
    uint8 timer2_reserved : 2; 
}timer2_t;

/* -------------- Software Interfaces Declarations -------------- */
/**
 * @brief Initializes Timer2 based on the provided configuration.
 * 
 * @param timer A pointer to the Timer2 configuration structure.
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The operation was successful.
 *         - E_NOT_OK: An error occurred during the operation.
 */
Std_ReturnType Timer2_Init(const timer2_t *timer);

/**
 * @brief De-Initializes the Timer2 Module.
 * 
 * @param timer A pointer to the Timer2 configuration structure.
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The operation was successful.
 *         - E_NOT_OK: An error occurred during the operation.
 */
Std_ReturnType Timer2_DeInit(const timer2_t *timer);

/**
 * @brief Writes a 8-bit value to Timer2. 
 * 
 * @param timer A pointer to the Timer2 configuration structure.
 * @param val The 8-bit value to write to Timer2.
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The operation was successful.
 *         - E_NOT_OK: An error occurred during the operation.
 */
Std_ReturnType Timer2_Write_Value(const timer2_t *timer, uint8 val);

/**
 * @brief Reads a 8-bit value from Timer2.
 * 
 * @param timer A pointer to the Timer2 configuration structure.
 * @param val A pointer to store the value.
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The operation was successful.
 *         - E_NOT_OK: An error occurred during the operation.
 */
Std_ReturnType Timer2_Read(const timer2_t *timer, uint8 *val);

#endif	/* TIMER2_H */

