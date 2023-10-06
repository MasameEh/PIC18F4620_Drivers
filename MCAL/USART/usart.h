/* 
 * File:   usart.h
 * Author: DeSkToP
 *
 * Created on October 5, 2023, 10:26 PM
 */

#ifndef USART_H
#define	USART_H

/* -------------- Includes -------------- */
#include "usart_cfg.h"
#include "../GPIO/gpio.h"
#include "../interrupt/internal_interrupt.h"

/* -------------- Macro Declarations ------------- */
//EUSART Mode Select
#define EUSART_SYNC_MODE_CFG    1
#define EUSART_ASYNC_MODE_CFG   0
//EUSART Asynchronous Baud Rate Select
#define EUSART_ASYNC_HIGH_SPEED_CFG    1
#define EUSART_ASYNC_LOW_SPEED_CFG     0
//Baud Rate Generator Register Size
#define EUSART_16BITS_BAUD_RATE_CFG    1
#define EUSART_8BITS_BAUD_RATE_CFG     0

//EUSART Transmit Enable or Disable
#define EUSART_TX_ENABLE_CFG    1
#define EUSART_TX_DISABLE_CFG   0
//EUSART Transmit Interrupt Enable or Disable
#define EUSART_ASYNC_INTERRUPT_TX_ENABLE_CFG    1
#define EUSART_ASYNC_INTERRUPT_TX_DISABLE_CFG   0
//EUSART 9-bit OR 8-bit Transmit Select
#define EUSART_9BITS_TX_CFG    1
#define EUSART_8BITS_TX_CFG    0

//EUSART Receive Enable or Disable
#define EUSART_RX_ENABLE_CFG    1
#define EUSART_RX_DISABLE_CFG   0
//EUSART Receive Interrupt Enable or Disable
#define EUSART_ASYNC_INTERRUPT_RX_ENABLE_CFG    1
#define EUSART_ASYNC_INTERRUPT_RX_DISABLE_CFG   0
//EUSART 9-bit OR 8-bit Receiver Select
#define EUSART_9BITS_RX_CFG    1
#define EUSART_8BITS_RX_CFG    0

//EUSART Framing Error 
#define EUSART_FRAMING_ER_DETECTED_CFG    1
#define EUSART_FRAMING_ER_CLEAR_CFG       0
//EUSART OVERRUN Error 
#define EUSART_OVERRUN_ER_DETECTED_CFG    1
#define EUSART_OVERRUN_ER_CLEAR_CFG       0

/* -------------- Macro Functions Declarations -------------- */
#define EUSART_TX_INTERRUPT_FLAG_CLEAR()   (PIR1bits.TXIF = 0)
#define EUSART_RX_INTERRUPT_FLAG_CLEAR()   (PIR1bits.RCIF = 0)

#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE
//This macro sets EUSART_TX interrupt as high priority.
#define EUSART_TX_INT_HIGH_PRIORITY()      (IPR1bits.TXIP = 1)
//This macro sets EUSART_TX interrupt as low priority.
#define EUSART_TX_INT_LOW_PRIORITY()       (IPR1bits.TXIP = 0)
#endif
/* -------------- Data Types Declarations ---------------------- */

/**
 * @brief Baud Rate Selection
 */
typedef enum
{
    EUSART_ASYNC_8BITS_LOW_SPEED_BAUDRATE = 0,
    EUSART_ASYNC_8BITS_HIGH_SPEED_BAUDRATE,
    EUSART_ASYNC_16BITS_LOW_SPEED_BAUDRATE,
    EUSART_ASYNC_16BITS_HIGH_SPEED_BAUDRATE,
    EUSART_SYNC_8BITS_BAUDRATE,
    EUSART_SYNC_16BITS_BAUDRATE,
}usart_baudrate_gen_t;

/**
 * @brief Transmit Configurations Structure
 */
typedef struct
{
    uint8 usart_tx_interrupt_enable : 1;
#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE
    interrupt_priority priority;    
#endif
    uint8 usart_tx_9bits_enable : 1;
    uint8 usart_tx_reserved : 6;
}usart_tx_cfg_t;

/**
 * @brief Receive Configurations Structure
 */
typedef struct
{
    uint8 usart_rx_interrupt_enable : 1;
#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE
    interrupt_priority priority;    
#endif
    uint8 usart_rx_9bits_enable : 1;
    uint8 usart_rx_reserved : 6;
}usart_rx_cfg_t;

/**
 * @brief Errors Status
 */
typedef union 
{
    struct 
    {
        uint8 usart_ferr : 1;
        uint8 usart_oerr : 1;
        uint8 usart_err_reserved : 6;
    };
    uint8 err_status;
}usart_error_status_cfg_t;

typedef struct
{
    uint32_t baudrate;                          // Desired Baud Rate
    usart_baudrate_gen_t baudrate_generator;          // @ref usart_baudrate_gen_t
    usart_tx_cfg_t tx_cfg;                      // @ref usart_tx_cfg_t
    usart_rx_cfg_t rx_cfg;                      // @ref usart_rx_cfg_t
    usart_error_status_cfg_t error_status;      // @ref usart_error_status_cfg_t
    void (* EUSART_TXInterruptHandler)(void);
    void (* EUSART_RXInterruptHandler)(void);
    void (* EUSART_FramingErrorHandler)(void);
    void (* EUSART_OverrunErrorHandler)(void);
}usart_t;

/* -------------- Software Interfaces Declarations -------------- */
Std_ReturnType Eusart_Async_Init(const usart_t *_usart);
Std_ReturnType Eusart_Async_DeInit(const usart_t *_usart);
Std_ReturnType Eusart_Async_Send_Blocking(uint8 data);
Std_ReturnType Eusart_Async_Receive_Blocking(uint8 *data);

#endif	/* USART_H */

