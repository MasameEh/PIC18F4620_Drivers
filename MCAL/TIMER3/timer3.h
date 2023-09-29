/* 
 * File:   timer3.h
 * Author: Mohamed Sameh
 *
 * Created on September 28, 2023, 3:59 AM
 */

#ifndef TIMER3_H
#define	TIMER3_H

/* -------------- Includes -------------- */
#include <pic18f4620.h>
#include "../std_types.h"
#include "../interrupt/internal_interrupt.h"

/* -------------- Macro Declarations ------------- */
//Timer3 Mode selection.
#define TIMER3_COUNTER_MODE_CFG      1
#define TIMER3_TIMER_MODE_CFG        0

//Timer3 Counter Mode Sync
#define TIMER3_ASYNC_COUNTER_CFG    1
#define TIMER3_SYNC_COUNTER_CFG     0

//Timer3 16-bit Read/Write Mode
#define TIMER3_16BITS_RW_MODE_CFG   1
#define TIMER3_8BITS_RW_MODE_CFG    0

/* -------------- Macro Functions Declarations -------------- */
//Timer3 enable or disable.
#define TIMER3_MODULE_ENABLE()   (T3CONbits.TMR3ON = 1)
#define TIMER3_MODULE_DISABLE()  (T3CONbits.TMR3ON = 0)

//Timer3 mode selection (Timer or Counter).
#define TIMER3_COUNTER_MODE_ENABLE()   (T3CONbits.TMR3CS = 1)
#define TIMER3_TIMER_MODE_ENABLE()     (T3CONbits.TMR3CS = 0)

//Timer3 External Clock Input Synchronization Select(Only when TMR3CS = 1 otherwise ignored).
#define TIMER3_ASYNC_COUNTER_ENABLE()    (T3CONbits.T3SYNC = 1)
#define TIMER3_SYNC_COUNTER_ENABLE()     (T3CONbits.T3SYNC = 0)

//Timer3 Read/Write in one 16-bit operation Mode Enable. 
#define TIMER3_16BITS_RW_ENABLE()     (T3CONbits.RD16 = 1)
//Timer3 Read/Write in two 8-bit operation Mode Enable.
#define TIMER3_8BITS_RW_ENABLE()      (T3CONbits.RD16 = 0)

/* -------------- Data Types Declarations --------------  */
/**
 * @brief Timer3 Prescaler values
 * 
 */
typedef enum
{
    TIMER3_PRESCALER_DIV_1 = 0,
    TIMER3_PRESCALER_DIV_2,
    TIMER3_PRESCALER_DIV_4,
    TIMER3_PRESCALER_DIV_8,
}timer3_prescaler_t;

typedef struct
{
#if TIMER3_INTERRUPT_ENABLE_FEATURE==INTERRUPT_FEATURE_ENABLE
    void (* TMR3_InterruptHandler)(void);
#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE
    interrupt_priority priority;    
#endif
#endif
    timer3_prescaler_t prescaler_val;    // @ref timer3_prescaler_t.
    uint16 timer3_preload;               // Value to write as start in TMR3L and TMR3H registers.
    uint8 timer3_mode : 1;               // Timer3 mode selection.
    uint8 timer3_counter_sync : 1;       // Timer3 External Clock Input Synchronization.
    uint8 timer3_rw_mode : 1;            // Timer3 Read/Write in one 16-bit or two 8-bit operation Mode.
    uint8 timer3_reserved : 5;    
}timer3_t;

/* -------------- Software Interfaces Declarations --------------*/
/**
 * @brief Initializes Timer3 based on the provided configuration.
 * 
 * @param timer A pointer to the Timer3 configuration structure.
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The operation was successful.
 *         - E_NOT_OK: An error occurred during the operation.
 */
Std_ReturnType Timer3_Init(const timer3_t *timer);

/**
 * @brief De-Initializes the Timer3 Module.
 * 
 * @param timer A pointer to the Timer3 configuration structure.
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The operation was successful.
 *         - E_NOT_OK: An error occurred during the operation.
 */
Std_ReturnType Timer3_DeInit(const timer3_t *timer);

/**
 * @brief Writes a 16-bit value to Timer3. 
 * 
 * @param timer A pointer to the Timer3 configuration structure.
 * @param val The 16-bit value to write to Timer3.
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The operation was successful.
 *         - E_NOT_OK: An error occurred during the operation.
 */
Std_ReturnType Timer3_Write_Value(const timer3_t *timer, uint16 val);

/**
 * @brief Reads a 16-bit value from Timer3.
 * 
 * @param timer A pointer to the Timer3 configuration structure.
 * @param val A pointer to store the value.
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The operation was successful.
 *         - E_NOT_OK: An error occurred during the operation.
 */
Std_ReturnType Timer3_Read(const timer3_t *timer, uint16 *val);

#endif	/* TIMER3_H */

