/*
mars.c

created: 2026.02.16
last modified: 2026.07.03
author: minpie
last modify: minpie
version: 0.0.1

*/
// start code:
// include:
#include "mars.h"


// function:
// Bnh: utility function:
MARS_API_EXPORT uint32_t BnhGetDigitsInBytes_LE(
    uint8_t * pBaIn,
    uint32_t lenBaIn
)
{
    /*
    uint32_t BnhGetDigitsInBytes_LE(
        uint8_t * pBaIn,
        uint32_t lenBaIn
    );

    Arg:
    - pBaIn: target (uint8_t) bytes array pointer
    - lenBaIn: length of (uint8_t) source array (pBaIn) refers

    Do:
    - Get ceil(log8((the raw number) | 1)), in other words, "size in bytes"

    Return:
    - the size as (uint32_t)

    Other info:
    - the byte data in (pBaIn) will be regarded as little endian.
    */
    //
    if((!pBaIn) || (!lenBaIn)){
        // exception: pBaIn is NULL OR lenBaIn is zero.
        return 0;
    }
    // else:
    uint32_t result = lenBaIn;
    for(uint32_t i=0; i<lenBaIn; i++){
        if(*(pBaIn + lenBaIn - 1 - i)){
            break;
        }else{
            result--;
        }
    }

    // return:
    return result;
}

MARS_API_EXPORT uint32_t BnhGetDigitsInBytes_BE(
    uint8_t * pBaIn,
    uint32_t lenBaIn
)
{
    /*
    uint32_t BnhGetDigitsInBytes_BE(
        uint8_t * pBaIn,
        uint32_t lenBaIn
    );

    Arg:
    - pBaIn: target (uint8_t) bytes array pointer
    - lenBaIn: length of (uint8_t) source array (pBaIn) refers

    Do:
    - Get ceil(log8((the raw number) | 1)), in other words, "size in bytes"

    Return:
    - the size as (uint32_t)

    Other info:
    - the byte data in (pBaIn) will be regarded as big endian.
    */
    //
    if((!pBaIn) || (!lenBaIn)){
        // exception: pBaIn is NULL OR lenBaIn is zero.
        return 0;
    }
    // else:
    uint32_t result = 0;
    result = lenBaIn;
    for(uint32_t i=0; i<lenBaIn; i++){
        if(*(pBaIn + i)){
            break;
        }else{
            result--;
        }
    }

    // return:
    return result;
}


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
    memset((pIn->pData), 0, (CONST_SIZE_DEFAULT_BNUZ_WORDS * sizeof(bnuword_t)));
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
    if(pIn->pData){
        // pIn->pData != NULL:
        free(pIn->pData);
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
    uint32_t digitsInBytes = 0;
    uint32_t neededWords = 0;
    digitsInBytes = BnhGetDigitsInBytes_BE(pBaIn, lenBaIn); // get digits in bytes
    neededWords = (uint32_t)(ceil(((double)digitsInBytes) / sizeof(bnuword_t))); // get needed words from needed bytes
    neededWords = MAX(neededWords, 1);
    pOut->pData = realloc((void *)(pOut->pData), (sizeof(bnuword_t) * neededWords)); // reallocate words
    pOut->allocated = neededWords;
    pOut->used = digitsInBytes;
    memset((pOut->pData), 0, (sizeof(bnuword_t) * neededWords));
    
    if(!(pOut->pData)){
        // exception: failed to realloc()
        return;
    }
    // else:
    for(uint32_t i=0; i<digitsInBytes; i++){
        *(((uint8_t *)(pOut->pData)) + i) = (uint8_t)(*(pBaIn + lenBaIn - 1 - i));
    }

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
    for(uint32_t i=0; ((i<lenBaOut) && (i<(pIn->used))); i++){
        *(pBaOut + lenBaOut - 1 - i) = *(((uint8_t *)(pIn->pData)) + i);
    }

    // return:
    return; 
}

MARS_API_EXPORT int32_t BnuzCompare(
    bnuzptr_t pIn1,
    bnuzptr_t pIn2
)
{
    /*
    int32_t BnuzCompare(
        bnuzptr_t pIn1,
        bnuzptr_t pIn2
    )

    Arg:
    - pIn1: target (bnuz_t) object 1 pointer
    - pIn2: target (bnuz_t) object 2 pointer

    Do:
    - Compare (pIn1) and (pIn2),
    return 0 if same,
    return 1 if (pIn1) > (pIn2),
    return -1 if (pIn1) < (pIn2)  

    Return:
    - (int32_t) value: -1 OR 0 OR 1

    Other info:
    - nope
    */
    //
    if((!pIn1) || (!pIn2)){
        // exception: pIn1 is NULL OR pIn2 is NULL
        return 0;
    }
    // else:


    for(uint32_t i=0; i<(MIN(pIn1->allocated, pIn2->allocated)); i++){
        if(*((pIn1->pData) + i) > *((pIn2->pData) + i)){
            return 1;
        }else if(*((pIn1->pData) + i) < *((pIn2->pData) + i)){
            return -1;
        }
    }
    // else:

    // return:
    return 0;
}

MARS_API_EXPORT int32_t BnuzAdd(
    bnuzptr_t pOut,
    bnuzptr_t pIn1,
    bnuzptr_t pIn2
)
{
    /*
    int32_t BnuzAdd(
        bnuzptr_t pOut,
        bnuzptr_t pIn1,
        bnuzptr_t pIn2
    )

    Arg:
    - pOut: target (bnuz_t) object output pointer
    - pIn1: target (bnuz_t) object input 1 pointer
    - pIn2: target (bnuz_t) object input 2 pointer

    Do:
    - Do (pOut) = (pIn1) + (pIn2),

    Return:
    - (NO RETURN)

    Other info:
    - nope
    */
    //
    if((!pOut) || (!pIn1) || (!pIn2)){
        // exception: pOut is NULL OR pIn1 is NULL OR pIn2 is NULL
        return 0;
    }
    // else:
    bnuword_t s = 0; // sum
    bnuword_t c = 0; // carry
    uint32_t estimatedWords = 0;
    bnuz_t tempOut;
    bnuzptr_t t = NULL;
    uint32_t tempIdx = 0;

    BnuzInit(tempOut);
    estimatedWords = MAX((pIn1->allocated), (pIn2->allocated));

    tempOut->pData = realloc((void *)(tempOut->pData), (sizeof(bnuword_t) * estimatedWords)); // reallocate words
    tempOut->allocated = estimatedWords;
    tempOut->used = MAX((pIn1->used), (pIn2->used));
    memset((tempOut->pData), 0, (sizeof(bnuword_t) * estimatedWords)); // clear to zero

    /*
    s = a + b + c_in

    ( (s < a) || (c_in && (s == a)) ) then overflow
    0 <= a <= UINT_MAX
    0 <= b <= UINT_MAX
    0 <= c_in <= 1
    a와 b의 대소관계 정해지지 않음.
    */
    //

    // addition:
    for(uint32_t i=0; i<(MIN(pIn1->allocated, pIn2->allocated)); i++){
        tempIdx = i;
        s = *((pIn1->pData) + tempIdx) + *((pIn2->pData) + tempIdx) + c;
        if((s < *((pIn1->pData) + tempIdx)) || (c && (s == *((pIn1->pData) + tempIdx)))){
        //if((s <= (*((pIn1->pData) + i))) && (s <= (*((pIn2->pData) + i)))){ // bugged
        // overflow detected:
            c = 1;
        }else{
            c = 0;
        }
        *((tempOut->pData) + tempIdx) = s;
    }

    // add rest of bigger one:
    if((pIn1->allocated) > (pIn2->allocated)){
        t = pIn1;
    }else if((pIn1->allocated) < (pIn2->allocated)){
        t = pIn2;
    }
    if(t){
        for(uint32_t i=0; i<((MAX(pIn1->allocated, pIn2->allocated)) - (MIN(pIn1->allocated, pIn2->allocated))); i++){
            tempIdx = i + (MIN(pIn1->allocated, pIn2->allocated));
            s = *((t->pData) + tempIdx) + c;
            if((s < *((t->pData) + tempIdx)) || (c && (s == *((t->pData) + tempIdx)))){
                // overflow detected:
                c = 1;
            }else{
                c = 0;
            }
            *((tempOut->pData) + tempIdx) = s;
        }
    }
    tempOut->used = BnhGetDigitsInBytes_LE((uint8_t *)(tempOut->pData), (sizeof(bnuword_t) * tempOut->allocated));

    if(c){
        tempOut->pData = realloc((void *)(tempOut->pData), (sizeof(bnuword_t) * (estimatedWords + 1))); // reallocate words
        tempOut->allocated = estimatedWords + 1;
        *((tempOut->pData) + tempIdx + 1) = 1;
        tempOut->used = (sizeof(bnuword_t) * ((tempOut->allocated) - 1)) + 1;
    }

    // tempOut to pOut:
    bnuword_t * oldData = pOut->pData;
    if(oldData){
        free(oldData);
    }
    pOut->pData = tempOut->pData;
    pOut->allocated = tempOut->allocated;
    pOut->used = tempOut->used;

    // return:
    return c;
}
// end code