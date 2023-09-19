/* 
 * File:   interrupt_manager.h
 * Author: Mohamed Sameh
 *
 * Created on September 18, 2023, 3:32 PM
 */

#ifndef INTERRUPT_MANAGER_H
#define	INTERRUPT_MANAGER_H

/* -------------- Includes -------------- */
#include "interrupt_config.h"

/* -------------- Macro Declarations ------------- */

/* -------------- Macro Functions Declarations --------------*/

/* -------------- Data Types Declarations --------------  */

/* -------------- Functions Declarations --------------*/
void INT0_ISR(void);
void INT1_ISR(void);
void INT2_ISR(void);

#endif	/* INTERRUPT_MANAGER_H */

