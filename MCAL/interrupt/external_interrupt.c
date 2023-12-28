/* 
 * File:   external_interrupt.c
 * Author: Mohamed Sameh
 *
 * Created on September 18, 2023, 3:31 PM
 */

#include "external_interrupt.h"

/* Pointer to function to hold the callbacks fo INTx*/
static void (*INT0_InterruptHandler)(void) = NULL;
static void (*INT1_InterruptHandler)(void) = NULL;
static void (*INT2_InterruptHandler)(void) = NULL;
/* Pointer to function to hold the callbacks fo RBx*/
static void (*RB4_InterruptHandler_HIGH)(void) = NULL;
static void (*RB4_InterruptHandler_LOW)(void) = NULL;
static void (*RB5_InterruptHandler_HIGH)(void) = NULL;
static void (*RB5_InterruptHandler_LOW)(void) = NULL;
static void (*RB6_InterruptHandler_HIGH)(void) = NULL;
static void (*RB6_InterruptHandler_LOW)(void) = NULL;
static void (*RB7_InterruptHandler_HIGH)(void) = NULL;
static void (*RB7_InterruptHandler_LOW)(void) = NULL;

static Std_ReturnType Interrupt_INTx_Enable(const ext_interrupt_INTx_t *ext_int);
static Std_ReturnType Interrupt_INTx_Disable(const ext_interrupt_INTx_t *ext_int);
static Std_ReturnType Interrupt_INTx_Priority_Set(const ext_interrupt_INTx_t *ext_int);
static Std_ReturnType Interrupt_INTx_Edge_Set(const ext_interrupt_INTx_t *ext_int);
static Std_ReturnType Interrupt_INTx_Pin_Init(const ext_interrupt_INTx_t *ext_int);
static Std_ReturnType Interrupt_INTx_Flag_Clear(const ext_interrupt_INTx_t *ext_int);

static Std_ReturnType INT0_SetInterruptHandler(void (*InterruptHandler)(void));
static Std_ReturnType INT1_SetInterruptHandler(void (*InterruptHandler)(void));
static Std_ReturnType INT2_SetInterruptHandler(void (*InterruptHandler)(void));
static Std_ReturnType Interrupt_INTx_SetInterruptHandler(const ext_interrupt_INTx_t *ext_int);

/**
 * @brief Initializes the external interrupt (INTx) based on the provided configuration.
 * 
 * This function configures the external interrupt settings, including enabling or disabling
 * the interrupt, setting the interrupt edge, configuring the interrupt pin, and specifying
 * an interrupt handler (callback) function.
 * 
 * @param ext_int A pointer to a structure containing the external interrupt configuration.
 * @return Std_ReturnType
 *   - E_OK: The external interrupt initialization was successful.
 *   - E_NOT_OK: The external interrupt initialization failed due to a NULL configuration pointer.
 */
Std_ReturnType Interrupt_INTx_Init(const ext_interrupt_INTx_t *ext_int)
    {
        Std_ReturnType ret = E_OK;

        if(NULL == ext_int)
        {
            ret = E_NOT_OK;
        }
        else 
        {
            //Disable the external interrupt.
            ret = Interrupt_INTx_Disable(ext_int);
            //Clear the interrupt flag.
            ret &= Interrupt_INTx_Flag_Clear(ext_int);
            //Configure the external interrupt edge.
            ret &= Interrupt_INTx_Edge_Set(ext_int);
            //Configure the external interrupt pin.
            ret &= Interrupt_INTx_Pin_Init(ext_int); 
            //Configure the external interrupt priority.
#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE
            ret &= Interrupt_INTx_Priority_Set(ext_int);
#endif
            //Configure the interrupt CallBack.
            ret &= Interrupt_INTx_SetInterruptHandler(ext_int);  
            //Enable the external interrupt.
            ret &= Interrupt_INTx_Enable(ext_int);
        }
        return ret;
    }

/**
 * @brief Deinitializes the external interrupt (INTx).
 *
 * This function disables the specified external interrupt based on the provided configuration.
 *
 * @param ext_int A pointer to a structure containing the external interrupt configuration.
 *
 * @return Std_ReturnType
 *   - E_OK: The external interrupt deinitialization was successful.
 *   - E_NOT_OK: The external interrupt deinitialization failed due to a NULL configuration pointer.
 */
Std_ReturnType Interrupt_INTx_DeInit(const ext_interrupt_INTx_t *ext_int)
{
    Std_ReturnType ret = E_OK;

    if(NULL == ext_int)
    {
        ret = E_NOT_OK;
    }
    else 
    {
        ret = Interrupt_INTx_Disable(ext_int);
    }
    return ret;
}

/**
 * @brief Initializes the external interrupt (RBx) based on the provided configuration.
 *
 * This function configures the external interrupt settings for RBx, including enabling or disabling
 * the interrupt, setting the interrupt edge, configuring the interrupt pin, specifying the interrupt
 * priority, and setting an interrupt handler (callback) function.
 *
 * @param ext_int A pointer to a structure containing the external interrupt configuration for RBx.
 *
 * @return Std_ReturnType
 *   - E_OK: The external interrupt initialization was successful.
 *   - E_NOT_OK: The external interrupt initialization failed due to a NULL configuration pointer
 *               or an invalid pin number.
 */
Std_ReturnType Interrupt_RBx_Init(const ext_interrupt_RBx_t *ext_int)
{
    Std_ReturnType ret = E_OK;

    if(NULL == ext_int)
    {
        ret = E_NOT_OK;
    }
    else 
    {
        //Disable the external interrupt.
        EXT_RBx_DISABLE();
        //Clear the interrupt flag.
        EXT_RBx_FLAG_CLEAR();
        //Configure the external interrupt priority.         
#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE
        //This macro will enable the priority levels on interrupts.
        INTERRUPT_PriorityLevelsEnable();
        if(INTERRUPT_LOW_PRIORITY == ext_int->priority)
        {
            //This macro will enable low-priority global interrupts.
            INTERRUPT_GlobalInterruptLowEnable();
            //This macro will set external interrupt (RBx) as low priority.
            EXT_RBIE_LOW_PRIORITY();
        }
        else if(INTERRUPT_HIGH_PRIORITY == ext_int->priority)
        {
            //This macro will enable high-priority global interrupts.
            INTERRUPT_GlobalInterruptHighEnable();
            //This macro will set external interrupt (RBx) as high priority.
            EXT_RBIE_HIGH_PRIORITY();    
        }else{}
#else            
        INTERRUPT_GlobalInterruptEnable();
        INTERRUPT_PeripheralInterruptEnable();
#endif  
        //initialize the pin direction to be input
        ret = gpio_pin_set_direction(&(ext_int->pin));  
        //Configure the interrupt CallBack.
        switch (ext_int->pin.pin_num)
        {
        case GPIO_PIN4:
            RB4_InterruptHandler_HIGH = ext_int->EXT_InterruptHandler_HIGH;
            RB4_InterruptHandler_LOW = ext_int->EXT_InterruptHandler_LOW;
            break;
        case GPIO_PIN5:
            RB5_InterruptHandler_HIGH = ext_int->EXT_InterruptHandler_HIGH;
            RB5_InterruptHandler_LOW = ext_int->EXT_InterruptHandler_LOW;
            break;
        case GPIO_PIN6:
            RB6_InterruptHandler_HIGH = ext_int->EXT_InterruptHandler_HIGH;
            RB6_InterruptHandler_LOW = ext_int->EXT_InterruptHandler_LOW;
            break;
        case GPIO_PIN7:
            RB7_InterruptHandler_HIGH = ext_int->EXT_InterruptHandler_HIGH;
            RB7_InterruptHandler_LOW = ext_int->EXT_InterruptHandler_LOW;
            break;
        default:
            ret = E_NOT_OK;
            break;
        }
        //Enable the external interrupt.
        EXT_RBx_ENABLE();
    }
    return ret;
}

/**
 * @brief Deinitializes the external interrupt (RBx).
 *
 * This function disables the specified external interrupt based on the provided configuration.
 *
 * @param ext_int A pointer to a structure containing the external interrupt configuration for RBx.
 *
 * @return Std_ReturnType
 *   - E_OK: The external interrupt deinitialization was successful.
 *   - E_NOT_OK: The external interrupt deinitialization failed due to a NULL configuration pointer.
 */
Std_ReturnType Interrupt_RBx_DeInit(const ext_interrupt_RBx_t *ext_int)
{
    Std_ReturnType ret = E_OK;

    if(NULL == ext_int)
    {
        ret = E_NOT_OK;
    }
    else 
    {
        //Disable the external interrupt.
        EXT_RBx_DISABLE();
    }
    return ret;
}

/**
 * @brief Enables the specified external interrupt (INTx).
 *
 * This function enables the specified external interrupt based on the provided configuration.
 *
 * @param ext_int A pointer to a structure containing the external interrupt configuration.
 *
 * @return Std_ReturnType
 *   - E_OK: The external interrupt enabling was successful.
 *   - E_NOT_OK: The external interrupt enabling failed due to a NULL configuration pointer.
 */
static Std_ReturnType Interrupt_INTx_Enable(const ext_interrupt_INTx_t *ext_int)
{
    Std_ReturnType ret = E_OK;

    if(NULL == ext_int)
    {
        ret = E_NOT_OK;
    }
    else 
    {
        switch (ext_int->source)
        {
        case INTERRUPT_EXTERNAL_INT0: 
#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE
            INTERRUPT_GlobalInterruptHighEnable(); 
#else            
            INTERRUPT_GlobalInterruptEnable();
            INTERRUPT_PeripheralInterruptEnable();
#endif            
            EXT_INT0_ENABLE();
            break;
            
        case INTERRUPT_EXTERNAL_INT1:
#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE
            INTERRUPT_PriorityLevelsEnable();
            if(INTERRUPT_LOW_PRIORITY == ext_int->priority)
            {
                INTERRUPT_GlobalInterruptLowEnable();
            }
            else if(INTERRUPT_HIGH_PRIORITY == ext_int->priority)
            {
                INTERRUPT_GlobalInterruptHighEnable();    
            }else{}
#else            
            INTERRUPT_GlobalInterruptEnable();
            INTERRUPT_PeripheralInterruptEnable();
#endif            
            EXT_INT1_ENABLE();
            break;            
        case INTERRUPT_EXTERNAL_INT2: 
#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE 
            INTERRUPT_PriorityLevelsEnable();
            if(INTERRUPT_LOW_PRIORITY == ext_int->priority)
            {
                INTERRUPT_GlobalInterruptLowEnable();
            }
            else if(INTERRUPT_HIGH_PRIORITY == ext_int->priority)
            {
                INTERRUPT_GlobalInterruptHighEnable();    
            }else{}            
#else             
            INTERRUPT_GlobalInterruptEnable();
            INTERRUPT_PeripheralInterruptEnable();
#endif            
            EXT_INT2_ENABLE(); 
            break;            
        default: 
            ret = E_NOT_OK; 
            break;
        }
    }
    return ret;
}

/**
 * @brief Disables the specified external interrupt (INTx).
 *
 * This function disables the specified external interrupt based on the provided configuration.
 *
 * @param ext_int A pointer to a structure containing the external interrupt configuration.
 *
 * @return Std_ReturnType
 *   - E_OK: The external interrupt disabling was successful.
 *   - E_NOT_OK: The external interrupt disabling failed due to a NULL configuration pointer.
 */
static Std_ReturnType Interrupt_INTx_Disable(const ext_interrupt_INTx_t *ext_int)
{
    Std_ReturnType ret = E_OK;

    if(NULL == ext_int)
    {
        ret = E_NOT_OK;
    }
    else 
    {
        switch (ext_int->source)
        {
        case INTERRUPT_EXTERNAL_INT0: 
            EXT_INT0_DISABLE(); 
            break;
        case INTERRUPT_EXTERNAL_INT1: 
            EXT_INT1_DISABLE(); 
            break;
        case INTERRUPT_EXTERNAL_INT2: 
            EXT_INT2_DISABLE(); 
            break;
        default: 
            ret = E_NOT_OK; 
            break;
        }
    }
    return ret;
}

/**
 * @brief Sets the priority level for the external interrupt (INTx).
 *
 * This function sets the priority level for the specified external interrupt based on the provided
 * configuration. Note that this function is conditionally compiled based on the INTERRUPT_PRIORITY_LEVELS_ENABLE
 * and INTERRUPT_FEATURE_ENABLE macros.
 *
 * @param ext_int A pointer to a structure containing the external interrupt configuration.
 *
 * @return Std_ReturnType
 *   - E_OK: The external interrupt priority setting was successful.
 *   - E_NOT_OK: The external interrupt priority setting failed due to a NULL configuration pointer
 *               or an invalid priority level.
 */
#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE
static Std_ReturnType Interrupt_INTx_Priority_Set(const ext_interrupt_INTx_t *ext_int)
{
    Std_ReturnType ret = E_OK;

    if(NULL == ext_int)
    {
        ret = E_NOT_OK;
    }
    else 
    {
        switch (ext_int->source)
        {
        case INTERRUPT_EXTERNAL_INT1: 
            if(INTERRUPT_LOW_PRIORITY == ext_int->priority)
            {
                EXT_INT1_LOW_PRIORITY();
            }
            else if(INTERRUPT_HIGH_PRIORITY == ext_int->priority)
            {
                EXT_INT1_HIGH_PRIORITY();
            } 
            else{}
            break;
            
        case INTERRUPT_EXTERNAL_INT2: 
            if(INTERRUPT_LOW_PRIORITY == ext_int->priority)
            {
                EXT_INT2_LOW_PRIORITY();
            }
            else if(INTERRUPT_HIGH_PRIORITY == ext_int->priority)
            {
                EXT_INT2_HIGH_PRIORITY();
            }
            else{} 
            break;

        default: 
            ret = E_NOT_OK; 
            break;
        }
    }
    return ret;
}
#endif
/**
 * @brief Sets the edge detection configuration for the external interrupt (INTx).
 *
 * This function configures the edge detection settings for the specified external interrupt (INTx)
 * based on the provided configuration.
 *
 * @param ext_int A pointer to a structure containing the external interrupt configuration.
 *
 * @return Std_ReturnType
 *   - E_OK: The external interrupt edge configuration was successful.
 *   - E_NOT_OK: The external interrupt edge configuration failed due to a NULL configuration pointer
 *               or an unsupported edge type.
 */
static Std_ReturnType Interrupt_INTx_Edge_Set(const ext_interrupt_INTx_t *ext_int)
{
    Std_ReturnType ret = E_OK;

    if(NULL == ext_int)
    {
        ret = E_NOT_OK;
    }
    else 
    {
        switch (ext_int->source)
        {
        case INTERRUPT_EXTERNAL_INT0: 
            if(INTERRUPT_FALLING_EDGE == ext_int->edge)
            {
                EXT_INT0_FALLING_EDGE_SET();
            }
            else if (INTERRUPT_RISING_EDGE == ext_int->edge)
            {
                EXT_INT0_RISING_EDGE_SET();
            }
            else{}
            break;

        case INTERRUPT_EXTERNAL_INT1: 
            if(INTERRUPT_FALLING_EDGE == ext_int->edge)
            {
                EXT_INT1_FALLING_EDGE_SET();
            }
            else if (INTERRUPT_RISING_EDGE == ext_int->edge)
            {
                EXT_INT1_RISING_EDGE_SET();
            }
            else{}
            break;

        case INTERRUPT_EXTERNAL_INT2: 
            if(INTERRUPT_FALLING_EDGE == ext_int->edge)
            {
                EXT_INT2_FALLING_EDGE_SET();
            }
            else if (INTERRUPT_RISING_EDGE == ext_int->edge)
            {
                EXT_INT2_RISING_EDGE_SET();
            }
            else{} 
            break;

        default: 
            ret = E_NOT_OK; 
            break;
        }
    }
    return ret;
}

/**
 * @brief Initializes the external interrupt (INTx) pin configuration.
 *
 * This function configures the pin direction for the external interrupt (INTx) based on the provided
 * configuration.
 *
 * @param ext_int A pointer to a structure containing the external interrupt configuration.
 *
 * @return Std_ReturnType
 *   - E_OK: The external interrupt pin initialization was successful.
 *   - E_NOT_OK: The external interrupt pin initialization failed due to a NULL configuration pointer
 *               or other errors.
 */
static Std_ReturnType Interrupt_INTx_Pin_Init(const ext_interrupt_INTx_t *ext_int)
{
    Std_ReturnType ret = E_OK;

    if(NULL == ext_int)
    {
        ret = E_NOT_OK;
    }
    else 
    {
        ret = gpio_pin_set_direction(&(ext_int->pin));
    }
    return ret;
}

/**
 * @brief Clears the interrupt flag for the external interrupt (INTx).
 *
 * This function clears the interrupt flag associated with the specified external interrupt (INTx)
 * based on the provided configuration.
 *
 * @param ext_int A pointer to a structure containing the external interrupt configuration.
 *
 * @return Std_ReturnType
 *   - E_OK: The external interrupt flag clearing was successful.
 *   - E_NOT_OK: The external interrupt flag clearing failed due to a NULL configuration pointer
 *               or other errors.
 */
static Std_ReturnType Interrupt_INTx_Flag_Clear(const ext_interrupt_INTx_t *ext_int)
{
    Std_ReturnType ret = E_OK;

    if(NULL == ext_int)
    {
        ret = E_NOT_OK;
    }
    else 
    {
        switch (ext_int->source)
        {
        case INTERRUPT_EXTERNAL_INT0: 
            EXT_INT0_FLAG_CLEAR(); 
            break;
        case INTERRUPT_EXTERNAL_INT1: 
            EXT_INT1_FLAG_CLEAR(); 
            break;
        case INTERRUPT_EXTERNAL_INT2: 
            EXT_INT2_FLAG_CLEAR();
            break;
        default: 
            ret = E_NOT_OK; 
            break;
        }
    }
    return ret;
}

/**
 * @brief Sets the interrupt handler (callback) for the external interrupt (INT0).
 *
 * This function sets the interrupt handler (callback) function for the specified external interrupt (INT0)
 * based on the provided configuration.
 *
 * @param InterruptHandler A function pointer to the interrupt handler.
 *
 * @return Std_ReturnType
 *   - E_OK: The interrupt handler setting was successful.
 *   - E_NOT_OK: The interrupt handler setting failed due to a NULL function pointer or other errors.
 */
static Std_ReturnType INT0_SetInterruptHandler(void (*InterruptHandler)(void))
{
    Std_ReturnType ret = E_OK;

    if(NULL == InterruptHandler)
    {
        ret = E_NOT_OK;
    }
    else 
    {
        INT0_InterruptHandler = InterruptHandler;
    }
    return ret;
}

/**
 * @brief Sets the interrupt handler (callback) for the external interrupt (INT1).
 *
 * This function sets the interrupt handler (callback) function for the specified external interrupt (INT1)
 * based on the provided configuration.
 *
 * @param InterruptHandler A function pointer to the interrupt handler.
 *
 * @return Std_ReturnType
 *   - E_OK: The interrupt handler setting was successful.
 *   - E_NOT_OK: The interrupt handler setting failed due to a NULL function pointer or other errors.
 */
static Std_ReturnType INT1_SetInterruptHandler(void (*InterruptHandler)(void))
{
    Std_ReturnType ret = E_OK;

    if(NULL == InterruptHandler)
    {
        ret = E_NOT_OK;
    }
    else 
    {
        INT1_InterruptHandler = InterruptHandler;
    }
    return ret;
}

/**
 * @brief Sets the interrupt handler (callback) for the external interrupt (INT2).
 *
 * This function sets the interrupt handler (callback) function for the specified external interrupt (INT2)
 * based on the provided configuration.
 *
 * @param InterruptHandler A function pointer to the interrupt handler.
 *
 * @return Std_ReturnType
 *   - E_OK: The interrupt handler setting was successful.
 *   - E_NOT_OK: The interrupt handler setting failed due to a NULL function pointer or other errors.
 */
static Std_ReturnType INT2_SetInterruptHandler(void (*InterruptHandler)(void))
{
    Std_ReturnType ret = E_OK;

    if(NULL == InterruptHandler)
    {
        ret = E_NOT_OK;
    }
    else 
    {
        INT2_InterruptHandler = InterruptHandler;
    }
    return ret;
}

/**
 * @brief Sets the interrupt handler (callback) for the external interrupt (INTx).
 *
 * This function sets the interrupt handler (callback) function for the specified external interrupt (INTx)
 * based on the provided configuration.
 *
 * @param ext_int A pointer to a structure containing the external interrupt configuration.
 *
 * @return Std_ReturnType
 *   - E_OK: The interrupt handler setting was successful.
 *   - E_NOT_OK: The interrupt handler setting failed due to a NULL function pointer or other errors.
 */
static Std_ReturnType Interrupt_INTx_SetInterruptHandler(const ext_interrupt_INTx_t *ext_int)
{
    Std_ReturnType ret = E_OK;

    if(NULL == ext_int)
    {
        ret = E_NOT_OK;
    }
    else 
    {
        switch (ext_int->source)
        {
        case INTERRUPT_EXTERNAL_INT0: 
            ret = INT0_SetInterruptHandler(ext_int->EXT_InterruptHandler);
            break;
        case INTERRUPT_EXTERNAL_INT1: 
            ret = INT1_SetInterruptHandler(ext_int->EXT_InterruptHandler);
            break;
        case INTERRUPT_EXTERNAL_INT2: 
            ret = INT2_SetInterruptHandler(ext_int->EXT_InterruptHandler);            
            break;
        default: 
            ret = E_NOT_OK; 
            break;
        }
    }
    return ret;
}

/**
 * @brief External interrupt INT0 MCAL helper function
 * 
 */
void INT0_ISR(void)
{
    //The external interrupt INT0 occurred, the flag must be cleared.
    EXT_INT0_FLAG_CLEAR();
    /* Code */

    //CallBack func gets called every time this ISR executes.
    if(INT0_InterruptHandler)
    {
        INT0_InterruptHandler();
    }
}

/**
 * @brief External interrupt INT1 MCAL helper function
 * 
 */
void INT1_ISR(void)
{
    //The external interrupt INT1 occurred, the flag must be cleared.
    EXT_INT1_FLAG_CLEAR();
    /* Code */

    //CallBack func gets called every time this ISR executes.
    if(INT1_InterruptHandler)
    {
        INT1_InterruptHandler();
    }
}

/**
 * @brief External interrupt INT2 MCAL helper function
 * 
 */
void INT2_ISR(void)
{
    //The external interrupt INT2 occurred, the flag must be cleared.
    EXT_INT2_FLAG_CLEAR();
    /* Code */

    //CallBack func gets called every time this ISR executes.
    if(INT2_InterruptHandler)
    {
        INT2_InterruptHandler();
    }
}

/**
 * @brief External interrupt RB4 MCAL helper function
 * 
 */
void RB4_ISR(uint8 RB_src)
{
    //The external interrupt RB4 occurred, the flag must be cleared.
    EXT_RBx_FLAG_CLEAR();
    //CallBack func gets called every time this ISR executes.
    if(0 == RB_src)
    {   
        if(RB4_InterruptHandler_LOW)
        {
            RB4_InterruptHandler_LOW();
        }else{}
    }
    else if(1 == RB_src)
    {
        if(RB4_InterruptHandler_HIGH)
        {
            RB4_InterruptHandler_HIGH();
        }
    }
}

/**
 * @brief External interrupt RB5 MCAL helper function
 * 
 */
void RB5_ISR(uint8 RB_src)
{
    //The external interrupt RB5 occurred, the flag must be cleared.
    EXT_RBx_FLAG_CLEAR();
    //CallBack func gets called every time this ISR executes.
    if(0 == RB_src)
    {   
        if(RB5_InterruptHandler_LOW)
        {
            RB5_InterruptHandler_LOW();
        }else{}
    }
    else if(1 == RB_src)
    {
        if(RB5_InterruptHandler_HIGH)
        {
            RB5_InterruptHandler_HIGH();
        }
    }
}

/**
 * @brief External interrupt RB6 MCAL helper function
 * 
 */
void RB6_ISR(uint8 RB_src)
{
    //The external interrupt RB6 occurred, the flag must be cleared.
    EXT_RBx_FLAG_CLEAR();
    //CallBack func gets called every time this ISR executes.
    if(0 == RB_src)
    {   
        if(RB6_InterruptHandler_LOW)
        {
            RB6_InterruptHandler_LOW();
        }else{}
    }
    else if(1 == RB_src)
    {
        if(RB6_InterruptHandler_HIGH)
        {
            RB6_InterruptHandler_HIGH();
        }
    }
}

/**
 * @brief External interrupt RB7 MCAL helper function
 * 
 */
void RB7_ISR(uint8 RB_src)
{
    //The external interrupt RB7 occurred, the flag must be cleared.
    EXT_RBx_FLAG_CLEAR();
    //CallBack func gets called every time this ISR executes.
    if(0 == RB_src)
    {   
        if(RB7_InterruptHandler_LOW)
        {
            RB7_InterruptHandler_LOW();
        }else{}
    }
    else if(1 == RB_src)
    {
        if(RB7_InterruptHandler_HIGH)
        {
            RB7_InterruptHandler_HIGH();
        }
    }
}