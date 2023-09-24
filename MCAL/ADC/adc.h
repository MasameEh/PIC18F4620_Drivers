/* 
 * File:   adc.h
 * Author: Mohamed Sameh
 *
 * Created on September 23, 2023, 2:53 AM
 */

#ifndef ADC_H
#define	ADC_H

/* -------------- Includes -------------- */
#include "../GPIO/gpio.h"
#include "../interrupt/internal_interrupt.h"
#include "adc_cfg.h"

/* -------------- Macro Declarations ------------- */
/**
 * @brief Analog-To-Digital Port Configuration Control
 * @note  When ADC_AN4_ANALOG_FUNCTIONALITY is configured, this means
 *        AN4, AN3, AN2, AN1, AN0 are Analog functionality.
 */
#define ADC_AN0_ANALOG_FUNCTIONALITY    0x0E
#define ADC_AN1_ANALOG_FUNCTIONALITY    0x0D
#define ADC_AN2_ANALOG_FUNCTIONALITY    0x0C    
#define ADC_AN3_ANALOG_FUNCTIONALITY    0x0B
#define ADC_AN4_ANALOG_FUNCTIONALITY    0x0A
#define ADC_AN5_ANALOG_FUNCTIONALITY    0x09
#define ADC_AN6_ANALOG_FUNCTIONALITY    0x08
#define ADC_AN7_ANALOG_FUNCTIONALITY    0x07
#define ADC_AN8_ANALOG_FUNCTIONALITY    0x06
#define ADC_AN9_ANALOG_FUNCTIONALITY    0x05
#define ADC_AN10_ANALOG_FUNCTIONALITY   0x04
#define ADC_AN11_ANALOG_FUNCTIONALITY   0x03
#define ADC_AN12_ANALOG_FUNCTIONALITY   0x02

#define ADC_ALL_ANALOG_FUNCTIONALITY    0x00
#define ADC_ALL_DIGITAL_FUNCTIONALITY   0x0F

#define ADC_RESULT_RIGHT    0x01U
#define ADC_RESULT_LEFT     0x00U

#define ADC_VOLT_REF_ENABLE   0x01U
#define ADC_VOLT_REF_DISABLE  0x00U

/* -------------- Macro Functions Declarations --------------*/

//A/D conversion cycle in progress or A/D conversion completed/not in progress
#define ADC_STATUS()  (ADCON0bits.GODONE)

//This macro will start the Analog-to-Digital Conversion
#define ADC_START_CONV()  (ADCON0bits.GODONE = 1)

//Enable or disable the ADC
#define ADC_ENABLE()   (ADCON0bits.ADON = 1)
#define ADC_DISABLE()  (ADCON0bits.ADON = 0)

/**
 * @brief  Voltage Reference Configuration
 * @note   ADC_VOLTAGE_REF_ENABLE(): Voltage reference is VREF- and VREF+
 *         ADC_VOLTAGE_REF_ENABLE(): Voltage reference is VSS and VDD
 */
#define ADC_VOLTAGE_REF_ENABLE()    do{ ADCON1bits.VCFG1 = 1;\
                                        ADCON1bits.VCFG1 = 1;\
                                    }while(0)
#define ADC_VOLTAGE_REF_DISABLE()   do{ ADCON1bits.VCFG1 = 0;\
                                        ADCON1bits.VCFG1 = 0;\
                                    }while(0)                                    

/**
 * @brief Analog-to-Digital Port Config Control
 * @note EXAMPLE: ADC_AN_DIG_PORT_CONFIG(ADC_ANx_ANALOG_FUNCTIONALITY):
 *                this means ANx and every pin before are Analog functionality.
 * @ref Analog-To-Digital Port Configuration Control
 */
#define ADC_AN_DIG_PORT_CONFIG(_CONFIG)  (ADCON1bits.PCFG = _CONFIG)

/**
 * @brief A/D Result Format Select
 */
#define ADC_RESULT_RIGHT_FORMAT()   (ADCON2bits.ADFM = 1) 
#define ADC_RESULT_LEFT_FORMAT()    (ADCON2bits.ADFM = 0)
 
/* -------------- Data Types Declarations --------------  */
/**
 * @brief Analog Channel Select
 * 
 */
typedef enum
{
    ADC_CHANNEL_AN0 = 0,
    ADC_CHANNEL_AN1,
    ADC_CHANNEL_AN2,
    ADC_CHANNEL_AN3,
    ADC_CHANNEL_AN4,
    ADC_CHANNEL_AN5,    
    ADC_CHANNEL_AN6,
    ADC_CHANNEL_AN7,
    ADC_CHANNEL_AN8,
    ADC_CHANNEL_AN9,
    ADC_CHANNEL_AN10,
    ADC_CHANNEL_AN11,
    ADC_CHANNEL_AN12,
}adc_channel_t;

/**
 * @brief A/D  Acquisition Time Select bits
 * @note  Acquisition Time is the required time to ADC 
 *        to capture the input signal during the sampling. 
 *        It mustn't be less tha 2.4 µs.        
 */
typedef enum 
{
    ADC_0_TAD = 0,
    ADC_2_TAD,
    ADC_4_TAD,
    ADC_6_TAD,
    ADC_8_TAD,
    ADC_12_TAD,
    ADC_16_TAD,
    ADC_20_TAD
}adc_acq_time_t ;

/**
 * @brief A/D Conversion Clock Select
 * 
 */
typedef enum 
{
    ADC_CLOCK_FOSC_DIV_2 = 0,
    ADC_CLOCK_FOSC_DIV_8,
    ADC_CLOCK_FOSC_DIV_32,
    ADC_CLOCK_FRC,
    ADC_CLOCK_FOSC_DIV_4,
    ADC_CLOCK_FOSC_DIV_16,
    ADC_CLOCK_FOSC_DIV_64
}adc_conversion_clock_t ;

typedef struct
{
    void (* ADC_InterruptHandler)(void);
    adc_acq_time_t acq_time;        /* @ref adc_acq_time_t */
    adc_conversion_clock_t clock;   /* @ref adc_conversion_clock_t */
    adc_channel_t channel;          /* @ref adc_channel_t */
    uint8 volt_reference : 1;           /* Voltage Reference Configuration */
    uint8 res_format : 1;           /* A/D Result Format Select */
}adc_config_t;

/* -------------- Functions Declarations --------------*/
Std_ReturnType ADC_Init(const adc_config_t *adc);
Std_ReturnType ADC_DeInit(const adc_config_t *adc);
Std_ReturnType ADC_Select_Channel(const adc_config_t *adc, adc_channel_t channel);
Std_ReturnType ADC_Start(const adc_config_t *adc);
Std_ReturnType ADC_IsDone(const adc_config_t *adc, uint8 *adc_status);
Std_ReturnType ADC_Get_Result(const adc_config_t *adc, uint16 *adc_res);
Std_ReturnType ADC_Get_Conversion_Blocking(const adc_config_t *adc, adc_channel_t channel, 
                                  uint16 *adc_res);

#endif	/* ADC_H */

