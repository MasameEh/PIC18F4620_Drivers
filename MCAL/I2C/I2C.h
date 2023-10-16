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
#define I2C_MASTER_RECEIVE_MODE_ENABLE_CFG        1
#define I2C_MASTER_RECEIVE_MODE_DISABLE_CFG       0  
//I2C Slew Rate enabled or disabled.
#define I2C_SLEW_RATE_ENABLE_CFG       1
#define I2C_SLEW_RATE_DISABLE_CFG      0
//I2C SMBUS enabled OR disabled.
#define I2C_SMBUS_ENABLE_CFG         1
#define I2C_SMBUS_DISABLE_CFG        0

#define I2C_START_BIT_DETECTED        1
#define I2C_START_BIT_NOT_DETECTED    0

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
//Indicates that a Stop bit has been detected last or not.
#define I2C_STOP_BIT_CHECK()           (SSPSTATbits.P)
//Indicates that a Start bit has been detected last or not.
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
Std_ReturnType I2C_Master_Init(const I2C_t *_i2c);
Std_ReturnType I2C_Slave_Init(const I2C_t *_i2c);
Std_ReturnType I2C_DeInit(const I2C_t *_i2c);

Std_ReturnType I2C_Master_Send_Start();
Std_ReturnType I2C_Master_Send_Repeated_Start();
Std_ReturnType I2C_Master_Send_Stop();

Std_ReturnType I2C_Master_Transmit(uint8 data, uint8 *_ack);
Std_ReturnType I2C_Master_Receive(const I2C_t *_i2c, uint8 *rec_data, uint8 _ack);

Std_ReturnType I2C_Slave_Trasmit(const uint8 data, uint8 *_ack);
#endif	/* I2C_H */

