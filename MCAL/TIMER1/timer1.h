/* 
 * File:   timer1.h
 * Author: Mohamed Sameh
 *
 * Created on September 27, 2023, 5:52 PM
 */

#ifndef TIMER1_H
#define	TIMER1_H

/* -------------- Includes -------------- */
#include <pic18f4620.h>
#include "../std_types.h"
#include "../interrupt/internal_interrupt.h"

/* -------------- Macro Declarations ------------- */
//Timer1 mode selection.
#define TIMER1_TIMER_MODE_CFG        0
#define TIMER1_COUNTER_MODE_CFG      1
//Timer1 Counter Sync
#define TIMER1_SYNC_COUNTER_CFG     0
#define TIMER1_ASYNC_COUNTER_CFG    1
//Timer1 Oscillator
#define TIMER1_OSC_ENABLE_CFG       1
#define TIMER1_OSC_DISABLE_CFG       0
//Timer1 16-bit Read/Write Mode
#define TIMER1_16BITS_RW_MODE_CFG   1
#define TIMER1_8BITS_RW_MODE_CFG    0

/* -------------- Macro Functions Declarations -------------- */
//Timer1 enable or disable.
#define TIMER1_MODULE_ENABLE()   (T1CONbits.TMR1ON = 1)
#define TIMER1_MODULE_DISABLE()  (T1CONbits.TMR1ON = 0)

//Timer1 mode selection (Timer or Counter).
#define TIMER1_TIMER_MODE_ENABLE()     (T1CONbits.TMR1CS = 0)
#define TIMER1_COUNTER_MODE_ENABLE()   (T1CONbits.TMR1CS = 1)

//Timer1 Oscillator (Enable or Disable) for the system.
#define TIMER1_OSC_ENABLE()     (T1CONbits.T1OSCEN = 1)
#define TIMER1_OSC_DISABLE()    (T1CONbits.T1OSCEN = 0)

//Timer1 External Clock Input Synchronization Select(Only when TMR1CS = 1 otherwise ignored).
#define TIMER1_SYNC_COUNTER_ENABLE()     (T1CONbits.T1SYNC = 0)
#define TIMER1_ASYNC_COUNTER_ENABLE()    (T1CONbits.T1SYNC = 1)

//Timer1 System Clock. 
#define TIMER1_SYSTEM_CLOCK_STATUS()     (T1CONbits.T1RUN)

//Timer1 Read/Write in one 16-bit operation Mode Enable. 
#define TIMER1_16BITS_RW_ENABLE()     (T1CONbits.RD16 = 1)
//Timer1 Read/Write in two 8-bit operation Mode Enable.
#define TIMER1_8BITS_RW_ENABLE()      (T1CONbits.RD16 = 0)

/* -------------- Data Types Declarations -------------- */
/**
 * @brief Timer1 Prescaler values
 * 
 */
typedef enum
{
    TIMER1_PRESCALER_DIV_1 = 0,
    TIMER1_PRESCALER_DIV_2,
    TIMER1_PRESCALER_DIV_4,
    TIMER1_PRESCALER_DIV_8,
}timer1_prescaler_t;

typedef struct
{
#if TIMER1_INTERRUPT_ENABLE_FEATURE==INTERRUPT_FEATURE_ENABLE
    void (* TMR1_InterruptHandler)(void);
#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE
    interrupt_priority priority;    
#endif
#endif
    timer1_prescaler_t prescaler_val;    // @ref timer1_prescaler_t.
    uint16 timer1_preload;               // Value to write as start in TMR1L and TMR1H registers.
    uint8 timer1_mode : 1;               // Timer1 mode selection.
    uint8 timer1_counter_sync : 1;       // Timer1 External Clock Input Synchronization.
    uint8 timer1_osc_enable : 1;         // Timer1 Oscillator (Enable or Disable) for the system.
    uint8 timer1_rw_mode : 1;            // Timer1 Read/Write in one 16-bit or two 8-bit operation Mode.
    uint8 timer1_reserved : 4;
}timer1_t;

/* -------------- Software Interfaces Declarations --------------*/
/**
 * @brief Initializes Timer1 based on the provided configuration.
 * 
 * @param timer A pointer to the Timer1 configuration structure.
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The operation was successful.
 *         - E_NOT_OK: An error occurred during the operation.
 */
Std_ReturnType Timer1_Init(const timer1_t *timer);

/**
 * @brief De-Initializes the Timer1 Module.
 * 
 * @param timer A pointer to the Timer1 configuration structure.
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The operation was successful.
 *         - E_NOT_OK: An error occurred during the operation.
 */
Std_ReturnType Timer1_DeInit(const timer1_t *timer);

/**
 * @brief Writes a 16-bit value to Timer1. 
 * 
 * @param timer A pointer to the Timer1 configuration structure.
 * @param val The 16-bit value to write to Timer1.
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The operation was successful.
 *         - E_NOT_OK: An error occurred during the operation.
 */
Std_ReturnType Timer1_Write_Value(const timer1_t *timer, uint16 val);

/**
 * @brief Reads a 16-bit value from Timer1.
 * 
 * @param timer A pointer to the Timer1 configuration structure.
 * @param val A pointer to store the value.
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The operation was successful.
 *         - E_NOT_OK: An error occurred during the operation.
 */
Std_ReturnType Timer1_Read(const timer1_t *timer, uint16 *val);

#endif	/* TIMER1_H */

