/******************************************************************************
 *
 * File Name: keypad.h
 *
 * Description: Commonly used bit-manipulation macros for 8-bit registers
 *
 * Author: Marwan Alaa
 *
 * Created on: Oct 4, 2026
 *
 *******************************************************************************/

#ifndef KEYPAD_H_
#define KEYPAD_H_

/* std_types.h provides uint8 and the logic levels LOGIC_HIGH / LOGIC_LOW */
#include "std_types.h"

/* Keypad size: 4 rows and 4 columns (set the columns to 3 for a 4x3 keypad) */
#define KEYPAD_COLS_NUMBER 											4
#define KEYPAD_ROWS_NUMBER 											4

#define KEYPAD_DEBOUNCE_MS											20

/* Rows: connected to consecutive pins of PORTB, starting at pin 4 (PB4 to PB7) */
#define KEYPAD_ROW_PORT_ID 											PORTB_ID
#define KEYPAD_ROW_PIN_ID 											PIN4_ID

/* Columns: connected to consecutive pins of PORTD, starting at pin 2 (PD2 to PD5) */
#define KEYPAD_COL_PORT_ID 											PORTD_ID
#define KEYPAD_COL_PIN_ID 											PIN2_ID

/* Logic levels of a key: a pressed key pulls its column low (the active row is low, columns idle high) */
#define KEYPAD_BUTTON_PRESSED 										LOGIC_LOW
#define KEYPAD_BUTTON_RELEASED 										LOGIC_HIGH

/* Key number counted row by row starting from 1: (columns * row) + (col + 1).
 * NOTE: row and col are not wrapped in parentheses, and the first parameter has the
 * same name as the KEYPAD_COLS_NUMBER macro.
*/
#define KEYPAD_CALCULATE_KEY_NUMBER(KEYPAD_COLS_NUMBER, row, col) 	(((KEYPAD_COLS_NUMBER*row)+(col+1)))

/* Block until a key is pressed, then return its value */
uint8 KEYPAD_getPressedKey(void);
/* Convert a key number to the value on the key.
 * NOTE: static prototypes in a header expect a definition in every file that includes it,
 * and keypad.c defines only one of the two (chosen by KEYPAD_COLS_NUMBER).
*/
static uint8 KEYPAD_adjustKeyNumber(uint8 button_number);

#endif /* KEYPAD_H_ */
