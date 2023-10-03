/* 
 * File:   std_typed.h
 * Author: Mohamed Sameh
 *
 * Created on August 22, 2023, 11:20 PM
 */

#ifndef STD_TYPED_H
#define	STD_TYPED_H

/* Section : Includes */
#include "compiler.h"
#include "std_libraries.h"

/* Section : Macro Declarations */
#define STD_HIGH        0x01
#define STD_LOW         0x00

#define STD_ON          0x01
#define STD_OFF         0x00

#define STD_ACIVE       0x01
#define STD_IDLE        0x00

#define CONFIG_ENABLE   0x01
#define CONFIG_DISABLE  0x00

#define E_OK           (Std_ReturnType)0x01
#define E_NOT_OK       (Std_ReturnType)0x00

#define ZERO_INIT            0

/* Section : Macro Functions Declarations */

/* Section : Data Types Declarations  */
typedef unsigned char boolen;
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned long uint32;
typedef signed char sint8;
typedef signed short sint16;
typedef signed long sint32;
typedef float float32;

typedef uint8 Std_ReturnType;



/* Section : Functions Declarations */


#endif	/* STD_TYPED_H */

