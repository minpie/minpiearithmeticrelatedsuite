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


// parameter:
#define PARM_SIZE_WORD 64 // word size(unit: bit)


// configure with the parameter:
#if((PARM_SIZE_WORD <= 64) && (PARM_SIZE_WORD > 32))
typedef uint64_t bnuword_t;
#elif(PARM_SIZE_WORD > 16)
typedef uint32_t bnuword_t;
#elif(PARM_SIZE_WORD > 8)
typedef uint16_t bnuword_t;
#elif(PARM_SIZE_WORD > 1)
typedef uint8_t bnuword_t;
#endif


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
void BnuzInit(bnuzptr_t pIn);
void BnuzFinal(bnuzptr_t pIn);


#endif
// end code