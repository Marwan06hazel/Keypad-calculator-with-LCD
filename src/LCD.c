/******************************************************************************
 *
 * File Name: LCD.c
 *
 * Author: Marwan Alaa
 *
 * Created on: Oct 7, 2026
 *
 *******************************************************************************/

/* LCD driver (HD44780): implementation of the init, command, character and string functions.
 * Works in 4-bit or 8-bit data mode, selected by LCD_DATA_BITS_MODE in lcd.h. */

#include <util/delay.h>
#include "common_macros.h"
#include "lcd.h"
#include "gpio.h"

/*
 * Description:
 * Configure the LCD control and data pins as outputs, then send the
 * initialization commands.
 */
void LCD_init(void){
	/* Control pins: RS and E are driven by the MCU */
	GPIO_setupPinDirection(LCD_RS_PORT_ID, LCD_RS_PIN_ID, PIN_OUTPUT);
	GPIO_setupPinDirection(LCD_E_PORT_ID,  LCD_E_PIN_ID,  PIN_OUTPUT);

	/* NOTE: only the 4-bit mode is initialized below. In 8-bit mode, the data port direction
	 * and the function set command (LCD_TWO_LINES_EIGHT_BITS_MODE) are never sent. */
#if(LCD_DATA_BITS_MODE == 4)
	/* Data pins DB4 to DB7 */
	GPIO_setupPinDirection(LCD_DATABUS_PORT_ID, LCD_DATABUS1_PIN_ID, PIN_OUTPUT);
	GPIO_setupPinDirection(LCD_DATABUS_PORT_ID, LCD_DATABUS2_PIN_ID, PIN_OUTPUT);
	GPIO_setupPinDirection(LCD_DATABUS_PORT_ID, LCD_DATABUS3_PIN_ID, PIN_OUTPUT);
	GPIO_setupPinDirection(LCD_DATABUS_PORT_ID, LCD_DATABUS4_PIN_ID, PIN_OUTPUT);

	/* Send for 4 bit initialization of LCD  */
	LCD_sendCommand(LCD_TWO_LINES_FOUR_BITS_MODE_INIT1);
	LCD_sendCommand(LCD_TWO_LINES_FOUR_BITS_MODE_INIT2);

	/* use 2-lines LCD + 4-bits Data Mode + 5*7 dot display Mode */
	LCD_sendCommand(LCD_TWO_LINES_FOUR_BITS_MODE);

#elif(LCD_DATA_BITS_MODE == 8)
	/* Configure the data port as output port */
	GPIO_setupPortDirection(LCD_DATABUS_PORT_ID,PORT_OUTPUT);

	/* use 2-lines LCD + 8-bits Data Mode + 5*7 dot display Mode */
	LCD_sendCommand(LCD_TWO_LINES_EIGHT_BITS_MODE);
#endif
	/* Clear the screen, then set the display and cursor state.
	 * NOTE: LCD_DISPLAY_OFF_CURSOR_OFF (0x08) turns the display OFF, so nothing is shown.
	 * LCD_DISPLAY_ON_CURSOR_OFF (0x0C) is needed to see the text. */
	LCD_sendCommand(LCD_DISPLAY_ON_CURSOR_OFF);
	LCD_sendCommand(LCD_CLEAR_SCREEN);
}

/*
 * Description:
 * Send an instruction byte to the LCD (RS = 0).
 * 4-bit mode: the upper nibble is sent first, then the lower nibble.
 * Each nibble is placed on the data pins while E is high, and is latched
 * by the LCD on the falling edge of E.
 */
void LCD_sendCommand(uint8 command){
	/* RS = 0: select the instruction register */
	GPIO_writePin(LCD_RS_PORT_ID, LCD_RS_PIN_ID, LOGIC_LOW);
	_delay_ms(1);
	/* Start the enable pulse */
	GPIO_writePin(LCD_E_PORT_ID, LCD_E_PIN_ID, LOGIC_HIGH);
	_delay_ms(1);

#if(LCD_DATA_BITS_MODE == 4)
	/* Upper nibble (bits 7 to 4) on DB4 to DB7 */
	GPIO_writePin(LCD_DATABUS_PORT_ID, LCD_DATABUS1_PIN_ID, GET_BIT(command, 4));
	GPIO_writePin(LCD_DATABUS_PORT_ID, LCD_DATABUS2_PIN_ID, GET_BIT(command, 5));
	GPIO_writePin(LCD_DATABUS_PORT_ID, LCD_DATABUS3_PIN_ID, GET_BIT(command, 6));
	GPIO_writePin(LCD_DATABUS_PORT_ID, LCD_DATABUS4_PIN_ID, GET_BIT(command, 7));

	/* E falling edge: the LCD latches the upper nibble */
	_delay_ms(1);
	GPIO_writePin(LCD_E_PORT_ID, LCD_E_PIN_ID, LOGIC_LOW);
	_delay_ms(1);
	/* Second enable pulse for the lower nibble */
	GPIO_writePin(LCD_E_PORT_ID, LCD_E_PIN_ID, LOGIC_HIGH);
	_delay_ms(1);

	/* Lower nibble (bits 3 to 0) on DB4 to DB7 */
	GPIO_writePin(LCD_DATABUS_PORT_ID, LCD_DATABUS1_PIN_ID, GET_BIT(command, 0));
	GPIO_writePin(LCD_DATABUS_PORT_ID, LCD_DATABUS2_PIN_ID, GET_BIT(command, 1));
	GPIO_writePin(LCD_DATABUS_PORT_ID, LCD_DATABUS3_PIN_ID, GET_BIT(command, 2));
	GPIO_writePin(LCD_DATABUS_PORT_ID, LCD_DATABUS4_PIN_ID, GET_BIT(command, 3));

	/* E falling edge: the LCD latches the lower nibble */
	_delay_ms(1);
	GPIO_writePin(LCD_E_PORT_ID, LCD_E_PIN_ID, LOGIC_LOW);
	_delay_ms(1);
#endif

#if(LCD_DATA_BITS_MODE == 8)
	/* 8-bit mode: the whole byte is written to the data port in one transfer */
	GPIO_portWrite(LCD_DATABUS_PORT_ID, command);

	/* E falling edge: the LCD latches the byte */
	_delay_ms(1);
	GPIO_pinWrite(LCD_E_PORT_ID, LCD_E_PIN_ID, LOGIC_LOW);
	_delay_ms(1);

#endif
}

/*
 * Description:
 * Display one character at the cursor position (RS = 1).
 * The transfer sequence is the same as LCD_sendCommand, but through the data register.
 */
void LCD_displayCharacter(uint8 data){
	/* RS = 1: select the data register */
	GPIO_writePin(LCD_RS_PORT_ID, LCD_RS_PIN_ID, LOGIC_HIGH);
	_delay_ms(1);
	/* Start the enable pulse */
	GPIO_writePin(LCD_E_PORT_ID, LCD_E_PIN_ID, LOGIC_HIGH);
	_delay_ms(1);

#if(LCD_DATA_BITS_MODE == 4)
	/* Upper nibble (bits 7 to 4) on DB4 to DB7 */
	GPIO_writePin(LCD_DATABUS_PORT_ID, LCD_DATABUS1_PIN_ID, GET_BIT(data, 4));
	GPIO_writePin(LCD_DATABUS_PORT_ID, LCD_DATABUS2_PIN_ID, GET_BIT(data, 5));
	GPIO_writePin(LCD_DATABUS_PORT_ID, LCD_DATABUS3_PIN_ID, GET_BIT(data, 6));
	GPIO_writePin(LCD_DATABUS_PORT_ID, LCD_DATABUS4_PIN_ID, GET_BIT(data, 7));

	/* E falling edge: the LCD latches the upper nibble */
	_delay_ms(1);
	GPIO_writePin(LCD_E_PORT_ID, LCD_E_PIN_ID, LOGIC_LOW);
	_delay_ms(1);
	/* Second enable pulse for the lower nibble */
	GPIO_writePin(LCD_E_PORT_ID, LCD_E_PIN_ID, LOGIC_HIGH);
	_delay_ms(1);

	/* Lower nibble (bits 3 to 0) on DB4 to DB7 */
	GPIO_writePin(LCD_DATABUS_PORT_ID, LCD_DATABUS1_PIN_ID, GET_BIT(data, 0));
	GPIO_writePin(LCD_DATABUS_PORT_ID, LCD_DATABUS2_PIN_ID, GET_BIT(data, 1));
	GPIO_writePin(LCD_DATABUS_PORT_ID, LCD_DATABUS3_PIN_ID, GET_BIT(data, 2));
	GPIO_writePin(LCD_DATABUS_PORT_ID, LCD_DATABUS4_PIN_ID, GET_BIT(data, 3));

	/* E falling edge: the LCD latches the lower nibble */
	_delay_ms(1);
	GPIO_writePin(LCD_E_PORT_ID, LCD_E_PIN_ID, LOGIC_LOW);
	_delay_ms(1);
#endif

#if(LCD_DATA_BITS_MODE == 8)
	/* 8-bit mode: the whole byte is written to the data port in one transfer */
	GPIO_writePort(LCD_DATABUS_PORT_ID, data);

	/* E falling edge: the LCD latches the byte */
	_delay_ms(1);
	GPIO_writePin(LCD_E_PORT_ID, LCD_E_PIN_ID, LOGIC_LOW);
	_delay_ms(1);

#endif
}

/*
 * Description:
 * Display a null-terminated string, one character at a time,
 * starting at the current cursor position.
 */
void LCD_displayString(const char *str){
	uint8 i = 0;

	/* Stop at the null terminator */
	while(str[i] != '\0'){
		LCD_displayCharacter(str[i]);
		i++;
	}
}

/*
 * Description:
 * Move the cursor to a given row and column by sending the
 * "Set DDRAM address" command: command = 0x80 | DDRAM address.
 * NOTE: LCD_SET_CURSOR_LOCATION is not defined in lcd.h (its value is 0x80).
 * NOTE: for any row other than 0 to 3, lcd_memory_address is never assigned.
 */
void LCD_moveCursor(uint8 row, uint8 col){
	uint8 lcd_memory_address;

	/* Start address of each line in DDRAM plus the column.
	 * Rows 2 and 3 use the 16x4 addresses (a 20x4 LCD uses 0x14 and 0x54). */
	switch(row){
	case 0:
		lcd_memory_address = col;
		break;
	case 1:
		lcd_memory_address = col + 0x40;
		break;
	case 2:
		lcd_memory_address = col + 0x10;
		break;
	case 3:
		lcd_memory_address = col + 0x50;
		break;
	}

	LCD_sendCommand(LCD_SET_CURSOR_LOCATION | lcd_memory_address);
}

/*
 * Description:
 * Display a string starting at a given row and column.
 */
void LCD_displayStringRowColumn(uint8 row, uint8 col, const char *str){
	LCD_moveCursor(row, col);
	LCD_displayString(str);
}

/*
 * Description:
 * Clear the screen and return the cursor to the first position.
 */
void LCD_clearScreen(void){
	LCD_sendCommand(LCD_CLEAR_SCREEN);
}

/*
 * Description:
 * Convert an integer to text and display it at the current cursor position.
 * NOTE: itoa is declared in <stdlib.h>, which is not included in this file.
 */
void LCD_integerToString(uint8 data){
	/* Buffer for the converted text (a 16-bit int needs at most 7 characters including the terminator) */
	char buff[16];
	/*
	 * value: the number to convert.
	 * buffer: a char array that receives the text.
	 * base: 10 for decimal, 2 for binary, 16 for hex.
	 */
	itoa(data,buff,10);
	LCD_displayString(buff);
}

/*
 * Description:
 * Convert a float to text and display it at the current cursor position.
 */
void LCD_floatToString(float32 data){
	char buff[16];
	dtostrf(data, 1, 2, buff);
	LCD_displayString(buff);
}
