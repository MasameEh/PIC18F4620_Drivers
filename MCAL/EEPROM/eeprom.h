/* 
 * File:   eeprom.h
 * Author: Mohamed Sameh
 *
 * Created on September 21, 2023, 5:46 PM
 */

#ifndef EEPROM_H
#define	EEPROM_H
/* -------------- Includes -------------- */
#include <pic18f4620.h>
#include "../std_types.h"
#include "../interrupt/internal_interrupt.h"

/* -------------- Macro Declarations ------------- */
#define ACCESS_FLASH_MEMORY             1
#define ACCESS_EEPROM_MEMORY            0

#define ACCESS_FLASH_EEPROM_MEMORY      0
#define ACCESS_CONFIH_REGS              1

#define ALLOW_WRITE_CYCLES     1
#define INHIBITS_WRITE_CYCLES  0

#define INITIATE_EEPROM_DATA_WRITE_ERASE   1
#define EEPROM_DATA_WRITE_ERASE_COMPLETED  0

#define INITIATE_EEPROM_DATA_READ          1

/* -------------- Macro Functions Declarations --------------*/

/* -------------- Data Types Declarations --------------  */

/* -------------- Functions Declarations --------------*/
/**
 * @brief Writes one byte of data to a specific EEPROM Address.
 * 
 * @param bAdd The EEPROM address to write at.
 * @param bData Data to be written.
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The operation was successful.
 *         - E_NOT_OK: An error occurred during the operation.
 */
Std_ReturnType EEPROM_WriteByte(uint16 bAdd, uint8 bData);

/**
 * @brief Reads one byte of data from a specific EEPROM Address.
 * 
 * @param bAdd The EEPROM address to read from..
 * @param bData A pointer to store the read data.
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The operation was successful.
 *         - E_NOT_OK: An error occurred during the operation.
 */
Std_ReturnType EEPROM_ReadByte(uint16 bAdd, uint8 *bData);

#endif	/* EEPROM_H */

