/* 
 * File:   application.c
 * Author: Mohamed Sameh
 *
 * Created on August 22, 2023, 10:49 PM
 */

#include "application.h"


volatile uint8 validation = 0 , validation1;
volatile uint8 read;

led_t led = {.led_status = LED_OFF, .pin = GPIO_PIN0, .port = PORTC_INDEX};
pin_config_t ss1_pin = { .pin_num = GPIO_PIN0, .port = PORTD_INDEX, .logic = GPIO_HIGH, .direction = GPIO_DIRECTION_OUTPUT};
void SPI__InterruptHandler()
{

}

spi_t spi_master = {
    .mode = SPI_MASTER_FOSC_DIV_16,
    .master_sample = SPI_MASTER_SAMPLE_MIDDLE_CFG,
    .master_waveform = SPI_CLK_IDLE_LOW_TX_LEADING_RISING,
    .SPI_InterruptHandler = SPI__InterruptHandler
};



Std_ReturnType ret = E_NOT_OK;


int main() {
    
    
    app_intialize();
    ret = SPI_Master_Init(&spi_master);
    ret = gpio_pin_initialize(&ss1_pin);

    while(1)
    {
        // ret = gpio_pin_write(&ss1_pin, GPIO_LOW);
        ret = SPI_Master_Transceiver('A', &ss1_pin, &read);
        if(read == 'A')
        {
            led_toggle(&led);
        }
        // ret = gpio_pin_write(&ss1_pin, GPIO_HIGH);
        __delay_ms(1000);
    }
    return 0;
}

void app_intialize(void)
{
    led_init(&led);
}   


