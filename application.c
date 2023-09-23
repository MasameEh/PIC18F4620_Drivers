/* 
 * File:   application.c
 * Author: Mohamed Sameh
 *
 * Created on August 22, 2023, 10:49 PM
 */

#include "application.h"

led_t led = {.led_status = LED_OFF, .pin = GPIO_PIN0, .port = PORTC_INDEX};
/*********************************/
//for real_time_clock
uint8 hours = 23, minutes = 59, seconds = 50;
uint8 counter = 0;
/*********************************/
Std_ReturnType ret = E_NOT_OK;

uint8 eeprom_val = 0, eeprom_read;

int main() {
    
    app_intialize();
    ret = EEPROM_WriteByte(0x3FF, 0);
    while(1)
    {
        ret = EEPROM_WriteByte(0x3FF, eeprom_val++); 
        __delay_ms(1000);
        ret = EEPROM_ReadByte(0x3FF, &eeprom_read);
        if(5 == eeprom_read)
        {
            led_turn_on(&led);
        }else led_turn_off(&led); 
    }
    
    return 0;
}

void app_intialize(void)
{
    led_init(&led);
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