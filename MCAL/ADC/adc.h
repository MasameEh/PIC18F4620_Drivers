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

/* -------------- Macro Functions Declarations --------------*/

//A/D conversion cycle in progress or A/D conversion completed/not in progress
#define ADC_STATUS()  (ADCON0bits.GODONE)

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
/* -------------- Functions Declarations --------------*/

#endif	/* ADC_H */

