/* 
 * File:   timer1.c
 * Author: Mohamed Sameh
 *
 * Created on September 27, 2023, 5:52 PM
 */

#include "timer1.h"

#if TIMER1_INTERRUPT_ENABLE_FEATURE==INTERRUPT_FEATURE_ENABLE
static void (*TMR1_InterruptHandler)(void) = NULL;
#endif

static uint16 preload = ZERO_INIT;

static inline void Timer1_Mode_Select(const timer1_t *timer);
static inline void Timer1_RW_Mode_Select(const timer1_t *timer);
static inline void Timer1_Osc_Config(const timer1_t *timer);

/**
 * @brief Initializes Timer1 based on the provided configuration.
 * 
 * @param timer A pointer to the Timer1 configuration structure.
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The operation was successful.
 *         - E_NOT_OK: An error occurred during the operation.
 */
Std_ReturnType Timer1_Init(const timer1_t *timer)
{
    Std_ReturnType ret = E_OK;

    if (NULL == timer)
    {
        ret = E_NOT_OK;
    }
    else
    {
        //Disable the Timer1 Module
        TIMER1_MODULE_DISABLE();
        //Configure the Prescaler
        T1CONbits.T1CKPS = timer->prescaler_val;
        //Select the Timer1 Mode
        Timer1_Mode_Select(timer);
        //Select the Read/Write operation mode
        Timer1_RW_Mode_Select(timer);
        //Write preload value if there is.
        TMR0H = (timer->timer1_preload >> 8);
        TMR0L = (uint8) (timer->timer1_preload);
        //Store the preload value 
        preload = timer->timer1_preload;
        //Configure Timer1 Oscllaitor 
        Timer1_Osc_Config(timer);
        //Configure the interrupt
#if TIMER1_INTERRUPT_ENABLE_FEATURE==INTERRUPT_FEATURE_ENABLE
        TIMER1_INTERRUPT_ENABLE();
        TIMER1_INTERRUPT_FLAG_CLEAR();
        TMR1_InterruptHandler = timer->TMR1_InterruptHandler;

        //Interrupt priority configurations
#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE
        INTERRUPT_PriorityLevelsEnable();
        if(INTERRUPT_HIGH_PRIORITY == timer->priority)
        {
            INTERRUPT_GlobalInterruptHighEnable();
            TIMER1_INT_HIGH_PRIORITY();
        }
        else if(INTERRUPT_LOW_PRIORITY == timer->priority)
        {
            INTERRUPT_GlobalInterruptLowEnable();
            TIMER1_INT_LOW_PRIORITY();
        }else{/* Nothing */}
#else 
        INTERRUPT_GlobalInterruptEnable();
        INTERRUPT_PeripheralInterruptEnable();
#endif
#endif
        //Enable the Timer1 Module
        TIMER1_MODULE_ENABLE();
    }
    return ret;
}

/**
 * @brief De-Initializes the Timer1 Module.
 * 
 * @param timer A pointer to the Timer1 configuration structure.
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The operation was successful.
 *         - E_NOT_OK: An error occurred during the operation.
 */
Std_ReturnType Timer1_DeInit(const timer1_t *timer)
{
    Std_ReturnType ret = E_OK;

    if (NULL == timer)
    {
        ret = E_NOT_OK;
    }
    else
    {
        //Disable Timer1 Module
        TIMER1_MODULE_DISABLE();
        //Disable Timer1 Interrupt
#if TIMER1_INTERRUPT_ENABLE_FEATURE==INTERRUPT_FEATURE_ENABLE
        TIMER1_INTERRUPT_DISABLE();
#endif
    }
    return ret;
}

/**
 * @brief Writes a 16-bit value to Timer1. 
 * 
 * @param timer A pointer to the Timer1 configuration structure.
 * @param val The 16-bit value to write to Timer1.
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The operation was successful.
 *         - E_NOT_OK: An error occurred during the operation.
 */
Std_ReturnType Timer1_Write_Value(const timer1_t *timer, uint16 val)
{
    Std_ReturnType ret = E_OK;

    if (NULL == timer)
    {
        ret = E_NOT_OK;
    }
    else
    {
        TMR1H = (uint8)(val >> 8);
        TMR1L = (uint8) (val);
    }
    return ret;
}

/**
 * @brief Reads a 16-bit value from Timer1.
 * 
 * @param timer A pointer to the Timer1 configuration structure.
 * @param val A pointer to store the value.
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The operation was successful.
 *         - E_NOT_OK: An error occurred during the operation.
 */
Std_ReturnType Timer1_Read(const timer1_t *timer, uint16 *val)
{
    Std_ReturnType ret = E_OK;
    uint8 l_tmr1l = ZERO_INIT, l_tmr1h = ZERO_INIT;

    if (NULL == timer || NULL == val)
    {
        ret = E_NOT_OK;
    }
    else
    {
        l_tmr1l = TMR1L;
        l_tmr1h = TMR1H;
        *val = (uint16) ((l_tmr1h << 8) + l_tmr1l);
    }
    return ret;
}

/**
 * @brief Helper function to select the mode (Timer or Counter).
 * 
 * @param timer A pointer to the Timer1 configuration structure.
 */
static inline void Timer1_Mode_Select(const timer1_t *timer)
{
    if(TIMER1_TIMER_MODE_CFG == timer->timer1_mode)
    {
        TIMER1_TIMER_MODE_ENABLE();
    }
    else if(TIMER1_COUNTER_MODE_CFG == timer->timer1_mode)
    {
        TIMER1_COUNTER_MODE_ENABLE();
        if(TIMER1_SYNC_COUNTER_CFG == timer->timer1_counter_sync)
        {
            TIMER1_SYNC_COUNTER_ENABLE();
        }
        else if(TIMER1_ASYNC_COUNTER_CFG == timer->timer1_counter_sync)
        {
            TIMER1_ASYNC_COUNTER_ENABLE();
        }else{/* Nothing */}
    }
    else{/* Nothing */}
}

/**
 * @brief Helper function to select the Read/Write operation mode (Timer or Counter).
 * 
 * @param timer A pointer to the Timer1 configuration structure.
 */
static inline void Timer1_RW_Mode_Select(const timer1_t *timer)
{
    if(TIMER1_16BITS_RW_MODE_CFG == timer->timer1_rw_mode)
    {
        TIMER1_16BITS_RW_ENABLE();
    }
    else if(TIMER1_8BITS_RW_MODE_CFG == timer->timer1_rw_mode)
    {
        TIMER1_8BITS_RW_ENABLE();
    }
    else{/* Nothing */}
}

/**
 * @brief Helper function to enable or disable the osclliator of Timer1.
 * 
 * @param timer A pointer to the Timer1 configuration structure.
 */
static inline void Timer1_Osc_Config(const timer1_t *timer)
{
    if(TIMER1_OSC_ENABLE_CFG == timer->timer1_osc_enable)
    {
        TIMER1_OSC_ENABLE();
    }
    else if(TIMER1_OSC_DISABLE_CFG == timer->timer1_osc_enable)
    {
        TIMER1_OSC_DISABLE();
    }
    else{/* Nothing */}
}

/**
 * @brief The Timer1 interrupt MCAL helper function
 * 
 */
void TMR1_ISR(void)
{
    //Timer1 interrupt occurred, the flag must be cleared.
    TIMER1_INTERRUPT_FLAG_CLEAR();
    //Write the preload value every time this ISR executes.
    TMR1H = (uint8)(preload >> 8);
    TMR1L = (uint8) (preload);
    //CallBack func gets called every time this ISR executes.
    if(TMR1_InterruptHandler)
    {
        TMR1_InterruptHandler();
    }else{/* Nothing */}
}
