/* 
 * File:   usart.c
 * Author: Mohamed Sameh
 *
 * Created on October 5, 2023, 10:26 PM
 */

#include "usart.h"

void (*EUSART_TXInterruptHandler)(void) = NULL;
//Must read the RCREG to clear the flag in ISR.
void (*EUSART_RXInterruptHandler)(void) = NULL;
void (*EUSART_FramingErrorHandler)(void) = NULL;

static inline void Eusart_Baudrate_Calc(const usart_t *_usart);
static inline void Eusart_Async_Tx_Init(const usart_t *_usart);
static inline void Eusart_Async_Rx_Init(const usart_t *_usart);
static inline void Eusart_Async_Rx_Restart(void);

/**
 * @brief  Initializes the EUSART module for asynchronous communication.
 * 
 * This function configures and enables the EUSART module based on the provided
 * configuration structure. Sets the Baud Rate with the desired value, initializes the TX
 * and RX pins and enables the serial port for communication.
 * 
 * @param _usart A pointer to the EUSART Configurations Structure.
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The operation was successful.
 *         - E_NOT_OK: An error occurred during the operation.
 */
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

/**
 * @brief  De-Initializes the EUSART module for asynchronous communication.
 * 
 * 
 * @param _usart A pointer to the EUSART Configurations Structure.
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The operation was successful.
 *         - E_NOT_OK: An error occurred during the operation.
 */
Std_ReturnType Eusart_Async_DeInit(const usart_t *_usart)
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
        if(_usart->tx_cfg.usart_tx_interrupt_enable == EUSART_ASYNC_INTERRUPT_TX_ENABLE_CFG)
        {
            EUSART_TX_INTERRUPT_DISABLE();
        }
        else if(_usart->rx_cfg.usart_rx_interrupt_enable == EUSART_ASYNC_INTERRUPT_RX_ENABLE_CFG)
        {
            EUSART_RX_INTERRUPT_DISABLE();
        }
    }
    return ret;
}

/**
 * @brief Sends a char over EUSART communication in a blocking manner.
 * 
 * @param data Data to send.
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The operation was successful.
 *         - E_NOT_OK: An error occurred during the operation.
 */
Std_ReturnType Eusart_Async_SendByte_Blocking(uint8 data)
{
    Std_ReturnType ret = E_OK;
    while(TXSTAbits.TRMT == 0); // waits until shift register is empty
    EUSART_TX_INTERRUPT_ENABLE();
    TXREG = data;
    return ret;
}

/**
 * @brief Sends a null-terminated string over EUSART communication in a blocking manner.
 * 
 * @param str A string to send.
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The operation was successful.
 *         - E_NOT_OK: An error occurred during the operation.
 */
Std_ReturnType Eusart_Async_SendString_Blocking(uint8 *str)
{
    Std_ReturnType ret = E_OK;
    uint8 i = ZERO_INIT;

    if(NULL == str)
    {
        ret = E_NOT_OK;
    }
    else
    {
        while(str[i] != '\0')
        {
            Eusart_Async_SendByte_Blocking(str[i]);
            i++;
        }   
    }
    return ret;
}

/**
 * @brief Receives a char over EUSART communication in a blocking manner.
 * 
 * @param str A pointer to store the received data. 
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The operation was successful.
 *         - E_NOT_OK: An error occurred during the operation.
 */
Std_ReturnType Eusart_Async_Receive_Blocking(uint8 *data)
{
    Std_ReturnType ret = E_OK;

    if(NULL == data)
    {
        ret = E_NOT_OK;
    }
    else
    {
        //Checks if there is an error
        if(RCSTAbits.OERR == 1)
        {
            Eusart_Async_Rx_Restart();
        }
        while(!PIR1bits.RCIF);
        *data = RCREG;
    }
    return ret;
}

/**
 * @brief Receives a char over EUSART communication in a non-blocking manner.
 * 
 * @param str A pointer to store the received data. 
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The operation was successful.
 *         - E_NOT_OK: An error occurred during the operation.
 */
Std_ReturnType Eusart_Async_Receive_NonBlocking(uint8 *data)
{
    Std_ReturnType ret = E_OK;

    if(NULL == data)
    {
        ret = E_NOT_OK;
    }
    else
    {
        //Checks if there is an error
        if(RCSTAbits.OERR == 1)
        {
            Eusart_Async_Rx_Restart();
        }else{/* Nothing */}

        if(1 == PIR1bits.RCIF)
        {
            *data = RCREG;
        }
        else 
        {
            ret = E_NOT_OK;
        }
    }
    return ret;
}

/**
 * @brief Sends a char over EUSART communication in a non-blocking manner.
 * 
 * @param data Data to send.
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The operation was successful.
 *         - E_NOT_OK: An error occurred during the operation.
 */
Std_ReturnType Eusart_Async_SendByte_NonBlocking(uint8 data)
{
    Std_ReturnType ret = E_OK;

    if(PIR1bits.TXIF == 1)
    {
        EUSART_TX_INTERRUPT_ENABLE();
        TXREG = data;
    }
    else 
    {
        ret = E_NOT_OK;
    }
    return ret;
}

/**
 * @brief Calculates and configures the baud rate for EUSART communication.
 * 
  * This function calculates and configures the appropriate baud rate for EUSART communication
 * based on the provided configuration settings. It supports both asynchronous and synchronous modes,
 * as well as 8-bit and 16-bit baud rate generators, high and low-speed modes.
 * 
 * @param _usart A pointer to the EUSART configuration structure containing baud rate settings.
 */
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

/**
 * @brief Helper function to set up an Asynchronous Transmission.
 * 
 */
static inline void Eusart_Async_Tx_Init(const usart_t *_usart)
{
    //Enable TX
    TXSTAbits.TXEN = EUSART_TX_ENABLE_CFG;
    //Configure the Interrupt
    if(EUSART_ASYNC_INTERRUPT_TX_ENABLE_CFG == _usart->tx_cfg.usart_tx_interrupt_enable)
    {
        //Enable Tx interrupt
        EUSART_TX_INTERRUPT_ENABLE();
        EUSART_TXInterruptHandler = _usart->EUSART_TXInterruptHandler;

        //Interrupt priority configurations
#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE
        INTERRUPT_PriorityLevelsEnable();
         if(INTERRUPT_HIGH_PRIORITY == _usart->tx_cfg.priority)
        {
            INTERRUPT_GlobalInterruptHighEnable();
            EUSART_TX_INT_HIGH_PRIORITY();
        }
        else if(INTERRUPT_LOW_PRIORITY == _usart->tx_cfg.priority)
        {
            INTERRUPT_GlobalInterruptLowEnable();
            EUSART_TX_INT_LOW_PRIORITY();
        }else{/* Nothing */}
#else 
        INTERRUPT_GlobalInterruptEnable();
        INTERRUPT_PeripheralInterruptEnable();
#endif        
    }
    else if(EUSART_ASYNC_INTERRUPT_TX_DISABLE_CFG == _usart->tx_cfg.usart_tx_interrupt_enable)
    {
        EUSART_TX_INTERRUPT_DISABLE();
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

/**
 * @brief Helper function to set up an Asynchronous Reception.
 * 
 */
static inline void Eusart_Async_Rx_Init(const usart_t *_usart)
{
    //Enable RX
    RCSTAbits.CREN = EUSART_RX_ENABLE_CFG;
    //Configure the Interrupt
    if(EUSART_ASYNC_INTERRUPT_RX_ENABLE_CFG == _usart->rx_cfg.usart_rx_interrupt_enable)
    {
        //Enable Rx interrupt
        EUSART_RX_INTERRUPT_ENABLE();
        EUSART_RXInterruptHandler = _usart->EUSART_RXInterruptHandler;
        EUSART_FramingErrorHandler = _usart->EUSART_FramingErrorHandler;
        //Interrupt priority configurations
#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE
        INTERRUPT_PriorityLevelsEnable();
         if(INTERRUPT_HIGH_PRIORITY == _usart->tx_cfg.priority)
        {
            INTERRUPT_GlobalInterruptHighEnable();
            EUSART_RX_INT_HIGH_PRIORITY();
        }
        else if(INTERRUPT_LOW_PRIORITY == _usart->tx_cfg.priority)
        {
            INTERRUPT_GlobalInterruptLowEnable();
            EUSART_RX_INT_LOW_PRIORITY();
        }else{/* Nothing */}
#else 
        INTERRUPT_GlobalInterruptEnable();
        INTERRUPT_PeripheralInterruptEnable();
#endif
    }
    else if(EUSART_ASYNC_INTERRUPT_RX_DISABLE_CFG == _usart->rx_cfg.usart_rx_interrupt_enable)
    {
        EUSART_RX_INTERRUPT_DISABLE();
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

/**
 * @brief Helper function to restart the receiver when there is an error.
 * 
 */
static inline void Eusart_Async_Rx_Restart(void)
{ 
    RCSTAbits.CREN = 0;             //If error -> Reset
    RCSTAbits.CREN = 1;             //If error -> Reset 
}

/**
 * @brief The EUSART_TX interrupt MCAL helper function
 * 
 */
void EUSART_TX_ISR(void)
{
    EUSART_TX_INTERRUPT_DISABLE();
    //CallBack func gets called every time this ISR executes.
    if(EUSART_TXInterruptHandler)
    {
        EUSART_TXInterruptHandler();
    }else{/* Nothing */}
}

/**
 * @brief The EUSART_RX interrupt MCAL helper function
 * 
 */
void EUSART_RX_ISR(void)
{
    //CallBack func gets called every time this ISR executes.
    if(EUSART_RXInterruptHandler)
    {
        EUSART_RXInterruptHandler();
    }else{/* Nothing */}
    if(EUSART_FramingErrorHandler)
    {
        EUSART_FramingErrorHandler();
    }else{/* Nothing */}
}