/* 
 * File:   timer3.c
 * Author: Mohamed Sameh
 *
 * Created on September 28, 2023, 3:59 AM
 */

#include "timer3.h"

#if TIMER3_INTERRUPT_ENABLE_FEATURE==INTERRUPT_FEATURE_ENABLE
static void (*TMR3_InterruptHandler)(void) = NULL;
#endif

static uint16 preload = ZERO_INIT;

static inline void Timer3_Mode_Select(const timer3_t *timer);
static inline void Timer3_RW_Mode_Select(const timer3_t *timer);

/**
 * @brief Initializes Timer3 based on the provided configuration.
 * 
 * @param timer A pointer to the Timer3 configuration structure.
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The operation was successful.
 *         - E_NOT_OK: An error occurred during the operation.
 */
Std_ReturnType Timer3_Init(const timer3_t *timer)
{
    Std_ReturnType ret = E_OK;

    if (NULL == timer)
    {
        ret = E_NOT_OK;
    }
    else
    {
        //Disable the Timer3 Module
        TIMER3_MODULE_DISABLE();
        //Configure the Prescaler
        T3CONbits.T3CKPS = timer->prescaler_val;
        //Select the Timer3 Mode
        Timer3_Mode_Select(timer);
        //Select the Read/Write operation mode
        Timer3_RW_Mode_Select(timer);
        //Write preload value if there is.
        TMR3H = (timer->timer3_preload >> 8);
        TMR3L = (uint8) (timer->timer3_preload);
        //Store the preload value 
        preload = timer->timer3_preload;

        //Configure the interrupt
#if TIMER3_INTERRUPT_ENABLE_FEATURE==INTERRUPT_FEATURE_ENABLE
        TIMER3_INTERRUPT_ENABLE();
        TIMER3_INTERRUPT_FLAG_CLEAR();
        TMR3_InterruptHandler = timer->TMR3_InterruptHandler;

        //Interrupt priority configurations
#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE
        INTERRUPT_PriorityLevelsEnable();
        if(INTERRUPT_HIGH_PRIORITY == timer->priority)
        {
            INTERRUPT_GlobalInterruptHighEnable();
            TIMER3_INT_HIGH_PRIORITY();
        }
        else if(INTERRUPT_LOW_PRIORITY == timer->priority)
        {
            INTERRUPT_GlobalInterruptLowEnable();
            TIMER3_INT_LOW_PRIORITY();
        }else{/* Nothing */}
#else 
        INTERRUPT_GlobalInterruptEnable();
        INTERRUPT_PeripheralInterruptEnable();
#endif
#endif
        //Enable the Timer3 Module
        TIMER3_MODULE_ENABLE();
    }
    return ret;
}

/**
 * @brief De-Initializes the Timer3 Module.
 * 
 * @param timer A pointer to the Timer3 configuration structure.
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The operation was successful.
 *         - E_NOT_OK: An error occurred during the operation.
 */
Std_ReturnType Timer3_DeInit(const timer3_t *timer)
{
    Std_ReturnType ret = E_OK;

    if (NULL == timer)
    {
        ret = E_NOT_OK;
    }
    else
    {
        //Disable Timer3 Module
        TIMER3_MODULE_DISABLE();
        //Disable Timer3 Interrupt
#if TIMER3_INTERRUPT_ENABLE_FEATURE==INTERRUPT_FEATURE_ENABLE
        TIMER3_INTERRUPT_DISABLE();
#endif
    }
    return ret;    
}

/**
 * @brief Writes a 16-bit value to Timer3. 
 * 
 * @param timer A pointer to the Timer3 configuration structure.
 * @param val The 16-bit value to write to Timer3.
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The operation was successful.
 *         - E_NOT_OK: An error occurred during the operation.
 */
Std_ReturnType Timer3_Write_Value(const timer3_t *timer, uint16 val)
{
    Std_ReturnType ret = E_OK;

    if (NULL == timer)
    {
        ret = E_NOT_OK;
    }
    else
    {
        TMR3H = (uint8)(val >> 8);
        TMR3L = (uint8) (val);
    }
    return ret;
}

/**
 * @brief Reads a 16-bit value from Timer3.
 * 
 * @param timer A pointer to the Timer3 configuration structure.
 * @param val A pointer to store the value.
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The operation was successful.
 *         - E_NOT_OK: An error occurred during the operation.
 */
Std_ReturnType Timer3_Read(const timer3_t *timer, uint16 *val)
{
    Std_ReturnType ret = E_OK;
    uint8 l_tmr3l = ZERO_INIT, l_tmr3h = ZERO_INIT;

    if (NULL == timer || NULL == val)
    {
        ret = E_NOT_OK;
    }
    else
    {
        l_tmr3l = TMR3L;
        l_tmr3h = TMR3H;
        *val = (uint16) ((l_tmr3h << 8) + l_tmr3l);
    }
    return ret;
}

/**
 * @brief Helper function to select the mode (Timer or Counter).
 * 
 * @param timer A pointer to the Timer3 configuration structure.
 */
static inline void Timer3_Mode_Select(const timer3_t *timer)
{
    if(TIMER3_TIMER_MODE_CFG == timer->timer3_mode)
    {
        TIMER3_TIMER_MODE_ENABLE();
    }
    else if(TIMER3_COUNTER_MODE_CFG == timer->timer3_mode)
    {
        TIMER3_COUNTER_MODE_ENABLE();
        if(TIMER3_SYNC_COUNTER_CFG == timer->timer3_counter_sync)
        {
            TIMER3_SYNC_COUNTER_ENABLE();
        }
        else if(TIMER3_ASYNC_COUNTER_CFG == timer->timer3_counter_sync)
        {
            TIMER3_ASYNC_COUNTER_ENABLE();
        }else{/* Nothing */}
    }
    else{/* Nothing */}
}

/**
 * @brief Helper function to select the Read/Write operation mode (Timer or Counter).
 * 
 * @param timer A pointer to the Timer3 configuration structure.
 */
static inline void Timer3_RW_Mode_Select(const timer3_t *timer)
{
    if(TIMER3_16BITS_RW_MODE_CFG == timer->timer3_rw_mode)
    {
        TIMER3_16BITS_RW_ENABLE();
    }
    else if(TIMER3_8BITS_RW_MODE_CFG == timer->timer3_rw_mode)
    {
        TIMER3_8BITS_RW_ENABLE();
    }
    else{/* Nothing */}
}

/**
 * @brief The Timer3 interrupt MCAL helper function
 * 
 */
void TMR3_ISR(void)
{
    //Timer3 interrupt occurred, the flag must be cleared.
    TIMER3_INTERRUPT_FLAG_CLEAR();
    //Write the preload value every time this ISR executes.
    TMR3H = (uint8)(preload >> 8);
    TMR3L = (uint8) (preload);
    //CallBack func gets called every time this ISR executes.
    if(TMR3_InterruptHandler)
    {
        TMR3_InterruptHandler();
    }else{/* Nothing */}
}