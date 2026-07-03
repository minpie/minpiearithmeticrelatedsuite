/*
mars.h

created: 2026.02.16
last modified: 2026.07.03
author: minpie
last modify: minpie
version: 0.0.1

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
#define CONST_SIZE_DEFAULT_BNZ_WORDS 1 // default value of number of words used when allocate pData in BnzInit()


// parameter:
#define PARM_SIZE_WORD 64 // word size(unit: bit)


// configure with the parameter:
#if(PARM_SIZE_WORD == 64)
typedef uint64_t bnword_t;
#elif(PARM_SIZE_WORD == 32)
typedef uint32_t bnword_t;
#elif(PARM_SIZE_WORD == 16)
typedef uint16_t bnword_t;
#elif(PARM_SIZE_WORD == 8)
typedef uint8_t bnword_t;
#endif


// define:
#define MARS_API_EXPORT extern // define macro for export function: i don't wanna be fxxked
#define MARS_API_LOCAL // define macro for local function


// define macro:
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) > (b) ? (b) : (a))


// datatype:
typedef struct __bnz_t{
    // unsigned integer type
    bnword_t * pData; //
    int32_t allocated; // (number of bnword_t in pData)
    int32_t used; // (ceil(log8((the raw number) | 1)) * sign), or ("size in bytes" * sign), sign will be 1(positive) or -1(negative)
}_bnz_t;
typedef _bnz_t bnz_t[1];
typedef _bnz_t * bnzptr_t;


// function prototype:
// Bnh: utility function:
MARS_API_EXPORT int32_t BnhGetDigitsInBytes_LE(uint8_t * pBaIn, int32_t lenBaIn);
MARS_API_EXPORT int32_t BnhGetDigitsInBytes_BE(uint8_t * pBaIn, int32_t lenBaIn);


// Bnz: unsigned integer related function:
MARS_API_EXPORT void BnzInit(bnzptr_t pIn);
MARS_API_EXPORT void BnzFinal(bnzptr_t pIn);
MARS_API_EXPORT void BnzBa2Bn(bnzptr_t pOut, uint8_t * pBaIn, int32_t lenBaIn);
MARS_API_EXPORT void BnzBn2Ba(uint8_t * pBaOut, int32_t lenBaOut, bnzptr_t pIn);
MARS_API_EXPORT int32_t BnzCompare(bnzptr_t pIn1, bnzptr_t pIn2);
MARS_API_EXPORT int32_t BnzAdd(bnzptr_t pOut, bnzptr_t pIn1, bnzptr_t pIn2);
MARS_API_EXPORT int32_t BnzSub(bnzptr_t pOut, bnzptr_t pIn1, bnzptr_t pIn2);

#endif
// end code