/* 
 * File:   spi.h
 * Author: Mohamed Sameh
 *
 * Created on October 8, 2023, 2:45 AM
 */

#ifndef SPI_H
#define	SPI_H

/* -------------- Includes -------------- */
#include "spi_cfg.h"
#include "../GPIO/gpio.h"
#include "../interrupt/internal_interrupt.h"

/* -------------- Macro Declarations ------------- */
//SPI Clock Polarity Configuration.
#define SPI_CLK_POLARITY_HIGH_CFG    1
#define SPI_CLK_POLARITY_LOW_CFG     0

//SPI Transmit occurs on which edge.
#define SPI_TRANSMIT_RISING_EDGE_CFG      1
#define SPI_TRANSMIT_FALLING_EDGE_CFG     0

//SPI Master Sample Configuration.
#define SPI_MASTER_SAMPLE_END_CFG        1
#define SPI_MASTER_SAMPLE_MIDDLE_CFG     0

#define SPI_WRITE_COLLISION_OCCURRED        1  
#define SPI_WRITE_COLLISION_UNOCCURRED      0  
/* -------------- Macro Functions Declarations -------------- */
//SPI Enable or Disable.
#define SPI_ENABLE()     (SSPCON1bits.SSPEN = 1)
#define SPI_DISABLE()    (SSPCON1bits.SSPEN = 0)
//SPI receive completed or not yet.
#define SPI_RECEIVE_STATUS()  (SSPSTATbits.BF)
//SPI Sample bit
#define SPI_MASTER_SAMPLE_END()       (SSPSTATbits.SMP = 1)
#define SPI_MASTER_SAMPLE_MIDDLE()    (SSPSTATbits.SMP = 0)
//SPI Mode Select
#define SPI_MODE_SELECT(_CONFIG)  (SSPCON1bits.SSPM = _CONFIG)
//SPI Mode Waveform (Master Mode)
#define SPI_MASTER_WAVEFORM_MODE1()     do { \
                                            SSPCON1bits.CKP = 0; \
                                            SSPSTATbits.CKE = 0; \
                                        } while (0)
                                      
#define SPI_MASTER_WAVEFORM_MODE2()  do { \
                                            SSPCON1bits.CKP = 1; \
                                            SSPSTATbits.CKE = 0; \
                                        } while (0)
#define SPI_MASTER_WAVEFORM_MODE3()  do { \
                                            SSPCON1bits.CKP = 0; \
                                            SSPSTATbits.CKE = 1; \
                                        } while (0)

#define SPI_MASTER_WAVEFORM_MODE4()  do { \
                                            SSPCON1bits.CKP = 1; \
                                            SSPSTATbits.CKE = 1; \
                                        } while (0)
/*
 * Checks if  a new byte is received while the SSPBUF register is still holding the previous data. 
 */
#define SPI_RECEIVER_OVERFLOW_CHECK()     (SSPCON1bits.SSPOV)   
#define SPI_RECEIVER_OVERFLOW_CLEAR()     (SSPCON1bits.SSPOV = 0)
/*
 * Checks if buffer is written while transmisting data
 */
#define SPI_TRANSMIT_COLLISION_CHECK()     (SSPCON1bits.WCOL)   
#define SPI_TRANSMIT_COLLISION_CLEAR()     (SSPCON1bits.WCOL = 0)                                    
/* -------------- Data Types Declarations ---------------------- */
//Synchronous Serial Port Mode Select bits

typedef enum
{
    SPI_MASTER_FOSC_DIV_4 = 0,
    SPI_MASTER_FOSC_DIV_16,
    SPI_MASTER_FOSC_DIV_64,
    SPI_MASTER_TMR2_DIV_2,
    SPI_SLAVE_SS_ENABLED,
    SPI_SLAVE_SS_DISABLED
}spi_mode_select_t;

typedef enum
{
    SPI_CLK_IDLE_LOW_TX_LEADING_RISING = 0,
    SPI_CLK_IDLE_HIGH_TX_LEADING_FALLING,
    SPI_CLK_IDLE_LOW_TX_TRAILING_FALLING,
    SPI_CLK_IDLE_HIGH_TX_TRAILING_RISING,
}spi_mode_waveform_t;

typedef struct 
{
    spi_mode_select_t mode;
    spi_mode_waveform_t master_waveform;
    uint8 master_sample : 1;
#if SPI_INTERRUPT_ENABLE_FEATURE==INTERRUPT_FEATURE_ENABLE
    void (* SPI_InterruptHandler)(void);
#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE
    interrupt_priority priority;    
#endif
#endif 
}spi_t;

/* -------------- Software Interfaces Declarations -------------- */
/**
 * @brief Initializes the SPI Master based on the provided configuration.
 * 
 * This function enables serial port and configures SCK, SDO and SS as outputs and serial port pins,
 * selects the sample postion of thc clock and the waveform (clock polairty and edge to latch data).
 * 
 * @param _spi A pointer the SPI configuration structure (spi_t).
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The operation was successful.
 *         - E_NOT_OK: An error occurred during the operation.
 */
Std_ReturnType SPI_Master_Init(const spi_t *_spi);

/**
 * @brief Initializes the SPI Slave based on the provided configuration.
 * 
 * @param _spi A pointer the SPI configuration structure (spi_t).
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The operation was successful.
 *         - E_NOT_OK: An error occurred during the operation.
 */
Std_ReturnType SPI_Slave_Init(const spi_t *_spi);

/**
 * @brief Transmits data via SPI and receives data from the communication partner.
 *  
 * If the master is going to use this function, it should select which slave to communicate in the application code. 
 * There is no slave select pin as parameter. 
 * 
 * @param data Data to be transmitted.
 * @return uint8 The received data.
 */
uint8 SPI_Transfer_data(uint8 data);

/**
 * @brief A master transmits and receives data from a slave.
 * 
 * @param data Data to transmit.
 * @param rec_data A pointer to store the received data.
 * @param slave_select A pointer to the slave select pin for communication.
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The operation was successful.
 *         - E_NOT_OK: An error occurred during the operation.
 */
Std_ReturnType SPI_Master_Transceiver(const uint8 data, pin_config_t *slave_select, uint8 *rec_data);

/**
 * @brief Receives the data from a slave in a blocking manner and 
 *  disables the trasmission (Master in only Receive mode).
 * 
 * @param Rec_data A pointer to store the received data.
 * @param slave_select A pointer to the slave select pin for communication.
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The operation was successful.
 *         - E_NOT_OK: An error occurred during the operation.
 */
Std_ReturnType SPI_Master_Recieve(uint8 *Rec_data, pin_config_t *slave_select);

/**
 * @brief De-Initializes the SPI module.
 * 
 * @param _spi A pointer the SPI configuration structure (spi_t).
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The operation was successful.
 *         - E_NOT_OK: An error occurred during the operation.
 */
Std_ReturnType SPI_DiInit(const spi_t *_spi);

#endif	/* SPI_H */

