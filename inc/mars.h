/*
mars.h

created: 2026.02.16
last modified: 2026.02.16
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


// constant:
#define CONST_SIZE_DEFAULT_BNUZ_WORDS 1 // default value of number of words used when allocate pData in BnuzInit()


// parameter:
#define PARM_SIZE_WORD 64 // word size(unit: bit)


// configure with the parameter:
#if(PARM_SIZE_WORD == 64)
typedef uint64_t bnuword_t;
#elif(PARM_SIZE_WORD == 32)
typedef uint32_t bnuword_t;
#elif(PARM_SIZE_WORD == 16)
typedef uint16_t bnuword_t;
#elif(PARM_SIZE_WORD == 8)
typedef uint8_t bnuword_t;
#endif


// define:
#define MARS_API_EXPORT extern // define macro for export function: i don't wanna be fxxked
#define MARS_API_LOCAL // define macro for local function


// datatype:
typedef struct __bnuz_t{
    // unsigned integer type
    bnuword_t * pData; //
    uint32_t allocated; // number of bnuword_t in pData
    uint32_t used; // log2(abs(the raw number) | 1)
}_bnuz_t;
typedef _bnuz_t bnuz_t[1];
typedef _bnuz_t * bnuzptr_t;


// function prototype:
// Bnuz: unsigned integer related function:
MARS_API_EXPORT void BnuzInit(bnuzptr_t pIn);
MARS_API_EXPORT void BnuzFinal(bnuzptr_t pIn);
MARS_API_EXPORT void BnuzBa2Bn(bnuzptr_t pOut, uint8_t * pBaIn, uint32_t lenBaIn);
MARS_API_EXPORT void BnuzBn2Ba(uint8_t * pBaOut, uint32_t lenBaOut, bnuzptr_t pIn);

#endif
// end code