/* 
 * File:   interrupt_manager.c
 * Author: Mohamed Sameh
 *
 * Created on September 18, 2023, 3:32 PM
 */

#include "interrupt_manager.h"

#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE
void __interrupt() InterruptManagerHigh(void)
{
    if(INTERRUPT_ENABLE == INTCONbits.INT0IE && INTERRUPT_ENABLE == INTCONbits.INT0IF)
    {
        INT0_ISR();
    }
    if(INTERRUPT_ENABLE == INTCON3bits.INT2IE && INTERRUPT_ENABLE == INTCON3bits.INT2IF)
    {
        INT2_ISR();
    }
}

void __interrupt(low_priority) InterruptManagerLow(void)
{
    if(INTERRUPT_ENABLE == INTCON3bits.INT1IE && INTERRUPT_ENABLE == INTCON3bits.INT1IF)
    {
        INT1_ISR();
    }
}

#else
void __interrupt() InterruptManager(void)
{
    if(INTERRUPT_ENABLE == INTCONbits.INT0IE && INTERRUPT_ENABLE == INTCONbits.INT0IF)
    {
        INT0_ISR();
    }
    if(INTERRUPT_ENABLE == INTCON3bits.INT1IE && INTERRUPT_ENABLE == INTCON3bits.INT1IF)
    {
        INT1_ISR();
    }
    if(INTERRUPT_ENABLE == INTCON3bits.INT2IE && INTERRUPT_ENABLE == INTCON3bits.INT2IF)
    {
        INT2_ISR();
    }
}

#endif