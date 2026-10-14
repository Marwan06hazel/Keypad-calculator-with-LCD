/******************************************************************************
 *
 * File Name: GPIO.h
 *
 * Author: Marwan Alaa
 *
 * Created on: Oct 4, 2026
 *
 *******************************************************************************/

#ifndef GPIO_H_
#define GPIO_H_

#include "std_types.h"

#define NUMBER_OF_PORTS 4
#define NUMBER_OF_PINS 8

#define PORTA_ID 0
#define PORTB_ID 1
#define PORTC_ID 2
#define PORTD_ID 3

#define PIN0_ID 0
#define PIN1_ID 1
#define PIN2_ID 2
#define PIN3_ID 3
#define PIN4_ID 4
#define PIN5_ID 5
#define PIN6_ID 6
#define PIN7_ID 7

/* Direction of a single GPIO pin */
typedef enum
{
	PIN_INPUT, PIN_OUTPUT
}GPIO_PINDIRECTIONTYPE;

/* Direction of a whole GPIO port */
typedef enum
{
	PORT_INPUT, PORT_OUTPUT
}GPIO_PORTDIRECTIONTYPE;


/*
 * Description:
 * Set the direction of a specific pin as input or output.
 *
 * Parameters:
 * port_num  - Port that contains the pin.
 * pin_num   - Pin number inside the port.
 * direction - PIN_INPUT or PIN_OUTPUT.
 */
void GPIO_setupPinDirection(uint8 port_num, uint8 pin_num, GPIO_PINDIRECTIONTYPE direction);

/*
 * Description:
 * Write a logic value to a specific pin.
 * The pin must be configured as output.
 *
 * Parameters:
 * port_num - Port that contains the pin.
 * pin_num  - Pin number inside the port.
 * value    - LOGIC_HIGH or LOGIC_LOW.
 */
void GPIO_writePin(uint8 port_num, uint8 pin_num, uint8 value);

/*
 * Description:
 * Read the logic value of a specific pin.
 * The pin must be configured as input.
 *
 * Parameters:
 * port_num - Port that contains the pin.
 * pin_num  - Pin number inside the port.
 * value    - Not used by a read operation.
 *
 * Return:
 * The pin state: LOGIC_HIGH or LOGIC_LOW.
 */
uint8 GPIO_readPin(uint8 port_num, uint8 pin_num);

/*
 * Description:
 * Set the direction of all pins of a port as input or output.
 *
 * Parameters:
 * port_num  - Port to configure.
 * pin_num   - Not used by a port operation.
 * direction - PORT_INPUT or PORT_OUTPUT.
 */
void GPIO_setupPortDirection(uint8 port_num, GPIO_PORTDIRECTIONTYPE direction);

/*
 * Description:
 * Write a value to all pins of a port.
 * The port must be configured as output.
 *
 * Parameters:
 * port_num - Port to write to.
 * pin_num  - Not used by a port operation.
 * value    - Value to write to the whole port (one bit per pin).
 */
void GPIO_writePort(uint8 port_num, uint8 value);

/*
 * Description:
 * Read the value of all pins of a port.
 * The port must be configured as input.
 *
 * Parameters:
 * port_num - Port to read from.
 * pin_num  - Not used by a port operation.
 * value    - Not used by a read operation.
 *
 * Return:
 * The value of the whole port (one bit per pin).
 */
uint8 GPIO_readPort(uint8 port_num);

#endif /* GPIO_H_ */
