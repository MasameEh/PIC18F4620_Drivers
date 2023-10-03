    /* 
     * File:   ccp.h
     * Author: Mohamed Sameh
     *
     * Created on September 29, 2023, 3:22 AM
     */

#ifndef CCP1_H
#define	CCP1_H

/* -------------- Includes -------------- */
#include "ccp_cfg.h"
#include "../GPIO/gpio.h"
#include "../interrupt/internal_interrupt.h"
/* -------------- Macro Declarations ------------- */
//CCP1 Module Mode Select
#define CCP_MODULE_DISABLE                  0x00
#define CCP_CAPTURE_MODE_1_FALLING_EDGE     0x04
#define CCP_CAPTURE_MODE_1_RISING_EDGE      0x05
#define CCP_CAPTURE_MODE_4_RISING_EDGE      0x06
#define CCP_CAPTURE_MODE_16_RISING_EDGE     0x07
#define CCP_COMPARE_MODE_TOGGLE_ON_MATCH    0x02
#define CCP_COMPARE_MODE_SET_PIN_HIGH       0x08
#define CCP_COMPARE_MODE_SET_PIN_LOW        0x09
#define CCP_COMPARE_MODE_GEN_SW_INTERRUPT   0x0A
#define CCP_COMPARE_MODE_GEN_EVENT          0x0B
#define CCP_PMW_MODE                        0x0C


//CCP Capture Mode State
#define CCP_CAPTURE_NOT_READY     0x00
#define CCP_CAPTURE_READY         0x01

//CCP Compare Mode State
#define CCP_COMPARE_NOT_READY     0x00
#define CCP_COMPARE_READY         0x01

/* Timer2 Postscaler values  */
#define CCP_TIMER2_POSTSCALER_DIV_1     1
#define CCP_TIMER2_POSTSCALER_DIV_2     2
#define CCP_TIMER2_POSTSCALER_DIV_3     3
#define CCP_TIMER2_POSTSCALER_DIV_4     4
#define CCP_TIMER2_POSTSCALER_DIV_5     5
#define CCP_TIMER2_POSTSCALER_DIV_6     6
#define CCP_TIMER2_POSTSCALER_DIV_7     7
#define CCP_TIMER2_POSTSCALER_DIV_8     8
#define CCP_TIMER2_POSTSCALER_DIV_9     9       
#define CCP_TIMER2_POSTSCALER_DIV_10    10
#define CCP_TIMER2_POSTSCALER_DIV_11    11
#define CCP_TIMER2_POSTSCALER_DIV_12    12
#define CCP_TIMER2_POSTSCALER_DIV_13    13
#define CCP_TIMER2_POSTSCALER_DIV_14    14
#define CCP_TIMER2_POSTSCALER_DIV_15    15  
#define CCP_TIMER2_POSTSCALER_DIV_16    16

/* Timer2 Prescaler values  */
#define CCP_TIMER2_PRESCALER_DIV_1     1
#define CCP_TIMER2_PRESCALER_DIV_4     2
#define CCP_TIMER2_PRESCALER_DIV_16    3

/* -------------- Macro Functions Declarations -------------- */
//CCP1 Module Selection
#define CCP1_SET_MODE(_CONGIF)      (CCP1CONbits.CCP1M = _CONGIF)

//CCP2 Module Selection
#define CCP2_SET_MODE(_CONGIF)      (CCP2CONbits.CCP2M = _CONGIF)

/* -------------- Data Types Declarations ---------------------- */
typedef enum
{
    CCP1_INST,
    CCP2_INST,
}ccp_inst_t;

typedef enum
{
    CCP1_CCP2_TIMER3 = 0,
    CCP1_TIMER1_CCP2_TIMER3,
    CCP1_CCP2_TIMER1
}ccp_capture_compare_timer_t;

typedef union 
{
    struct
    {
        uint8 ccprx_low;    //CCPRx Low Register
        uint8 ccprx_high;   //CCPRx High Register
    };
    struct
    {
        uint16 ccprx_16Bits; /* Read CCPRx as 16-Bit value */
    };
}cpp_period_reg_t;

/**
 * @brief CCP1 Mode Select. 
 */
typedef enum
{
    CCP_CAPTURE_MD = 0,
    CCP_COMPARE_MD,
    CCP_PMW_MD
}ccp_mode_t;

/**
 * @brief CCP1 Module Configurations.
 * 
 */
typedef struct
{
    ccp_inst_t CCPx;
    ccp_mode_t mode;                                       //CCP Main Mode
    uint8 mode_variant;                                    //CCP Mode Variant
    pin_config_t pin;                                       //CCP Pin Configurations
    ccp_capture_compare_timer_t ccp_timer;
#if CCP1_INTERRUPT_ENABLE_FEATURE==INTERRUPT_FEATURE_ENABLE 
    void (* CCP1_InterruptHandler)(void);                   //Call back used for all CCP1 modes
#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE
    interrupt_priority CCP1_priority;                            //Configure the priority
#endif
#endif
#if CCP2_INTERRUPT_ENABLE_FEATURE==INTERRUPT_FEATURE_ENABLE 
    void (* CCP2_InterruptHandler)(void);                   //Call back used for all CCP1 modes
#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE
    interrupt_priority CCP2_priority;                            //Configure the priority
#endif
#endif
#if CCP1_CFG_SELECTED_MODE==CCP_CFG_PMW_MODE_SELECTED || CCP2_CFG_SELECTED_MODE==CCP_CFG_PMW_MODE_SELECTED
    uint32 PMW_Freq;                                        //CCP PMW Mode Frequency
    uint8 timer2_postscaler : 4;
    uint8 timer2_prescaler : 2;
#endif
}ccp_t;


/* -------------- Software Interfaces Declarations -------------- */
/**
 * @brief Initializes the CCP Module based on the provided configuration.
 * 
 * @param _ccp1 A pointer to the CCP configuration structure.
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The operation was successful.
 *         - E_NOT_OK: An error occurred during the operation.
 */
Std_ReturnType CCP_Init(const ccp_t *_ccp1);

/**
 * @brief De-initializes the CCP Module.
 * 
 * @param _ccp1 A pointer to the CCP configuration structure.
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The operation was successful.
 *         - E_NOT_OK: An error occurred during the operation.
 */
Std_ReturnType CCP_DeInit(const ccp_t *_ccp1);

#if CCP1_CFG_SELECTED_MODE==CCP1_CFG_CAPTURE_MODE_SELECTED
/**
 * @brief Checks whether the data is ready or not.
 * 
 * @param _ccp A pointer to the CCP configuration structure.
 * @param status A pointer to return the status of the data.
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The operation was successful.
 *         - E_NOT_OK: An error occurred during the operation.
 */
Std_ReturnType CCP_Is_Captured_Data_Ready(const ccp_t *_ccp, uint8 *status);

/**
 * @brief Reads the Captured Data.
 * 
 * @param _ccp A pointer to the CCP configuration structure.
 * @param val A pointer to Stores the Captured Data.
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The operation was successful.
 *         - E_NOT_OK: An error occurred during the operation.
 */
Std_ReturnType CCP_Capture_Read_Val(const ccp_t *_ccp, uint16 *val);
#endif

#if CCP1_CFG_SELECTED_MODE==CCP1_CFG_COMPARE_MODE_SELECTED
/**
 * @brief Checks whether the comparsion is done or not yet.
 * 
 * @param _ccp A pointer to the CCP configuration structure.
 * @param status A pointer to return the status of the comparsion.
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The operation was successful.
 *         - E_NOT_OK: An error occurred during the operation.
 */
Std_ReturnType CCP_Is_Compare_Completed(const ccp_t *_ccp, uint8 *status);

/**
 * @brief Writes the value to be compared.
 * 
 * @param _ccp A pointer to the CCP configuration structure.
 * @param com_value A value to be compared with.
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The operation was successful.
 *         - E_NOT_OK: An error occurred during the operation.
 */
Std_ReturnType CCP_Compare_Set_Value(const ccp_t *_ccp, uint16 com_value);
#endif

#if CCP1_CFG_SELECTED_MODE==CCP_CFG_PMW_MODE_SELECTED
/**
 * @brief Sets the Duty Cycle.
 * 
 * @param _ccp A pointer to the CCP configuration structure.
 * @param duty Duty cycle on the scale from 0 to 100.
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The operation was successful.
 *         - E_NOT_OK: An error occurred during the operation.
 */
Std_ReturnType CCP_PMW_Set_Duty(const ccp_t *_ccp, uint8 duty);

/**
 * @brief Stops the PMW.
 * 
 * @param _ccp A pointer to the CCP configuration structure.
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The operation was successful.
 *         - E_NOT_OK: An error occurred during the operation.
 */
Std_ReturnType CCP_PMW_Stop(const ccp_t *_ccp);      
#endif

#endif	/* CCP1_H */

