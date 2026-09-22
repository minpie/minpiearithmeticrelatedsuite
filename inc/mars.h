/*
mars.h

created: 2026.02.16
last modified: 2026.09.22
author: minpie
last modify: minpie
version: 1.0.0

*/
// start code:
#ifndef _MARS_H_
#define _MARS_H_


// include:
#include <stdint.h>
#include <malloc.h>
#include <math.h>
#include <string.h>


// constant:
#define CONST_SIZE_DEFAULT_MARSZ_WORDS 1 // default value of number of words used when allocate pData in Marsz_Init()
#define CONST_SIGN_POSITIVE 1 //
#define CONST_SIGN_NEGATIVE -1 //


// parameter:
#define PARM_SIZE_WORD 64 // word size(unit: bit)


// configure with the parameter:
#if(PARM_SIZE_WORD == 64)
typedef uint64_t marsword_t;
#elif(PARM_SIZE_WORD == 32)
typedef uint32_t marsword_t;
#elif(PARM_SIZE_WORD == 16)
typedef uint16_t marsword_t;
#elif(PARM_SIZE_WORD == 8)
typedef uint8_t marsword_t;
#endif


// define:
#define MARS_API_EXPORT extern // define macro for export function: i don't wanna be fxxked
#define MARS_API_LOCAL // define macro for local function


// define macro:
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) > (b) ? (b) : (a))
#define ABS(a) (((a) < 0) ? (-(a)) : (a))


// datatype:
typedef struct __marsz_t{
    // signed integer type
    marsword_t * pData; //
    int32_t allocated; // ((sign) * (number of marsword_t in pData)), sign will be 1(0 or positive) or -1(negative)
}_marsz_t;
typedef _marsz_t marsz_t[1];
typedef _marsz_t * marszptr_t;


// function prototype:
// Marsh_: utility function:
MARS_API_EXPORT void Marsh_Memcpy(void * pOut, void * pIn, int32_t len);
MARS_API_EXPORT void Marsh_Memset(void * pOut, uint8_t val, int32_t len);
MARS_API_EXPORT int32_t Marsh_Memcmp(void * pIn1, void * pIn2, int32_t len);
MARS_API_EXPORT void Marsh_Zeroize(void * pIn, int32_t len);
MARS_API_EXPORT int32_t Marsh_GetDigitsInBytes_LE(uint8_t * pBaIn, int32_t lenBaIn);
MARS_API_EXPORT int32_t Marsh_GetDigitsInBytes_BE(uint8_t * pBaIn, int32_t lenBaIn);
MARS_API_EXPORT int32_t Marsh_GetDigitsInBits_LE(uint8_t * pBaIn, int32_t lenBaIn);
MARS_API_EXPORT int32_t Marsh_GetDigitsInBits_BE(uint8_t * pBaIn, int32_t lenBaIn);


// Marsz_: signed integer related function:
MARS_API_EXPORT void Marsz_Init(marszptr_t pIn);
MARS_API_EXPORT void Marsz_Final(marszptr_t pIn);
MARS_API_EXPORT void Marsz_Ba2Bn(marszptr_t pOut, uint8_t * pBaIn, int32_t lenBaIn, int32_t sign);
MARS_API_EXPORT int32_t Marsz_Bn2Ba(uint8_t * pBaOut, int32_t lenBaOut, marszptr_t pIn);
MARS_API_EXPORT int32_t Marsz_CompareAbs(marszptr_t pIn1, marszptr_t pIn2);
MARS_API_EXPORT int32_t Marsz_Compare(marszptr_t pIn1, marszptr_t pIn2);
MARS_API_EXPORT void Marsz_Assign(marszptr_t pOut, marszptr_t pIn);
MARS_API_EXPORT int32_t Marsz_Sgn(marszptr_t pIn);
MARS_API_EXPORT int32_t Marsz_BitwiseAnd(marszptr_t pOut, marszptr_t pIn1, marszptr_t pIn2);
MARS_API_EXPORT int32_t Marsz_BitwiseOr(marszptr_t pOut, marszptr_t pIn1, marszptr_t pIn2);
MARS_API_EXPORT int32_t Marsz_BitwiseXor(marszptr_t pOut, marszptr_t pIn1, marszptr_t pIn2);
MARS_API_EXPORT int32_t Marsz_BitwiseNot(marszptr_t pOut, marszptr_t pIn);
MARS_API_EXPORT int32_t Marsz_BitwiseLeftShift(marszptr_t pOut, marszptr_t pIn, int32_t shift);
MARS_API_EXPORT int32_t Marsz_BitwiseRightShift(marszptr_t pOut, marszptr_t pIn, int32_t shift);
MARS_API_EXPORT int32_t Marsz_Add(marszptr_t pOut, marszptr_t pIn1, marszptr_t pIn2);
MARS_API_EXPORT int32_t Marsz_Sub(marszptr_t pOut, marszptr_t pIn1, marszptr_t pIn2);
MARS_API_EXPORT int32_t Marsz_Mul(marszptr_t pOut, marszptr_t pIn1, marszptr_t pIn2);
MARS_API_EXPORT int32_t Marsz_Div(marszptr_t pOut1, marszptr_t pOut2, marszptr_t pIn1, marszptr_t pIn2);

#endif
// end code