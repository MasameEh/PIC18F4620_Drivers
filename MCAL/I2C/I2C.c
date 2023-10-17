/* 
 * File:   I2C.c
 * Author: Mohamed Sameh
 *
 * Created on October 12, 2023, 9:21 PM
 */
#include "I2C.h"

#if I2C_INTERRUPT_ENABLE_FEATURE==INTERRUPT_FEATURE_ENABLE
static void (* I2C_InterruptHandler)(void) = NULL;
static void (* I2C_Interrupt_Write_Col)(void) = NULL;
#endif

static void inline I2C_Gpio_Configurations();
static void inline I2C_Interrupt_Configure(const I2C_t *_i2c);
static Std_ReturnType inline I2C_Slave_Mode_Select(const I2C_t *_i2c);

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
Std_ReturnType I2C_Master_Init(const I2C_t *_i2c)
{
    Std_ReturnType ret = E_OK;

    if (NULL == _i2c)
    {
        ret = E_NOT_OK;
    }
    else
    {
        //Disable MSSP I2C
        I2C_DISABLE();
        //GPIO Configurations
        I2C_Gpio_Configurations();
        //Select Master Mode
        switch (_i2c->i2c_mode)
        {
        case I2C_MASTER_DEFINED_CLOCK:       
            I2C_MODE_SELECT(I2C_MASTER_DEFINED_CLOCK); 
            SSPADD = (uint8)((_XTAL_FREQ/(4*_i2c->i2c_clock_freq)) - 1);
            break;
        case I2C_MASTER_FIRMWARE_CONTROLLED: I2C_MODE_SELECT(I2C_MASTER_FIRMWARE_CONTROLLED); break;
        default: ret = E_NOT_OK; break;
        }
        //Select Master Receive Mode Enable or Disabled.
        if(I2C_MASTER_RECEIVE_MODE_ENABLE_CFG == _i2c->master_rec_mode_status)
        {
            I2C_MASTER_RECEIVE_MODE_ENABLE();
        }else{I2C_MASTER_RECEIVE_MODE_IDLE();}
        //Slew Rate Control
        if(I2C_SLEW_RATE_ENABLE_CFG == _i2c->i2c_slew_rate)
        {
            I2C_SLEW_RATE_ENABLE();
        }else{I2C_SLEW_RATE_DISABLE();}
        //SMBus Select
        if(I2C_SMBUS_ENABLE_CFG == _i2c->i2c_SMBus_control)
        {
            I2C_SMBUS_ENABLE();
        }else{I2C_SMBUS_DISABLE();}
        //Interrupt Configurations
        I2C_Interrupt_Configure(_i2c);
        //Enable MSSP I2C
        I2C_ENABLE();
    }
    return ret;
}

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
Std_ReturnType I2C_Slave_Init(const I2C_t *_i2c)
{
    Std_ReturnType ret = E_OK;
    
    if (NULL == _i2c)
    {
        ret = E_NOT_OK;
    }
    else
    {
        //Disable MSSP I2C
        I2C_DISABLE();
        //GPIO Configurations
        I2C_Gpio_Configurations();
        //Select Slave Mode.
        ret = I2C_Slave_Mode_Select(_i2c);
        //Write the Slave Address.
        SSPADD = _i2c->i2c_slave_address; 
        //General Call Status.
        if(I2C_SLAVE_GENERAL_CALL_ENABLE_CFG == _i2c->i2c_slave_general_call_control)
        {
            I2C_SLAVE_GENERAL_CALL_ENABLE();
        }else{I2C_SLAVE_GENERAL_CALL_DISABLE();}
        //Clock Stretching Status.
        if(I2C_SLAVE_CLK_STRETCING_ENABLE_CFG == _i2c->i2c_slave_clk_stretching_control)
        {
            I2C_CLOCK_STRETCHING_ENABLE();
            //Release Clock
            I2C_SLAVE_RELEASE_CLOCK();
        }else{I2C_CLOCK_STRETCHING_DISABLE(); }
        //Slew Rate Control
        if(I2C_SLEW_RATE_ENABLE_CFG == _i2c->i2c_slew_rate)
        {
            I2C_SLEW_RATE_ENABLE();
        }else{I2C_SLEW_RATE_DISABLE();}
        //SMBus Select
        if(I2C_SMBUS_ENABLE_CFG == _i2c->i2c_SMBus_control)
        {
            I2C_SMBUS_ENABLE();
        }else{I2C_SMBUS_DISABLE();}
        //Interrupt Configurations
        I2C_Interrupt_Configure(_i2c);
        //Enable MSSP I2C
        I2C_ENABLE();
    }
    return ret;
}

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
Std_ReturnType I2C_DeInit(const I2C_t *_i2c)
{
    Std_ReturnType ret = E_OK;
    
    if (NULL == _i2c)
    {
        ret = E_NOT_OK;
    }
    else
    {
        I2C_DISABLE();
#if I2C_INTERRUPT_ENABLE_FEATURE==INTERRUPT_FEATURE_ENABLE
        I2C_INTERRUPT_DISABLE();
        I2C_BUS_COL_INTERRUPT_DISABLE(); 
#endif
    }
    return ret;
}

/**
 * @brief Sends a start condition for I2C communication.
 * 
 * This function initiates an I2C start condition for data transmission. It waits for the completion of the start condition.
 * 
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The start condition was detected successfully.
 *         - E_NOT_OK: The start condition was not detected or an error occurred.
 */
Std_ReturnType I2C_Master_Send_Start()
{
    Std_ReturnType ret = E_OK;

    I2C_INITIATE_START_CONDITION();
    //Wait for the completion of the start condition 
    while(SSPCON2bits.SEN);
    PIR1bits.SSPIF = 0; /* Clear The Interrupt flag */
    if(I2C_START_BIT_DETECTED == I2C_START_BIT_CHECK())
    {
        ret = E_OK;
    }
    else if(I2C_START_BIT_NOT_DETECTED == I2C_START_BIT_CHECK())
    {
        ret = E_NOT_OK;
    }
    return ret;
}

/**
 * @brief Sends a repeated start condition for I2C communication.
 * 
 * This function initiates a repeated start condition for I2C data transmission. It waits for the completion of the repeated start condition.
 * 
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The repeated start condition was detected successfully.
 *         - E_NOT_OK: The repeated start condition was not detected or an error occurred.
 */
Std_ReturnType I2C_Master_Send_Repeated_Start()
{
    Std_ReturnType ret = E_OK;

    I2C_INITIATE_REPEATED_START_CONDITION();
    //Wait for the completion of the repeated start condition 
    while(SSPCON2bits.RSEN);
    PIR1bits.SSPIF = 0; /* Clear The Interrupt flag */
    return ret;
}

/**
 * @brief Sends a stop condition for I2C communication.
 * 
 * This function initiates an I2C stop condition to terminate data transmission. It waits for the completion of the stop condition.
 * 
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The stop condition was detected successfully.
 *         - E_NOT_OK: The stop condition was not detected or an error occurred.
 */
Std_ReturnType I2C_Master_Send_Stop()
{
    Std_ReturnType ret = E_OK;

    I2C_INITIATE_STOP_CONDITION();
    //Wait for the completion of the stop condition 
    while(SSPCON2bits.PEN);
    PIR1bits.SSPIF = 0; /* Clear The Interrupt flag */
    if(I2C_STOP_BIT_DETECTED == I2C_STOP_BIT_CHECK())
    {
        ret = E_OK;
    }
    else if(I2C_STOP_BIT_NOT_DETECTED == I2C_STOP_BIT_CHECK())
    {
        ret = E_NOT_OK;
    }
    return ret;
}

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
Std_ReturnType I2C_Master_Transmit(uint8 data, uint8 *_ack)
{
    Std_ReturnType ret = E_OK;
    
    if (NULL == _ack)
    {
        ret = E_NOT_OK;
    }
    else
    {
        SSPBUF = data;
        //Checks if SSPBUF register is written while it is still transmitting the previous word.
        if(I2C_TRANSMIT_COLLISION_CHECK() == I2C_WRITE_COLLISION_OCCURRED) 
        {
            I2C_TRANSMIT_COLLISION_CLEAR();
            SSPBUF = data;
        }
        while(I2C_BUFFER_STATUS());  /* Waits until the transmission is complete. */
        PIR1bits.SSPIF = 0;          /* Clear The Interrupt flag */
        if(I2C_ACK == I2C_MASTER_ACK_CHECK())
        {
            *_ack = I2C_ACK;
        }else{ *_ack = I2C_NOT_ACK;}
    }
    return ret;
}

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
Std_ReturnType I2C_Master_Receive(const I2C_t *_i2c, uint8 *rec_data, uint8 _ack)
{
    Std_ReturnType ret = E_OK;
    
    if(NULL == _i2c)
    {
        ret = E_NOT_OK;
    }
    else
    {
        I2C_MASTER_RECEIVE_MODE_ENABLE(); 
        //Waits until the reception is complete
        while(!I2C_BUFFER_STATUS());
        *rec_data = SSPBUF;
        //Checks if a byte is received while the SSPBUF register is still holding the previous byte       
        if(I2C_RECEIVER_OVERFLOW_CHECK() == I2C_RECEIVER_OVERFLOW_OCCURRED)
        {
            I2C_RECEIVER_OVERFLOW_CLEAR();
            *rec_data = SSPBUF; 
        }
        //ACK status
        if(I2C_ACK == _ack)
        {
            I2C_MASTER_RECEIVE_ACK();               /* Acknowledge */
            I2C_MASTER_RECEIVE_INITIATE_ACK_SEQ();  /* Initiate Acknowledge sequence on SDA and SCL pins and transmit ACKDT data bit */
        }
        else if(I2C_NOT_ACK == _ack)
        {
            I2C_MASTER_RECEIVE_NACK();               /* Not Acknowledge */
            I2C_MASTER_RECEIVE_INITIATE_ACK_SEQ();  /* Initiate Acknowledge sequence on SDA and SCL pins and transmit ACKDT data bit */
        }else{/* Nothing */} 
    }
    return ret;
}

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
Std_ReturnType I2C_Slave_Transmit(const uint8 data, uint8 *_ack)
{
    Std_ReturnType ret = E_OK;
    uint8 dummy_data = ZERO_INIT;

    if(NULL == _ack) 
    {
        ret = E_NOT_OK;
    }
    else if(I2C_READ_OPPERATION == I2C_R_W_CHECK() && I2C_LAST_BYTE_ADDRESS == I2C_SLAVE_DATA_ADDRESS_CHECK())
    {
        dummy_data = SSPBUF;  /* Read the last Byte to clear the buffer */
        SSPBUF = data;
        if(I2C_TRANSMIT_COLLISION_CHECK() == I2C_WRITE_COLLISION_OCCURRED)
        {
            I2C_TRANSMIT_COLLISION_CLEAR();
            SSPBUF = data;
        }
        //Waits until the transmission is complete
        while(!PIR1bits.SSPIF);
        PIR1bits.SSPIF = 0;
    }
    return ret; 
}

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
Std_ReturnType I2C_Slave_Recieve(uint8 *rec_data)
{
    Std_ReturnType ret = E_OK;
    uint8 dummy_data = ZERO_INIT;

    if(NULL != rec_data && I2C_LAST_BYTE_ADDRESS == I2C_SLAVE_DATA_ADDRESS_CHECK() && 
    I2C_WRITE_OPPERATION == I2C_R_W_CHECK())
    {
        dummy_data = SSPBUF;        /* Read the last Byte to clear the buffer */
        I2C_SLAVE_HOLD_CLOCK_LOW(); /* Hold the clock until the operation is over */
        while(!I2C_BUFFER_STATUS()); /* Waits until the reception is complete */
        PIR1bits.SSPIF = 0;          /* Clear The Interrupt flag */
        *rec_data = SSPBUF;         /* Read the received data */
        I2C_SLAVE_RELEASE_CLOCK();  /* Release the Clock */
    }
    else
    {
        ret = E_NOT_OK;
    }
    return ret; 
}

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
Std_ReturnType I2C_Master_Send_1Byte(uint8 slave_address, uint8 data, uint8 *_ack)
{
    Std_ReturnType ret = E_OK;
    ret &= I2C_Master_Send_Start();
    ret &= I2C_Master_Transmit(slave_address, _ack);
    ret &= I2C_Master_Transmit(data, _ack);
    ret &= I2C_Master_Send_Stop();
    return ret; 
}
/**
 * @brief MSSP I2C interrupt MCAL helper function
 */
void I2C_ISR()
{
#if I2C_INTERRUPT_ENABLE_FEATURE==INTERRUPT_FEATURE_ENABLE
    //MSSP I2C interrupt occurred, the flag must be cleared.
    I2C_INTERRUPT_FLAG_CLEAR();
    //CallBack func gets called every time this ISR executes.
    if(I2C_InterruptHandler)
    {
        I2C_InterruptHandler();
    }else{/* Nothing */}
#endif    
}

/**
 * @brief MSSP I2C Bus Collision interrupt MCAL helper function
 */

void I2C_BUS_COL_ISR()
{
#if I2C_INTERRUPT_ENABLE_FEATURE==INTERRUPT_FEATURE_ENABLE
    //MSSP I2C interrupt occurred, the flag must be cleared
    I2C_BUS_COL_INTERRUPT_FLAG_CLEAR();
    //CallBack func gets called every time this ISR executes.
    if(I2C_Interrupt_Write_Col)
    {
        I2C_Interrupt_Write_Col();
    }else{/* Nothing */}
#endif
}

/**
 * @brief Helper function to Select Master Mode  
 *   Options:
 *   I2C_SLAVE_7BITS_ADD 
 *   I2C_SLAVE_10BITS_ADD
 *   I2C_SLAVE_7BITS_ADD_S_P_ENABLED
 *   I2C_SLAVE_10BITS_ADD_S_P_ENABLED 
 */
static Std_ReturnType inline I2C_Slave_Mode_Select(const I2C_t *_i2c)
{   
    Std_ReturnType ret = E_OK;
    switch (_i2c->i2c_mode)
    {
        case I2C_SLAVE_7BITS_ADD:              I2C_MODE_SELECT(I2C_SLAVE_7BITS_ADD);              break;
        case I2C_SLAVE_10BITS_ADD:             I2C_MODE_SELECT(I2C_SLAVE_10BITS_ADD);             break;
        case I2C_SLAVE_7BITS_ADD_S_P_ENABLED:  I2C_MODE_SELECT(I2C_SLAVE_7BITS_ADD_S_P_ENABLED);  break;
        case I2C_SLAVE_10BITS_ADD_S_P_ENABLED: I2C_MODE_SELECT(I2C_SLAVE_10BITS_ADD_S_P_ENABLED); break;
        default: ret = E_NOT_OK; break;
    }
    return ret;
}

static void inline I2C_Interrupt_Configure(const I2C_t *_i2c)
{
#if I2C_INTERRUPT_ENABLE_FEATURE==INTERRUPT_FEATURE_ENABLE
        I2C_INTERRUPT_ENABLE();
        I2C_BUS_COL_INTERRUPT_ENABLE(); 
        I2C_INTERRUPT_FLAG_CLEAR();
        I2C_BUS_COL_INTERRUPT_FLAG_CLEAR();
        I2C_InterruptHandler = _i2c->I2C_InterruptHandler;
        I2C_Interrupt_Write_Col = _i2c->I2C_Interrupt_Write_Col;

        //Interrupt priority configurations
#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE
        INTERRUPT_PriorityLevelsEnable();
        if(INTERRUPT_HIGH_PRIORITY == _i2c->i2c_priority)
        {
            INTERRUPT_GlobalInterruptHighEnable();
            I2C_INT_HIGH_PRIORITY();
        }
        else if(INTERRUPT_LOW_PRIORITY == _i2c->i2c_priority)
        {
            INTERRUPT_GlobalInterruptLowEnable();
            I2C_INT_LOW_PRIORITY();
        }else{/* Nothing */}
        if(INTERRUPT_HIGH_PRIORITY == _i2c->i2c_bus_col_priority)
        {
            INTERRUPT_GlobalInterruptHighEnable();
            I2C_BUS_COL_INT_HIGH_PRIORITY();
        }
        else if(INTERRUPT_LOW_PRIORITY == _i2c->i2c_bus_col_priority)
        {
            INTERRUPT_GlobalInterruptLowEnable();
            I2C_BUS_COL_INT_LOW_PRIORITY();
        }else{/* Nothing */}
#else 
        INTERRUPT_GlobalInterruptEnable();
        INTERRUPT_PeripheralInterruptEnable();
#endif
#endif
}

/**
 * @brief Intializes SCL and SDA for I2C communication.
 */
static void inline I2C_Gpio_Configurations()
{
    TRISCbits.RC3 = GPIO_DIRECTION_INPUT;  /* SCL */
    TRISCbits.RC4 = GPIO_DIRECTION_INPUT;  /* SDA */
}