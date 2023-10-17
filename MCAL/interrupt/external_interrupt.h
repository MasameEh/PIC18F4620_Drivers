/* 
 * File:   external_interrupt.h
 * Author: Mohamed Sameh
 *
 * Created on September 18, 2023, 3:31 PM
 */
#ifndef EXTERNAL_INTERRUPT_H
#define	EXTERNAL_INTERRUPT_H

/* -------------- Includes -------------- */
#include "interrupt_config.h"

/* -------------- Macro Declarations ------------- */
//This macro will enable external interrupt, INT0.
#define EXT_INT0_ENABLE()               (INTCONbits.INT0E = 1)
//This macro will disable external interrupt, INT0.
#define EXT_INT0_DISABLE()              (INTCONbits.INT0E = 0)
//This macro will clear external interrupt flag, INT0.
#define EXT_INT0_FLAG_CLEAR()           (INTCONbits.INT0IF = 0)
//This macro will set external interrupt (INT0) edge detection  to rising edge.
#define EXT_INT0_RISING_EDGE_SET()      (INTCON2bits.INTEDG0 = 1)
//This macro will set external interrupt (INT0) edge detection  to falling edge.
#define EXT_INT0_FALLING_EDGE_SET()     (INTCON2bits.INTEDG0 = 0)

//This macro will enable external interrupt, INT1.
#define EXT_INT1_ENABLE()               (INTCON3bits.INT1IE = 1)
//This macro will disable external interrupt, INT1.
#define EXT_INT1_DISABLE()              (INTCON3bits.INT1IE = 0)
//This macro will clear external interrupt flag, INT1.
#define EXT_INT1_FLAG_CLEAR()           (INTCON3bits.INT1IF = 0)
//This macro will set external interrupt (INT1) edge detection  to rising edge.
#define EXT_INT1_RISING_EDGE_SET()      (INTCON2bits.INTEDG1 = 1)
//This macro will set external interrupt (INT1) edge detection  to falling edge.
#define EXT_INT1_FALLING_EDGE_SET()     (INTCON2bits.INTEDG1 = 0)

//This macro will enable external interrupt, INT2.
#define EXT_INT2_ENABLE()               (INTCON3bits.INT2IE = 1)
//This macro will disable external interrupt, INT2.
#define EXT_INT2_DISABLE()              (INTCON3bits.INT2IE = 0)
//This macro will clear external interrupt flag, INT2.
#define EXT_INT2_FLAG_CLEAR()           (INTCON3bits.INT2IF = 0)
//This macro will set external interrupt (INT2) edge detection  to rising edge.
#define EXT_INT2_RISING_EDGE_SET()      (INTCON2bits.INTEDG2 = 1)
//This macro will set external interrupt (INT2) edge detection  to falling edge.
#define EXT_INT2_FALLING_EDGE_SET()     (INTCON2bits.INTEDG2 = 0)


#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE
//This macro will set external interrupt (INT1) as high priority.
#define EXT_INT1_HIGH_PRIORITY()       (INTCON3bits.INT1IP = 1)
//This macro will set external interrupt (INT1) as low priority.
#define EXT_INT1_LOW_PRIORITY()        (INTCON3bits.INT1IP = 0)
//This macro will set external interrupt (INT2) as high priority.
#define EXT_INT2_HIGH_PRIORITY()       (INTCON3bits.INT2IP = 1)
//This macro will set external interrupt (INT2) as low priority.
#define EXT_INT2_LOW_PRIORITY()        (INTCON3bits.INT2IP = 0)
#endif


//This macro will enable external interrupt, RBx.
#define EXT_RBx_ENABLE()               (INTCONbits.RBIE = 1)
//This macro will disable external interrupt, RBx.
#define EXT_RBx_DISABLE()              (INTCONbits.RBIE  = 0)
//This macro will clear external interrupt flag, RBx.
#define EXT_RBx_FLAG_CLEAR()           (INTCONbits.RBIF  = 0)

#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE
//This macro will set external interrupt (RBx) as high priority.
#define EXT_RBIE_HIGH_PRIORITY()       (INTCON2bits.RBIP = 1)
//This macro will set external interrupt (RBx) as low priority.
#define EXT_RBIE_LOW_PRIORITY()        (INTCON2bits.RBIP = 0)
#endif

/* -------------- Macro Functions Declarations --------------*/

/* -------------- Data Types Declarations --------------  */
typedef enum
{
    INTERRUPT_FALLING_EDGE = 0,
    INTERRUPT_RISING_EDGE,
}interrupt_INTx_EDGE;

typedef enum
{
    INTERRUPT_EXTERNAL_INT0 = 0,
    INTERRUPT_EXTERNAL_INT1,
    INTERRUPT_EXTERNAL_INT2,
}interrupt_INTx_scr;

typedef struct
{
    void (* EXT_InterruptHandler)(void);
    pin_config_t pin;
    interrupt_INTx_scr source;
    interrupt_priority priority;
    interrupt_INTx_EDGE edge;
}ext_interrupt_INTx_t;

typedef struct
{
    void (*EXT_InterruptHandler_HIGH)(void);
    void (*EXT_InterruptHandler_LOW)(void);
    pin_config_t pin;
    interrupt_priority priority;    
}ext_interrupt_RBx_t;

/* -------------- Functions Declarations --------------*/
/**
 * @brief Initializes the external interrupt (INTx) based on the provided configuration.
 * 
 * This function configures the external interrupt settings, including enabling or disabling
 * the interrupt, setting the interrupt edge, configuring the interrupt pin, and specifying
 * an interrupt handler (callback) function.
 * 
 * @param ext_int A pointer to a structure containing the external interrupt configuration.
 * @return Std_ReturnType
 *   - E_OK: The external interrupt initialization was successful.
 *   - E_NOT_OK: The external interrupt initialization failed due to a NULL configuration pointer.
 */
Std_ReturnType Interrupt_INTx_Init(const ext_interrupt_INTx_t *ext_int);

/**
 * @brief Deinitializes the external interrupt (INTx).
 *
 * This function disables the specified external interrupt based on the provided configuration.
 *
 * @param ext_int A pointer to a structure containing the external interrupt configuration.
 *
 * @return Std_ReturnType
 *   - E_OK: The external interrupt deinitialization was successful.
 *   - E_NOT_OK: The external interrupt deinitialization failed due to a NULL configuration pointer.
 */
Std_ReturnType Interrupt_INTx_DeInit(const ext_interrupt_INTx_t *ext_int);

/**
 * @brief Initializes the external interrupt (RBx) based on the provided configuration.
 *
 * This function configures the external interrupt settings for RBx, including enabling or disabling
 * the interrupt, setting the interrupt edge, configuring the interrupt pin, specifying the interrupt
 * priority, and setting an interrupt handler (callback) function.
 *
 * @param ext_int A pointer to a structure containing the external interrupt configuration for RBx.
 *
 * @return Std_ReturnType
 *   - E_OK: The external interrupt initialization was successful.
 *   - E_NOT_OK: The external interrupt initialization failed due to a NULL configuration pointer
 *               or an invalid pin number.
 */
Std_ReturnType Interrupt_RBx_Init(const ext_interrupt_RBx_t *ext_int);

/**
 * @brief Deinitializes the external interrupt (RBx).
 *
 * This function disables the specified external interrupt based on the provided configuration.
 *
 * @param ext_int A pointer to a structure containing the external interrupt configuration for RBx.
 *
 * @return Std_ReturnType
 *   - E_OK: The external interrupt deinitialization was successful.
 *   - E_NOT_OK: The external interrupt deinitialization failed due to a NULL configuration pointer.
 */
Std_ReturnType Interrupt_RBx_DeInit(const ext_interrupt_RBx_t *ext_int);

#endif	/* EXTERNAL_INTERRUPT_H */

