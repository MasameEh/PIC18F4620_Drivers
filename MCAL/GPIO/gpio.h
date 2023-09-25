/* 
 * File:   gpio.h
 * Author: Mohamed Sameh
 * 
 * Description:
 * This header file provides the interface and data structures for controlling GPIO pins and ports.
 * The functions defined in this file allow setting the direction, writing logic levels, reading logic levels,
 * toggling, and initializing GPIO pins and ports. The header also defines various macros and enumerations
 * for easy manipulation of GPIO pins and ports.
 * 
 * Created on August 22, 2023, 11:09 PM
 */

#ifndef GPIO_H
#define	GPIO_H

/* Section : Includes */
#include <pic18f4620.h>
#include "../std_types.h"
#include "../device_config.h"

/* Section : Macro Declarations */
#define BIT_MASK       (uint8)1

#define PORT_PIN_MAX_NUM    8
#define PORT_MAX_NUM        5
#define PORT_MASK           0xFF

#define GPIO_PORT_PIN_CONFIGURATION   CONFIG_ENABLE
#define GPIO_PORT_CONFIGURATION       CONFIG_ENABLE

/* Section : Macro Functions Declarations */
#define HWREG8(X)      (*((volatile uint8*)(X)))

#define REGISTER_SIZE 8
#define SET_BIT(REG,BIT_POS)    (REG |= (BIT_MASK << BIT_POS))
#define CLR_BIT(REG,BIT_POS)    (REG &= (~(BIT_MASK << BIT_POS)))
#define TOG_BIT(REG,BIT_POS)    (REG ^= (BIT_MASK << BIT_POS))
#define READ_BIT(REG,BIT_POS)    ((REG & (BIT_MASK << BIT_POS)) >> BIT_POS)
#define IS_BIT_SET(REG,BIT_POS)  ((REG & (BIT_MASK << BIT_POS)) >> BIT_POS)
#define IS_BIT_CLR(REG,BIT_POS)  (!((REG & (BIT_MASK << BIT_POS)) >> BIT_POS))

/* Section : Data Types Declarations  */
typedef enum
{
    GPIO_LOW = 0,
    GPIO_HIGH
} logic_t;

typedef enum
{
    GPIO_DIRECTION_OUTPUT = 0,
    GPIO_DIRECTION_INPUT
} direction_t;

typedef enum
{
    GPIO_PIN0 = 0,
    GPIO_PIN1,
    GPIO_PIN2,
    GPIO_PIN3,
    GPIO_PIN4,
    GPIO_PIN5,
    GPIO_PIN6,
    GPIO_PIN7,
} pin_index_t;

typedef enum
{
    PORTA_INDEX = 0,
    PORTB_INDEX,
    PORTC_INDEX,
    PORTD_INDEX,
    PORTE_INDEX,
} port_index_t;

typedef struct 
{
    uint8 port : 3;         // @ref port_index_t
    uint8 pin_num : 3;      // @ref pin_index_t
    uint8 direction : 1;    // @ref direction_t
    uint8 logic : 1;        // @ref logic_t
} pin_config_t;

/* Section : Functions Declarations */
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
Std_ReturnType gpio_pin_set_direction(const pin_config_t *pin);

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
Std_ReturnType gpio_pin_get_direction(const pin_config_t *pin, direction_t *direction_status);

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
Std_ReturnType gpio_pin_write(const pin_config_t *pin, logic_t logic);

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
Std_ReturnType gpio_pin_read(const pin_config_t *pin, logic_t *logic);

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
Std_ReturnType gpio_pin_toggle(const pin_config_t *pin);

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
Std_ReturnType gpio_pin_initialize(const pin_config_t *pin);

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
Std_ReturnType gpio_port_set_direction(port_index_t port, uint8 direction);

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
Std_ReturnType gpio_port_get_direction(port_index_t port, uint8 *direction_status);

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
Std_ReturnType gpio_port_write(port_index_t port, uint8 logic);

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
Std_ReturnType gpio_port_read(port_index_t port, uint8 *logic);

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
Std_ReturnType gpio_port_toggle(port_index_t port);

#endif	/* GPIO_H */

