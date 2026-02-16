/*
mars.c

created: 2026.02.16
last modified: 2026.02.16
author: minpie
last modify: minpie
version: 0.0.1

*/
// start code:
// include:
#include "mars.h"


// function:
// Bnuz: unsigned integer related function:
MARS_API_EXPORT void BnuzInit(
    bnuzptr_t pIn
)
{
    /*
    void BnuzInit(
        bnuzptr_t pIn
    );

    Arg:
    - pIn: target (bnuz_t) object pointer

    Do:
    - Allocate and initialize the target (bunz_t) object (pIn) refers.

    Return:
    - (NO RETURN)

    Other info:
    - nope
    */
    //
    if(!pIn){
        // exception: pIn is NULL
        return;
    }
    // else:
    pIn->pData = (bnuword_t *)malloc(CONST_SIZE_DEFAULT_BNUZ_WORDS * sizeof(bnuword_t)); // allocate
    if(!(pIn->pData)){
        // exception: pData is NULL = failed to malloc()
        return;
    }
    // else:
    pIn->allocated = CONST_SIZE_DEFAULT_BNUZ_WORDS;
    pIn->used = 0;
    

    // return:
    return; 
}

MARS_API_EXPORT void BnuzFinal(
    bnuzptr_t pIn
)
{
    /*
    void BnuzFinal(
        bnuzptr_t pIn
    );

    Arg:
    - pIn: target (bnuz_t) object pointer

    Do:
    - Zerorize and Deallocate the target (bunz_t) object (pIn) refers.

    Return:
    - (NO RETURN)

    Other info:
    - nope
    */
    //
    if(!pIn){
        // exception: pIn is NULL
        return;
    }
    // else:
    bnuword_t * ptr = pIn->pData;
    for(uint32_t i=0; i<(pIn->allocated); i++){
        ptr = (pIn->pData) + i;
        free((void *)ptr);
    }
    pIn->pData = NULL;
    pIn->allocated = 0;
    pIn->used = 0;

    // return:
    return; 
}

MARS_API_EXPORT void BnuzBa2Bn(
    bnuzptr_t pOut,
    uint8_t * pBaIn,
    uint32_t lenBaIn
)
{
    /*
    void BnuzBa2Bn(
        bnuzptr_t pOut,
        uint8_t * pBaIn,
        uint32_t lenBaIn
    );

    Arg:
    - pOut: target (bnuz_t) object pointer
    - pBaIn: source (uint8_t) byte array pointer
    - lenBaIn: length of (uint8_t) source array (pBaIn) refers

    Do:
    - Convert (lenBaIn) bytes in (pBaIn) to target big number (bnuz_t) object (pOut) refers.

    Return:
    - (NO RETURN)

    Other info:
    - the byte data in (pBaIn) will be regarded as big endian.
    */
    //
    if((!pBaIn) || (!lenBaIn) || (!pOut)){
        // exception: pBaIn is NULL OR lenBaIn is zero OR pOut is NULL
        return;
    }
    // else:

    // return:
    return; 
}

MARS_API_EXPORT void BnuzBn2Ba(
    uint8_t * pBaOut,
    uint32_t lenBaOut,
    bnuzptr_t pIn
)
{
    /*
    void BnuzBn2Ba(
        uint8_t * pBaOut,
        uint32_t lenBaOut,
        bnuzptr_t pIn
    )

    Arg:
    - pBaOut: target (uint8_t) byte array pointer
    - lenBaOut: length of (uint8_t) target array (pBaOut) refers
    - pBaIn: source (bnuz_t) object pointer

    Do:
    - Convert (lenBaOut) bytes of big number in (pIn) to target (uint8_t) bytes array (pBaOut) refers.

    Return:
    - (NO RETURN)

    Other info:
    - the number data will be stored to (pBaOut) as big-endian.
    */
    //
    if((!pIn) || (!pBaOut) || (!lenBaOut)){
        // exception: pIn is NULL OR pBaOut is NULL OR lenBaOut is zero
        return;
    }
    // else:

    // return:
    return; 
}
// end code