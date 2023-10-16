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

static Std_ReturnType inline I2C_Slave_Mode_Select(const I2C_t *_i2c);

/**
 * @brief 
 * 
 * @param _i2c 
 * @return Std_ReturnType 
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
        TRISCbits.RC3 = GPIO_DIRECTION_INPUT;  /* SCL */
        TRISCbits.RC4 = GPIO_DIRECTION_INPUT;  /* SDA */
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
        //Enable MSSP I2C
        I2C_ENABLE();
    }
    return ret;
}

/**
 * @brief 
 * 
 * @param _i2c 
 * @return Std_ReturnType 
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
        TRISCbits.RC3 = GPIO_DIRECTION_INPUT;  /* SCL */
        TRISCbits.RC4 = GPIO_DIRECTION_INPUT;  /* SDA */
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
        //Enable MSSP I2C
        I2C_ENABLE();
    }
    return ret;
}

/**
 * @brief 
 * 
 * @param _i2c 
 * @return Std_ReturnType 
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
 * @brief 
 * 
 * @return Std_ReturnType 
 */
Std_ReturnType I2C_Master_Send_Start()
{
    Std_ReturnType ret = E_OK;

    I2C_INITIATE_START_CONDITION();
    //Wait for the completion of the start condition 
    while(SSPCON2bits.SEN);
    I2C_INTERRUPT_FLAG_CLEAR(); /* Clear The Interrupt flag */
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
 * @brief 
 * 
 * @return Std_ReturnType 
 */
Std_ReturnType I2C_Master_Send_Repeated_Start()
{
    Std_ReturnType ret = E_OK;

    I2C_INITIATE_REPEATED_START_CONDITION();
    //Wait for the completion of the repeated start condition 
    while(SSPCON2bits.RSEN);
    I2C_INTERRUPT_FLAG_CLEAR(); /* Clear The Interrupt flag */
    return ret;
}

/**
 * @brief 
 * 
 * @return Std_ReturnType 
 */
Std_ReturnType I2C_Master_Send_Stop()
{
    Std_ReturnType ret = E_OK;

    I2C_INITIATE_STOP_CONDITION();
    //Wait for the completion of the stop condition 
    while(SSPCON2bits.PEN);
    I2C_INTERRUPT_FLAG_CLEAR(); /* Clear The Interrupt flag */
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
 * @brief 
 * 
 * @param data 
 * @param _ack 
 * @return Std_ReturnType 
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
        //Waits until the transmission is complete.
        while(!PIR1bits.SSPIF);
        I2C_INTERRUPT_FLAG_CLEAR();
        if(I2C_ACK == I2C_MASTER_ACK_CHECK())
        {
            *_ack = I2C_ACK;
        }else{ *_ack = I2C_NOT_ACK;}
    }
    return ret;
}

/**
 * @brief 
 * 
 * @param _i2c 
 * @param rec_data 
 * @param _ack 
 * @return Std_ReturnType 
 */
Std_ReturnType I2C_Master_Receive(const I2C_t *_i2c, uint8 *rec_data, uint8 _ack)
{
    Std_ReturnType ret = E_OK;
    
    if(NULL == _i2c || I2C_MASTER_RECEIVE_MODE_DISABLE_CFG == _i2c->master_rec_mode_status)
    {
        ret = E_NOT_OK;
    }
    else
    {
        //Waits until the reception is complete
        while(!PIR1bits.SSPIF);
        I2C_INTERRUPT_FLAG_CLEAR();
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
            I2C_MASTER_RECEIVE_ACK();     /* Acknowledge */
            I2C_MASTER_RECEIVE_INITIATE_ACK_SEQ();  /* Initiate Acknowledge sequence on SDA and SCL pins and transmit ACKDT data bit */
        }
        else if(I2C_NOT_ACK == _ack)
        {
            I2C_MASTER_RECEIVE_NACK();    /* Not Acknowledge */
            I2C_MASTER_RECEIVE_INITIATE_ACK_SEQ();  /* Initiate Acknowledge sequence on SDA and SCL pins and transmit ACKDT data bit */
        }else{/* Nothing */} 
    }
    return ret;
}

/**
 * @brief 
 * 
 * @param data 
 * @param _ack 
 * @return Std_ReturnType 
 */
Std_ReturnType I2C_Slave_Trasmit(const uint8 data, uint8 *_ack)
{
    Std_ReturnType ret = E_OK;
    
    if (NULL == _ack)
    {
        ret = E_NOT_OK;
    }
    else
    {
        SSPBUF = data;
        if(I2C_TRANSMIT_COLLISION_CHECK() == I2C_WRITE_COLLISION_OCCURRED)
        {
            I2C_TRANSMIT_COLLISION_CLEAR();
            SSPBUF = data;
        }
    }
    return ret; 
}

/**
 * @brief MSSP I2C interrupt MCAL helper function
 */
void I2C_ISR()
{
    //MSSP I2C interrupt occurred, the flag must be cleared.
    I2C_INTERRUPT_FLAG_CLEAR();
    //CallBack func gets called every time this ISR executes.
    if(I2C_InterruptHandler)
    {
        I2C_InterruptHandler();
    }else{/* Nothing */}

}

/**
 * @brief MSSP I2C Bus Collision interrupt MCAL helper function
 */
void I2C_BUS_COL_ISR()
{
    //MSSP I2C interrupt occurred, the flag must be cleared
    I2C_BUS_COL_INTERRUPT_FLAG_CLEAR();
    //CallBack func gets called every time this ISR executes.
    if(I2C_Interrupt_Write_Col)
    {
        I2C_Interrupt_Write_Col();
    }else{/* Nothing */}
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