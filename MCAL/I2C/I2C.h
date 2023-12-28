/* 
 * File:   I2C.h
 * Author: Mohamed Sameh
 *
 * Created on October 12, 2023, 9:21 PM
 */

#ifndef I2C_H
#define	I2C_H

/* -------------- Includes -------------- */
#include "I2C_cfg.h"
#include "../GPIO/gpio.h"
#include "../interrupt/internal_interrupt.h"
/* -------------- Macro Declarations ------------- */
//I2C Master Receive mode enable or disable.
#define I2C_MASTER_RECEIVE_MODE_ENABLE_CFG        1
#define I2C_MASTER_RECEIVE_MODE_DISABLE_CFG       0  
//I2C Slew Rate enabled or disabled.
#define I2C_SLEW_RATE_ENABLE_CFG       1
#define I2C_SLEW_RATE_DISABLE_CFG      0
//I2C SMBUS enabled OR disabled.
#define I2C_SMBUS_ENABLE_CFG         1
#define I2C_SMBUS_DISABLE_CFG        0
//Indicates that a Start bit has been detected or not.
#define I2C_START_BIT_DETECTED        1
#define I2C_START_BIT_NOT_DETECTED    0
//Indicates that a Stop bit has been detected or not.
#define I2C_STOP_BIT_DETECTED         1
#define I2C_STOP_BIT_NOT_DETECTED     0
//I2C Slave general call enabled or disabled.
#define I2C_SLAVE_GENERAL_CALL_ENABLE_CFG    1
#define I2C_SLAVE_GENERAL_CALL_DISABLE_CFG   0
//I2C Slave Clock Stretching enabled or disabled.
#define I2C_SLAVE_CLK_STRETCING_ENABLE_CFG    1
#define I2C_SLAVE_CLK_STRETCING_DISABLE_CFG   0

#define I2C_WRITE_COLLISION_OCCURRED        1  
#define I2C_WRITE_COLLISION_UNOCCURRED      0 

#define I2C_RECEIVER_OVERFLOW_OCCURRED        1  
#define I2C_RECEIVER_OVERFLOW_UNOCCURRED      0

#define I2C_LAST_BYTE_DATA          1  
#define I2C_LAST_BYTE_ADDRESS       0

#define I2C_READ_OPPERATION          1  
#define I2C_WRITE_OPPERATION         0

#define I2C_ACK         0  
#define I2C_NOT_ACK     1
/* -------------- Macro Functions Declarations -------------- */
//MSSP I2C Enable or Disable.
#define I2C_ENABLE()     (SSPCON1bits.SSPEN = 1)
#define I2C_DISABLE()    (SSPCON1bits.SSPEN = 0)
//I2C Mode Select.
#define I2C_MODE_SELECT(_CONFIG)  (SSPCON1bits.SSPM = _CONFIG)

//I2C Master Receive mode enable or disable.
#define I2C_MASTER_RECEIVE_MODE_ENABLE()         (SSPCON2bits.RCEN = 1)
#define I2C_MASTER_RECEIVE_MODE_IDLE()           (SSPCON2bits.RCEN = 0)
//I2C Master initiate ACK SEQ (Receive mode only).
#define I2C_MASTER_RECEIVE_INITIATE_ACK_SEQ()    (SSPCON2bits.ACKEN = 1)
//I2C Master ACK (Receive mode only).
#define I2C_MASTER_RECEIVE_ACK()                 (SSPCON2bits.ACKDT = 0)
//I2C Master NACK (Receive mode only).
#define I2C_MASTER_RECEIVE_NACK()                (SSPCON2bits.ACKDT = 1)
//I2C Master checks if an ACK has been received from a slave (Transmit mode only).
#define I2C_MASTER_ACK_CHECK()                   (SSPCON2bits.ACKSTAT)

//I2C Master initiate Start condition.
#define I2C_INITIATE_START_CONDITION()           (SSPCON2bits.SEN = 1)
//I2C Master initiate Start condition.
#define I2C_INITIATE_REPEATED_START_CONDITION()  (SSPCON2bits.RSEN = 1)
//I2C Master initiate Stop condition.
#define I2C_INITIATE_STOP_CONDITION()            (SSPCON2bits.PEN = 1)

//I2C Slave general call enabled or disabled.
#define I2C_SLAVE_GENERAL_CALL_ENABLE()          (SSPCON2bits.GCEN = 1)
#define I2C_SLAVE_GENERAL_CALL_DISABLE()         (SSPCON2bits.GCEN = 0)
//I2C Slave Clock Stretching enabled or disabled.
#define I2C_CLOCK_STRETCHING_ENABLE()              (SSPCON2bits.SEN = 1)
#define I2C_CLOCK_STRETCHING_DISABLE()             (SSPCON2bits.SEN = 0)
//I2C Slew Rate enabled or disabled.
#define I2C_SLEW_RATE_ENABLE()              (SSPSTATbits.SMP = 1)
#define I2C_SLEW_RATE_DISABLE()             (SSPSTATbits.SMP = 0)
//I2C SMBUS enabled OR disabled.
#define I2C_SMBUS_ENABLE()              (SSPSTATbits.CKE = 1)
#define I2C_SMBUS_DISABLE()             (SSPSTATbits.CKE = 0)

//I2C Slave Clock Stretching.
#define I2C_SLAVE_HOLD_CLOCK_LOW()     (SSPCON1bits.CKP = 0)
//I2C Slave Release Clock.
#define I2C_SLAVE_RELEASE_CLOCK()      (SSPCON1bits.CKP = 1)

//Indicates that the last byte received or transmitted was data or address.
#define I2C_SLAVE_DATA_ADDRESS_CHECK() (SSPSTATbits.D_nA)
//Indicates that a Stop bit has been detected or not.
#define I2C_STOP_BIT_CHECK()           (SSPSTATbits.P)
//Indicates that a Start bit has been detected or not.
#define I2C_START_BIT_CHECK()          (SSPSTATbits.S)
//I2C Slave checks Read or write operation.
#define I2C_R_W_CHECK()                (SSPSTATbits.R_W)
//I2C trasnfer completed or not yet.
#define I2C_BUFFER_STATUS()            (SSPSTATbits.BF)
/*
 * Checks if buffer is written while transmitting data.
 */
#define I2C_TRANSMIT_COLLISION_CHECK()     (SSPCON1bits.WCOL)   
#define I2C_TRANSMIT_COLLISION_CLEAR()     (SSPCON1bits.WCOL = 0)
/*
 * Checks if  A new byte is received while the SSPBUF register is still holding the previous data. 
 */
#define I2C_RECEIVER_OVERFLOW_CHECK()     (SSPCON1bits.SSPOV)   
#define I2C_RECEIVER_OVERFLOW_CLEAR()     (SSPCON1bits.SSPOV = 0)   
/* -------------- Data Types Declarations ---------------------- */
typedef enum
{
    I2C_SLAVE_7BITS_ADD = 6,
    I2C_SLAVE_10BITS_ADD = 7,
    I2C_MASTER_DEFINED_CLOCK = 8,
    I2C_MASTER_FIRMWARE_CONTROLLED = 11,
    I2C_SLAVE_7BITS_ADD_S_P_ENABLED = 14,
    I2C_SLAVE_10BITS_ADD_S_P_ENABLED = 15
}I2C_mode_select_t;

typedef struct 
{
    uint32 i2c_clock_freq;
    I2C_mode_select_t i2c_mode;
    uint8 i2c_slave_address;
    uint8 master_rec_mode_status : 1;
    uint8 i2c_slew_rate : 1;
    uint8 i2c_SMBus_control: 1;
    uint8 i2c_slave_general_call_control : 1;
    uint8 i2c_slave_clk_stretching_control: 1;
    uint8 i2c_reserved : 3;
#if I2C_INTERRUPT_ENABLE_FEATURE==INTERRUPT_FEATURE_ENABLE
    void (* I2C_InterruptHandler)(void);
    void (* I2C_Interrupt_Write_Col)(void);
#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE
    interrupt_priority i2c_priority; 
    interrupt_priority i2c_bus_col_priority;   
#endif
#endif 
}I2C_t;

/* -------------- Software Interfaces Declarations -------------- */
/**
 * @brief Initializes the I2C module in master mode based on the provided configuration.
 * 
 * This function configures and enables the I2C interface for master mode operation. It allows you to specify
 * the desired clock frequency, master receiver mode, slew rate control, SMBus control, and interrupt settings.
 * 
 * @param _i2c A pointer to the I2C configuration structure (I2C_t).
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The operation was successful.
 *         - E_NOT_OK: An error occurred during the operation, or an invalid configuration was provided.
 */
Std_ReturnType I2C_Master_Init(const I2C_t *_i2c);

/**
 * @brief Initializes the I2C module in slave mode based on the provided configuration.
 * 
 * This function configures and enables the I2C interface for slave mode operation. It allows you to set the slave
 * address, enable general call support, clock stretching, slew rate control, SMBus control, and interrupt settings.
 * 
 * @param _i2c A pointer to the I2C configuration structure (I2C_t).
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The operation was successful.
 *         - E_NOT_OK: An error occurred during the operation, or an invalid configuration was provided.
 */
Std_ReturnType I2C_Slave_Init(const I2C_t *_i2c);

/**
 * @brief Deinitializes the I2C module.
 * 
 * This function disables the I2C interface and clears any associated interrupt configurations.
 * 
 * @param _i2c A pointer to the I2C configuration structure (I2C_t).
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The operation was successful.
 *         - E_NOT_OK: An error occurred during the operation.
 */
Std_ReturnType I2C_DeInit(const I2C_t *_i2c);

/**
 * @brief Sends a start condition for I2C communication.
 * 
 * This function initiates an I2C start condition for data transmission. It waits for the completion of the start condition.
 * 
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The start condition was detected successfully.
 *         - E_NOT_OK: The start condition was not detected or an error occurred.
 */
Std_ReturnType I2C_Master_Send_Start();

/**
 * @brief Sends a repeated start condition for I2C communication.
 * 
 * This function initiates a repeated start condition for I2C data transmission. It waits for the completion of the repeated start condition.
 * 
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The repeated start condition was detected successfully.
 *         - E_NOT_OK: The repeated start condition was not detected or an error occurred.
 */
Std_ReturnType I2C_Master_Send_Repeated_Start();

/**
 * @brief Sends a stop condition for I2C communication.
 * 
 * This function initiates an I2C stop condition to terminate data transmission. It waits for the completion of the stop condition.
 * 
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The stop condition was detected successfully.
 *         - E_NOT_OK: The stop condition was not detected or an error occurred.
 */
Std_ReturnType I2C_Master_Send_Stop();

/**
 * @brief Transmits a byte of data in master mode via the I2C communication.
 * 
 * This function sends a data byte on the I2C bus while waiting for completion. It handles data collision, clears the
 * buffer, and provides acknowledgment status.
 * 
 * @param data The byte of data to be transmitted.
 * @param _ack A pointer to store the acknowledgment status.
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The operation was successful.
 *         - E_NOT_OK: An error occurred during the operation, or an invalid acknowledgment status was provided.
 */
Std_ReturnType I2C_Master_Transmit(uint8 data, uint8 *_ack);

/**
 * @brief Receives a byte of data in master mode via the I2C communication.
 * 
 * This function receives a data byte from the I2C bus while waiting for completion. It handles data overflow, provides
 * acknowledgment status, and allows specifying the acknowledgment behavior.
 * 
 * @param _i2c A pointer to the I2C configuration structure (I2C_t).
 * @param rec_data A pointer to store the received data.
 * @param _ack The acknowledgment status (I2C_ACK or I2C_NOT_ACK).
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The operation was successful.
 *         - E_NOT_OK: An error occurred during the operation, or an invalid configuration was provided.
 */
Std_ReturnType I2C_Master_Receive(const I2C_t *_i2c, uint8 *rec_data, uint8 _ack);

/**
 * @brief Transmits data as a slave in I2C communication.
 * 
 * This function allows a slave device to transmit data on the I2C bus. It handles transmission collision and waits for
 * the operation to complete. The acknowledgment behavior depends on the I2C communication.
 * 
 * @param data The data to be transmitted.
 * @param _ack A pointer to store the acknowledgment status.
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The operation was successful.
 *         - E_NOT_OK: An error occurred during the operation.
 */
Std_ReturnType I2C_Slave_Transmit(const uint8 data, uint8 *_ack);

/**
 * @brief Receives data as a slave in I2C communication.
 * 
 * This function allows a slave device to receive data from the I2C bus. It waits for data reception, handles buffer
 * clearing, and manages the clock for a successful operation.
 * 
 * @param rec_data A pointer to store the received data.
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The operation was successful.
 *         - E_NOT_OK: An error occurred during the operation.
 */
Std_ReturnType I2C_Slave_Recieve(uint8 *rec_data);


/**
 * @brief Sends a single byte of data in master mode via the I2C communication.
 * 
 * This function simplifies the process of sending a single byte of data to a specified slave address using I2C
 * communication. It manages the start condition, data transmission, acknowledgment, and stop condition.
 * 
 * @param slave_address The address of the target slave device.
 * @param data The byte of data to be transmitted.
 * @param _ack A pointer to store the acknowledgment status.
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The operation was successful.
 *         - E_NOT_OK: An error occurred during the operation.
 */
Std_ReturnType I2C_Master_Send_1Byte(uint8 slave_address, uint8 data, uint8 *_ack);

#endif	/* I2C_H */

