/* 
 * File:   application.c
 * Author: Mohamed Sameh
 *
 * Created on August 22, 2023, 10:49 PM
 */

#include "application.h"

void TMR3_Defaullt_INTERRUPT(void);
void CCP1_Defaullt_INTERRUPT(void);

volatile uint8 CCP1_flag;

ccp_t ccp_obj = 
{
    .CCP1_InterruptHandler = CCP1_Defaullt_INTERRUPT,
    .CCPx = CCP1_INST,
    .mode = CCP_COMPARE_MD,
    .mode_variant = CCP_COMPARE_MODE_SET_PIN_HIGH,
    .ccp_timer = CCP1_CCP2_TIMER3,
    .pin.pin_num = GPIO_PIN2,
    .pin.port = PORTC_INDEX,
    .pin.direction = GPIO_DIRECTION_OUTPUT
}; 

timer3_t timer3 = {
    .TMR3_InterruptHandler = NULL,
    .timer3_mode = TIMER3_TIMER_MODE_CFG,
    .timer3_preload = 0,
    .prescaler_val = TIMER3_PRESCALER_DIV_1,
    .timer3_rw_mode = TIMER3_16BITS_RW_MODE_CFG
};

/*********************************/
//for real_time_clock
uint8 hours = 23, minutes = 59, seconds = 50;
uint8 counter = 0;
/*********************************/
Std_ReturnType ret = E_NOT_OK;

int main() {
    
    CCP_Compare_Set_Value(&ccp_obj, 37500);
    app_intialize();

    while(1)
    {

    }
    
    return 0;
}

void app_intialize(void)
{
    CCP_Init(&ccp_obj);
    Timer3_Init(&timer3);
}   

void TMR3_Defaullt_INTERRUPT(void)
{

}

void CCP1_Defaullt_INTERRUPT(void)
{
    Std_ReturnType ret = E_NOT_OK;
    CCP1_flag++;
    ret = Timer3_Write_Value(&timer3, 0);
    
    if(CCP1_flag == 1)
    {
        ret = CCP_Compare_Set_Value(&ccp_obj, 12500);
        CCP1_SET_MODE(CCP_COMPARE_MODE_SET_PIN_LOW);
    }
    else if(CCP1_flag == 2)
    {
        ret = CCP_Compare_Set_Value(&ccp_obj, 37500);
        CCP1_SET_MODE(CCP_COMPARE_MODE_SET_PIN_HIGH);
        CCP1_flag = 0;
    }
}

void real_time_clock()
{
    
    gpio_port_set_direction(PORTC_INDEX, 0xF0);
    gpio_port_set_direction(PORTD_INDEX, 0xC0);

    for(counter = 0; counter < 50; counter++)
        {
            ret = gpio_port_write(PORTD_INDEX, 0x3E);
            ret = gpio_port_write(PORTC_INDEX, (uint8)(hours/10));
            __delay_us(3333);
            ret = gpio_port_write(PORTD_INDEX, 0x3D);
            ret = gpio_port_write(PORTC_INDEX, (uint8)(hours%10));
            __delay_us(3333);
            ret = gpio_port_write(PORTD_INDEX, 0x3B);
            ret = gpio_port_write(PORTC_INDEX, (uint8)(minutes/10));
            __delay_us(3333);
            ret = gpio_port_write(PORTD_INDEX, 0x37);
            ret = gpio_port_write(PORTC_INDEX, (uint8)(minutes%10));
            __delay_us(3333);
            ret = gpio_port_write(PORTD_INDEX, 0x2F);
            ret = gpio_port_write(PORTC_INDEX, (uint8)(seconds/10));
            __delay_us(3333);
            ret = gpio_port_write(PORTD_INDEX, 0x1F);
            ret = gpio_port_write(PORTC_INDEX, (uint8)(seconds%10));
            __delay_us(3333);
        }
        seconds++;
        if(seconds == 60)
        {
            seconds = 0;
            minutes++;
        }

        if(minutes == 60)
        {
            minutes = 0;
            hours++;
        }

        if(hours == 24)
        {
            hours = 0;
        }

}