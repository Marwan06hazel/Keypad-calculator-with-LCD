/******************************************************************************
 *
 * Module: Common - Macros
 *
 * File Name: common_macros.h
 *
 * Author: Marwan Alaa
 *
 * Created on: Oct 4, 2026
 *
 *******************************************************************************/

#ifndef COMMON_MACROS_H_
#define COMMON_MACROS_H_

/* Set a specific bit in the register */
#define SET_BIT(REG,BIT)                (REG |= (1 << (BIT)))

/* Get a specific bit in the register */
#define GET_BIT(REG,BIT)                (((REG) >> (BIT)) & 0x01)

/* Clear a specific bit in the register */
#define CLEAR_BIT(REG,BIT)              (REG &= ~(1 << (BIT)))

/* Toggle a specific bit in the register */
#define TOGGLE_BIT(REG,BIT)             (REG ^= (1 << (BIT)))


/* Check if a specific bit is set */
#define CHECK_BIT_IS_SET(REG,BIT)       ((REG) & (1 << (BIT)))

/* Check if a specific bit is cleared */
#define CHECK_BIT_IS_CLEAR(REG,BIT)     (!((REG) & (1 << (BIT))))

/* Set all bits of an 8-bit register to 1 */
#define SET_ALL_REGISTER(REG)           (REG = 0xFF)

/* Clear all bits of an 8-bit register to 0 */
#define CLEAR_ALL_REGISTER(REG)         (REG = 0x00)

/* Check if a specific Register is set */
#define CHECK_REGISTER_IS_SET(REG)      ((REG) == 0xFF)

/* Check if a specific Register is cleared */
#define CHECK_REGISTER_IS_CLEAR(REG)    ((REG) == 0X00)

/* Rotate the register right by STEPS bits.
 * Bits shifted out on the right re-enter from the left
*/
#define ROR(REG,STEPS)                  ((REG) = (((REG) >> (STEPS)) | ((REG) << (8 - (STEPS)))))

/* Rotate the register left by STEPS bits.
 * Bits shifted out on the left re-enter from the right
*/
#define ROL(REG,STEPS)                  ((REG) = (((REG) << (STEPS)) | ((REG) >> (8 - (STEPS)))))

#endif /* COMMON_MACROS_H_ */
