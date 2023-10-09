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

void EUSART_RX_InterruptHandler()
{
   validation1++;
   Eusart_Async_Receive_NonBlocking(&read);
   switch (read)
   {
   case 'a': led_turn_on(&led); 
             Eusart_Async_SendString_Blocking("Led is on\r"); break;
   case 'b': led_turn_off(&led); 
             Eusart_Async_SendString_Blocking("Led is off\r"); break;
   default:
        led_turn_off(&led);
       break;
   }
}

void EUSART_TX_InterruptHandler()
{
    validation++;
}
usart_t o = 
{
    .baudrate = 9600,
    .baudrate_generator = EUSART_ASYNC_8BITS_LOW_SPEED_BAUDRATE,

    .tx_cfg.usart_tx_9bits_enable = EUSART_8BITS_TX_CFG,
    .tx_cfg.usart_tx_interrupt_enable = EUSART_ASYNC_INTERRUPT_TX_ENABLE_CFG,

    .rx_cfg.usart_rx_9bits_enable = EUSART_8BITS_RX_CFG,
    .rx_cfg.usart_rx_interrupt_enable = EUSART_ASYNC_INTERRUPT_RX_DISABLE_CFG,
    .EUSART_RXInterruptHandler = EUSART_RX_InterruptHandler,
    .EUSART_TXInterruptHandler = EUSART_TX_InterruptHandler
};



Std_ReturnType ret = E_NOT_OK;


int main() {
    
    
    app_intialize();


    while(1)
    {
        Eusart_Async_SendByte_NonBlocking('a');
        __delay_ms(500);
    }
    
    return 0;
}

void app_intialize(void)
{
    Eusart_Async_Init(&o);
    led_init(&led);
}   


