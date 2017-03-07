
#ifndef STDINT_H
#define STDINT_H

#if defined __C51__

#define TRUE						(1)
#define FALSE						(0)

#define uint8_t unsigned char
#define uint16_t unsigned short
#define uint32_t unsigned long
typedef uint32_t uint64_t[2];

typedef signed char int8_t;
typedef short int16_t;
typedef long int32_t;
typedef int32_t int64_t[2];

#endif

#endif
