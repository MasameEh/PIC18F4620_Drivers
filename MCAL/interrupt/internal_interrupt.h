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
//This macro enables the interrupt for the ADC module.
#define ADC_INTERRUPT_ENABLE()      (PIE1bits.ADIE = 1)
//This macro disables the interrupt for the ADC module.
#define ADC_INTERRUPT_DISABLE()     (PIE1bits.ADIE = 0)
//This macro clears the interrupt flag for the ADC.
#define ADC_INTERRUPT_FLAG_CLEAR()  (PIR1bits.ADIF = 0)

#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE
//This macro sets the ADC interrupt as high priority.
#define ADC_INT_HIGH_PRIORITY()      (IPR1bits.ADIP = 1)
//This macro sets the ADC interrupt as low priority.
#define ADC_INT_LOW_PRIORITY()       (IPR1bits.ADIP = 0)
#endif
#endif

#if TIMER0_INTERRUPT_ENABLE_FEATURE==INTERRUPT_FEATURE_ENABLE
//This macro enables the interrupt for Timer0 module.
#define TIMER0_INTERRUPT_ENABLE()      (INTCONbits.TMR0IE = 1)
//This macro disables the interrupt for Timer0 module.
#define TIMER0_INTERRUPT_DISABLE()     (INTCONbits.TMR0IE = 0)
//This macro clears the interrupt flag for Timer0.
#define TIMER0_INTERRUPT_FLAG_CLEAR()  (INTCONbits.TMR0IF = 0)

#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE
//This macro sets Timer0 interrupt as high priority.
#define TIMER0_INT_HIGH_PRIORITY()      (INTCON2bits.TMR0IP = 1)
//This macro sets Timer0 interrupt as low priority.
#define TIMER0_INT_LOW_PRIORITY()       (INTCON2bits.TMR0IP = 0)
#endif
#endif

#if TIMER1_INTERRUPT_ENABLE_FEATURE==INTERRUPT_FEATURE_ENABLE
//This macro enables the interrupt for Timer1 module.
#define TIMER1_INTERRUPT_ENABLE()      (PIE1bits.TMR1IE = 1)
//This macro disables the interrupt for Timer1 module.
#define TIMER1_INTERRUPT_DISABLE()     (PIE1bits.TMR1IE  = 0)
//This macro clears the interrupt flag for Timer1.
#define TIMER1_INTERRUPT_FLAG_CLEAR()  (PIR1bits.TMR1IF = 0)

#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE
//This macro sets Timer1 interrupt as high priority.
#define TIMER1_INT_HIGH_PRIORITY()      (IPR1bits.TMR1IP = 1)
//This macro sets Timer1 interrupt as low priority.
#define TIMER1_INT_LOW_PRIORITY()       (IPR1bits.TMR1IP = 0)
#endif
#endif

#if TIMER2_INTERRUPT_ENABLE_FEATURE==INTERRUPT_FEATURE_ENABLE
//This macro enables the interrupt for Timer2 module.
#define TIMER2_INTERRUPT_ENABLE()      (PIE1bits.TMR2IE = 1)
//This macro disables the interrupt for Timer2 module.
#define TIMER2_INTERRUPT_DISABLE()     (PIE1bits.TMR2IE  = 0)
//This macro clears the interrupt flag for Timer2.
#define TIMER2_INTERRUPT_FLAG_CLEAR()  (PIR1bits.TMR2IF = 0)

#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE
//This macro sets Timer2 interrupt as high priority.
#define TIMER2_INT_HIGH_PRIORITY()      (IPR1bits.TMR2IP = 1)
//This macro sets Timer2 interrupt as low priority.
#define TIMER2_INT_LOW_PRIORITY()       (IPR1bits.TMR2IP = 0)
#endif
#endif

#if TIMER3_INTERRUPT_ENABLE_FEATURE==INTERRUPT_FEATURE_ENABLE
//This macro enables the interrupt for Timer3 module.
#define TIMER3_INTERRUPT_ENABLE()      (PIE2bits.TMR3IE = 1)
//This macro disables the interrupt for Timer3 module.
#define TIMER3_INTERRUPT_DISABLE()     (PIE2bits.TMR3IE  = 0)
//This macro clears the interrupt flag for Timer3.
#define TIMER3_INTERRUPT_FLAG_CLEAR()  (PIR2bits.TMR3IF = 0)

#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE
//This macro sets Timer3 interrupt as high priority.
#define TIMER3_INT_HIGH_PRIORITY()      (IPR2bits.TMR3IP = 1)
//This macro sets Timer3 interrupt as low priority.
#define TIMER3_INT_LOW_PRIORITY()       (IPR2bits.TMR3IP = 0)
#endif
#endif

#if CCP1_INTERRUPT_ENABLE_FEATURE==INTERRUPT_FEATURE_ENABLE
//This macro enables the interrupt for CCP1 module.
#define CCP1_INTERRUPT_ENABLE()      (PIE1bits.CCP1IE = 1)
//This macro disables the interrupt for CCP1 module.
#define CCP1_INTERRUPT_DISABLE()     (PIE1bits.CCP1IE  = 0)
//This macro clears the interrupt flag for CCP1.
#define CCP1_INTERRUPT_FLAG_CLEAR()  (PIR1bits.CCP1IF = 0)

#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE
//This macro sets CCP1 interrupt as high priority.
#define CCP1_INT_HIGH_PRIORITY()      (IPR1bits.CCP1IP = 1)
//This macro sets CCP1 interrupt as low priority.
#define CCP1_INT_LOW_PRIORITY()       (IPR1bits.CCP1IP = 0)
#endif
#endif

#if CCP2_INTERRUPT_ENABLE_FEATURE==INTERRUPT_FEATURE_ENABLE
//This macro enables the interrupt for CCP2 module.
#define CCP2_INTERRUPT_ENABLE()      (PIE2bits.CCP2IE = 1)
//This macro disables the interrupt for CCP2 module.
#define CCP2_INTERRUPT_DISABLE()     (PIE2bits.CCP2IE  = 0)
//This macro clears the interrupt flag for CCP2.
#define CCP2_INTERRUPT_FLAG_CLEAR()  (PIR2bits.CCP2IF = 0)

#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE
//This macro sets CCP2 interrupt as high priority.
#define CCP2_INT_HIGH_PRIORITY()      (IPR2bits.CCP2IP = 1)
//This macro sets CCP2 interrupt as low priority.
#define CCP2_INT_LOW_PRIORITY()       (IPR2bits.CCP2IP = 0)
#endif
#endif

#if I2C_INTERRUPT_ENABLE_FEATURE==INTERRUPT_FEATURE_ENABLE
//This macro enables the interrupt for MSSP I2C Mode.
#define I2C_INTERRUPT_ENABLE()              (PIE1bits.SSPIE = 1)
#define I2C_BUS_COL_INTERRUPT_ENABLE()      (PIE2bits.BCLIE = 1)
//This macro disables the interrupt for MSSP I2C Mode.
#define I2C_INTERRUPT_DISABLE()             (PIE1bits.SSPIE = 0)
#define I2C_BUS_COL_INTERRUPT_DISABLE()     (PIE2bits.BCLIE = 0)
//This macro clears the interrupt flag for MSSP I2C Mode.
#define I2C_INTERRUPT_FLAG_CLEAR()          (PIR1bits.SSPIF = 0)
#define I2C_BUS_COL_INTERRUPT_FLAG_CLEAR()  (PIR2bits.BCLIF = 0)
#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE
//This macro sets MSSP I2C Mode interrupt as high priority.
#define I2C_INT_HIGH_PRIORITY()             (IPR1bits.SSPIP = 1)
#define I2C_BUS_COL_INT_HIGH_PRIORITY()     (IPR2bits.BCLIP = 1)
//This macro sets MSSP I2C Mode interrupt as low priority.
#define I2C_INT_LOW_PRIORITY()              (IPR1bits.SSPIP = 0)
#define I2C_BUS_COL_INT_LOW_PRIORITY()      (IPR2bits.BCLIP = 0)
#endif
#endif

#if SPI_INTERRUPT_ENABLE_FEATURE==INTERRUPT_FEATURE_ENABLE
//This macro enables the interrupt for MSSP SPI Mode.
#define SPI_INTERRUPT_ENABLE()      (PIE1bits.SSPIE = 1)
//This macro disables the interrupt for MSSP SPI Mode.
#define SPI_INTERRUPT_DISABLE()     (PIE1bits.SSPIE  = 0)
//This macro clears the interrupt flag for MSSP SPI Mode.
#define SPI_INTERRUPT_FLAG_CLEAR()  (PIR1bits.SSPIF = 0)

#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE
//This macro sets MSSP SPI Mode interrupt as high priority.
#define SPI_INT_HIGH_PRIORITY()      (IPR1bits.SSPIP = 1)
//This macro sets MSSP SPI Mode interrupt as low priority.
#define SPI_INT_LOW_PRIORITY()       (IPR1bits.SSPIP = 0)
#endif
#endif

//This macro enables the interrupt for EUSART_TX module.
#define EUSART_TX_INTERRUPT_ENABLE()      (PIE1bits.TXIE = 1)
//This macro disables the interrupt for EUSART_TX module.
#define EUSART_TX_INTERRUPT_DISABLE()     (PIE1bits.TXIE  = 0)

//This macro enables the interrupt for EUSART_RX module.
#define EUSART_RX_INTERRUPT_ENABLE()      (PIE1bits.RCIE = 1)
//This macro disables the interrupt for EUSART_RX module.
#define EUSART_RX_INTERRUPT_DISABLE()     (PIE1bits.RCIE  = 0)

#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE
//This macro sets EUSART_TX interrupt as high priority.
#define EUSART_TX_INT_HIGH_PRIORITY()      (IPR1bits.TXIP = 1)
//This macro sets EUSART_TX interrupt as low priority.
#define EUSART_TX_INT_LOW_PRIORITY()       (IPR1bits.TXIP = 0)

//This macro sets EUSART_RX interrupt as high priority.
#define EUSART_RX_INT_HIGH_PRIORITY()      (IPR1bits.RCIP = 1)
//This macro sets EUSART_RX interrupt as low priority.
#define EUSART_RX_INT_LOW_PRIORITY()       (IPR1bits.RCIP = 0)
#endif

/* -------------- Macro Functions Declarations --------------*/

/* -------------- Data Types Declarations --------------  */

/* -------------- Functions Declarations --------------*/



#endif	/* INTERNAL_INTERRUPT_H */

