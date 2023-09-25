/* 
 * File:   internal_interrupt.h
 * Author: Mohamed Sameh
 *
 * Created on September 18, 2023, 3:31 PM
 */

#ifndef INTERNAL_INTERRUPT_H
#define	INTERNAL_INTERRUPT_H

/* -------------- Includes -------------- */
#include "interrupt_config.h"

/* -------------- Macro Declarations ------------- */
#if ADC_INTERRUPT_ENABLE_FEATURE==INTERRUPT_FEATURE_ENABLE
//This macro enables the ADC.
#define ADC_INTERRUPT_ENABLE()      (PIE1bits.ADIE = 1)
//This macro disables the ADC.
#define ADC_INTERRUPT_DISABLE()     (PIE1bits.ADIE = 0)
//This macro clears the interrupt flag for the ADC.
#define ADC_INTERRUPT_FLAG_CLEAR()  (PIR1bits.ADIF = 0)

#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE
//This macro will set the ADC interrupt as high priority.
#define ADC_INT_HIGH_PRIORITY()      (IPR1bits.ADIP = 1)
//This macro will set the ADC interrupt as low priority.
#define ADC_INT_LOW_PRIORITY()       (IPR1bits.ADIP = 0)
#endif

#endif
/* -------------- Macro Functions Declarat  ions --------------*/

/* -------------- Data Types Declarations --------------  */

/* -------------- Functions Declarations --------------*/



#endif	/* INTERNAL_INTERRUPT_H */

