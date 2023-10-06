/* 
 * File:   usart.c
 * Author: DeSkToP
 *
 * Created on October 5, 2023, 10:26 PM
 */

#include "usart.h"

void (* EUSART_TXInterruptHandler)(void);
void (* EUSART_RXInterruptHandler)(void);
void (* EUSART_FramingErrorHandler)(void);
void (* EUSART_OverrunErrorHandler)(void);

static inline void Eusart_Baudrate_Calc(const usart_t *_usart);
static inline void Eusart_Async_Tx_Init(const usart_t *_usart);
static inline void Eusart_Async_Rx_Init(const usart_t *_usart);

Std_ReturnType Eusart_Async_Init(const usart_t *_usart)
{
    Std_ReturnType ret = E_OK;

    if (NULL == _usart)
    {
        ret = E_NOT_OK;
    }
    else
    {
        //Disable the Serial Port
        RCSTAbits.SPEN = 0;
        //Init TX, RX pins as input
        TRISCbits.RC6 = 1; 
        TRISCbits.RC7 = 1; 
        //Initialize the SPBRGH:SPBRG registers for the appropriate baud rate
        Eusart_Baudrate_Calc(_usart);
        //Inittialize TX, RX
        Eusart_Async_Tx_Init(_usart);
        Eusart_Async_Rx_Init(_usart);
        //Enable the Serial Port
        RCSTAbits.SPEN = 1;
    }
    return ret;
}

Std_ReturnType Eusart_Async_DeInit(const usart_t *_usart)
{
        Std_ReturnType ret = E_OK;

    if (NULL == _usart)
    {
        ret = E_NOT_OK;
    }
    else
    {

    }
    return ret;
}


Std_ReturnType Eusart_Async_Send_Blocking( uint8 data)
{
    Std_ReturnType ret = E_OK;

    while(TXSTAbits.TRMT == 0);
    TXREG = data;
    return ret;
}


Std_ReturnType Eusart_Async_Receive_Blocking( uint8 *data)
{
    Std_ReturnType ret = E_OK;

    if (NULL == data)
    {
        ret = E_NOT_OK;
    }
    else
    {

    }
    return ret;
}


static inline void Eusart_Baudrate_Calc(const usart_t *_usart)
{
    float32 baudrate_reg_bits = ZERO_INIT;

    if(EUSART_ASYNC_8BITS_HIGH_SPEED_BAUDRATE == _usart->baudrate_generator)
    {
        //Asynchronous mode
        TXSTAbits.SYNC = EUSART_ASYNC_MODE_CFG;
        //8-bit Baud Rate Generator    
        BAUDCONbits.BRG16 = EUSART_8BITS_BAUD_RATE_CFG;
        //High Speed
        TXSTAbits.BRGH = EUSART_ASYNC_HIGH_SPEED_CFG; 

        baudrate_reg_bits = (_XTAL_FREQ/(float32)(16*_usart->baudrate)) - 1;
    }
    else if (EUSART_ASYNC_8BITS_LOW_SPEED_BAUDRATE == _usart->baudrate_generator)
    {
        //Asynchronous mode
        TXSTAbits.SYNC = EUSART_ASYNC_MODE_CFG;
        //8-bit Baud Rate Generator 
        BAUDCONbits.BRG16 = EUSART_8BITS_BAUD_RATE_CFG;
        //Low Speed
        TXSTAbits.BRGH = EUSART_ASYNC_LOW_SPEED_CFG;

        baudrate_reg_bits = (_XTAL_FREQ/(float32)(64*_usart->baudrate)) - 1;
    }
    else if (EUSART_ASYNC_16BITS_HIGH_SPEED_BAUDRATE == _usart->baudrate_generator)
    {
        //Asynchronous mode
        TXSTAbits.SYNC = EUSART_ASYNC_MODE_CFG;
        //16-bit Baud Rate Generator 
        BAUDCONbits.BRG16 = EUSART_16BITS_BAUD_RATE_CFG;
        //High Speed
        TXSTAbits.BRGH = EUSART_ASYNC_HIGH_SPEED_CFG;

        baudrate_reg_bits = (_XTAL_FREQ/(float32)(4*_usart->baudrate)) - 1;
    } 
    else if (EUSART_ASYNC_16BITS_LOW_SPEED_BAUDRATE == _usart->baudrate_generator)
    {
        //Asynchronous mode
        TXSTAbits.SYNC = EUSART_ASYNC_MODE_CFG;
        //16-bit Baud Rate Generator 
        BAUDCONbits.BRG16 = EUSART_16BITS_BAUD_RATE_CFG;
        //Low Speed
        TXSTAbits.BRGH = EUSART_ASYNC_LOW_SPEED_CFG;

        baudrate_reg_bits = (_XTAL_FREQ/(float32)(16*_usart->baudrate)) - 1;
    }    
    else if (EUSART_SYNC_8BITS_BAUDRATE == _usart->baudrate_generator)
    {
        //Synchronous mode
        TXSTAbits.SYNC = EUSART_SYNC_MODE_CFG;
        //8-bit Baud Rate Generator 
        BAUDCONbits.BRG16 = EUSART_8BITS_BAUD_RATE_CFG;
        baudrate_reg_bits = (_XTAL_FREQ/(float32)(4*_usart->baudrate)) - 1;
    }
    else if (EUSART_SYNC_16BITS_BAUDRATE == _usart->baudrate_generator)
    {
        //Synchronous mode
        TXSTAbits.SYNC = EUSART_SYNC_MODE_CFG;
        //16-bit Baud Rate Generator 
        BAUDCONbits.BRG16 = EUSART_16BITS_BAUD_RATE_CFG;
        baudrate_reg_bits = (_XTAL_FREQ/(float32)(4*_usart->baudrate)) - 1;
    }
    SPBRG = (uint8)(baudrate_reg_bits);
    SPBRGH = (uint8)((uint32)baudrate_reg_bits >> 8);
}

static inline void Eusart_Async_Tx_Init(const usart_t *_usart)
{
    //Enable TX
    TXSTAbits.TXEN = EUSART_TX_ENABLE_CFG;
    //Configure the Interrupt
    if(EUSART_ASYNC_INTERRUPT_TX_ENABLE_CFG == _usart->tx_cfg.usart_tx_interrupt_enable)
    {
        PIE1bits.TXIE = EUSART_ASYNC_INTERRUPT_TX_ENABLE_CFG;
    }
    else if(EUSART_ASYNC_INTERRUPT_TX_DISABLE_CFG == _usart->tx_cfg.usart_tx_interrupt_enable)
    {
        PIE1bits.TXIE = EUSART_ASYNC_INTERRUPT_TX_DISABLE_CFG;
    }
    else{/* Nothing */}
    //Configure transmitted data size
    if(EUSART_9BITS_TX_CFG == _usart->tx_cfg.usart_tx_9bits_enable)
    {
        TXSTAbits.TX9 = EUSART_9BITS_TX_CFG;
    }
    else if(EUSART_8BITS_TX_CFG == _usart->tx_cfg.usart_tx_9bits_enable)
    {
        TXSTAbits.TX9 = EUSART_8BITS_TX_CFG;
    }
    else{/* Nothing */}
}

static inline void Eusart_Async_Rx_Init(const usart_t *_usart)
{
    //Enable RX
    RCSTAbits.CREN = EUSART_RX_ENABLE_CFG;
    //Configure the Interrupt
    if(EUSART_ASYNC_INTERRUPT_RX_ENABLE_CFG == _usart->rx_cfg.usart_rx_interrupt_enable)
    {
        PIE1bits.RCIE = EUSART_ASYNC_INTERRUPT_RX_ENABLE_CFG;
    }
    else if(EUSART_ASYNC_INTERRUPT_RX_DISABLE_CFG == _usart->rx_cfg.usart_rx_interrupt_enable)
    {
        PIE1bits.RCIE = EUSART_ASYNC_INTERRUPT_RX_DISABLE_CFG;
    }
    else{/* Nothing */}
    //Configure the received data size
    if(EUSART_9BITS_RX_CFG == _usart->rx_cfg.usart_rx_9bits_enable)
    {
        RCSTAbits.RX9 = EUSART_9BITS_RX_CFG;
    }
    else if(EUSART_8BITS_TX_CFG == _usart->rx_cfg.usart_rx_9bits_enable)
    {
        RCSTAbits.RX9 = EUSART_8BITS_RX_CFG;
    }
    else{/* Nothing */}
}

void EUSART_TX_ISR(void)
{
    //Eusart transmit interrupt occurred, the flag must be cleared.
    EUSART_TX_INTERRUPT_FLAG_CLEAR();
    //CallBack func gets called every time this ISR executes.
    if(EUSART_TXInterruptHandler)
    {
        EUSART_TXInterruptHandler();
    }else{/* Nothing */}
}

void EUSART_RX_ISR(void)
{
    //Eusart transmit interrupt occurred, the flag must be cleared.
    EUSART_RX_INTERRUPT_FLAG_CLEAR();
    //CallBack func gets called every time this ISR executes.
    if(EUSART_RXInterruptHandler)
    {
        EUSART_RXInterruptHandler();
    }else{/* Nothing */}
}