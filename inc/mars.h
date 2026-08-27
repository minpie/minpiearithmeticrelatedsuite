/*
mars.h

created: 2026.02.16
last modified: 2026.08.27
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
#define CONST_SIGN_POSITIVE 1 //
#define CONST_SIGN_NEGATIVE -1 //


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
#define ABS(a) (((a) < 0) ? (-(a)) : (a))


// datatype:
typedef struct __bnz_t{
    // signed integer type
    bnword_t * pData; //
    int32_t allocated; // (number of bnword_t in pData)
    int32_t used; // (max(log8((the raw number) | 1), 1) * sign) or ("size in bytes" * sign), sign will be 1(positive) or -1(negative)
}_bnz_t;
typedef _bnz_t bnz_t[1];
typedef _bnz_t * bnzptr_t;


// function prototype:
// Bnh: utility function:
MARS_API_EXPORT void BnhMemcpy(void * pOut, void * pIn, int32_t len);
MARS_API_EXPORT void BnhMemset(void * pOut, uint8_t val, int32_t len);
MARS_API_EXPORT int32_t BnhMemcmp(void * pIn1, void * pIn2, int32_t len);
MARS_API_EXPORT void BnhZeroize(void * pIn, int32_t len);
MARS_API_EXPORT int32_t BnhGetDigitsInBytes_LE(uint8_t * pBaIn, int32_t lenBaIn);
MARS_API_EXPORT int32_t BnhGetDigitsInBytes_BE(uint8_t * pBaIn, int32_t lenBaIn);
MARS_API_EXPORT int32_t BnhGetDigitsInBits_LE(uint8_t * pBaIn, int32_t lenBaIn);
MARS_API_EXPORT int32_t BnhGetDigitsInBits_BE(uint8_t * pBaIn, int32_t lenBaIn);


// Bnz: signed integer related function:
MARS_API_EXPORT void BnzInit(bnzptr_t pIn);
MARS_API_EXPORT void BnzFinal(bnzptr_t pIn);
MARS_API_EXPORT void BnzBa2Bn(bnzptr_t pOut, uint8_t * pBaIn, int32_t lenBaIn, int32_t sign);
MARS_API_EXPORT int32_t BnzBn2Ba(uint8_t * pBaOut, int32_t lenBaOut, bnzptr_t pIn);
MARS_API_EXPORT int32_t BnzCompare(bnzptr_t pIn1, bnzptr_t pIn2);
MARS_API_EXPORT void BnzAssign(bnzptr_t pOut, bnzptr_t pIn);
MARS_API_EXPORT int32_t BnzSgn(bnzptr_t pIn);
MARS_API_EXPORT int32_t BnzBitwiseAnd(bnzptr_t pOut, bnzptr_t pIn1, bnzptr_t pIn2);
MARS_API_EXPORT int32_t BnzBitwiseOr(bnzptr_t pOut, bnzptr_t pIn1, bnzptr_t pIn2);
MARS_API_EXPORT int32_t BnzBitwiseXor(bnzptr_t pOut, bnzptr_t pIn1, bnzptr_t pIn2);
MARS_API_EXPORT int32_t BnzBitwiseNot(bnzptr_t pOut, bnzptr_t pIn);
MARS_API_EXPORT int32_t BnzBitwiseLeftShift(bnzptr_t pOut, bnzptr_t pIn, int32_t shift);
MARS_API_EXPORT int32_t BnzBitwiseRightShift(bnzptr_t pOut, bnzptr_t pIn, int32_t shift);
MARS_API_EXPORT int32_t BnzAdd(bnzptr_t pOut, bnzptr_t pIn1, bnzptr_t pIn2);
MARS_API_EXPORT int32_t BnzSub(bnzptr_t pOut, bnzptr_t pIn1, bnzptr_t pIn2);
MARS_API_EXPORT int32_t BnzMul(bnzptr_t pOut, bnzptr_t pIn1, bnzptr_t pIn2);
MARS_API_EXPORT int32_t BnzDiv(bnzptr_t pOut1, bnzptr_t pOut2, bnzptr_t pIn1, bnzptr_t pIn2);

#endif
// end code