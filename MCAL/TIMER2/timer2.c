/* 
 * File:   timer2.c
 * Author: Mohamed Sameh
 *
 * Created on September 28, 2023, 3:59 AM
 */

#include "timer2.h"

#if TIMER2_INTERRUPT_ENABLE_FEATURE==INTERRUPT_FEATURE_ENABLE
static void (*TMR2_InterruptHandler)(void) = NULL;
#endif

static uint8 preload = ZERO_INIT;

/**
 * @brief Initializes Timer2 based on the provided configuration.
 * 
 * @param timer A pointer to the Timer2 configuration structure.
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The operation was successful.
 *         - E_NOT_OK: An error occurred during the operation.
 */
Std_ReturnType Timer2_Init(const timer2_t *timer)
{
    Std_ReturnType ret = E_OK;

    if (NULL == timer)
    {
        ret = E_NOT_OK;
    }
    else
    {
        //Disable the Timer2 Module
        TIMER2_MODULE_DISABLE();
        //Configure the Prescaler
        T2CONbits.T2CKPS = timer->prescaler_val;
        //Configure the Postscaler
        T2CONbits.TOUTPS = timer->postscaler_val;
        //Write preload value if there is.
        TMR2 = timer->timer2_preload;
        //Store the preload value 
        preload = timer->timer2_preload;

        //Configure the interrupt
#if TIMER2_INTERRUPT_ENABLE_FEATURE==INTERRUPT_FEATURE_ENABLE
        TIMER2_INTERRUPT_ENABLE();
        TIMER2_INTERRUPT_FLAG_CLEAR();
        TMR2_InterruptHandler = timer->TMR2_InterruptHandler;

        //Interrupt priority configurations
#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE
        INTERRUPT_PriorityLevelsEnable();
        if(INTERRUPT_HIGH_PRIORITY == timer->priority)
        {
            INTERRUPT_GlobalInterruptHighEnable();
            TIMER2_INT_HIGH_PRIORITY();
        }
        else if(INTERRUPT_LOW_PRIORITY == timer->priority)
        {
            INTERRUPT_GlobalInterruptLowEnable();
            TIMER2_INT_LOW_PRIORITY();
        }else{/* Nothing */}
#else 
        INTERRUPT_GlobalInterruptEnable();
        INTERRUPT_PeripheralInterruptEnable();
#endif
#endif
        //Enable the Timer2 Module
        TIMER2_MODULE_ENABLE();
    }
    return ret;
}

/**
 * @brief De-Initializes the Timer2 Module.
 * 
 * @param timer A pointer to the Timer2 configuration structure.
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The operation was successful.
 *         - E_NOT_OK: An error occurred during the operation.
 */
Std_ReturnType Timer2_DeInit(const timer2_t *timer)
{
    Std_ReturnType ret = E_OK;

    if (NULL == timer)
    {
        ret = E_NOT_OK;
    }
    else
    {
        //Disable Timer2 Module
        TIMER2_MODULE_DISABLE();
        //Disable Timer2 Interrupt
#if TIMER2_INTERRUPT_ENABLE_FEATURE==INTERRUPT_FEATURE_ENABLE
        TIMER2_INTERRUPT_DISABLE();
#endif
    }
    return ret;
}

/**
 * @brief Writes a 8-bit value to Timer2. 
 * 
 * @param timer A pointer to the Timer2 configuration structure.
 * @param val The 8-bit value to write to Timer2.
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The operation was successful.
 *         - E_NOT_OK: An error occurred during the operation.
 */
Std_ReturnType Timer2_Write_Value(const timer2_t *timer, uint8 val)
{
    Std_ReturnType ret = E_OK;

    if (NULL == timer)
    {
        ret = E_NOT_OK;
    }
    else
    {
        TMR2 = val;
    }
    return ret;
}

/**
 * @brief Reads a 8-bit value from Timer2.
 * 
 * @param timer A pointer to the Timer2 configuration structure.
 * @param val A pointer to store the value.
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The operation was successful.
 *         - E_NOT_OK: An error occurred during the operation.
 */
Std_ReturnType Timer2_Read(const timer2_t *timer, uint8 *val)
{
    Std_ReturnType ret = E_OK;

    if (NULL == timer || NULL == val)
    {
        ret = E_NOT_OK;
    }
    else
    {
        *val = TMR2;
    }
    return ret;
}

/**
 * @brief The Timer2 interrupt MCAL helper function
 * 
 */
void TMR2_ISR(void)
{
    //Timer2 interrupt occurred, the flag must be cleared.
    TIMER2_INTERRUPT_FLAG_CLEAR();
    //Write the preload value every time this ISR executes.
    TMR2 = preload;
    //CallBack func gets called every time this ISR executes.
    if(TMR2_InterruptHandler)
    {
        TMR2_InterruptHandler();
    }else{/* Nothing */}
}
