/* 
 * File:   external_interrupt.c
 * Author: Mohamed Sameh
 *
 * Created on September 18, 2023, 3:31 PM
 */

#include "external_interrupt.h"

static void (*INT0_InterruptHandler)(void) = NULL;
static void (*INT1_InterruptHandler)(void) = NULL;
static void (*INT2_InterruptHandler)(void) = NULL;

static void (*RB4_InterruptHandler)(void) = NULL;
static void (*RB5_InterruptHandler)(void) = NULL;
static void (*RB6_InterruptHandler)(void) = NULL;
static void (*RB7_InterruptHandler)(void) = NULL;

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

static Std_ReturnType Interrupt_RBx_Enable(const ext_interrupt_RBx_t *ext_int);
static Std_ReturnType Interrupt_RBx_Disable(const ext_interrupt_RBx_t *ext_int);
static Std_ReturnType Interrupt_RBx_Priority_Set(const ext_interrupt_RBx_t *ext_int);
static Std_ReturnType Interrupt_RBx_Pin_Init(const ext_interrupt_RBx_t *ext_int);

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
        ret = Interrupt_INTx_Flag_Clear(ext_int);
        //Configure the external interrupt edge.
        ret = Interrupt_INTx_Edge_Set(ext_int);
        //Configure the external interrupt pin.
        ret = Interrupt_INTx_Pin_Init(ext_int); 
        //Configure the external interrupt priority.
        #if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE
        ret = Interrupt_INTx_Priority_Set(ext_int);
        #endif
        //Configure the interrupt CallBack.
        ret = Interrupt_INTx_SetInterruptHandler(ext_int);  
        //Enable the external interrupt.
        ret = Interrupt_INTx_Enable(ext_int);
    }
    return ret;
}

void INT0_ISR(void)
{
    //The external interrupt INT0 occured, the flag must be cleared.
    EXT_INT0_FLAG_CLEAR();
    /* Code */

    //CallBack func gets called every time this ISR executes.
    if(INT0_InterruptHandler)
    {
        INT0_InterruptHandler();
    }
}

void INT1_ISR(void)
{
    //The external interrupt INT1 occured, the flag must be cleared.
    EXT_INT1_FLAG_CLEAR();
    /* Code */

    //CallBack func gets called every time this ISR executes.
    if(INT1_InterruptHandler)
    {
        INT1_InterruptHandler();
    }
}

void INT2_ISR(void)
{
    //The external interrupt INT2 occured, the flag must be cleared.
    EXT_INT2_FLAG_CLEAR();
    /* Code */

    //CallBack func gets called every time this ISR executes.
    if(INT2_InterruptHandler)
    {
        INT2_InterruptHandler();
    }
}

/**
 * @brief 
 * 
 * @param ext_int 
 * @return Std_ReturnType 
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
 * @brief 
 * 
 * @param ext_int 
 * @return Std_ReturnType 
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
        EXT_RBx_ENABLE();

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
        //Enable the external interrupt.
        EXT_RBx_DISABLE();
    }
    return ret;
}

/**
 * @brief 
 * 
 * @param ext_int 
 * @return Std_ReturnType 
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

    }
    return ret;
}

/**
 * @brief 
 * 
 * @param ext_int 
 * @return Std_ReturnType 
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
 * @brief 
 * 
 * @param ext_int 
 * @return Std_ReturnType 
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
 * @brief 
 * 
 * @param ext_int 
 * @return Std_ReturnType 
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
 * @brief 
 * 
 * @param ext_int 
 * @return Std_ReturnType 
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
 * @brief 
 * 
 * @param ext_int 
 * @return Std_ReturnType 
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