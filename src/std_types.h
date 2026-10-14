/******************************************************************************
 *
 * File Name: std_types.h
 *
 * Description: Commonly used bit-manipulation macros for 8-bit registers
 *
 * Author: Marwan Alaa
 *
 * Created on: Oct 4, 2026
 *
 *******************************************************************************/

#ifndef STD_TYPES_H_
#define STD_TYPES_H_

typedef unsigned char 		bool; 				/* true - false (0-255)*/

#define false 				(0u)
#define true 				(1u)

#define LOGIC_HIGH			(1u)
#define LOGIC_LOW			(0u)

#define NULL			((void*) 0)


typedef unsigned char 		uint8;				/* 	        0           -          255		 	*/
typedef signed char 		sint8;				/* 		  -128 			-          127 			*/

typedef unsigned short 		uint16;				/* 		    0           -         65535   		*/
typedef signed short 		sint16;				/* 		 -32768         -         32767 		*/

typedef unsigned long 		uint32;				/* 		    0           - 	   4294967295 	    */
typedef signed long 		sint32;				/*	   -2147483648 	    -      2147483647		*/

typedef unsigned long long 	uint64;				/* 			0 			-  18446744073709551615 */
typedef signed long long 	sint64;				/* -9223372036854775808 -  9223372036854775807  */

typedef float 				float32;			/*	    -2147483648     -       2147483647	    */
typedef double 				float64;			/* -9223372036854775808 -  9223372036854775807  */

#endif /* STD_TYPES_H_ */
