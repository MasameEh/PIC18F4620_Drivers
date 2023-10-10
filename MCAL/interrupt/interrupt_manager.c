/* 
 * File:   interrupt_manager.c
 * Author: Mohamed Sameh
 *
 * Created on September 18, 2023, 3:32 PM
 */

#include "interrupt_manager.h"

static volatile uint8 RB4_Flag = 1;
static volatile uint8 RB5_Flag = 1;
static volatile uint8 RB6_Flag = 1;
static volatile uint8 RB7_Flag = 1;

#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE
void __interrupt() InterruptManagerHigh(void)
{
    if(INTERRUPT_ENABLE == INTCONbits.INT0IE && INTERRUPT_OCCURRED == INTCONbits.INT0IF)
    {
        INT0_ISR();
    }
    if(INTERRUPT_ENABLE == INTCON3bits.INT2IE && INTERRUPT_OCCURRED == INTCON3bits.INT2IF)
    {
        INT2_ISR();
    }
}

void __interrupt(low_priority) InterruptManagerLow(void)
{
    if(INTERRUPT_ENABLE == INTCON3bits.INT1IE && INTERRUPT_OCCURRED == INTCON3bits.INT1IF)
    {
        INT1_ISR();
    }
}

#else
void __interrupt() InterruptManager(void)
{
    /*_________________________ INTX START _________________________________*/
    if(INTERRUPT_ENABLE == INTCONbits.INT0IE && INTERRUPT_OCCURRED == INTCONbits.INT0IF)
    {
        INT0_ISR(); /*EXTERNAL INTERRUPT INT0*/
    }
    if(INTERRUPT_ENABLE == INTCON3bits.INT1IE && INTERRUPT_OCCURRED == INTCON3bits.INT1IF)
    {
        INT1_ISR(); /*EXTERNAL INTERRUPT INT1*/
    }
    if(INTERRUPT_ENABLE == INTCON3bits.INT2IE && INTERRUPT_OCCURRED == INTCON3bits.INT2IF)
    {
        INT2_ISR(); /*EXTERNAL INTERRUPT INT2*/
    }
    /*_________________________ INTX END _________________________________*/

    /*_________________________ PORTB external on change interrupt start _________________________________*/
    /*_________________________ RB4 START _________________________________*/
    if(INTERRUPT_ENABLE == INTCONbits.RBIE && INTERRUPT_OCCURRED == INTCONbits.RBIF 
    && PORTBbits.RB4 == GPIO_HIGH && RB4_Flag == 1)
    {
        RB4_Flag = 0; 
        RB4_ISR(1);
    }
    if(INTERRUPT_ENABLE == INTCONbits.RBIE && INTERRUPT_OCCURRED == INTCONbits.RBIF 
    && PORTBbits.RB4 == GPIO_LOW && RB4_Flag == 0)
    {
        RB4_Flag = 1;       
        RB4_ISR(0);
    }
    /*_________________________ RB4 END _________________________________*/
    /*_________________________ RB5 START _________________________________*/
    if(INTERRUPT_ENABLE == INTCONbits.RBIE && INTERRUPT_OCCURRED == INTCONbits.RBIF 
    && PORTBbits.RB5 == GPIO_HIGH && RB5_Flag == 1)
    {
        RB5_Flag = 0;
        RB5_ISR(1);
    }
    if(INTERRUPT_ENABLE == INTCONbits.RBIE && INTERRUPT_OCCURRED == INTCONbits.RBIF 
    && PORTBbits.RB5 == GPIO_LOW && RB5_Flag == 0)
    {
        RB5_Flag = 1;
        RB5_ISR(0);
    }
    /*_________________________ RB5 END _________________________________*/
    /*_________________________ RB6 START _________________________________*/
    if(INTERRUPT_ENABLE == INTCONbits.RBIE && INTERRUPT_OCCURRED == INTCONbits.RBIF 
    && PORTBbits.RB6 == GPIO_HIGH && RB6_Flag == 1)
    {
        RB6_Flag = 0;
        RB6_ISR(1);
    }
    if(INTERRUPT_ENABLE == INTCONbits.RBIE && INTERRUPT_OCCURRED == INTCONbits.RBIF 
    && PORTBbits.RB6 == GPIO_LOW && RB6_Flag == 0)
    {
        RB6_Flag = 1;
        RB6_ISR(0);
    }
    /*_________________________ RB6 END _________________________________*/
    /*_________________________ RB7 START _________________________________*/
    if(INTERRUPT_ENABLE == INTCONbits.RBIE && INTERRUPT_OCCURRED == INTCONbits.RBIF 
    && PORTBbits.RB7 == GPIO_HIGH && RB7_Flag == 1)
    {
        RB7_Flag = 0;
        RB7_ISR(1);
    }
    if(INTERRUPT_ENABLE == INTCONbits.RBIE && INTERRUPT_OCCURRED == INTCONbits.RBIF 
    && PORTBbits.RB7 == GPIO_LOW && RB7_Flag == 0)
    {
        RB7_Flag = 1;
        RB7_ISR(0);
    }
    /*_________________________ RB6 END _________________________________*/
    /*_________________________ PORTB external on change interrupt end _________________________________*/

    if(INTERRUPT_ENABLE == PIE1bits.ADIE && INTERRUPT_OCCURRED == PIR1bits.ADIF)
    {
        ADC_ISR(); /* ADC INTERRUPT */
    }

    /*_________________________ TIMER START _________________________________*/
    if(INTERRUPT_ENABLE == INTCONbits.TMR0IE && INTERRUPT_OCCURRED == INTCONbits.TMR0IF)
    {
        TMR0_ISR(); /* TIMER0 INTERRUPT */
    }

    if(INTERRUPT_ENABLE == PIE1bits.TMR1IE && INTERRUPT_OCCURRED == PIR1bits.TMR1IF)
    {
        TMR1_ISR(); /* TIMER1 INTERRUPT */
    }

    if(INTERRUPT_ENABLE == PIE1bits.TMR2IE && INTERRUPT_OCCURRED == PIR1bits.TMR2IF)
    {
        TMR2_ISR(); /* TIMER2 INTERRUPT */
    }

    if(INTERRUPT_ENABLE == PIE2bits.TMR3IE && INTERRUPT_OCCURRED == PIR2bits.TMR3IF)
    {
        TMR3_ISR(); /* TIMER3 INTERRUPT */
    }
    /*_________________________ TIMER END _________________________________*/
    /*_________________________ CCP START _________________________________*/
    if(INTERRUPT_ENABLE == PIE1bits.CCP1IE && INTERRUPT_OCCURRED == PIR1bits.CCP1IF)
    {
        CCP1_ISR(); /* CCP1 INTERRUPT */
    }

    if(INTERRUPT_ENABLE == PIE2bits.CCP2IE && INTERRUPT_OCCURRED == PIR2bits.CCP2IF)
    {
        CCP2_ISR(); /* CCP2 INTERRUPT */
    }
    /*_________________________ CCP END ______________________________________*/
    /*_________________________ EUSART START _________________________________*/
    if(INTERRUPT_ENABLE == PIE1bits.TXIE && INTERRUPT_OCCURRED == PIR1bits.TXIF)
    {
        EUSART_TX_ISR(); /* EUSART TX INTERRUPT */
    }

    if(INTERRUPT_ENABLE == PIE1bits.RCIE && INTERRUPT_OCCURRED == PIR1bits.RCIF)
    {
        EUSART_RX_ISR(); /* EUSART RX INTERRUPT */
    }
    /*_________________________ EUSART END _________________________________*/
    /*_________________________ SPI START _________________________________*/
    if(INTERRUPT_ENABLE == PIE1bits.SSPIE && INTERRUPT_OCCURRED == PIR1bits.SSPIF)
    {
        SPI_ISR(); /* SPI INTERRUPT */
    }
    /*_________________________ SPI END _________________________________*/


}

#endif