/* 
 * File:   timer0.h
 * Author: Mohamed Sameh
 *
 * Created on September 26, 2023, 5:59 PM
 */

#ifndef TIMER0_H
#define	TIMER0_H

/* -------------- Includes -------------- */
#include "../GPIO/gpio.h"
#include "../interrupt/internal_interrupt.h"

/* -------------- Macro Declarations ------------- */
//Prescaler enable or not.
#define TIMER0_PRESCALER_ENABLE_CFG      1
#define TIMER0_PRESCALER_DISABLE_CFG     0

//Timer0 counter mode trigger edge selection .
#define TIMER0_RISING_EDGE_CFG        1
#define TIMER0_FALLING_EDGE_CFG       0

//Timer0 mode selection.
#define TIMER0_TIMER_MODE        1
#define TIMER0_COUNTER_MODE      0

//Timer0 Register mode selection.
#define TIMER0_8BIT_REGISTER_MODE          1
#define TIMER0_16BIT_REGISTER_MODE         0
/* -------------- Macro Functions Declarations --------------*/
//This macro enables timer0.
#define TIMER0_MODULE_ENABLE()   (T0CONbits.TMR0ON = 1)
//This macro disables timer0.
#define TIMER0_MODULE_DISABLE()  (T0CONbits.TMR0ON = 0)

//This macro enables timer0 prescaler.
#define TIMER0_PRESCALER_ENABLE()   (T0CONbits.PSA = 0)
//This macro disables timer0 prescaler.
#define TIMER0_PRESCALER_DISABLE()  (T0CONbits.PSA = 1)

//Timer0 Source Rising Edge Select.
#define TIMER0_RISING_EDGE_SELECT()   (T0CONbits.T0SE = 0)
//Timer0 Source Falling Edge Select.
#define TIMER0_FALLING_EDGE_SELECT()  (T0CONbits.T0SE = 1)

//Timer0 mode selection.
#define TIMER0_TIMER_MODE_ENABLE()     (T0CONbits.T0CS = 0)
#define TIMER0_COUNTER_MODE_ENABLE()   (T0CONbits.T0CS = 1)

//Timer0 Register mode selection.
#define TIMER0_8BIT_REGISTER_MODE_ENABLE()   (T0CONbits.T08BIT = 1)
#define TIMER0_16BIT_REGISTER_MODE_ENABLE()  (T0CONbits.T08BIT = 0)
/* -------------- Data Types Declarations --------------  */
typedef enum
{
    TIMER0_PRESCALER_DIV_2 = 0,
    TIMER0_PRESCALER_DIV_4,
    TIMER0_PRESCALER_DIV_8,
    TIMER0_PRESCALER_DIV_16,
    TIMER0_PRESCALER_DIV_32,
    TIMER0_PRESCALER_DIV_64,
    TIMER0_PRESCALER_DIV_128,
    TIMER0_PRESCALER_DIV_256
}timer0_prescaler_t;

typedef struct
{
#if TIMER0_INTERRUPT_ENABLE_FEATURE==INTERRUPT_FEATURE_ENABLE
    void (* TMR0_InterruptHandler)(void);
#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE
    interrupt_priority priority;    
#endif
#endif
    uint16 timer0_preload;               // Value to write as start in TMR0L and TMR0H registers
    uint8 prescaler_status : 1;          // Prescaler enable or not.
    uint8 timer0_counter_edge : 1;       // Timer0 counter mode trigger edge selection. 
    uint8 timer0_mode : 1;               // Timer0 mode selection.
    uint8 timer0_reg_size : 1;           //Timer0 Register mode selection.
    timer0_prescaler_t prescaler_val;    // @ref timer0_prescaler_t
    uint8 timer0_reserved : 4;
}timer0_t;
/* -------------- Functions Declarations --------------*/
Std_ReturnType Timer0_Init(const timer0_t *timer0);
Std_ReturnType Timer0_DeInit(const timer0_t *timer0);
Std_ReturnType Timer0_Write_Value(const timer0_t *timer0, uint16 val);
Std_ReturnType Timer0_Read(const timer0_t *timer0, uint16 *val);

#endif	/* TIMER0_H */

