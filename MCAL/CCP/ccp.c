/* 
 * File:   ccp.c
 * Author: Mohamed Sameh
 *
 * Created on September 29, 2023, 3:22 AM
 */
#include "ccp.h"

#if CCP1_INTERRUPT_ENABLE_FEATURE==INTERRUPT_FEATURE_ENABLE
static void (*CCP1_InterruptHandler)(void) = NULL;
#endif

#if CCP2_INTERRUPT_ENABLE_FEATURE==INTERRUPT_FEATURE_ENABLE
static void (*CCP2_InterruptHandler)(void) = NULL;
#endif

static void inline CCP_Interrupt_Config(const ccp_t *_ccp);
static void inline CCP_Mode_Timer_Select(const ccp_t *_ccp);
static Std_ReturnType inline CCP_Capture_Config(const ccp_t *_ccp);
static Std_ReturnType inline CCP_Compare_Config(const ccp_t *_ccp);

/**
 * @brief Initializes the CCP Module based on the provided configuration.
 * 
 * @param _ccp1 A pointer to the CCP configuration structure.
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The operation was successful.
 *         - E_NOT_OK: An error occurred during the operation.
 */
Std_ReturnType CCP_Init(const ccp_t *_ccp)
{
    Std_ReturnType ret = E_OK;

    if (NULL == _ccp)
    {
        ret = E_NOT_OK;
    }
    else
    {
        if(CCP_CAPTURE_MD == _ccp->mode)
        {
            CCP_Capture_Config(_ccp);
            CCP_Mode_Timer_Select(_ccp);
        }
        else if(CCP_COMPARE_MD == _ccp->mode)
        {
            CCP_Compare_Config(_ccp);
            CCP_Mode_Timer_Select(_ccp);
        }
#if CCP1_CFG_SELECTED_MODE==CCP_CFG_PMW_MODE_SELECTED || CCP2_CFG_SELECTED_MODE==CCP_CFG_PMW_MODE_SELECTED        
        else if(CCP_PMW_MD == _ccp->mode)
        {
            //PMW Freq Initialization
            PR2 = (uint8)((_XTAL_FREQ / (_ccp->PMW_Freq * 4.0 * _ccp->timer2_postscaler 
            *_ccp->timer2_prescaler)) - 1);

            if(CCP1_INST == _ccp->CCPx)
            {
                //Enable the PMW
                CCP1_SET_MODE(CCP_PMW_MODE);
            }
            else if (CCP2_INST == _ccp->CCPx)
            {
                //Enable the PMW
                CCP2_SET_MODE(CCP_PMW_MODE);   
            }
            else{/* Nothing */}
        }
#endif        
        else{/* Nothing */}

        //PINS Configurations
        gpio_pin_initialize(&(_ccp->pin));

        //Configure the interrupt
        CCP_Interrupt_Config(_ccp);
    }
    return ret;
}

/**
 * @brief De-initializes the CCP Module.
 * 
 * @param _ccp1 A pointer to the CCP configuration structure.
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The operation was successful.
 *         - E_NOT_OK: An error occurred during the operation.
 */
Std_ReturnType CCP_DeInit(const ccp_t *_ccp)
{
    Std_ReturnType ret = E_OK;

    if (NULL == _ccp)
    {
        ret = E_NOT_OK;
    }
    else
    {
        if(CCP1_INST == _ccp->CCPx)
        {
            //Disable CCP1 Module
            CCP1_SET_MODE(CCP_MODULE_DISABLE);
            //Disable CCP1 Interrupt
#if CCP1_INTERRUPT_ENABLE_FEATURE==INTERRUPT_FEATURE_ENABLE
            CCP1_INTERRUPT_DISABLE();
#endif 
        }
        else if (CCP2_INST == _ccp->CCPx)
        {
            //Disable CCP1 Module
            CCP2_SET_MODE(CCP_MODULE_DISABLE);
            //Disable CCP1 Interrupt
#if CCP2_INTERRUPT_ENABLE_FEATURE==INTERRUPT_FEATURE_ENABLE
            CCP2_INTERRUPT_DISABLE();
#endif 
        }     
    }
    return ret;
}

#if CCP1_CFG_SELECTED_MODE==CCP1_CFG_CAPTURE_MODE_SELECTED
/**
 * @brief Checks whether the data is ready or not.
 * 
 * @param _ccp A pointer to the CCP configuration structure.
 * @param status A pointer to return the status of the data.
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The operation was successful.
 *         - E_NOT_OK: An error occurred during the operation.
 */
Std_ReturnType CCP_Is_Captured_Data_Ready(const ccp_t *_ccp, uint8 *status)
{
    Std_ReturnType ret = E_OK;

    if (NULL == status)
    {
        ret = E_NOT_OK;
    }
    else
    {
        if(CCP1_INST == _ccp->CCPx)
        {
            if(CCP_CAPTURE_READY == PIR1bits.CCP1IF)
            {
                *status = CCP_CAPTURE_READY;
                CCP1_INTERRUPT_FLAG_CLEAR();
            }
            else if(CCP_CAPTURE_NOT_READY == PIR1bits.CCP1IF)
            {
                *status = CCP_CAPTURE_NOT_READY;
            }
            else{/* Nothing */} 
        }
        else if (CCP2_INST == _ccp->CCPx)
        {
            if(CCP_CAPTURE_READY == PIR2bits.CCP2IF)
            {
                *status = CCP_CAPTURE_READY;
                CCP2_INTERRUPT_FLAG_CLEAR();
            }
            else if(CCP_CAPTURE_NOT_READY == PIR2bits.CCP2IF)
            {
                *status = CCP_CAPTURE_NOT_READY;
            }
            else{/* Nothing */}
        } 
      
    }
    return ret;
}

/**
 * @brief Reads the Captured Data.
 * 
 * @param _ccp A pointer to the CCP configuration structure.
 * @param val A pointer to Stores the Captured Data.
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The operation was successful.
 *         - E_NOT_OK: An error occurred during the operation.
 */
Std_ReturnType CCP_Capture_Read_Val(const ccp_t *_ccp, uint16 *val)
{
    Std_ReturnType ret = E_OK;
    cpp_period_reg_t ccpx_reg = {.ccprx_high = 0, .ccprx_low = 0};

    if (NULL == val)
    {
        ret = E_NOT_OK;
    }   
    else
    {   
        if(CCP1_INST == _ccp->CCPx)
        {
            ccpx_reg.ccprx_low = CCPR1L;
            ccpx_reg.ccprx_high = CCPR1H;
            *val = ccpx_reg.ccprx_16Bits;
        }
        else if (CCP2_INST == _ccp->CCPx)
        {
            ccpx_reg.ccprx_low = CCPR2L;
            ccpx_reg.ccprx_high = CCPR2H;
            *val = ccpx_reg.ccprx_16Bits;
        }
        else{/* Nothing */}
    }
    return ret;
}

#endif

#if CCP1_CFG_SELECTED_MODE==CCP1_CFG_COMPARE_MODE_SELECTED
/**
 * @brief Checks whether the comparsion is done or not yet.
 * 
 * @param _ccp A pointer to the CCP configuration structure.
 * @param status A pointer to return the status of the comparsion.
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The operation was successful.
 *         - E_NOT_OK: An error occurred during the operation.
 */
Std_ReturnType CCP_Is_Compare_Completed(const ccp_t *_ccp, uint8 *status)
{
    Std_ReturnType ret = E_OK;

    if (NULL == status)
    {
        ret = E_NOT_OK;
    }
    else
    {
        if(CCP1_INST == _ccp->CCPx)
        {
            if(CCP_COMPARE_READY == PIR1bits.CCP1IF)
            {
                *status = CCP_COMPARE_READY;
                CCP1_INTERRUPT_FLAG_CLEAR();
            }
            else if(CCP_COMPARE_NOT_READY == PIR1bits.CCP1IF)
            {
                *status = CCP_COMPARE_NOT_READY;
            }
            else{/* Nothing */} 
        }
        else if (CCP2_INST == _ccp->CCPx)
        {
            if(CCP_COMPARE_READY == PIR2bits.CCP2IF)
            {
                *status = CCP_COMPARE_READY;
                CCP2_INTERRUPT_FLAG_CLEAR();
            }
            else if(CCP_COMPARE_NOT_READY == PIR2bits.CCP2IF)
            {
                *status = CCP_COMPARE_NOT_READY;
            }
            else{/* Nothing */}
        }       
    }
    return ret;
}

/**
 * @brief Writes the value to be compared.
 * 
 * @param _ccp A pointer to the CCP configuration structure.
 * @param com_value A value to be compared with.
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The operation was successful.
 *         - E_NOT_OK: An error occurred during the operation.
 */
Std_ReturnType CCP_Compare_Set_Value(const ccp_t *_ccp, uint16 com_value)
{
    Std_ReturnType ret = E_OK;
    cpp_period_reg_t ccp_reg = {.ccprx_high = 0, .ccprx_low = 0};
    ccp_reg.ccprx_16Bits = com_value;

    if (NULL == _ccp)
    {
        ret = E_NOT_OK;
    }
    else
    {
        if(CCP1_INST == _ccp->CCPx)
        {
            CCPR1L = ccp_reg.ccprx_low;
            CCPR1H = ccp_reg.ccprx_high;
        }
        else if (CCP2_INST == _ccp->CCPx)
        {
            CCPR2L = ccp_reg.ccprx_low;
            CCPR2H = ccp_reg.ccprx_high;
        }
        else{/* Nothing */}
    }
    return ret;
}
#endif

#if CCP1_CFG_SELECTED_MODE==CCP_CFG_PMW_MODE_SELECTED
/**
 * @brief Sets the Duty Cycle.
 * 
 * @param _ccp A pointer to the CCP configuration structure.
 * @param duty Duty cycle on the scale from 0 to 100.
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The operation was successful.
 *         - E_NOT_OK: An error occurred during the operation.
 */
Std_ReturnType CCP_PMW_Set_Duty(const ccp_t *_ccp, uint8 duty)
{
    Std_ReturnType ret = E_OK;
    uint16 duty_temp = ZERO_INIT;

    if (NULL == _ccp)
    {
        ret = E_NOT_OK;
    }
    else
    {
        duty_temp = (uint16)((PR2 + 1) * (duty / 100.0) * 4.0);

        if(CCP1_INST == _ccp->CCPx)
        {
            CCP1CONbits.DC1B = (uint8)(duty_temp & 0x0003);
            CCPR1L = (uint8)(duty_temp >> 2);
        }
        else if (CCP2_INST == _ccp->CCPx)
        {
            CCP2CONbits.DC2B = (uint8)(duty_temp & 0x0003);
            CCPR2L = (uint8)(duty_temp >> 2);
        }
        else{/* Nothing */}
    }
    return ret;
}

/**
 * @brief Stops the PMW.
 * 
 * @param _ccp A pointer to the CCP configuration structure.
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The operation was successful.
 *         - E_NOT_OK: An error occurred during the operation.
 */
Std_ReturnType CCP_PMW_Stop(const ccp_t *_ccp)  
{
    Std_ReturnType ret = E_OK;

    if (NULL == _ccp)
    {
        ret = E_NOT_OK;
    }
    else
    {
        if(CCP1_INST == _ccp->CCPx)
        {
            //CCP1 Mode disable
            CCP1_SET_MODE(CCP_MODULE_DISABLE);
        }
        else if (CCP2_INST == _ccp->CCPx)
        {
            //CCP2 Mode disable
            CCP2_SET_MODE(CCP_MODULE_DISABLE);   
        }
    }
    return ret;
}   
#endif


/**
 * @brief The CCP1 interrupt MCAL helper function
 * 
 */
void CCP1_ISR(void)
{
    //CCP1 interrupt occurred, the flag must be cleared.
    CCP1_INTERRUPT_FLAG_CLEAR();
    //CallBack func gets called every time this ISR executes.
    if(CCP1_InterruptHandler)
    {
        CCP1_InterruptHandler();
    }else{/* Nothing */}
}

/**
 * @brief The Timer0 interrupt MCAL helper function
 * 
 */
void CCP2_ISR(void)
{
    //CCP2 interrupt occurred, the flag must be cleared.
    CCP2_INTERRUPT_FLAG_CLEAR();
    //CallBack func gets called every time this ISR executes.
    if(CCP2_InterruptHandler)
    {
        CCP2_InterruptHandler();
    }else{/* Nothing */}
}


static void inline CCP_Interrupt_Config(const ccp_t *_ccp)
{  
//Configure CCP1 Interrupt
if(CCP1_INST == _ccp->CCPx)
{
#if CCP1_INTERRUPT_ENABLE_FEATURE==INTERRUPT_FEATURE_ENABLE
    CCP1_INTERRUPT_ENABLE();
    CCP1_INTERRUPT_FLAG_CLEAR();
    CCP1_InterruptHandler = _ccp->CCP1_InterruptHandler;
        //Interrupt priority configurations
#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE
    INTERRUPT_PriorityLevelsEnable();
    if(INTERRUPT_HIGH_PRIORITY == CCP1->CCP1_priority)
    {
        INTERRUPT_GlobalInterruptHighEnable();
        CCP1_INT_HIGH_PRIORITY();
    }
    else if(INTERRUPT_LOW_PRIORITY == CCP1->CCP1_priority)
    {
        INTERRUPT_GlobalInterruptLowEnable();
        CCP1_INT_LOW_PRIORITY();
    }else{/* Nothing */}
#else 
    INTERRUPT_GlobalInterruptEnable();
    INTERRUPT_PeripheralInterruptEnable();
#endif
#endif
}
//Configure CCP2 Interrupt
else if (CCP2_INST == _ccp->CCPx)
{
#if CCP2_INTERRUPT_ENABLE_FEATURE==INTERRUPT_FEATURE_ENABLE
    CCP2_INTERRUPT_ENABLE();
    CCP2_INTERRUPT_FLAG_CLEAR();
    CCP2_InterruptHandler = _ccp->CCP2_InterruptHandler;
    //Interrupt priority configurations
#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE
    INTERRUPT_PriorityLevelsEnable();
    if(INTERRUPT_HIGH_PRIORITY == CCP2->CCP2_priority)
    {
        INTERRUPT_GlobalInterruptHighEnable();
        CCP2_INT_HIGH_PRIORITY();
    }
    else if(INTERRUPT_LOW_PRIORITY == CCP2->CCP2_priority)
    {
        INTERRUPT_GlobalInterruptLowEnable();
        CCP2_INT_LOW_PRIORITY();
    }else{/* Nothing */}
#else 
    INTERRUPT_GlobalInterruptEnable();
    INTERRUPT_PeripheralInterruptEnable();
#endif
#endif
}
else{/* Nothing */}
}

static void inline CCP_Mode_Timer_Select(const ccp_t *_ccp)
{
    if(CCP1_CCP2_TIMER3 == _ccp->ccp_timer)
    {
        /* Timer3 is the capture/compare clock source for the CCP modules */
        T3CONbits.T3CCP1 = 0;
        T3CONbits.T3CCP2 = 1;
    }
    else if(CCP1_TIMER1_CCP2_TIMER3 == _ccp->ccp_timer)
    {
        /* Timer3 is the capture/compare clock source for CCP2;
           Timer1 is the capture/compare clock source for CCP1 */
        T3CONbits.T3CCP1 = 1;
        T3CONbits.T3CCP2 = 0;
    }
    else if(CCP1_CCP2_TIMER1 == _ccp->ccp_timer)
    {
        /* Timer1 is the capture/compare clock source for the CCP modules */
        T3CONbits.T3CCP1 = 0;
        T3CONbits.T3CCP2 = 0;
    }
    else{/* Nothing */}
}

static Std_ReturnType inline CCP_Capture_Config(const ccp_t *_ccp)
{
    Std_ReturnType ret = E_OK;

    if(CCP1_INST == _ccp->CCPx)
    {
        /* CCP1 Module Capture mode Init */
        switch (_ccp->mode_variant)
        {
            case CCP_CAPTURE_MODE_1_FALLING_EDGE: CCP1_SET_MODE(CCP_CAPTURE_MODE_1_FALLING_EDGE); break;
            case CCP_CAPTURE_MODE_1_RISING_EDGE:  CCP1_SET_MODE(CCP_CAPTURE_MODE_1_RISING_EDGE);  break;
            case CCP_CAPTURE_MODE_4_RISING_EDGE:  CCP1_SET_MODE(CCP_CAPTURE_MODE_4_RISING_EDGE);  break;
            case CCP_CAPTURE_MODE_16_RISING_EDGE: CCP1_SET_MODE(CCP_CAPTURE_MODE_16_RISING_EDGE); break;                                            
            default: ret = E_NOT_OK; break;
        }
    }
    else if (CCP2_INST == _ccp->CCPx)
    {
        /* CCP2 Module Capture mode Init */
        switch (_ccp->mode_variant)
        {
            case CCP_CAPTURE_MODE_1_FALLING_EDGE: CCP2_SET_MODE(CCP_CAPTURE_MODE_1_FALLING_EDGE); break;
            case CCP_CAPTURE_MODE_1_RISING_EDGE:  CCP2_SET_MODE(CCP_CAPTURE_MODE_1_RISING_EDGE);  break;
            case CCP_CAPTURE_MODE_4_RISING_EDGE:  CCP2_SET_MODE(CCP_CAPTURE_MODE_4_RISING_EDGE);  break;
            case CCP_CAPTURE_MODE_16_RISING_EDGE: CCP2_SET_MODE(CCP_CAPTURE_MODE_16_RISING_EDGE); break;                                            
            default: ret = E_NOT_OK; break;
        }
    }
    return ret;
}

static Std_ReturnType inline CCP_Compare_Config(const ccp_t *_ccp)
{
    Std_ReturnType ret = E_OK;

    if(CCP1_INST == _ccp->CCPx)
    {
        /* CCP1 Module Compare mode Init */
        switch (_ccp->mode_variant)
        {
            case CCP_COMPARE_MODE_TOGGLE_ON_MATCH:   CCP1_SET_MODE(CCP_COMPARE_MODE_TOGGLE_ON_MATCH);  break;
            case CCP_COMPARE_MODE_SET_PIN_LOW:       CCP1_SET_MODE(CCP_COMPARE_MODE_SET_PIN_LOW);      break;
            case CCP_COMPARE_MODE_SET_PIN_HIGH:      CCP1_SET_MODE(CCP_COMPARE_MODE_SET_PIN_HIGH);     break;
            case CCP_COMPARE_MODE_GEN_SW_INTERRUPT:  CCP1_SET_MODE(CCP_COMPARE_MODE_GEN_SW_INTERRUPT); break;
            case CCP_COMPARE_MODE_GEN_EVENT:         CCP1_SET_MODE(CCP_COMPARE_MODE_GEN_EVENT);        break;                                               
            default: ret = E_NOT_OK; break;
        }
    }
    else if (CCP2_INST == _ccp->CCPx)
    {
        /* CCP2 Module Compare mode Init */
        switch (_ccp->mode_variant)
        {
            case CCP_COMPARE_MODE_TOGGLE_ON_MATCH:  CCP2_SET_MODE(CCP_COMPARE_MODE_TOGGLE_ON_MATCH);  break;
            case CCP_COMPARE_MODE_SET_PIN_LOW:      CCP2_SET_MODE(CCP_COMPARE_MODE_SET_PIN_LOW);      break;
            case CCP_COMPARE_MODE_SET_PIN_HIGH:     CCP2_SET_MODE(CCP_COMPARE_MODE_SET_PIN_HIGH);     break;
            case CCP_COMPARE_MODE_GEN_SW_INTERRUPT: CCP2_SET_MODE(CCP_COMPARE_MODE_GEN_SW_INTERRUPT); break;
            case CCP_COMPARE_MODE_GEN_EVENT:        CCP2_SET_MODE(CCP_COMPARE_MODE_GEN_EVENT);        break;                                               
            default: ret = E_NOT_OK; break;
        }
    }
    return ret;
}