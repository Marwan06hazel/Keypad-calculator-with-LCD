/******************************************************************************
 *
 * File Name: LCD.h
 *
 * Author: Marwan Alaa
 *
 * Created on: Oct 7, 2026
 *
 *******************************************************************************/

#ifndef LCD_H_
#define LCD_H_

#include "std_types.h"

/* Number of data lines used to talk to the LCD: 4 (DB4 to DB7) or 8 (DB0 to DB7) */
#define LCD_DATA_BITS_MODE 					4

/* Compile-time check: stop the build if an unsupported mode is selected */
#if((LCD_DATA_BITS_MODE != 8) && (LCD_DATA_BITS_MODE != 4))
#error "Number of Data bits should be equal to 4 or 8"
#endif

/* RS (Register Select): 0 = command register, 1 = data register */
#define LCD_RS_PORT_ID 						PORTA_ID
#define LCD_RS_PIN_ID 						PIN1_ID

/* E (Enable): pulsed to make the LCD latch the data on the bus */
#define LCD_E_PORT_ID 						PORTA_ID
#define LCD_E_PIN_ID 						PIN2_ID

/* Port connected to the LCD data bus */
#define LCD_DATABUS_PORT_ID 				PORTA_ID

/* Data pins used in 4-bit mode (DB4 to DB7).
 * NOTE: the same macro name is defined four times with different values, so only the
 * last definition (PIN6_ID) remains. Each pin needs its own name, or a single start pin. */
#if(LCD_DATA_BITS_MODE == 4)
#define LCD_DATABUS1_PIN_ID 				PIN3_ID
#define LCD_DATABUS2_PIN_ID 				PIN4_ID
#define LCD_DATABUS3_PIN_ID 				PIN5_ID
#define LCD_DATABUS4_PIN_ID 				PIN6_ID
#endif

/* Display control commands */
#define LCD_CLEAR_SCREEN                    0x01
#define LCD_RETURN_HOME                     0x02

/* Entry mode: what happens after each character is written */
#define LCD_CURSOR_LEFT_ON_WRITE            0x04
#define LCD_DISPLAY_SHIFT_RIGHT_ON_WRITE    0x05
#define LCD_CURSOR_RIGHT_ON_WRITE           0x06
#define LCD_DISPLAY_SHIFT_LEFT_ON_WRITE     0x07

/* Display and cursor on/off control */
#define LCD_SET_CURSOR_LOCATION             0x80
#define LCD_DISPLAY_OFF_CURSOR_OFF          0x08
#define LCD_DISPLAY_OFF_CURSOR_ON           0x0A
#define LCD_DISPLAY_ON_CURSOR_OFF           0x0C
#define LCD_DISPLAY_ON_CURSOR_ON            0x0E
#define LCD_DISPLAY_ON_CURSOR_BLINK         0x0F

/* One-time shift of the cursor or of the whole display */
#define LCD_SHIFT_CURSOR_LEFT               0x10
#define LCD_SHIFT_CURSOR_RIGHT              0x14
#define LCD_SHIFT_DISPLAY_LEFT              0x18
#define LCD_SHIFT_DISPLAY_RIGHT             0x1C

/* Cursor position: DDRAM address 0x00 (line 1) and 0x40 (line 2) with bit 7 set */
#define LCD_CURSOR_LINE1_BEGIN              0x80
#define LCD_CURSOR_LINE2_BEGIN              0xC0

/* Function set: 8-bit interface, 2 lines, 5x8 dots */
#define LCD_TWO_LINES_EIGHT_BITS_MODE       0x38
/* Function set: 4-bit interface, 2 lines, 5x8 dots */
#define LCD_TWO_LINES_FOUR_BITS_MODE        0x28
/* 4-bit mode wake-up sequence: sends the nibbles 3, 3 */
#define LCD_TWO_LINES_FOUR_BITS_MODE_INIT1  0x33
/* 4-bit mode wake-up sequence: sends the nibbles 3, 2 (the 2 switches the LCD to 4-bit) */
#define LCD_TWO_LINES_FOUR_BITS_MODE_INIT2  0x32

/* Initialize the LCD pins and send the configuration commands */
void LCD_init(void);
/* Send an instruction to the LCD (RS = 0) */
void LCD_sendCommand(uint8 command);
/* Display one character at the current cursor position (RS = 1) */
void LCD_displayCharacter(uint8 data);
/* Display a null-terminated string starting at the current cursor position */
void LCD_displayString(const char *str);
/* Move the cursor to the given row and column */
void LCD_moveCursor(uint8 row, uint8 col);
/* Move the cursor to the given row and column, then display the string */
void LCD_displayStringRowColumn(uint8 row, uint8 col, const char *str);
/* Clear the screen and return the cursor to the first position */
void LCD_clearScreen(void);
/* Convert an integer to text and display it at the current cursor position */
void LCD_integerToString(uint8 data);
/* Convert a float to text and display it at the current cursor position */
void LCD_floatToString(float32 data);

#endif /* LCD_H_ */
