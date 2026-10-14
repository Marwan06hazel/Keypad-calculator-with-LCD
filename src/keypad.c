/******************************************************************************
 *
 * File Name: keypad.c
 *
 * Description: Commonly used bit-manipulation macros for 8-bit registers
 *
 * Author: Marwan Alaa
 *
 * Created on: Oct 4, 2026
 *
 *******************************************************************************/

/* Keypad driver: scans a 4x4 or 4x3 matrix keypad and returns the pressed key.
 * Rows are driven as outputs (one at a time) and columns are read as inputs. */

#include <util/delay.h>
#include "keypad.h"
#include "GPIO.h"

/*
 * Description:
 * Wait (blocking) until a key is pressed, then return its value.
 * Scan method: each row is driven low one at a time, and the columns are read.
 * A column reading low means the key at (active row, that column) is pressed.
 */
uint8 KEYPAD_getPressedKey(void){
	uint8 row, col, key;
	/* Configure all rows and columns as inputs before scanning */
	for(row=0; row<KEYPAD_ROWS_NUMBER; row++){
		GPIO_setupPinDirection(KEYPAD_ROW_PORT_ID, KEYPAD_ROW_PIN_ID+row, PIN_INPUT);
	}

	for(col=0; col<KEYPAD_COLS_NUMBER; col++){
		GPIO_setupPinDirection(KEYPAD_COL_PORT_ID, KEYPAD_COL_PIN_ID+col, PIN_INPUT);
		GPIO_writePin(KEYPAD_COL_PORT_ID, KEYPAD_COL_PIN_ID+col, KEYPAD_BUTTON_RELEASED);  /* PORT=1, DDR=0 -> pull-up */
	}

	/* Keep scanning until a key press is found */
	for(;;){
		for(row=0; row<KEYPAD_ROWS_NUMBER; row++){
			/* Activate the current row: preload the pressed level, then make the pin an output */
			GPIO_writePin(KEYPAD_ROW_PORT_ID, KEYPAD_ROW_PIN_ID+row, KEYPAD_BUTTON_PRESSED);
			GPIO_setupPinDirection(KEYPAD_ROW_PORT_ID, KEYPAD_ROW_PIN_ID+row, PIN_OUTPUT);

			/* NOTE: the loop condition tests 'row' instead of 'col', so the loop is not bounded by the number of columns */
			for(col=0; col<KEYPAD_COLS_NUMBER; col++){
				/* NOTE: this reads the ROW port/pin; a column read should use KEYPAD_COL_PORT_ID and KEYPAD_COL_PIN_ID+col */
				if(GPIO_readPin(KEYPAD_COL_PORT_ID, KEYPAD_COL_PIN_ID+col) == KEYPAD_BUTTON_PRESSED){
					_delay_ms(KEYPAD_DEBOUNCE_MS);

					if(GPIO_readPin(KEYPAD_COL_PORT_ID, KEYPAD_COL_PIN_ID+col) == KEYPAD_BUTTON_PRESSED){
						key = KEYPAD_adjustKeyNumber(KEYPAD_CALCULATE_KEY_NUMBER(KEYPAD_COLS_NUMBER, row, col));

						/* Handling release button debouncing */
						while(GPIO_readPin(KEYPAD_COL_PORT_ID, KEYPAD_COL_PIN_ID+col) == KEYPAD_BUTTON_PRESSED);
						_delay_ms(KEYPAD_DEBOUNCE_MS);
						/* Key found: convert (row, col) to a key number, then to its value for the keypad type */

						GPIO_setupPinDirection(KEYPAD_ROW_PORT_ID, KEYPAD_ROW_PIN_ID+row, PIN_INPUT);
						return key;
					}
				}
			}
			/* No key in this row: release the row (back to input) and wait 10 ms before the next row */
			GPIO_setupPinDirection(KEYPAD_ROW_PORT_ID, KEYPAD_ROW_PIN_ID+row, PIN_INPUT);
			_delay_ms(KEYPAD_DEBOUNCE_MS);
		}
	}
}

/* Mapping for the 4x4 keypad (calculator layout):
 * 1 2 3 4 are the first row, 5 6 7 8 the second row, and so on. */
static uint8 KEYPAD_adjustKeyNumber(uint8 button_number){
/*
 * Description:
 * Convert the button number (1 to 16, counted row by row) to the value printed on the key.
 * Digits are returned as numbers, and operators as ASCII characters.
*/
#if(KEYPAD_COLS_NUMBER == 4)
	static uint8 const keys[16] = {
		7,   8,   9,   '/',
		4,   5,   6,   '*',
		1,   2,   3,   '-',
		'C', 0,   '=', '+'
	};
/*
 * Description:
 * Convert the button number (1 to 12, counted row by row) to the value printed on the key.
 * NOTE: only buttons 10, 11 and 12 are mapped here. For the other buttons,
 * keypad_number is never assigned, so the returned value is undefined.
*/
#elif(KEYPAD_COLS_NUMBER == 3)
	static uint8 const keys[9] = {
		1,   2,   3,
		4,   5,   6,
		7,   8,   9,
		'*', 0,  '#'
	};
#endif

	return keys[button_number-1];
}
