/* 
 * File:   chr_lcd.c
 * Author: Mohamed Sameh
 * Description:
 * Source file for controlling a character LCD using a 4-bit interface and a 8-bit interface.
 * Created on September 2, 2023, 2:51 AM
 */

#include "chr_lcd.h"

static Std_ReturnType lcd_send_4bits(const lcd_4bit_t *lcd, uint8 _data_cmd);
static Std_ReturnType lcd_4bits_send_enable_signal(const lcd_4bit_t *lcd);
static Std_ReturnType lcd_8bits_send_enable_signal(const lcd_8bit_t *lcd);
static Std_ReturnType lcd_8bit_set_cursor(const lcd_8bit_t *lcd, uint8 row, uint8 column);
static Std_ReturnType lcd_4bit_set_cursor(const lcd_4bit_t *lcd, uint8 row, uint8 column);

/**
 * @brief Initializes a 4-bit mode character LCD.
 * 
 * @param lcd A pointer to the LCD configuration structure.
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The operation was successful.
 *         - E_NOT_OK: An error occurred during initialization.
 */
Std_ReturnType lcd_4bit_init(const lcd_4bit_t *lcd)
{
    Std_ReturnType ret = E_OK;
    uint8 pins_counter = ZERO_INIT;

    if(NULL == lcd)
    {
        ret = E_NOT_OK;
    }
    else
    {   
       ret = gpio_pin_initialize(&(lcd->lcd_rs)); 
       ret = gpio_pin_initialize(&(lcd->lcd_en)); 
       for (pins_counter = ZERO_INIT; pins_counter < 4; pins_counter++)
       {
            ret = gpio_pin_initialize(&(lcd->lcd_data[pins_counter]));
       }
        __delay_ms(20);
         ret = lcd_4bit_send_cmd(lcd , LCD_8BIT_MODE_2_LINES);
        __delay_ms(5);
        ret = lcd_4bit_send_cmd(lcd , LCD_8BIT_MODE_2_LINES);
        __delay_us(120);
        ret = lcd_4bit_send_cmd(lcd , LCD_8BIT_MODE_2_LINES);
        
        ret = lcd_4bit_send_cmd(lcd , LCD_CLEAR);
        __delay_ms(1);
        ret = lcd_4bit_send_cmd(lcd , LCD_RETURN_HOME);
        ret = lcd_4bit_send_cmd(lcd , LCD_ENTRY_MODE);
        ret = lcd_4bit_send_cmd(lcd , LCD_CURSOR_OFF_DISPLAY_ON);
        ret = lcd_4bit_send_cmd(lcd , LCD_4BIT_MODE_2_LINES);
        ret = lcd_4bit_send_cmd(lcd , 0x80); 
    }
    return ret;
}

/**
 * @brief Sends a command to the LCD in 4-bit mode.
 * 
 * @param lcd A pointer to the LCD configuration structure.
 * @param cmd The command to send.
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The command was sent successfully.
 *         - E_NOT_OK: An error occurred during the operation.
 */
Std_ReturnType lcd_4bit_send_cmd(const lcd_4bit_t *lcd, uint8 cmd)
{
    Std_ReturnType ret = E_OK;

    if(NULL == lcd)
    {
        ret = E_NOT_OK;
    }
    else
    {   
        ret = gpio_pin_write(&(lcd->lcd_rs), GPIO_LOW);

        ret = lcd_send_4bits(lcd, cmd >> 4);
        ret = lcd_4bits_send_enable_signal(lcd);

        ret = lcd_send_4bits(lcd, cmd);
        ret = lcd_4bits_send_enable_signal(lcd);
    }
    return ret;

}

/**
 * @brief Sends a character to display on the LCD in 4-bit mode.
 * 
 * @param lcd A pointer to the LCD configuration structure.
 * @param data The character to display.
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The character was sent successfully.
 *         - E_NOT_OK: An error occurred during the operation.
 */
Std_ReturnType lcd_4bit_send_char(const lcd_4bit_t *lcd, uint8 data)
{
    Std_ReturnType ret = E_OK;

    if(NULL == lcd)
    {
        ret = E_NOT_OK;
    }
    else
    {   
        ret = gpio_pin_write(&(lcd->lcd_rs), GPIO_HIGH);

        ret = lcd_send_4bits(lcd, data >> 4);
        ret = lcd_4bits_send_enable_signal(lcd);

        ret = lcd_send_4bits(lcd, data);
        ret = lcd_4bits_send_enable_signal(lcd);
    }
    return ret;

}

/**
 * @brief Sets the cursor position on the LCD in 4-bit mode.
 * 
 * @param lcd A pointer to the LCD configuration structure.
 * @param row The row where the cursor should be positioned (1 or 2 or 3 or 4 for a 4x20 LCD).
 * @param column The column where the cursor should be positioned (1 to 20).
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The cursor position was set successfully.
 *         - E_NOT_OK: An error occurred during the operation.
 */
Std_ReturnType lcd_4bit_send_char_pos(const lcd_4bit_t *lcd, uint8 data, uint8 row, uint8 column)
{
    Std_ReturnType ret = E_OK;

    if(NULL == lcd)
    {
        ret = E_NOT_OK;
    }
    else
    {   
        ret =lcd_4bit_set_cursor(lcd, row, column);
        ret =lcd_4bit_send_char(lcd, data);
    }
    return ret;
}

/**
 * @brief Sends a string to display on the LCD in 4-bit mode.
 * 
 * @param lcd A pointer to the LCD configuration structure.
 * @param str The string to display.
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The string was sent and displayed successfully.
 *         - E_NOT_OK: An error occurred during the operation.
 */
Std_ReturnType lcd_4bit_send_string(const lcd_4bit_t *lcd, uint8 *str)
{
    Std_ReturnType ret = E_OK;

    if(NULL == lcd || NULL == str)
    {
        ret = E_NOT_OK;
    }
    else
    {   
        while (*str)
        {
            ret = lcd_4bit_send_char(lcd, *str++);
        }
    }
    return ret;
}

/**
 * @brief Sends a string to display on the LCD in 4-bit mode at a specified position.
 * 
 * @param lcd A pointer to the LCD configuration structure.
 * @param str The string to display.
 * @param row The row where the string should be displayed (1 or 2 for a 2x16 LCD).
 * @param column The column where the string should start (1 to 16).
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The string was sent and displayed successfully.
 *         - E_NOT_OK: An error occurred during the operation.
 */
Std_ReturnType lcd_4bit_send_string_pos(const lcd_4bit_t *lcd, uint8 *str, uint8 row, uint8 column)
{
    Std_ReturnType ret = E_OK;

    if(NULL == lcd || NULL == str)
    {
        ret = E_NOT_OK;
    }
    else
    {   
        ret =lcd_4bit_set_cursor(lcd, row, column);
        ret = lcd_4bit_send_string(lcd, str);
    }
    return ret;
}

/**
 * @brief Sends a custom character to the LCD in 4-bit mode.
 * 
 * @param lcd A pointer to the LCD configuration structure.
 * @param chr An array representing the custom character (must be 8 bytes).
 * @param row The row where the custom character should be displayed (1 or 2 for a 2x16 LCD).
 * @param column The column where the custom character should be displayed (1 to 16).
 * @param mem_pos The memory position where the custom character should be stored (0 to 7).
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The custom character was sent and displayed successfully.
 *         - E_NOT_OK: An error occurred during the operation.
 */
Std_ReturnType lcd_4bit_send_custom_char(const lcd_4bit_t *lcd, const uint8 chr[], 
                                        uint8 row, uint8 column, uint8 mem_pos)
{
    Std_ReturnType ret = E_OK;
    uint8 l_counter = ZERO_INIT;

    if(NULL == lcd)
    {
        ret = E_NOT_OK;
    }
    else
    {   
        ret = lcd_4bit_send_cmd(lcd, LCD_CGRAM_START + (mem_pos*8));
        for (l_counter = ZERO_INIT; l_counter < 7; l_counter++)
        {
            ret = lcd_4bit_send_char(lcd, chr[l_counter]);
        }
        ret = lcd_4bit_send_char_pos(lcd , mem_pos, row, column);
    }
    return ret;
}

/**
 * @brief Initializes a 8-bit mode character LCD.
 * 
 * @param lcd A pointer to the LCD configuration structure.
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The operation was successful.
 *         - E_NOT_OK: An error occurred during initialization.
 */
Std_ReturnType lcd_8bit_init(const lcd_8bit_t *lcd)
{
    Std_ReturnType ret = E_OK;
    uint8 pins_counter = ZERO_INIT;

    if(NULL == lcd)
    {
        ret = E_NOT_OK;
    }
    else
    {   
        ret = gpio_pin_initialize(&(lcd->lcd_rs)); 
        ret = gpio_pin_initialize(&(lcd->lcd_en)); 
        for (pins_counter = ZERO_INIT; pins_counter < 8; pins_counter++)
        {
            gpio_pin_initialize(&(lcd->lcd_data[pins_counter]));
        }
        __delay_ms(20);
         ret = lcd_8bit_send_cmd(lcd , LCD_8BIT_MODE_2_LINES);
        __delay_ms(5);
        ret = lcd_8bit_send_cmd(lcd , LCD_8BIT_MODE_2_LINES);
        __delay_us(120);
        ret = lcd_8bit_send_cmd(lcd , LCD_8BIT_MODE_2_LINES);
        
        ret = lcd_8bit_send_cmd(lcd , LCD_CLEAR);
        __delay_ms(1);
        ret = lcd_8bit_send_cmd(lcd , LCD_RETURN_HOME);
        ret = lcd_8bit_send_cmd(lcd , LCD_ENTRY_MODE);
        ret = lcd_8bit_send_cmd(lcd , LCD_CURSOR_OFF_DISPLAY_ON);
        ret = lcd_8bit_send_cmd(lcd , 0x80);
    }
    return ret;
}

/**
 * @brief Sends a command to the LCD in 8-bit mode.
 * 
 * @param lcd A pointer to the LCD configuration structure.
 * @param cmd The command to send.
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The command was sent successfully.
 *         - E_NOT_OK: An error occurred during the operation.
 */
Std_ReturnType lcd_8bit_send_cmd(const lcd_8bit_t *lcd, uint8 cmd)
{
    Std_ReturnType ret = E_OK;
    uint8 pins_counter = ZERO_INIT;

    if(NULL == lcd)
    {
        ret = E_NOT_OK;
    }
    else
    {   
        ret = gpio_pin_write(&(lcd->lcd_rs), GPIO_LOW);
        for (pins_counter = ZERO_INIT; pins_counter < 8; pins_counter++)
        {
            gpio_pin_write(&(lcd->lcd_data[pins_counter]), (cmd >> pins_counter) & 0x01);
        }
        ret = lcd_8bits_send_enable_signal(lcd);
    }
    return ret;
    
}

/**
 * @brief Sends a character to display on the LCD in 8-bit mode.
 * 
 * @param lcd A pointer to the LCD configuration structure.
 * @param data The character to display.
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The character was sent successfully.
 *         - E_NOT_OK: An error occurred during the operation.
 */
Std_ReturnType lcd_8bit_send_char(const lcd_8bit_t *lcd, uint8 data)
{
    Std_ReturnType ret = E_OK;
    uint8 pins_counter = ZERO_INIT;

    if(NULL == lcd)
    {
        ret = E_NOT_OK;
    }
    else
    {   
        ret = gpio_pin_write(&(lcd->lcd_rs), GPIO_HIGH);
        for (pins_counter = ZERO_INIT; pins_counter < 8; pins_counter++)
        {
            gpio_pin_write(&(lcd->lcd_data[pins_counter]), (data >> pins_counter) & 0x01);
        }
        ret = lcd_8bits_send_enable_signal(lcd);     
    }
    return ret;
}

/**
 * @brief Sets the cursor position on the LCD in 8-bit mode.
 * 
 * @param lcd A pointer to the LCD configuration structure.
 * @param row The row where the cursor should be positioned (1 or 2 or 3 or 4 for a 4x20 LCD).
 * @param column The column where the cursor should be positioned (1 to 20).
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The cursor position was set successfully.
 *         - E_NOT_OK: An error occurred during the operation.
 */
Std_ReturnType lcd_8bit_send_char_pos(const lcd_8bit_t *lcd, uint8 data, uint8 row, uint8 column)
{
    Std_ReturnType ret = E_OK;

    if(NULL == lcd)
    {
        ret = E_NOT_OK;
    }
    else
    {   
        ret =lcd_8bit_set_cursor(lcd, row, column);
        ret =lcd_8bit_send_char(lcd, data);
    }
    return ret;
}

/**
 * @brief Sends a string to display on the LCD in 8-bit mode.
 * 
 * @param lcd A pointer to the LCD configuration structure.
 * @param str The string to display.
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The string was sent and displayed successfully.
 *         - E_NOT_OK: An error occurred during the operation.
 */
Std_ReturnType lcd_8bit_send_string(const lcd_8bit_t *lcd, uint8 *str)
{
    Std_ReturnType ret = E_OK;

    if(NULL == lcd || NULL == str)
    {
        ret = E_NOT_OK;
    }
    else
    {   
        while (*str)
        {
            ret = lcd_8bit_send_char(lcd, *str++);
        } 
    }
    return ret;
}

/**
 * @brief Sends a string to display on the LCD in 48-bit mode at a specified position.
 * 
 * @param lcd A pointer to the LCD configuration structure.
 * @param str The string to display.
 * @param row The row where the string should be displayed (1 or 2 for a 2x16 LCD).
 * @param column The column where the string should start (1 to 16).
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The string was sent and displayed successfully.
 *         - E_NOT_OK: An error occurred during the operation.
 */
Std_ReturnType lcd_8bit_send_string_pos(const lcd_8bit_t *lcd, uint8 *str, uint8 row, uint8 column)
{
    Std_ReturnType ret = E_OK;

    if(NULL == lcd || NULL == str)
    {
        ret = E_NOT_OK;
    }
    else
    {   
        ret =lcd_8bit_set_cursor(lcd, row, column);
        ret = lcd_8bit_send_string(lcd, str);
    }
    return ret;
}

/**
 * @brief Sends a custom character to the LCD in 8-bit mode.
 * 
 * @param lcd A pointer to the LCD configuration structure.
 * @param chr An array representing the custom character (must be 8 bytes).
 * @param row The row where the custom character should be displayed (1 or 2 for a 2x16 LCD).
 * @param column The column where the custom character should be displayed (1 to 16).
 * @param mem_pos The memory position where the custom character should be stored (0 to 7).
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The custom character was sent and displayed successfully.
 *         - E_NOT_OK: An error occurred during the operation.
 */
Std_ReturnType lcd_8bit_send_custom_char(const lcd_8bit_t *lcd, const uint8 chr[], 
                                        uint8 row, uint8 column, uint8 mem_pos)
{
    Std_ReturnType ret = E_OK;
    uint8 l_counter = ZERO_INIT;

    if(NULL == lcd)
    {
        ret = E_NOT_OK;
    }
    else
    {   
        ret = lcd_8bit_send_cmd(lcd, LCD_CGRAM_START + (mem_pos*8));
        for (l_counter = ZERO_INIT; l_counter < 7; l_counter++)
        {
            ret = lcd_8bit_send_char(lcd, chr[l_counter]);
        }
        ret = lcd_8bit_send_char_pos(lcd , mem_pos, row, column);
    }
    return ret;
}

/**
 * @brief Converts an unsigned 8-bit integer to a string.
 * 
 * @param value The value to convert.
 * @param str A pointer to the destination string where the converted value will be stored.
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The conversion was successful.
 *         - E_NOT_OK: An error occurred during the operation.
 */
Std_ReturnType convert_uint8_to_string(uint8 value, uint8 *str)
{
    Std_ReturnType ret = E_OK;

    if(NULL == str)
    {
        ret = E_NOT_OK;
    }
    else
    {   
        memset(str, '\0', 4);
        sprintf(str, "%u", value);
    }
    return ret;
}

/**
 * @brief Converts an unsigned 16-bit integer to a string.
 * 
 * @param value The value to convert.
 * @param str A pointer to the destination string where the converted value will be stored.
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The conversion was successful.
 *         - E_NOT_OK: An error occurred during the operation.
 */
Std_ReturnType convert_uint16_to_string(uint16 value, uint8 *str)
{
    Std_ReturnType ret = E_OK;
    uint8 tempStr[6] = {0};
    uint8 dataCounter = 0;

    if(NULL == str)
    {
        ret = E_NOT_OK;
    }
    else
    {   
        memset(str, ' ', 5);
        str[5] = '\0';

        sprintf(tempStr, "%u", value);
        while (tempStr[dataCounter] != '\0')
        {
            str[dataCounter] = tempStr[dataCounter];
            dataCounter++;
        }
             
    }
    return ret;
}

/**
 * @brief Converts an unsigned 32-bit integer to a string.
 * 
 * @param value The value to convert.
 * @param str A pointer to the destination string where the converted value will be stored.
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The conversion was successful.
 *         - E_NOT_OK: An error occurred during the operation.
 */
Std_ReturnType convert_uint32_to_string(uint32 value, uint16 *str)
{
    Std_ReturnType ret = E_OK;

    if(NULL == str)
    {
        ret = E_NOT_OK;
    }
    else
    {   
        memset(str, '\0', 11);
        sprintf(str, "%u", value);
    }
    return ret;
}

/**
 * @brief Sends 4 bits of data to the LCD.
 * 
 * @param lcd A pointer to the LCD configuration structure.
 * @param _data_cmd The 4 bits of data to send.
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The data was sent successfully.
 *         - E_NOT_OK: An error occurred during the operation.
 */
static Std_ReturnType lcd_send_4bits(const lcd_4bit_t *lcd, uint8 _data_cmd)
{
    Std_ReturnType ret = E_OK;

    ret = gpio_pin_write(&(lcd->lcd_data[0]), (_data_cmd >> 0) & 0x01);
    ret = gpio_pin_write(&(lcd->lcd_data[1]), (_data_cmd >> 1) & 0x01);
    ret = gpio_pin_write(&(lcd->lcd_data[2]), (_data_cmd >> 2) & 0x01);
    ret = gpio_pin_write(&(lcd->lcd_data[3]), (_data_cmd >> 3) & 0x01); 
    
    return ret;
}

/**
 * @brief Sends the enable signal to the LCD in 4-bit mode.
 * 
 * @param lcd A pointer to the LCD configuration structure.
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The enable signal was sent successfully.
 *         - E_NOT_OK: An error occurred during the operation.
 */
static Std_ReturnType lcd_4bits_send_enable_signal(const lcd_4bit_t *lcd)
{
    Std_ReturnType ret = E_OK;

    ret = gpio_pin_write(&(lcd->lcd_en), GPIO_HIGH);
    __delay_us(5);
    ret = gpio_pin_write(&(lcd->lcd_en), GPIO_LOW);

    return ret;
}

/**
 * @brief Sends the enable signal to the LCD in 8-bit mode.
 * 
 * @param lcd A pointer to the LCD configuration structure.
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The enable signal was sent successfully.
 *         - E_NOT_OK: An error occurred during the operation.
 */
static Std_ReturnType lcd_8bits_send_enable_signal(const lcd_8bit_t *lcd)
{
    Std_ReturnType ret = E_OK;

    ret = gpio_pin_write(&(lcd->lcd_en), GPIO_HIGH);
    __delay_us(5);
    ret = gpio_pin_write(&(lcd->lcd_en), GPIO_LOW);

    return ret;
}

/**
 * @brief Sets the cursor position on the LCD in 8-bit mode.
 * 
 * @param lcd A pointer to the LCD configuration structure.
 * @param row The row where the cursor should be positioned (1 to 4).
 * @param column The column where the cursor should be positioned (1 to 20).
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The cursor position was set successfully.
 *         - E_NOT_OK: An error occurred during the operation.
 */
static Std_ReturnType lcd_8bit_set_cursor(const lcd_8bit_t *lcd, uint8 row, uint8 column)
{
    Std_ReturnType ret = E_OK;
    column--;
    switch (row)
    {
    case ROW1:
            ret = lcd_8bit_send_cmd(lcd , (0x80 + column));
        break;
    case ROW2:
            ret = lcd_8bit_send_cmd(lcd , (0xC0 + column));
        break;
    case ROW3:
            ret = lcd_8bit_send_cmd(lcd , (0x94 + column));
        break;
    case ROW4:
            ret = lcd_8bit_send_cmd(lcd , (0xD4 + column));
        break;        
    default:
        break;
    }
    return ret;
}

/**
 * @brief Sets the cursor position on the LCD in 4-bit mode.
 * 
 * @param lcd A pointer to the LCD configuration structure.
 * @param row The row where the cursor should be positioned (1 to 4).
 * @param column The column where the cursor should be positioned (1 to 20).
 * @return Std_ReturnType A status indicating the success or failure of the operation.
 *         - E_OK: The cursor position was set successfully.
 *         - E_NOT_OK: An error occurred during the operation.
 */
static Std_ReturnType lcd_4bit_set_cursor(const lcd_4bit_t *lcd, uint8 row, uint8 column)
{
    Std_ReturnType ret = E_OK;
    column--;
    switch (row)
    {
    case ROW1:
            ret = lcd_4bit_send_cmd(lcd , (0x80 + column));
        break;
    case ROW2:
            ret = lcd_4bit_send_cmd(lcd , (0xC0 + column));
        break;
    case ROW3:
            ret = lcd_4bit_send_cmd(lcd , (0x94 + column));
        break;
    case ROW4:
            ret = lcd_4bit_send_cmd(lcd , (0xD4 + column));
        break;        
    default:
        break;
    }
    return ret;
}