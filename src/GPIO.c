/******************************************************************************
 *
 * File Name: GPIO.c
 *
 * Author: Marwan Alaa
 *
 * Created on: Oct 4, 2026
 *
 *******************************************************************************/

#include <avr/io.h>
#include "GPIO.h"
#include "common_macros.h"

/*
 * Description:
 * Set the direction of a specific pin as input or output by
 * clearing or setting its bit in the DDRx (Data Direction) register.
 * DDRx bit = 0 -> input, DDRx bit = 1 -> output.
 */
void GPIO_setupPinDirection(uint8 port_num, uint8 pin_num, GPIO_PINDIRECTIONTYPE direction){
	/* Validate the inputs: ignore the request if the port or pin number is out of range */
	if((port_num >= NUMBER_OF_PORTS) || (pin_num >= NUMBER_OF_PINS)){
		/* Do Nothing */
	}else{
		/* Select the direction register of the requested port */
		switch(port_num){
		case PORTA_ID:
			/* PIN_INPUT clears the pin's bit, otherwise the bit is set (output) */
			if(direction == PIN_INPUT){
				CLEAR_BIT(DDRA, pin_num);
			}else{
				SET_BIT(DDRA, pin_num);
			}
			break;
		/* Same logic as PORTA, applied to DDRB */
		case PORTB_ID:
			if(direction == PIN_INPUT){
				CLEAR_BIT(DDRB, pin_num);
			}else{
				SET_BIT(DDRB, pin_num);
			}
			break;
		/* Same logic as PORTA, applied to DDRC */
		case PORTC_ID:
			if(direction == PIN_INPUT){
				CLEAR_BIT(DDRC, pin_num);
			}else{
				SET_BIT(DDRC, pin_num);
			}
			break;
		/* Same logic as PORTA, applied to DDRD */
		case PORTD_ID:
			if(direction == PIN_INPUT){
				CLEAR_BIT(DDRD, pin_num);
			}else{
				SET_BIT(DDRD, pin_num);
			}
			break;
		}
	}
}

/*
 * Description:
 * Write a logic value to a specific pin.
 * NOTE: the code below clears or sets the pin's bit in the DDRx (direction)
 * register, so it changes the pin direction. To drive the output level, the
 * bit of the PORTx register should be written instead.
 */
void GPIO_writePin(uint8 port_num, uint8 pin_num, uint8 value){
	/* Validate the inputs: ignore the request if the port or pin number is out of range */
	if((port_num >= NUMBER_OF_PORTS) || (pin_num >= NUMBER_OF_PINS)){
		/* Do Nothing */
	}else{
		/* Select the register of the requested port */
		switch(port_num){
		case PORTA_ID:
			/* LOGIC_LOW clears the pin's bit, any other value sets it */
			if(value == LOGIC_LOW){
				CLEAR_BIT(PORTA, pin_num);
			}else{
				SET_BIT(PORTA, pin_num);
			}
			break;
		/* Same logic as PORTA, applied to DDRB */
		case PORTB_ID:
			if(value == LOGIC_LOW){
				CLEAR_BIT(PORTB, pin_num);
			}else{
				SET_BIT(PORTB, pin_num);
			}
			break;
		/* Same logic as PORTA, applied to DDRC */
		case PORTC_ID:
			if(value == LOGIC_LOW){
				CLEAR_BIT(PORTC, pin_num);
			}else{
				SET_BIT(PORTC, pin_num);
			}
			break;
		/* Same logic as PORTA, applied to DDRD */
		case PORTD_ID:
			if(value == LOGIC_LOW){
				CLEAR_BIT(PORTD, pin_num);
			}else{
				SET_BIT(PORTD, pin_num);
			}
			break;
		}
	}
}

/*
 * Description:
 * Read the logic value of a specific pin and return it as LOGIC_HIGH or LOGIC_LOW.
 * NOTE: the code below checks the pin's bit in the DDRx (direction) register,
 * so it returns the configured direction. To read the actual pin level, the
 * bit of the PINx register should be checked instead.
 */
uint8 GPIO_readPin(uint8 port_num, uint8 pin_num){
	/* Default return value, kept if the inputs are invalid */
	uint8 pin_value = LOGIC_LOW;

	/* Validate the inputs: return the default value if the port or pin number is out of range */
	if((port_num >= NUMBER_OF_PORTS) || (pin_num >= NUMBER_OF_PINS)){
		/* Do Nothing */
	}else{
		/* Select the register of the requested port */
		switch(port_num){
		case PORTA_ID:
			/* Return LOGIC_HIGH if the pin's bit is 1, otherwise LOGIC_LOW */
			if(CHECK_BIT_IS_SET(PINA, pin_num)){
				pin_value = LOGIC_HIGH;
			}else{
				pin_value = LOGIC_LOW;
			}
			break;
		/* Same logic as PORTA, applied to DDRB */
		case PORTB_ID:
			if(CHECK_BIT_IS_SET(PINB, pin_num)){
				pin_value = LOGIC_HIGH;
			}else{
				pin_value = LOGIC_LOW;
			}
			break;
		/* Same logic as PORTA, applied to DDRC */
		case PORTC_ID:
			if(CHECK_BIT_IS_SET(PINC, pin_num)){
				pin_value = LOGIC_HIGH;
			}else{
				pin_value = LOGIC_LOW;
			}
			break;
		/* Same logic as PORTA, applied to DDRD */
		case PORTD_ID:
			if(CHECK_BIT_IS_SET(PIND, pin_num)){
				pin_value = LOGIC_HIGH;
			}else{
				pin_value = LOGIC_LOW;
			}
			break;
		}
	}

	return pin_value;
}

/*
 * Description:
 * Set the direction of all the pins of a port as input or output
 * by clearing or setting the whole DDRx register.
 * DDRx = 0x00 -> all pins are inputs, DDRx = 0xFF -> all pins are outputs.
 */
void GPIO_setupPortDirection(uint8 port_num, GPIO_PORTDIRECTIONTYPE direction){
	/* Validate the input: ignore the request if the port number is out of range */
	if(port_num >= NUMBER_OF_PORTS){
		/* Do Nothing */
	}else{
		/* Select the direction register of the requested port */
		switch(port_num){
		case PORTA_ID:
			/* PIN_INPUT (value 0) clears the whole register, otherwise all bits are set */
			DDRA = direction;
			break;
		/* Same logic as PORTA, applied to DDRB */
		case PORTB_ID:
			DDRB = direction;
			break;
		/* Same logic as PORTA, applied to DDRC */
		case PORTC_ID:
			DDRC = direction;
			break;
		/* Same logic as PORTA, applied to DDRD */
		case PORTD_ID:
			DDRD = direction;
			break;
		}
	}
}

/*
 * Description:
 * Write a value to all the pins of a port.
 * NOTE: the code below compares the value with PIN_INPUT (0) and clears or sets
 * the whole DDRx (direction) register, so it changes the port direction and
 * does not write the value itself. To output a value, the PORTx register
 * should be written instead.
 */
void GPIO_writePort(uint8 port_num, uint8 value){
	/* Validate the input: ignore the request if the port number is out of range */
	if(port_num >= NUMBER_OF_PORTS){
		/* Do Nothing */
	}else{
		/* Select the register of the requested port */
		switch(port_num){
		case PORTA_ID:
			/* A value of 0 clears the whole register, any other value sets all bits */
			PORTA = value;
			break;
		/* Same logic as PORTA, applied to DDRB */
		case PORTB_ID:
			PORTB = value;
			break;
		/* Same logic as PORTA, applied to DDRC */
		case PORTC_ID:
			PORTC = value;
			break;
		/* Same logic as PORTA, applied to DDRD */
		case PORTD_ID:
			PORTD = value;
			break;
		}
	}
}

/*
 * Description:
 * Read the value of a port.
 * NOTE: the code below checks the DDRx (direction) register and returns only
 * LOGIC_LOW (register is 0) or LOGIC_HIGH (register is not 0). To read the
 * actual value of the port pins, the PINx register should be returned instead.
 */
uint8 GPIO_readPort(uint8 port_num){
	/* Default return value, kept if the input is invalid */
	uint8 port_value = LOGIC_LOW;
	/* Validate the input: return the default value if the port number is out of range */
	if(port_num >= NUMBER_OF_PORTS){
		/* Do Nothing */
	}else{
		/* Select the register of the requested port */
		switch(port_num){
		case PORTA_ID:
			/* LOGIC_LOW if every bit of the register is 0, otherwise LOGIC_HIGH */
			port_value = PINA;
			break;
		/* Same logic as PORTA, applied to DDRB */
		case PORTB_ID:
			port_value = PINB;
			break;
		/* Same logic as PORTA, applied to DDRC */
		case PORTC_ID:
			port_value = PINC;
			break;
		/* Same logic as PORTA, applied to DDRD */
		case PORTD_ID:
			port_value = PIND;
			break;
		}
	}

	return port_value;
}
