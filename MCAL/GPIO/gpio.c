/* 
 * File:   gpio.c
 * Author: Mohamed Sameh
 * 
 * Description:
 * This source file provides the implementation of functions for controlling GPIO pins and ports.
 * These functions allow setting the direction, writing logic levels, reading logic levels, toggling,
 * and initializing GPIO pins and ports. The functions are designed to work with a configuration structure
 * and provide an interface for easy manipulation of GPIO pins and ports.
 * 
 * Created on August 22, 2023, 11:09 PM
 */

#include "gpio.h"
/* Reference to data direction control registers */
volatile uint8 *tris_registers[] = {&TRISA, &TRISB, &TRISC, &TRISD, &TRISE};
/* Reference to data latch registers (Read and Write to data latch) */
volatile uint8 *lat_registers[] = {&LATA, &LATB, &LATC, &LATD, &LATE};
/* Reference to port registers */
volatile uint8 *port_registers[] = {&PORTA, &PORTB, &PORTC, &PORTD, &PORTE};

/**
 * @brief Sets the direction of a GPIO pin.
 * 
 * This function configures the direction (input or output) of a specific GPIO pin based
 * on the information provided in the pin configuration structure.
 * 
 * @param pin A pointer to the pin configuration structure.
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The operation was successful.
 *         - E_NOT_OK: An error occurred during the operation.
 */
#if GPIO_PORT_PIN_CONFIGURATION==CONFIG_ENABLE
Std_ReturnType gpio_pin_set_direction(const pin_config_t *pin)
{
    Std_ReturnType ret = E_OK;

    if(NULL == pin || pin->pin_num > PORT_PIN_MAX_NUM - 1)
    {
        ret = E_NOT_OK;
    }
    else
    {
        switch (pin->direction)
        {
        case GPIO_DIRECTION_INPUT:
            SET_BIT(*tris_registers[pin->port], pin->pin_num);
            break;
        case GPIO_DIRECTION_OUTPUT:
            CLR_BIT(*tris_registers[pin->port], pin->pin_num);
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
 * @brief Retrieves the direction (input or output) of a specific GPIO pin.
 * 
 * This function reads and returns the direction (input or output) of a specific GPIO pin
 * based on the provided pin configuration. It reads the corresponding bit in the TRIS register.
 * 
 * @param pin A pointer to the pin configuration structure.
 * @param direction_status Pointer to store the direction status (input or output).
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The operation was successful.
 *         - E_NOT_OK: An error occurred during the operation.
 */
#if GPIO_PORT_PIN_CONFIGURATION==CONFIG_ENABLE
Std_ReturnType gpio_pin_get_direction(const pin_config_t *pin, direction_t *direction_status)
{
    Std_ReturnType ret = E_OK;

    if(NULL == pin || NULL == direction_status || pin->pin_num > PORT_PIN_MAX_NUM - 1)
    {
        ret = E_NOT_OK;
    }
    else
    {
        *direction_status = READ_BIT(*tris_registers[pin->port], pin->pin_num);
    }
    return ret;
}
#endif

/**
 * @brief Writes a logic level (high or low) to a specific GPIO pin.
 * 
 * This function writes a logic level (high or low) to a specific GPIO pin based on the provided
 * pin configuration. It sets or clears the corresponding bit in the LAT register to achieve
 * the desired logic level.
 * 
 * @param pin A pointer to the pin configuration structure.
 * @param logic The desired logic level (GPIO_HIGH or GPIO_LOW).
 * @return Std_ReturnType A status indicating the success or failure of the operation. 
 *          - E_OK: The operation was successful.
 *          - E_NOT_OK: An error occurred during the operation.
 */
#if GPIO_PORT_PIN_CONFIGURATION==CONFIG_ENABLE
Std_ReturnType gpio_pin_write(const pin_config_t *pin, logic_t logic)
{
    Std_ReturnType ret = E_OK;

    if(NULL == pin || pin->pin_num > PORT_PIN_MAX_NUM - 1) 
    {
        ret = E_NOT_OK;
    }
    else
    {
        switch (logic)
        {
        case GPIO_LOW:
            CLR_BIT(*lat_registers[pin->port], pin->pin_num);
            break;
        case GPIO_HIGH:
            CLR_BIT(*lat_registers[pin->port], pin->pin_num);
            SET_BIT(*lat_registers[pin->port], pin->pin_num);
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
 * @brief Reads the logic level (high or low) of a specific GPIO pin.
 * 
 * This function reads and returns the logic level (high or low) of a specific GPIO pin
 * based on the provided pin configuration. It reads the corresponding bit in the PORT register.
 * 
 * @param pin A pointer to the pin configuration structure.
 * @param logic Pointer to store the logic level (GPIO_HIGH or GPIO_LOW).
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *          - E_OK: The operation was successful.
 *          - E_NOT_OK: An error occurred during the operation.
 */
#if GPIO_PORT_PIN_CONFIGURATION==CONFIG_ENABLE
Std_ReturnType gpio_pin_read(const pin_config_t *pin, logic_t *logic)
{
    Std_ReturnType ret = E_OK;

    if(NULL == pin || NULL == logic || pin->pin_num > PORT_PIN_MAX_NUM - 1)
    {
        ret = E_NOT_OK;
    }
    else
    {
        *logic = READ_BIT(*port_registers[pin->port], pin->pin_num);
    }

    return ret;
}
#endif

/**
 * @brief Toggles the logic level of a specific GPIO pin.
 * 
 * This function toggles (inverts) the logic level of a specific GPIO pin based on the provided
 * pin configuration. It XORs the corresponding bit in the LAT register to achieve the toggle effect.
 * 
 * @param pin A pointer to the pin configuration structure.
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *          - E_OK: The operation was successful.
 *          - E_NOT_OK: An error occurred during the operation.
 */
#if GPIO_PORT_PIN_CONFIGURATION==CONFIG_ENABLE
Std_ReturnType gpio_pin_toggle(const pin_config_t *pin)
{
    Std_ReturnType ret = E_OK;

    if(NULL == pin || pin->pin_num > PORT_PIN_MAX_NUM - 1)
    {
        ret = E_NOT_OK;
    }
    else
    {
        TOG_BIT(*lat_registers[pin->port], pin->pin_num);
    }

    return ret;
}
#endif

/**
 * @brief Initializes a specific GPIO pin's direction and logic level.
 * 
 * This function initializes a specific GPIO pin's direction and logic level based on the provided
 * pin configuration. It sets the direction and logic level by calling the appropriate functions.
 * 
 * @param pin A pointer to the pin configuration structure.
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *          - E_OK: The operation was successful.
 *          - E_NOT_OK: An error occurred during the operation.
 */
#if GPIO_PORT_PIN_CONFIGURATION==CONFIG_ENABLE
Std_ReturnType gpio_pin_intialize(const pin_config_t *pin)
{
    Std_ReturnType ret = E_OK;

    if(NULL == pin || pin->pin_num > PORT_PIN_MAX_NUM - 1)
    {
        ret = E_NOT_OK;
    }
    else
    {
        ret = gpio_pin_set_direction(pin);
        ret = gpio_pin_write(pin, pin->logic);
    }

    return ret;
}
#endif

/**
 * @brief Sets the direction (input or output) of an entire GPIO port.
 * 
 * This function sets the direction (input or output) of an entire GPIO port by writing the specified
 * direction value to the corresponding TRIS register.
 * 
 * @param port The index of the port to configure.
 * @param direction The direction value to set for the entire port.
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *          - E_OK: The operation was successful.
 *          - E_NOT_OK: An error occurred during the operation.
 */
#if GPIO_PORT_CONFIGURATION==CONFIG_ENABLE
Std_ReturnType gpio_port_set_direction(port_index_t port, uint8 direction)
{
    Std_ReturnType ret = E_OK;
    if(port > PORT_MAX_NUM - 1)
    {
        ret = E_NOT_OK;
    }
    else
    {
        *tris_registers[port] = direction;
    }

    return ret;
}
#endif

/**
 * @brief Retrieves the direction (input or output) status of an entire GPIO port.
 * 
 * This function reads and returns the direction (input or output) status of an entire GPIO port
 * based on the specified port index. It reads the corresponding TRIS register.
 * 
 * @param port The index of the port to query.
 * @param direction_status Pointer to store the direction status of the port.
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *          - E_OK: The operation was successful.
 *          - E_NOT_OK: An error occurred during the operation.
 */
#if GPIO_PORT_CONFIGURATION==CONFIG_ENABLE
Std_ReturnType gpio_port_get_direction(port_index_t port, uint8 *direction_status)
{
    Std_ReturnType ret = E_OK;

    if(NULL == direction_status || port > PORT_MAX_NUM - 1)
    {
        ret = E_NOT_OK;
    }
    else
    {
        *direction_status = *tris_registers[port];
    }
    return ret;
}
#endif

/**
 * @brief Writes logic levels to an entire GPIO port.
 * 
 * This function writes logic levels to an entire GPIO port by setting the specified logic value
 * to the corresponding LAT register of the port.
 * 
 * @param port The index of the port to write to.
 * @param logic The logic value to write to the port.
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *          - E_OK: The operation was successful.
 *          - E_NOT_OK: An error occurred during the operation.
 */
#if GPIO_PORT_CONFIGURATION==CONFIG_ENABLE
Std_ReturnType gpio_port_write(port_index_t port, uint8 logic)
{
    Std_ReturnType ret = E_OK;

    if(port > PORT_MAX_NUM - 1)
    {
        ret = E_NOT_OK;
    }
    else
    {
        *lat_registers[port] = logic;
    }
    return ret;
}
#endif

/**
 * @brief Reads logic levels from an entire GPIO port.
 * 
 * This function reads and returns the logic levels of an entire GPIO port based on the specified
 * port index. It reads the corresponding LAT register of the port.
 * 
 * @param port The index of the port to read from.
 * @param logic Pointer to store the logic levels of the port.
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *          - E_OK: The operation was successful.
 *          - E_NOT_OK: An error occurred during the operation.
 */
#if GPIO_PORT_CONFIGURATION==CONFIG_ENABLE
Std_ReturnType gpio_port_read(port_index_t port, uint8 *logic)
{
    Std_ReturnType ret = E_OK;

    if(NULL == logic || port > PORT_MAX_NUM - 1)
    {
        ret = E_NOT_OK;
    }
    else
    {
        *logic = *lat_registers[port];
    }
    return ret;
}
#endif

/**
 * @brief Toggles logic levels of an entire GPIO port.
 * 
 * This function toggles (inverts) the logic levels of an entire GPIO port based on the specified
 * port index. It XORs the specified mask with the LAT register of the port to achieve the toggle effect.
 * 
 * @param port The index of the port to toggle.t 
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *          - E_OK: The operation was successful.
 *          - E_NOT_OK: An error occurred during the operation.
 */
#if GPIO_PORT_CONFIGURATION==CONFIG_ENABLE
Std_ReturnType gpio_port_toggle(port_index_t port)
{
    Std_ReturnType ret = E_OK;

    if(port > PORT_MAX_NUM - 1)
    {
        ret = E_NOT_OK;
    }
    else
    {
        *lat_registers[port] ^= PORT_MASK;
    }
    return ret;
}
#endif