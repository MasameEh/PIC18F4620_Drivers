/* 
 * File:   I2C.c
 * Author: Mohamed Sameh
 *
 * Created on October 12, 2023, 9:21 PM
 */
#include "I2C.h"

#if I2C_INTERRUPT_ENABLE_FEATURE==INTERRUPT_FEATURE_ENABLE
static void (* I2C_InterruptHandler)(void) = NULL;
#endif

static Std_ReturnType inline I2C_SLAVE_Mode_Select(const I2C_t *_i2c);

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
        if(I2C_MASTER_RECEIVE_MODE_ENABLE_CFG == _i2c->i2c_slew_rate)
        {
            I2C_SLEW_RATE_ENABLE();
        }else{I2C_SLEW_RATE_DISABLE();}
        //SMBus Select
        if(I2C_MASTER_RECEIVE_MODE_ENABLE_CFG == _i2c->i2c_SMBus_control)
        {
            I2C_SMBUS_ENABLE();
        }else{I2C_SMBUS_DISABLE();}
        //Enable MSSP I2C
        I2C_ENABLE();
    }
    return ret;
}
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
        //Select Slave Mode
        ret = I2C_SLAVE_Mode_Select(_i2c);
        //General Call Status.
        if(I2C_SLAVE_GENERAL_CALL_ENABLE_CFG == _i2c->i2c_slave_general_call_control)
        {
            I2C_SLAVE_GENERAL_CALL_ENABLE();
        }else{I2C_SLAVE_GENERAL_CALL_DISABLE();}
        //Clock Stretching Status.
        if(I2C_SLAVE_CLK_STRETCING_ENABLE_CFG == _i2c->i2c_slave_clk_stretching_control)
        {
            I2C_CLOCK_STRETCHING_ENABLE();
        }else{I2C_CLOCK_STRETCHING_DISABLE();}
        //Slew Rate Control
        if(I2C_MASTER_RECEIVE_MODE_ENABLE_CFG == _i2c->i2c_slew_rate)
        {
            I2C_SLEW_RATE_ENABLE();
        }else{I2C_SLEW_RATE_DISABLE();}
        //SMBus Select
        if(I2C_MASTER_RECEIVE_MODE_ENABLE_CFG == _i2c->i2c_SMBus_control)
        {
            I2C_SMBUS_ENABLE();
        }else{I2C_SMBUS_DISABLE();}
        //Enable MSSP I2C
        I2C_ENABLE();
    }
    return ret;
}

Std_ReturnType I2C_DeInit(const I2C_t *_i2c)
{
    Std_ReturnType ret = E_OK;
    
    if (NULL == _i2c)
    {
        ret = E_NOT_OK;
    }
    else
    {
        
    }
    return ret;
}

Std_ReturnType I2C_Master_Send_Start(const I2C_t *_i2c)
{
    Std_ReturnType ret = E_OK;
    
    if (NULL == _i2c)
    {
        ret = E_NOT_OK;
    }
    else
    {
        
    }
    return ret;
}

Std_ReturnType I2C_Master_Send_Repeated_Start(const I2C_t *_i2c)
{
    Std_ReturnType ret = E_OK;
    
    if (NULL == _i2c)
    {
        ret = E_NOT_OK;
    }
    else
    {
        
    }
    return ret;
}

Std_ReturnType I2C_Master_Send_Stop(const I2C_t *_i2c)
{
    Std_ReturnType ret = E_OK;
    
    if (NULL == _i2c)
    {
        ret = E_NOT_OK;
    }
    else
    {
        
    }
    return ret;
}

Std_ReturnType I2C_Trasmit(const I2C_t *_i2c, const uint8 data)
{
    Std_ReturnType ret = E_OK;
    
    if (NULL == _i2c)
    {
        ret = E_NOT_OK;
    }
    else
    {
        
    }
    return ret;
}

Std_ReturnType I2C_Receive(const I2C_t *_i2c, uint8 *rec_data)
{
    Std_ReturnType ret = E_OK;
    
    if (NULL == _i2c)
    {
        ret = E_NOT_OK;
    }
    else
    {
        
    }
    return ret;
}



void I2C_ISR()
{

}


/**
 * @brief Helper function to Select Master Mode  
 *   Options:
 *   I2C_SLAVE_7BITS_ADD 
 *   I2C_SLAVE_10BITS_ADD
 *   I2C_SLAVE_7BITS_ADD_S_P_ENABLED
 *   I2C_SLAVE_10BITS_ADD_S_P_ENABLED 
 */
static Std_ReturnType inline I2C_SLAVE_Mode_Select(const I2C_t *_i2c)
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