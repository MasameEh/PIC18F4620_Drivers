/* 
 * File:   spi.c
 * Author: Mohamed Sameh
 *
 * Created on October 8, 2023, 2:45 AM
 */

#include "spi.h"

#if SPI_INTERRUPT_ENABLE_FEATURE==INTERRUPT_FEATURE_ENABLE
static void (*SPI_InterruptHandler)(void) = NULL;
#endif

static Std_ReturnType inline SPI_Master_Mode_Select(const spi_t *_spi);
static Std_ReturnType inline SPI_Master_Sample_Select(const spi_t *_spi);
static Std_ReturnType inline SPI_Master_WaveForm_Select(const spi_t *_spi);

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
Std_ReturnType SPI_Master_Init(const spi_t *_spi)
{
    Std_ReturnType ret = E_OK;
    
    if (NULL == _spi)
    {
        ret = E_NOT_OK;
    }
    else
    {
        //Disable SPI
        SPI_DISABLE();
        /* Initialize pins */
        //SDO
        TRISCbits.RC5 = GPIO_DIRECTION_OUTPUT;
        //SCK
        TRISCbits.RC3 = GPIO_DIRECTION_OUTPUT;
        //SS
        TRISAbits.RA5 = GPIO_DIRECTION_OUTPUT;
        //SPI Master Mode Select
        ret &= SPI_Master_Mode_Select(_spi);
        //Sample
        ret &= SPI_Master_Sample_Select(_spi);
        //SPI Mode Waveform (Master Mode)
        ret &= SPI_Master_WaveForm_Select(_spi);
        //Configure the interrupt
#if SPI_INTERRUPT_ENABLE_FEATURE==INTERRUPT_FEATURE_ENABLE
        SPI_INTERRUPT_ENABLE();
        SPI_INTERRUPT_FLAG_CLEAR();
        SPI_InterruptHandler = _spi->SPI_InterruptHandler;

        //Interrupt priority configurations
#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE
        INTERRUPT_PriorityLevelsEnable();
        if(INTERRUPT_HIGH_PRIORITY == _spi->priority)
        {
            INTERRUPT_GlobalInterruptHighEnable();
            SPI_INT_HIGH_PRIORITY();
        }
        else if(INTERRUPT_LOW_PRIORITY == _spi->priority)
        {
            INTERRUPT_GlobalInterruptLowEnable();
            SPI_INT_LOW_PRIORITY();
        }else{/* Nothing */}
#else 
        INTERRUPT_GlobalInterruptEnable();
        INTERRUPT_PeripheralInterruptEnable();
#endif
#endif
        //Enable SPI
        SPI_ENABLE();
    }
    return ret;
}

/**
 * @brief Initializes the SPI Slave based on the provided configuration.
 * 
 * @param _spi A pointer the SPI configuration structure (spi_t).
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The operation was successful.
 *         - E_NOT_OK: An error occurred during the operation.
 */
Std_ReturnType SPI_Slave_Init(const spi_t *_spi)
{
    Std_ReturnType ret = E_OK;
    
    if (NULL == _spi)
    {
        ret = E_NOT_OK;
    }
    else
    {
        //Disable SPI
        SPI_DISABLE();
        /* Initialize pins */
        //SDO
        TRISCbits.RC5 = GPIO_DIRECTION_OUTPUT;
        //SCK
        TRISCbits.RC3 = GPIO_DIRECTION_INPUT;
        //SS
        TRISAbits.RA5 = GPIO_DIRECTION_INPUT;
        //SPI Slave mode select
        switch (_spi->mode)
        {
        case SPI_SLAVE_SS_ENABLED: SPI_MODE_SELECT(SPI_SLAVE_SS_ENABLED); break;
        case SPI_SLAVE_SS_DISABLED: SPI_MODE_SELECT(SPI_SLAVE_SS_DISABLED); break;
        default: ret = E_NOT_OK; break;
        }
        //SPI Mode Waveform (Must be as Master Mode)
        ret &= SPI_Master_WaveForm_Select(_spi);
        //Sample (Must be cleared in Slave Mode)
        SSPSTATbits.SMP = 0;
        //Enable SPI
        SPI_ENABLE();
    }
    return ret;
}

/**
 * @brief Trasmits the data from a master to slave in a blocking manner.
 * 
 * @param data Data to transmit.
 * @param slave_select  A pointer to the slave select pin for communication.
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The operation was successful.
 *         - E_NOT_OK: An error occurred during the operation.
 */
Std_ReturnType SPI_Master_Trasnmit(const uint8 data, pin_config_t *slave_select)
{
    Std_ReturnType ret = E_OK;
    uint8 data_flush = ZERO_INIT;
    gpio_pin_initialize(slave_select);
    if(NULL == slave_select)
    {
        ret = E_NOT_OK;
    }
    else
    {
        gpio_pin_write(slave_select, GPIO_LOW);
#if SPI_INTERRUPT_ENABLE_FEATURE==INTERRUPT_FEATURE_ENABLE
        SPI_INTERRUPT_ENABLE();
#endif
        SSPBUF = data;
        if(SPI_TRANSMIT_COLLISION_CHECK() == SPI_WRITE_COLLISION_OCCURRED)
        {
            SPI_TRANSMIT_COLLISION_CLEAR();
            SSPBUF = data;
        }
        while(!PIR1bits.SSPIF);
        SPI_INTERRUPT_FLAG_CLEAR();
        data_flush = SSPBUF;
        gpio_pin_write(slave_select, GPIO_HIGH);
    }
    return ret;
}

/**
 * @brief Receives the data from a slave in a blocking manner.
 * 
 * @param Rec_data A pointer to store the received data.
 * @param slave_select A pointer to the slave select pin for communication.
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The operation was successful.
 *         - E_NOT_OK: An error occurred during the operation.
 */
Std_ReturnType SPI_Master_Recieve(uint8 *Rec_data, pin_config_t *slave_select)
{
    Std_ReturnType ret = E_OK;
    gpio_pin_initialize(slave_select);

    if(NULL == Rec_data)
    {
        ret = E_NOT_OK;
    }
    else
    {
        gpio_pin_write(slave_select, GPIO_LOW);
#if SPI_INTERRUPT_ENABLE_FEATURE==INTERRUPT_FEATURE_ENABLE
        SPI_INTERRUPT_ENABLE();
#endif
        //SDO INPUT (Disable spi transmiting)
        TRISCbits.RC5 = GPIO_DIRECTION_INPUT;
        SSPBUF = 0;
        while(!SPI_RECEIVE_STATUS());
        SPI_INTERRUPT_FLAG_CLEAR();
        *Rec_data = SSPBUF;
        //SDO OUTPUT (Enable spi transmiting)
        TRISCbits.RC5 = GPIO_DIRECTION_OUTPUT;
        gpio_pin_write(slave_select, GPIO_HIGH);
    }
    return ret;
}

/**
 * @brief Transmits data from a slave to a master.
 * 
 * @param data Data to transmit.
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The operation was successful.
 *         - E_NOT_OK: An error occurred during the operation.
 */
Std_ReturnType SPI_Slave_Transmit(uint8 data)
{
    Std_ReturnType ret = E_OK;
    uint8 data_flush = ZERO_INIT;
    SSPBUF = data;
    data_flush = SSPBUF;
    return ret;
}

/**
 * @brief Receives data from a master.
 * 
 * @param data A pointer to store the received data.
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The operation was successful.
 *         - E_NOT_OK: An error occurred during the operation.
 */
Std_ReturnType SPI_Slave_Receive(uint8 *data)
{
    Std_ReturnType ret = E_OK;
    *data = SSPBUF;
    return ret;
}

/**
 * @brief De-Initializes the SPI module.
 * 
 * @param _spi A pointer the SPI configuration structure (spi_t).
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The operation was successful.
 *         - E_NOT_OK: An error occurred during the operation.
 */
Std_ReturnType SPI_DiInit(const spi_t *_spi)
{   
    Std_ReturnType ret = E_OK;
    if(NULL == _spi)
    {
        ret = E_NOT_OK;
    }
    else
    {
        SPI_DISABLE();
#if SPI_INTERRUPT_ENABLE_FEATURE==INTERRUPT_FEATURE_ENABLE
        SPI_INTERRUPT_DISABLE();
#endif
    }
    return ret;
}

/**
 * @brief The SPI interrupt MCAL helper function
 * 
 */
void SPI_ISR(void)
{
    //SPI interrupt occurred, the flag must be cleared.
    SPI_INTERRUPT_DISABLE();
    //CallBack func gets called every time this ISR executes.
    if(SPI_InterruptHandler)
    {
        SPI_InterruptHandler();
    }else{/* Nothing */}
}

/**
 * @brief Helper function to Select Master Mode  
 *   Options:
 *   SPI_MASTER_FOSC_DIV_4 
 *   SPI_MASTER_FOSC_DIV_16
 *   SPI_MASTER_FOSC_DIV_64
 *   SPI_MASTER_TMR2_DIV_2 
 */
static Std_ReturnType inline SPI_Master_Mode_Select(const spi_t *_spi)
{   
    Std_ReturnType ret = E_OK;
    switch (_spi->mode)
    {
        case SPI_MASTER_FOSC_DIV_4:  SPI_MODE_SELECT(SPI_MASTER_FOSC_DIV_4);  break;
        case SPI_MASTER_FOSC_DIV_16: SPI_MODE_SELECT(SPI_MASTER_FOSC_DIV_16); break;
        case SPI_MASTER_FOSC_DIV_64: SPI_MODE_SELECT(SPI_MASTER_FOSC_DIV_64); break;
        case SPI_MASTER_TMR2_DIV_2:  SPI_MODE_SELECT(SPI_MASTER_TMR2_DIV_2);  break;
        default: ret = E_NOT_OK; break;
    }
    return ret;
}

/**
 * @brief Helper function to Select Master Sample   
 */
static Std_ReturnType inline SPI_Master_Sample_Select(const spi_t *_spi)
{
    Std_ReturnType ret = E_OK;

    switch (_spi->master_sample)
    {
        case SPI_MASTER_SAMPLE_END_CFG: SPI_MASTER_SAMPLE_END(); break;
        case SPI_MASTER_SAMPLE_MIDDLE_CFG: SPI_MASTER_SAMPLE_MIDDLE(); break;
        default: ret = E_NOT_OK; break;
    }
    return ret;
}

/**
 * @brief Helper function to Select Master WaveForm  
 */
static Std_ReturnType inline SPI_Master_WaveForm_Select(const spi_t *_spi)
{
    Std_ReturnType ret = E_OK;

    switch (_spi->master_waveform)
    {
        case SPI_CLK_IDLE_LOW_TX_LEADING_RISING:    SPI_MASTER_WAVEFORM_MODE1(); break;
        case SPI_CLK_IDLE_HIGH_TX_LEADING_FALLING:  SPI_MASTER_WAVEFORM_MODE2(); break;
        case SPI_CLK_IDLE_LOW_TX_TRAILING_FALLING:  SPI_MASTER_WAVEFORM_MODE3(); break;
        case SPI_CLK_IDLE_HIGH_TX_TRAILING_RISING:  SPI_MASTER_WAVEFORM_MODE4(); break;
        default: ret = E_NOT_OK; break;
    }
    return ret;
}