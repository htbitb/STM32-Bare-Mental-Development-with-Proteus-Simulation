#include <stdint.h>
#include <stdbool.h>

#ifndef INC_MACRO_H_
#define INC_MACRO_H_

#define __IO volatile

typedef uint8_t  U08;
typedef uint16_t U16;
typedef uint32_t U32;

#define ENABLE      1
#define DISABLE     0

#define SET         1
#define RESET       0
#define FLAG_SET    SET
#define FLAG_RESET  RESET

#define TRUE        1
#define FALSE       0

#define PRIVATE static
#define GLOBAL

#define NULL        0

#define ON          1
#define OFF         0


































#endif