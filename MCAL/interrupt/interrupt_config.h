/* 
 * File:   interrupt_config.h
 * Author: Mohamed Sameh
 *
 * Created on September 18, 2023, 3:31 PM
 */

#ifndef INTERRUPT_CONFIG_H
#define	INTERRUPT_CONFIG_H

/* -------------- Includes -------------- */
#include "interrupt_gen_config.h"
#include "../GPIO/gpio.h"
/* -------------- Macro Declarations ------------- */
#define INTERRUPT_ENABLE                    1
#define INTERRUPT_DISABLE                   0
#define INTERRUPT_OCCURRED                  1
#define INTERRUPT_UNOCCURRED                0
   
#define INTERRUPT_PRIORITY_ENABLE           1
#define INTERRUPT_PRIORITY_DISABLE          0

/* -------------- Macro Functions Declarations --------------*/
#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE
//This macro will enable the priority levels on interrupts.
#define INTERRUPT_PriorityLevelsEnable()          (RCONbits.IPEN = 1)
//This macro will disable the priority levels on interrupts.
#define INTERRUPT_PriorityLevelsDisable()         (RCONbits.IPEN = 0)

//This macro enables high-priority global interrupts.
#define INTERRUPT_GlobalInterruptHighEnable()     (INTCONbits.GIEH = 1)
//This macro disables high-priority global interrupts.
#define INTERRUPT_GlobalInterruptHighDisable()    (INTCONbits.GIEH = 0)

//This macro will enable low-priority global interrupts.
#define INTERRUPT_GlobalInterruptLowEnable()      (INTCONbits.GIEL = 1)
//This macro will disable low-priority global interrupts.
#define INTERRUPT_GlobalInterruptLowDisable()     (INTCONbits.GIEL = 0)

#else
//This macro will enable global interrupts.
#define INTERRUPT_GlobalInterruptEnable()         (INTCONbits.GIE = 1)
//This macro will disable global interrupts.
#define INTERRUPT_GlobalInterruptDisable()        (INTCONbits.GIE = 0)

//This macro will enable peripheral interrupts.
#define INTERRUPT_PeripheralInterruptEnable()     (INTCONbits.PEIE = 1)
//This macro will disable peripheral interrupts.
#define INTERRUPT_PeripheralInterruptDisable()    (INTCONbits.PEIE = 0)
#endif
     
/* -------------- Data Types Declarations --------------  */
typedef enum 
{
    INTERRUPT_LOW_PRIORITY = 0,
    INTERRUPT_HIGH_PRIORITY,
}interrupt_priority;
/* -------------- Functions Declarations --------------*/


#endif	/* INTERRUPT_CONFIG_H */

