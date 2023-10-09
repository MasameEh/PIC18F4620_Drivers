/* 
 * File:   application.h
 * Author: Mohamed Sameh
 *
 * Created on August 22, 2023, 11:33 PM
 */

#ifndef APPLICATION_H
#define	APPLICATION_H

/* Section : Includes */
#include "HAL/LED/led.h"
#include "HAL/Button/button.h"
#include "HAL/Relay/relay.h"
#include "HAL/Dc_Motor/dc_motor.h"
#include "HAL/7_Seg/seven_seg.h"
#include "HAL/Keypad/keypad.h"
#include "HAL/Chr_LCD/chr_lcd.h"
#include "MCAL/interrupt/external_interrupt.h"
#include "MCAL/EEPROM/eeprom.h"
#include "MCAL/ADC/adc.h"
#include "MCAL/TIMER0/timer0.h"
#include "MCAL/TIMER1/timer1.h"
#include "MCAL/TIMER2/timer2.h"
#include "MCAL/TIMER3/timer3.h"
#include "MCAL/CCP/ccp.h"
#include "MCAL/USART/usart.h"
#include "MCAL/SPI/spi.h"
/* Section : Macro Declarations */


/* Section : Macro Functions Declarations */


/* Section : Data Types Declarations  */


/* Section : Functions Declarations */
void app_intialize(void);
void real_time_clock(void);

#endif	/* APPLICATION_H */

