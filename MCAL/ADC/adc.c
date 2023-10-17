/* 
 * File:   adc.c
 * Author: Mohamed Sameh
 *
 * Created on September 23, 2023, 2:53 AM
 */
#include "adc.h"
#if ADC_INTERRUPT_ENABLE_FEATURE==INTERRUPT_FEATURE_ENABLE
static void (*ADC_InterruptHandler)(void) = NULL;
#endif
static inline void ADC_Input_Channel_Pin_Config(adc_channel_t channel);
static inline void ADC_Select_Result_Format(const adc_config_t *adc);
static inline void ADC_Select_Volt_Ref(const adc_config_t *adc);

/**
 * @brief Initializes the ADC based on the provided configuration.
 * 
 * This function is called before any other ADC function is called, This function configures
 * the adc settings, including enabling or disabling the ADC, setting the result format. 
 * 
 * @param adc A pointer to the ADC configuration structure.
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The operation was successful.
 *         - E_NOT_OK: An error occurred during the operation.
 */
Std_ReturnType ADC_Init(const adc_config_t *adc)
{
    Std_ReturnType ret = E_OK;

    if (NULL == adc)
    {
        ret = E_NOT_OK;
    }
    else
    {
        //Disable the ADC
        ADC_DISABLE();
        //Configure the acquisition time 
        ADCON2bits.ACQT = adc->acq_time;
        //Configure the conversion clock
        ADCON2bits.ADCS = adc->clock;
        //Configure the default channel
        ADCON0bits.CHS = adc->channel;
        ADC_Input_Channel_Pin_Config(adc->channel);
        //Configure the interrupt
#if ADC_INTERRUPT_ENABLE_FEATURE==INTERRUPT_FEATURE_ENABLE
        ADC_INTERRUPT_ENABLE();
        ADC_INTERRUPT_FLAG_CLEAR();
#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE
        INTERRUPT_PriorityLevelsEnable();
        if(INTERRUPT_HIGH_PRIORITY == adc->priority)
        {
            INTERRUPT_GlobalInterruptHighEnable();
            ADC_INT_HIGH_PRIORITY();
        }
        else if(INTERRUPT_LOW_PRIORITY == adc->priority)
        {
            INTERRUPT_GlobalInterruptLowEnable();
            ADC_INT_LOW_PRIORITY();
        }else{/* Nothing */}
#else
        INTERRUPT_GlobalInterruptEnable();
        INTERRUPT_PeripheralInterruptEnable();
#endif
        ADC_InterruptHandler = adc->ADC_InterruptHandler;
#endif
        //Configure the Result format
        ADC_Select_Result_Format(adc);  
        //Configure the voltage ref
        ADC_Select_Volt_Ref(adc);
        //Enable the ADC
        ADC_ENABLE();
    }
    return ret;
}

/**
 * @brief De-Initializes the ADC.
 * 
 * @param adc A pointer to the ADC configuration structure.
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The operation was successful.
 *         - E_NOT_OK: An error occurred during the operation.
 */
Std_ReturnType ADC_DeInit(const adc_config_t *adc)
{
    Std_ReturnType ret = E_OK;

    if (NULL == adc)
    {
        ret = E_NOT_OK;
    }
    else
    {
        //Disable the ADC
        ADC_DISABLE();
        //Disable the interrupt
#if ADC_INTERRUPT_ENABLE_FEATURE==INTERRUPT_FEATURE_ENABLE
        ADC_INTERRUPT_DISABLE();
#endif

    }
    return ret;
}

/**
 * @brief Selects the ADC channel for conversion.
 * 
 * This function configures the ADC to use the specified channel for analog-to-digital conversion.
 *
 * @param adc A pointer to the ADC configuration structure.
 * @param channel The channel to be selected.
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The operation was successful.
 *         - E_NOT_OK: An error occurred during the operation.
 */
Std_ReturnType ADC_Select_Channel(const adc_config_t *adc, adc_channel_t channel)
{
    Std_ReturnType ret = E_OK;

    if (NULL == adc)
    {
        ret = E_NOT_OK;
    }
    else
    {
        //Change the default channel
        ADCON0bits.CHS = channel;
        ADC_Input_Channel_Pin_Config(channel);
    }
    return ret;
}

/**
 * @brief Starts the ADC.
 * 
 * @param adc A pointer to the ADC configuration structure.
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The operation was successful.
 *         - E_NOT_OK: An error occurred during the operation.
 */
Std_ReturnType ADC_Start(const adc_config_t *adc)
{
    Std_ReturnType ret = E_OK;

    if (NULL == adc)
    {
        ret = E_NOT_OK;
    }
    else
    {
        ADC_START_CONV();
    }
    return ret;
}

/**
 * @brief Indicates whether the conversion is done or not yet.
 * 
 * @param adc A pointer to the ADC configuration structure.
 * @param adc_status A status indicating the conversion status
 *                   - true : the conversion is complete
 *                   - false : the conversion is not complete
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The operation was successful.
 *         - E_NOT_OK: An error occurred during the operation.
 */
Std_ReturnType ADC_IsDone(const adc_config_t *adc, uint8 *adc_status)
{
    Std_ReturnType ret = E_OK;

    if (NULL == adc || NULL == adc_status)
    {
        ret = E_NOT_OK;
    }
    else
    {
        *adc_status = (uint8)!(ADCON0bits.GODONE);
    }
    return ret;
}

/**
 * @brief Retrieves the result of the conversion.
 * 
 * @param adc A pointer to the ADC configuration structure.
 * @param adc_res A pointer to store the digital result of the conversion.
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The operation was successful.
 *         - E_NOT_OK: An error occurred during the operation.
 */
Std_ReturnType ADC_Get_Result(const adc_config_t *adc, uint16 *adc_res)
{
    Std_ReturnType ret = E_OK;

    if (NULL == adc || NULL == adc_res)
    {
        ret = E_NOT_OK;
    }
    else
    {
        if(ADC_RESULT_RIGHT == adc->res_format)
        {
            *adc_res = (uint16)((ADRESH << 8) + ADRESL);
        }
        else if(ADC_RESULT_LEFT == adc->res_format)
        {
            *adc_res = (uint16)(((ADRESH << 8) + ADRESL) >> 6);
        }
        else
        {
            *adc_res = (uint16)((ADRESH << 8) + ADRESL);
        }
    }
    return ret;
}

/**
 * @brief Performs a blocking ADC conversion.
 * 
 * This function selects the ADC channel, starts the conversion, waits until it's completed,
 * and retrieves the digital result.
 * 
 * @param adc A pointer to the ADC configuration structure.
 * @param channel The channel to be selected.
 * @param adc_res A pointer to store the digital result of the conversion.
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The operation was successful.
 *         - E_NOT_OK: An error occurred during the operation.
 */
Std_ReturnType ADC_Get_Conversion_Blocking(const adc_config_t *adc, adc_channel_t channel, 
                                  uint16 *adc_res)
{
    Std_ReturnType ret = E_OK;
    uint8 l_conv_status = ZERO_INIT;

    if (NULL == adc || NULL == adc_res)
    {
        ret = E_NOT_OK;
    }
    else
    {
        //Select the channel
        ret = ADC_Select_Channel(adc, channel);
        //Start the ADC
        ret = ADC_Start(adc);
        //Check if conversion is completed
        while (ADC_STATUS());
        ret = ADC_Get_Result(adc, adc_res);
    }
    return ret;
}

/**
 * @brief Initiates a non-blocking ADC conversion with interrupt    .
 * 
 * This function selects the ADC channel, starts the conversion, 
 * interrupts should be enabled to handle the completion of the conversion.        
 * 
 * @param adc A pointer to the ADC configuration structure.
 * @param channel The channel to be selected.
 * @param adc_res A pointer to store the digital result of the conversion.
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The operation was successful.
 *         - E_NOT_OK: An error occurred during the operation.
 */
Std_ReturnType ADC_Start_Conversion_Interrupt(const adc_config_t *adc, adc_channel_t channel)
{
    Std_ReturnType ret = E_OK;
    uint8 l_conv_status = ZERO_INIT;

    if (NULL == adc)
    {
        ret = E_NOT_OK;
    }
    else
    {
        //Select the channel
        ret = ADC_Select_Channel(adc, channel);
        //Start the ADC
        ret = ADC_Start(adc);
    }
    return ret;
}

/**
 * @brief Selects channel as input 
 * 
 * @param channel The channel to select
 */
static inline void ADC_Input_Channel_Pin_Config(adc_channel_t channel)
{
    //Disable the digital output driver
    switch (channel)
    {
    case ADC_CHANNEL_AN0 : SET_BIT(TRISA, _TRISA_RA0_POSN); break;
    case ADC_CHANNEL_AN1 : SET_BIT(TRISA, _TRISA_RA1_POSN); break;
    case ADC_CHANNEL_AN2 : SET_BIT(TRISA, _TRISA_RA2_POSN); break;
    case ADC_CHANNEL_AN3 : SET_BIT(TRISA, _TRISA_RA3_POSN); break;
    case ADC_CHANNEL_AN4 : SET_BIT(TRISA, _TRISA_RA5_POSN); break;

    case ADC_CHANNEL_AN5 : SET_BIT(TRISE, _TRISE_RE0_POSN); break;
    case ADC_CHANNEL_AN6 : SET_BIT(TRISE, _TRISE_RE1_POSN); break;
    case ADC_CHANNEL_AN7 : SET_BIT(TRISE, _TRISE_RE2_POSN); break;

    case ADC_CHANNEL_AN8 : SET_BIT(TRISB, _TRISB_RB2_POSN); break;
    case ADC_CHANNEL_AN9 : SET_BIT(TRISB, _TRISB_RB3_POSN); break;
    case ADC_CHANNEL_AN10 : SET_BIT(TRISB, _TRISB_RB1_POSN); break;
    case ADC_CHANNEL_AN11 : SET_BIT(TRISB, _TRISB_RB4_POSN); break;
    case ADC_CHANNEL_AN12 : SET_BIT(TRISB, _TRISB_RB0_POSN); break;
    default: break;
    }
}

/**
 * @brief Chooses the format of the resault 
 * 
 * @param adc A pointer to the ADC configuration structure.
 */
static inline void ADC_Select_Result_Format(const adc_config_t *adc)
{
    if(ADC_RESULT_RIGHT == adc->res_format)
    {
        ADC_RESULT_RIGHT_FORMAT();
    }
    else if(ADC_RESULT_LEFT == adc->res_format)
    {
        ADC_RESULT_LEFT_FORMAT();
    }
    else ADC_RESULT_RIGHT_FORMAT(); 
}

/**
 * @brief Selects voltage reference 
 * 
 * @param adc A pointer to the ADC configuration structure.
 */
static inline void ADC_Select_Volt_Ref(const adc_config_t *adc)
{
    if(ADC_VOLT_REF_ENABLE == adc->volt_reference)
    {
        ADC_VOLTAGE_REF_ENABLE();
    }
    else if(ADC_VOLT_REF_DISABLE == adc->volt_reference)
    {
        ADC_VOLTAGE_REF_DISABLE();
    }
    else ADC_VOLTAGE_REF_DISABLE();
}

/**
 * @brief The ADC interrupt MCAL helper function
 * 
 */

void ADC_ISR(void)
{
#if ADC_INTERRUPT_ENABLE_FEATURE==INTERRUPT_FEATURE_ENABLE
    //The ADC interrupt occurred, the flag must be cleared.
    ADC_INTERRUPT_FLAG_CLEAR();
    /* Code */

    //CallBack func gets called every time this ISR executes.
    if(ADC_InterruptHandler)
    {
        ADC_InterruptHandler();
    }
#endif    
}
