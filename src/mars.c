/*
mars.c

created: 2026.02.16
last modified: 2026.09.23
author: minpie
last modify: minpie
version: 1.0.0

*/
// start code:
// include:
#include "mars.h"

// global variable:
const marsword_t _marsword_zero[CONST_SIZE_DEFAULT_MARSZ_WORDS] = {0, };
const marsword_t _marsword_one[CONST_SIZE_DEFAULT_MARSZ_WORDS] = {1, };
const marsz_t mars_zero = {
    // pData:
    _marsword_zero,
    // allocated:
    (CONST_SIGN_POSITIVE * CONST_SIZE_DEFAULT_MARSZ_WORDS)
}; // constant for 0
const marsz_t mars_one = {
    // pData:
    _marsword_one,
    // allocated:
    (CONST_SIGN_POSITIVE * CONST_SIZE_DEFAULT_MARSZ_WORDS)
}; // constant for 1


// for dev:
void DbgPrintHex_BE(void * pData, int32_t len){
    // print as big endian.
    for(int32_t i=len-1; i>=0; i--){
        if((i != len-1) && (!((i + 1) % 8))){
            printf(" ");
        }
        printf("%02x", ((uint8_t *)pData)[i]);
    }
    return;
}

void DbgPrintHex_LE(void * pData, int32_t len){
    // print as little endian.
    for(int32_t i=0; i<len; i++){
        if(i && (!(i % 8))){
            printf(" ");
        }
        printf("%02x", ((uint8_t *)pData)[i]);
    }
    return;
}

#define DbgPrintMarsz_(pIn) {\
    printf("%s", (((pIn)->allocated < 0) ? "-" : "+"));\
    DbgPrintHex_BE((pIn)->pData, (sizeof(marsword_t) * ABS((pIn)->allocated))); \
}
// end for dev


// function:
// Marsh_: utility function:
MARS_API_EXPORT void Marsh_Memcpy(
    void * pOut,
    void * pIn,
    int32_t len
)
{
    /*
    void Marsh_Memcpy(
        void * pOut,
        void * pIn,
        int32_t len
    );

    Arg:
    - pOut: destination (void *) pointer
    - pIn: source (void *) pointer
    - len: length to copy in bytes

    Do:
    - copy (len) bytes to (pOut) from (pIn).

    Return:
    - (NO RETURN)

    Other info:
    - nope
    */
    //

    // check exception condition:
    if((!pOut) || (!pIn) || (!len)){
        // exception: pOut is NULL OR pIn is NULL OR len is 0
        return;
    }
    // else:
    memmove(pOut, pIn, (size_t)(ABS(len))); // copy

    // return:
    return;
}

MARS_API_EXPORT void Marsh_Memset(
    void * pOut,
    uint8_t val,
    int32_t len
)
{
    /*
    void Marsh_Memset(
        void * pOut,
        uint8_t val,
        int32_t len
    );

    Arg:
    - pOut: destination (void *) pointer
    - val: (uint8_t) value to copy
    - len: length to copy in bytes

    Do:
    - copy (len) of (val) to (pOut).

    Return:
    - (NO RETURN)

    Other info:
    - nope
    */
    // check exception condition:
    if((!pOut) || (!len)){
        // exception: pOut is NULL OR len is 0
        return;
    }
    // else:
    memset(pOut, val, (size_t)(ABS(len))); // fill

    // return:
    return;
}

MARS_API_EXPORT int32_t Marsh_Memcmp(
    void * pIn1,
    void * pIn2,
    int32_t len
)
{
    /*
    void Marsh_Memcmp(
        void * pIn1,
        void * pIn2,
        int32_t len
    );

    Arg:
    - pIn1: source (void *) pointer 1
    - pIn2: source (void *) pointer 2
    - len: length to compare in bytes

    Do:
    - compare (len) bytes from (pIn1) and (pIn2).
    - if all bytes in (pIn1) and (pIn2) are same, return 0
    - else if pIn1[n] > pIn2[n], return 1
    - else if pIn1[n] < pIn2[n], return -1

    Return:
    - 0 or 1 or -1

    Other info:
    - nope
    */
    // check exception condition:
    if((!pIn1) || (!pIn2) || (!len)){
        // exception: pIn1 is NULL OR pIn2 is NULL OR len is 0
        return 0;
    }
    // else:
    int32_t result = 0;
    result = memcmp(pIn1, pIn2, (size_t)(ABS(len))); // compare

    // return:
    return result;
}

MARS_API_EXPORT void Marsh_Zeroize(
    void * pOut,
    int32_t len
)
{
    /*
    void Marsh_Zeroize(
        void * pOut,
        int32_t len
    );

    Arg:
    - pOut: destination (void *) pointer
    - len: length to copy in bytes

    Do:
    - set 0 to (len) bytes of (pOut).

    Return:
    - (NO RETURN)

    Other info:
    - nope
    */
    // check exception condition:
    if((!pOut) || (!len)){
        // exception: pOut is NULL OR len is 0
        return;
    }
    // else:
    memset(pOut, 0, (size_t)(ABS(len))); // zeroize

    // return:
    return;
}

MARS_API_EXPORT int32_t Marsh_GetDigitsInBytes_LE(
    uint8_t * pBaIn,
    int32_t lenBaIn
)
{
    /*
    int32_t Marsh_GetDigitsInBytes_LE(
        uint8_t * pBaIn,
        int32_t lenBaIn
    );

    Arg:
    - pBaIn: target (uint8_t) bytes array pointer
    - lenBaIn: length of (uint8_t) source array (pBaIn) refers

    Do:
    - Get ceil(log8((the raw number) | 1)), in other words, "size in bytes"

    Return:
    - the size as (int32_t)

    Other info:
    - the byte data in (pBaIn) will be regarded as little endian.
    */
    // check exception condition:
    if((!pBaIn) || (!lenBaIn)){
        // exception: pBaIn is NULL OR lenBaIn is zero.
        return 0;
    }
    // else:
    int32_t result = lenBaIn;
    for(int32_t i=0; i<lenBaIn; i++){
        if(*(pBaIn + lenBaIn - 1 - i)){
            break;
        }else{
            result--;
        }
    }

    // return:
    return result;
}

MARS_API_EXPORT int32_t Marsh_GetDigitsInBytes_BE(
    uint8_t * pBaIn,
    int32_t lenBaIn
)
{
    /*
    int32_t Marsh_GetDigitsInBytes_BE(
        uint8_t * pBaIn,
        int32_t lenBaIn
    );

    Arg:
    - pBaIn: target (uint8_t) bytes array pointer
    - lenBaIn: length of (uint8_t) source array (pBaIn) refers

    Do:
    - Get ceil(log8((the raw number) | 1)), in other words, "size in bytes"

    Return:
    - the size as (int32_t)

    Other info:
    - the byte data in (pBaIn) will be regarded as big endian.
    */
    // check exception condition:
    if((!pBaIn) || (!lenBaIn)){
        // exception: pBaIn is NULL OR lenBaIn is zero.
        return 0;
    }
    // else:
    int32_t result = 0;
    result = lenBaIn;
    for(int32_t i=0; i<lenBaIn; i++){
        if(*(pBaIn + i)){
            break;
        }else{
            result--;
        }
    }

    // return:
    return result;
}

MARS_API_EXPORT int32_t Marsh_GetDigitsInBits_LE(
    uint8_t * pBaIn,
    int32_t lenBaIn
)
{
    /*
    int32_t Marsh_GetDigitsInBits_LE(
        uint8_t * pBaIn,
        int32_t lenBaIn
    );

    Arg:
    - pBaIn: target (uint8_t) bytes array pointer
    - lenBaIn: length of (uint8_t) source array (pBaIn) refers

    Do:
    - Get ceil(log2((the raw number) | 1)), in other words, "size in bits"

    Return:
    - the size as (int32_t)

    Other info:
    - the byte data in (pBaIn) will be regarded as little endian.
    */
    // check exception condition:
    if((!pBaIn) || (!lenBaIn)){
        // exception: pBaIn is NULL OR lenBaIn is zero.
        return 0;
    }
    // else:
    int32_t result = lenBaIn; // Convert bytes to bits
    int32_t t1 = -1;
    uint8_t t2 = 0;
    for(int32_t i=0; i<lenBaIn; i++){
        if(*(pBaIn + lenBaIn - 1 - i)){
            t1 = i;
            break;
        }else{
            result--;
        }
    }

    if(!result || (t1 == -1)){
        return 0;
    }

    t2 = *(pBaIn + lenBaIn - 1 - t1);
    result = (result - 1) << 3;
    while(t2){
        t2 >>= 1;
        result++;
    }

    // return:
    return result;
}
MARS_API_EXPORT int32_t Marsh_GetDigitsInBits_BE(
    uint8_t * pBaIn,
    int32_t lenBaIn
)
{
    /*
    int32_t Marsh_GetDigitsInBits_BE(
        uint8_t * pBaIn,
        int32_t lenBaIn
    );

    Arg:
    - pBaIn: target (uint8_t) bytes array pointer
    - lenBaIn: length of (uint8_t) source array (pBaIn) refers

    Do:
    - Get ceil(log2((the raw number) | 1)), in other words, "size in bits"

    Return:
    - the size as (int32_t)

    Other info:
    - the byte data in (pBaIn) will be regarded as big endian.
    */
    // check exception condition:
    if((!pBaIn) || (!lenBaIn)){
        // exception: pBaIn is NULL OR lenBaIn is zero.
        return 0;
    }
    // else:
    int32_t result = lenBaIn; // Convert bytes to bits
    int32_t t1 = -1;
    uint8_t t2 = 0;
    for(int32_t i=0; i<lenBaIn; i++){
        if(*(pBaIn + i)){
            t1 = i;
            break;
        }else{
            result--;
        }
    }
    if(!result || (t1 == -1)){
        return 0;
    }

    t2 = *(pBaIn + t1);
    result = (result - 1) * 8;
    while(t2){
        t2 >>= 1;
        result++;
    }

    // return:
    return result;
}

// Marsz_: signed integer related function:
MARS_API_EXPORT void Marsz_Init(
    marszptr_t pIn
)
{
    /*
    void Marsz_Init(
        marszptr_t pIn
    );

    Arg:
    - pIn: target (marsz_t) object pointer

    Do:
    - Allocate and initialize the target (bunz_t) object (pIn) refers.

    Return:
    - (NO RETURN)

    Other info:
    - nope
    */
    // check exception condition:
    if(!pIn){
        // exception: pIn is NULL
        return;
    }
    // else:
    pIn->pData = (marsword_t *)malloc(CONST_SIZE_DEFAULT_MARSZ_WORDS * sizeof(marsword_t)); // allocate
    if(!(pIn->pData)){
        // exception: pData is NULL = failed to malloc()
        return;
    }
    // else:
    Marsh_Zeroize((void *)(pIn->pData), (CONST_SIZE_DEFAULT_MARSZ_WORDS * sizeof(marsword_t))); // reset to 0
    pIn->allocated = CONST_SIGN_POSITIVE * CONST_SIZE_DEFAULT_MARSZ_WORDS;
    

    // return:
    return; 
}

MARS_API_EXPORT void Marsz_Final(
    marszptr_t pIn
)
{
    /*
    void Marsz_Final(
        marszptr_t pIn
    );

    Arg:
    - pIn: target (marsz_t) object pointer

    Do:
    - Zerorize and Deallocate the target (bunz_t) object (pIn) refers.

    Return:
    - (NO RETURN)

    Other info:
    - nope
    */
    // check exception condition:
    if(!pIn){
        // exception: pIn is NULL
        return;
    }
    // else:
    if(pIn->pData){
        // pIn->pData != NULL:
        Marsh_Zeroize((void *)(pIn->pData), (ABS(pIn->allocated) * sizeof(marsword_t))); // reset to 0
        free(pIn->pData);
    }
    pIn->pData = NULL;
    pIn->allocated = 0;

    // return:
    return; 
}

MARS_API_EXPORT void Marsz_Ba2Bn(
    marszptr_t pOut,
    uint8_t * pBaIn,
    int32_t lenBaIn,
    int32_t sign
)
{
    /*
    void Marsz_Ba2Bn(
        marszptr_t pOut,
        uint8_t * pBaIn,
        int32_t lenBaIn,
        int32_t sign
    );

    Arg:
    - pOut: target (marsz_t) object pointer
    - pBaIn: source (uint8_t) byte array pointer
    - lenBaIn: length of (uint8_t) source array (pBaIn) refers
    - sign: sign of the number, 1 to positive, -1 to negative

    Do:
    - Convert (lenBaIn) bytes in (pBaIn) to target big number (marsz_t) object (pOut) refers.

    Return:
    - (NO RETURN)

    Other info:
    - the byte data in (pBaIn) will be regarded as big endian.
    */
    // check exception condition:
    if((!pBaIn) || (!lenBaIn) || (!pOut)){
        // exception: pBaIn is NULL OR lenBaIn is zero OR pOut is NULL
        return;
    }
    // else:
    int32_t digitsInBytes = 0;
    int32_t neededWords = 0;
    marsword_t * newData = NULL;
    digitsInBytes = Marsh_GetDigitsInBytes_BE(pBaIn, lenBaIn); // get digits in bytes
    neededWords = (digitsInBytes + (sizeof(marsword_t) - 1)) / sizeof(marsword_t); // get needed words from needed bytes
    neededWords = MAX(neededWords, 1);

    newData = (marsword_t *)malloc((sizeof(marsword_t) * neededWords));
    free(pOut->pData);
    pOut->pData = newData;
    if(!(pOut->pData)){
        // exception: failed to realloc()
        return;
    }
    // else:
    pOut->allocated = sign * neededWords;
    Marsh_Zeroize((void *)(pOut->pData), (sizeof(marsword_t) * neededWords)); // reset to 0
    for(int32_t i=0; i<digitsInBytes; i++){
        *(((uint8_t *)(pOut->pData)) + i) = (uint8_t)(*(pBaIn + lenBaIn - 1 - i));
    }

    // return:
    return; 
}

MARS_API_EXPORT int32_t Marsz_Bn2Ba(
    uint8_t * pBaOut,
    int32_t lenBaOut,
    marszptr_t pIn
)
{
    /*
    int32_t Marsz_Bn2Ba(
        uint8_t * pBaOut,
        int32_t lenBaOut,
        marszptr_t pIn
    )

    Arg:
    - pBaOut: target (uint8_t) byte array pointer
    - lenBaOut: length of (uint8_t) target array (pBaOut) refers
    - pBaIn: source (marsz_t) object pointer

    Do:
    - Convert (lenBaOut) bytes of big number in (pIn) to target (uint8_t) bytes array (pBaOut) refers.

    Return:
    - sign of the number, 1 to positive, -1 to negative

    Other info:
    - the number data will be stored to (pBaOut) as big-endian.
    */
    // check exception condition:
    if((!pIn) || (!pBaOut) || (!lenBaOut)){
        // exception: pIn is NULL OR pBaOut is NULL OR lenBaOut is zero
        return 0;
    }
    // else:
    int32_t sign = 0;
    for(int32_t i=0; ((i<lenBaOut) && (i<(ABS(pIn->allocated) * sizeof(marsword_t)))); i++){
        *(pBaOut + lenBaOut - 1 - i) = *(((uint8_t *)(pIn->pData)) + i);
    }
    sign = (((pIn->allocated) < 0) ? CONST_SIGN_NEGATIVE : CONST_SIGN_POSITIVE);

    // return:
    return sign; 
}

MARS_API_EXPORT int32_t Marsz_CompareAbs(
    marszptr_t pIn1,
    marszptr_t pIn2
)
{
    /*
    int32_t Marsz_CompareAbs(
        marszptr_t pIn1,
        marszptr_t pIn2
    )

    Arg:
    - pIn1: target (marsz_t) object 1 pointer
    - pIn2: target (marsz_t) object 2 pointer

    Do:
    - Compare ABS(pIn1) and ABS(pIn2),
    return 0 if same,
    return 1 if ABS(pIn1) > ABS(pIn2),
    return -1 if ABS(pIn1) < ABS(pIn2)  

    Return:
    - (int32_t) value: -1 OR 0 OR 1

    Other info:
    - nope
    */
    // check exception condition:
    if((!pIn1) || (!pIn2)){
        // exception: pIn1 is NULL OR pIn2 is NULL
        return 0;
    }
    // else:
    if(ABS(pIn1->allocated) > ABS(pIn2->allocated)){
        // ABS(pIn1) > ABS(pIn2):
        return CONST_SIGN_POSITIVE;
    }else if(ABS(pIn1->allocated) < ABS(pIn2->allocated)){
        // ABS(pIn1) < ABS(pIn2):
        return CONST_SIGN_NEGATIVE;
    }else{
        // else: 워드 수 같음
        for(int32_t i=0; i<ABS(pIn1->allocated); i++){
            if(*((pIn1->pData) + ABS(pIn1->allocated) - 1 - i) > *((pIn2->pData) + ABS(pIn1->allocated) - 1 - i)){
                return CONST_SIGN_POSITIVE;
            }else if(*((pIn1->pData) + ABS(pIn1->allocated) - 1 - i) < *((pIn2->pData) + ABS(pIn1->allocated) - 1 - i)){
                return CONST_SIGN_NEGATIVE;
            }
        }
    }
    // else: 값 같음
    // return:
    return 0;
}

MARS_API_EXPORT int32_t Marsz_Compare(
    marszptr_t pIn1,
    marszptr_t pIn2
)
{
    /*
    int32_t Marsz_Compare(
        marszptr_t pIn1,
        marszptr_t pIn2
    )

    Arg:
    - pIn1: target (marsz_t) object 1 pointer
    - pIn2: target (marsz_t) object 2 pointer

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
    // check exception condition:
    if((!pIn1) || (!pIn2)){
        // exception: pIn1 is NULL OR pIn2 is NULL
        return 0;
    }
    // else:
    if((pIn1->allocated) > (pIn2->allocated)){
        return CONST_SIGN_POSITIVE;
    }else if((pIn1->allocated) < (pIn2->allocated)){
        return CONST_SIGN_NEGATIVE;
    }
    // else: 부호, 워드 수 같음
    int absCompared = 0;
    absCompared = Marsz_CompareAbs(pIn1, pIn2);
    if((pIn1->allocated) < 0){
        // 음수이므로 결과 반전
        if(absCompared == CONST_SIGN_POSITIVE){
            return CONST_SIGN_NEGATIVE;
        }else if(absCompared == CONST_SIGN_NEGATIVE){
            return CONST_SIGN_POSITIVE;
        }
    }
    // return:
    return absCompared;
}

MARS_API_EXPORT void Marsz_Assign(
    marszptr_t pOut,
    marszptr_t pIn
)
{
    /*
    void Marsz_Assign(
        marszptr_t pOut,
        marszptr_t pIn
    )

    Arg:
    - pOut: target (marsz_t) object output pointer
    - pIn: target (marsz_t) object input pointer

    Do:
    - Do (pOut) = (pIn)

    Return:
    - (NO RETURN)

    Other info:
    - nope
    */
    // check exception condition:
    if((!pOut) || (!pIn)){
        // exception: pOut is NULL OR pIn is NULL
        return;
    }
    if(pOut == pIn){
        // exception: pOut == pIn
        return;
    }
    // else:
    marsword_t * newData = NULL;
    newData = (marsword_t *)malloc((sizeof(marsword_t) * ABS(pIn->allocated)));
    Marsh_Zeroize((void *)(newData), (sizeof(marsword_t) * ABS(pIn->allocated)));
    free(pOut->pData);
    pOut->pData = newData;
    Marsh_Memcpy((void *)(pOut->pData), (void *)(pIn->pData), (sizeof(marsword_t) * ABS(pIn->allocated)));
    pOut->allocated = pIn->allocated;

    // return:
    return;
}

MARS_API_EXPORT int32_t Marsz_Sgn(
    marszptr_t pIn
)
{
    /*
    int32_t Marsz_Sgn(
        marszptr_t pIn
    )

    Arg:
    - pIn: target (marsz_t) object pointer

    Do:
    - Check sign of (pIn)
    return 1 if (pIn) >= 0
    return -1 if (pIn) < 0

    Return:
    - (int32_t) value: -1 OR 1

    Other info:
    - nope
    */
    // check exception condition:
    if(!pIn){
        // exception: pIn is NULL
        return 0;
    }
    // else:
    int32_t result = 0;
    result = (((pIn->allocated) < 0) ? CONST_SIGN_NEGATIVE : CONST_SIGN_POSITIVE);

    // return:
    return result;
}

MARS_API_EXPORT int32_t Marsz_BitwiseAnd(
    marszptr_t pOut,
    marszptr_t pIn1,
    marszptr_t pIn2
)
{
    /*
    int32_t Marsz_BitwiseAnd(
        marszptr_t pOut,
        marszptr_t pIn1,
        marszptr_t pIn2
    )

    Arg:
    - pOut: target (marsz_t) object output pointer
    - pIn1: target (marsz_t) object input 1 pointer
    - pIn2: target (marsz_t) object input 2 pointer

    Do:
    - Do (pOut) = (pIn1) & (pIn2)

    Return:
    - (TODO)

    Other info:
    - nope
    */
    // check exception condition:
    if((!pOut) || (!pIn1) || (!pIn2)){
        // exception: pOut is NULL OR pIn1 is NULL OR pIn2 is NULL
        return 0;
    }

    // else:
    for(int32_t i=0; i<(MIN(ABS(pIn1->allocated), ABS(pIn2->allocated))); i++){
        *((pOut->pData) + i) = *((pIn1->pData) + i) & *((pIn2->pData) + i); // word-wise AND
    }
    for(int32_t i=(MIN(ABS(pIn1->allocated), ABS(pIn2->allocated))); i<(MAX(ABS(pIn1->allocated), ABS(pIn2->allocated))); i++){
        *((pOut->pData) + i) = 0; // x AND 0 = 0
    }

    // return:
    return 0;
}

MARS_API_EXPORT int32_t Marsz_BitwiseOr(
    marszptr_t pOut,
    marszptr_t pIn1,
    marszptr_t pIn2
)
{
    /*
    int32_t Marsz_BitwiseOr(
        marszptr_t pOut,
        marszptr_t pIn1,
        marszptr_t pIn2
    )

    Arg:
    - pOut: target (marsz_t) object output pointer
    - pIn1: target (marsz_t) object input 1 pointer
    - pIn2: target (marsz_t) object input 2 pointer

    Do:
    - Do (pOut) = (pIn1) | (pIn2)

    Return:
    - (TODO)

    Other info:
    - nope
    */
    // check exception condition:
    if((!pOut) || (!pIn1) || (!pIn2)){
        // exception: pOut is NULL OR pIn1 is NULL OR pIn2 is NULL
        return 0;
    }

    // else:
    marszptr_t t1 = NULL;
    marszptr_t t2 = NULL;
    marsz_t t3;
    Marsz_Init(t3);
    if(ABS(pIn1->allocated) > ABS(pIn2->allocated)){
        t1 = pIn1;
        t2 = pIn2;
    }else{
        t1 = pIn2;
        t2 = pIn1;
    }

    Marsz_Assign(t3, t1); // t3 = t1
    for(int32_t i=0; i<ABS(t2->allocated); i++){
        *((t3->pData) + i) = *((t1->pData) + i) | *((t2->pData) + i); // word-wise OR
    }
    Marsz_Assign(pOut, t3); // pOut = t3

    // return:
    Marsz_Final(t3);
    return 0;
}

MARS_API_EXPORT int32_t Marsz_BitwiseXor(
    marszptr_t pOut,
    marszptr_t pIn1,
    marszptr_t pIn2
)
{
    /*
    int32_t Marsz_BitwiseXor(
        marszptr_t pOut,
        marszptr_t pIn1,
        marszptr_t pIn2
    )

    Arg:
    - pOut: target (marsz_t) object output pointer
    - pIn1: target (marsz_t) object input 1 pointer
    - pIn2: target (marsz_t) object input 2 pointer

    Do:
    - Do (pOut) = (pIn1) ^ (pIn2)

    Return:
    - (TODO)

    Other info:
    - nope
    */
    // check exception condition:
    if((!pOut) || (!pIn1) || (!pIn2)){
        // exception: pOut is NULL OR pIn1 is NULL OR pIn2 is NULL
        return 0;
    }

    // else:
    marszptr_t t1 = NULL;
    marszptr_t t2 = NULL;
    marsz_t t3;
    Marsz_Init(t3);
    if(ABS(pIn1->allocated) > ABS(pIn2->allocated)){
        t1 = pIn1;
        t2 = pIn2;
    }else{
        t1 = pIn2;
        t2 = pIn1;
    }

    Marsz_Assign(t3, t1); // t3 = t1
    for(int32_t i=0; i<ABS(t2->allocated); i++){
        *((t3->pData) + i) = *((t1->pData) + i) ^ *((t2->pData) + i); // word-wise XOR
    }
    Marsz_Assign(pOut, t3); // pOut = t3


    // return:
    Marsz_Final(t3);
    return 0;
}

MARS_API_EXPORT int32_t Marsz_BitwiseNot(
    marszptr_t pOut,
    marszptr_t pIn
)
{
    /*
    int32_t Marsz_BitwiseNot(
        marszptr_t pOut,
        marszptr_t pIn
    )

    Arg:
    - pOut: target (marsz_t) object output pointer
    - pIn: target (marsz_t) object input pointer

    Do:
    - Do (pOut) = ~(pIn)

    Return:
    - (TODO)

    Other info:
    - nope
    */
    // check exception condition:
    if((!pOut) || (!pIn)){
        // exception: pOut is NULL OR pIn is NULL
        return 0;
    }

    // else:
    marsz_t t1;
    Marsz_Init(t1);
    for(int32_t i=0; i<(ABS(pIn->allocated)); i++){
        *((t1->pData) + i) = ~(*((pIn->pData) + i)); // word-wise NOT
    }
    Marsz_Assign(pOut, t1); // pOut = t1

    // return:
    Marsz_Final(t1);
    return 0;
}

MARS_API_EXPORT int32_t Marsz_BitwiseLeftShift(
    marszptr_t pOut,
    marszptr_t pIn,
    int32_t shift
)
{
    /*
    int32_t Marsz_BitwiseLeftShift(
        marszptr_t pOut,
        marszptr_t pIn,
        int32_t shift
    )

    Arg:
    - pOut: target (marsz_t) object output pointer
    - pIn: target (marsz_t) object input pointer
    - shift: number of positions to shift

    Do:
    - Do (pOut) = (pIn) << (shift)

    Return:
    - (TODO)

    Other info:
    - nope
    */
    // check exception condition:
    if((!pOut) || (!pIn)){
        // exception: pOut is NULL OR pIn is NULL
        return 0;
    }

    // else:
    marsz_t t1;
    Marsz_Init(t1);

    Marsz_Assign(t1, pIn); // t1 = pIn
    for(int32_t i=0; i<shift; i++){
        Marsz_Add(t1, t1, t1); // t1 = t1 << 1
    }
    Marsz_Assign(pOut, t1); // pOut = t1
    
    Marsz_Final(t1);
    return 0;
}

MARS_API_EXPORT int32_t Marsz_BitwiseRightShift(
    marszptr_t pOut,
    marszptr_t pIn,
    int32_t shift
)
{
    /*
    int32_t Marsz_BitwiseRightShift(
        marszptr_t pOut,
        marszptr_t pIn,
        int32_t shift
    )

    Arg:
    - pOut: target (marsz_t) object output pointer
    - pIn: target (marsz_t) object input pointer
    - shift: number of positions to shift

    Do:
    - Do (pOut) = (pIn) >> (shift)

    Return:
    - (TODO)

    Other info:
    - nope
    */
    // check exception condition:
    if((!pOut) || (!pIn)){
        // exception: pOut is NULL OR pIn is NULL
        return 0;
    }

    if(!shift){
        // shift is 0, so just assign pIn to pOut
        Marsz_Assign(pOut, pIn); // pOut = pIn
        return 0;
    }

    // else:
    marsz_t t1;
    marsword_t * newData = NULL;
    int32_t orgSign = 0;
    int32_t nonZeroIdx = -1;

    Marsz_Init(t1);
    orgSign = Marsz_Sgn(pIn);
    newData = (marsword_t *)malloc((sizeof(marsword_t) * ABS(pIn->allocated)));
    Marsh_Zeroize((void *)(newData), (sizeof(marsword_t) * ABS(pIn->allocated)));
    free(t1->pData);
    t1->pData = newData;
    t1->allocated = ABS(pIn->allocated);

    // shift operation:
    for(int32_t i=(ABS(pIn->allocated) - 1); i>=0; i--){
        int32_t srcidx = i;
        int32_t destidx = srcidx - (shift / (sizeof(marsword_t) << 3));

        if(destidx < 0){
            break;
        }
        *((t1->pData) + destidx) |= *((pIn->pData) + srcidx) >> (shift % (sizeof(marsword_t) << 3));
        if(destidx){
            if((shift % (sizeof(marsword_t) << 3))){
                *((t1->pData) + destidx - 1) |= *((pIn->pData) + srcidx) << ((sizeof(marsword_t) << 3) - (shift % (sizeof(marsword_t) << 3)));
            }
        }
    }

    // remove zero filled words:
    for(int32_t i=(ABS(t1->allocated) - 1); i>=0; i--){
        if(*((t1->pData) + i)){
            if(i == (ABS(t1->allocated) - 1)){
                nonZeroIdx = -2;
            }else{
                nonZeroIdx = i;
            }
            break;
        }
    }
    // return result:
    if(nonZeroIdx == -2){
        // no zero filled words:
        t1->allocated = orgSign * (t1->allocated);
        Marsz_Assign(pOut, t1); // pOut = t1
    }else if(nonZeroIdx != -1){
        // some zero filled words:
        newData = (marsword_t *)malloc((sizeof(marsword_t) * (nonZeroIdx + 1)));
        Marsh_Zeroize((void *)(newData), (sizeof(marsword_t) * (nonZeroIdx + 1)));
        Marsh_Memcpy((void *)(newData), (void *)(t1->pData), (sizeof(marsword_t) * (nonZeroIdx + 1)));
        free(t1->pData);
        t1->pData = newData;
        t1->allocated = orgSign * (nonZeroIdx + 1);
        Marsz_Assign(pOut, t1); // pOut = t1
    }else{
        // all words are zero:
        Marsz_Assign(pOut, mars_zero); // pOut = 0
    }

    // return:
    Marsz_Final(t1);
    return 0;
}

MARS_API_EXPORT int32_t Marsz_Add(
    marszptr_t pOut,
    marszptr_t pIn1,
    marszptr_t pIn2
)
{
    /*
    int32_t Marsz_Add(
        marszptr_t pOut,
        marszptr_t pIn1,
        marszptr_t pIn2
    )

    Arg:
    - pOut: target (marsz_t) object output pointer
    - pIn1: target (marsz_t) object input 1 pointer
    - pIn2: target (marsz_t) object input 2 pointer

    Do:
    - Do (pOut) = (pIn1) + (pIn2),

    Return:
    - return 1 if last carry was 1, else return 0

    Other info:
    - nope
    */
    // check exception condition:
    if((!pOut) || (!pIn1) || (!pIn2)){
        // exception: pOut is NULL OR pIn1 is NULL OR pIn2 is NULL
        return 0;
    }
    // else:
    marsword_t s = 0; // sum
    marsword_t c = 0; // carry
    marsword_t * oldData = NULL;
    marsword_t * newData = NULL;
    int32_t tempIdx = 0;
    int32_t resultSign = 0;
    int32_t estimatedWords = 0;
    marsz_t t1;
    marszptr_t t2 = NULL;

    if(Marsz_Sgn(pIn1) == Marsz_Sgn(pIn2)){
        // case 1. pIn1.sign == pIn2.sign: 그대로 더하기
        if((pIn1->allocated) < 0){
            // 음수 + 음수:
            resultSign = CONST_SIGN_NEGATIVE; 
        }else{
            // 양수 + 양수:
            resultSign = CONST_SIGN_POSITIVE;
        }
        
        Marsz_Init(t1);
        estimatedWords = MAX(ABS(pIn1->allocated), ABS(pIn2->allocated));

        // reallocate t1->pData to estimatedWords:
        newData = (marsword_t *)malloc((sizeof(marsword_t) * estimatedWords));
        Marsh_Zeroize((void *)(newData), (sizeof(marsword_t) * estimatedWords));
        free(t1->pData);
        t1->pData = newData;
        t1->allocated = estimatedWords;
        
        // s = a + b + c_in
        // ( (s < a) || (c_in && (s == a)) ) then overflow
        // 0 <= a <= UINT_MAX
        // 0 <= b <= UINT_MAX
        // 0 <= c_in <= 1
        // a와 b의 대소관계 정해지지 않음.
        //

        // addition:
        for(int32_t i=0; i<(MIN(ABS(pIn1->allocated), ABS(pIn2->allocated))); i++){
            tempIdx = i;
            s = *((pIn1->pData) + tempIdx) + *((pIn2->pData) + tempIdx) + c;
            if((s < *((pIn1->pData) + tempIdx)) || (c && (s == *((pIn1->pData) + tempIdx)))){
            // overflow detected:
                c = 1;
            }else{
                c = 0;
            }
            *((t1->pData) + tempIdx) = s;
        }

        // add rest of bigger one with carry:
        if(ABS(pIn1->allocated) > ABS(pIn2->allocated)){
            t2 = pIn1;
        }else if(ABS(pIn1->allocated) < ABS(pIn2->allocated)){
            t2 = pIn2;
        }
        
        if(t2){
            for(int32_t i=0; i<((MAX(ABS(pIn1->allocated), ABS(pIn2->allocated))) - (MIN(ABS(pIn1->allocated), ABS(pIn2->allocated)))); i++){
                tempIdx = i + (MIN(ABS(pIn1->allocated), ABS(pIn2->allocated)));
                s = *((t2->pData) + tempIdx) + c;
                if((s < *((t2->pData) + tempIdx)) || (c && (s == *((t2->pData) + tempIdx)))){
                    // overflow detected:
                    c = 1;
                }else{
                    c = 0;
                }
                *((t1->pData) + tempIdx) = s;
            }
        }

        if(c){
            // reallocate t1->pData to (estimatedWords + 1):
            newData = (marsword_t *)malloc((sizeof(marsword_t) * (estimatedWords + 1)));
            Marsh_Zeroize((void *)(newData), (sizeof(marsword_t) * (estimatedWords + 1)));
            Marsh_Memcpy((void *)(newData), (void *)(t1->pData), (sizeof(marsword_t) * estimatedWords));
            free(t1->pData);
            t1->pData = newData;
            t1->allocated = estimatedWords + 1;
            *((t1->pData) + tempIdx + 1) = 1;
        }
        t1->allocated = resultSign * (t1->allocated);

        // t1 to pOut:
        oldData = pOut->pData;
        if(oldData){
            free(oldData);
        }
        pOut->pData = t1->pData;
        pOut->allocated = t1->allocated;
    }else{
        // case 2. pIn1.sign != pIn2.sign: 절댓값 뺄셈 수행
        Marsz_Init(t1);

        if(Marsz_Sgn(pIn1) > Marsz_Sgn(pIn2)){
            // pIn1: 양수 -> pIn2를 양수로 바꾸어 뺄셈:
            Marsz_Assign(t1, pIn2);
            t1->allocated = (CONST_SIGN_NEGATIVE) * (t1->allocated);
            c = Marsz_Sub(pOut, pIn1, t1);
        }else{
            // pIn2: 양수 -> pIn1를 양수로 바꾸어 뺄셈:
            Marsz_Assign(t1, pIn1);
            t1->allocated = (CONST_SIGN_NEGATIVE) * (t1->allocated);
            c = Marsz_Sub(pOut, pIn2, t1);
        }
        Marsz_Final(t1);
    }

    // return:
    return c;
}

MARS_API_EXPORT int32_t Marsz_Sub(
    marszptr_t pOut,
    marszptr_t pIn1,
    marszptr_t pIn2
)
{
    /*
    int32_t Marsz_Sub(
        marszptr_t pOut,
        marszptr_t pIn1,
        marszptr_t pIn2
    )

    Arg:
    - pOut: target (marsz_t) object output pointer
    - pIn1: target (marsz_t) object input 1 pointer
    - pIn2: target (marsz_t) object input 2 pointer

    Do:
    - Do (pOut) = (pIn1) - (pIn2),

    Return:
    - return 1 if last carry was 1, else return 0

    Other info:
    - nope
    */
    // check exception condition:
    if((!pOut) || (!pIn1) || (!pIn2)){
        // exception: pOut is NULL OR pIn1 is NULL OR pIn2 is NULL
        return 0;
    }
    // else:
    marsword_t s = 0; // sum
    marsword_t b = 0; // borrow
    marsword_t * oldData = NULL;
    marsword_t * newData = NULL;
    int32_t tempIdx = 0;
    int32_t compareResult = 0;
    int32_t estimatedWords = 0;
    int32_t nonZeroIdx = -1;
    marsz_t t1;
    marsz_t tBig;
    marsz_t tSmall;
    marszptr_t pSmall = NULL;

    if(Marsz_Sgn(pIn1) == Marsz_Sgn(pIn2)){
        // case1. pIn1.sign == pIn2.sign: 절댓값 뺄셈 수행
        Marsz_Init(tBig);
        Marsz_Init(tSmall);

        compareResult = Marsz_CompareAbs(pIn1, pIn2);
        if(compareResult == CONST_SIGN_POSITIVE){
            // ABS(pIn1) > ABS(pIn2):
            Marsz_Assign(tBig, pIn1); // tBig = pIn1
            Marsz_Assign(tSmall, pIn2); // tSmall = pIn2
            pSmall = pIn2;
        }else if(compareResult == CONST_SIGN_NEGATIVE){
            // ABS(pIn1) < ABS(pIn2):
            Marsz_Assign(tBig, pIn2); // tBig = pIn2
            Marsz_Assign(tSmall, pIn1); // tSmall = pIn1
            pSmall = pIn1;
        }else{
            // ABS(pIn1) == ABS(pIn2):
            Marsz_Assign(tBig, pIn1); // tBig = pIn1
            Marsz_Assign(tSmall, pIn2); // tSmall = pIn2
            pSmall = NULL;
        }
        
        Marsz_Init(t1);
        estimatedWords = ABS(tBig->allocated);

        // reallocate t1->pData to estimatedWords:
        newData = (marsword_t *)malloc((sizeof(marsword_t) * estimatedWords));
        Marsh_Zeroize((void *)(newData), (sizeof(marsword_t) * estimatedWords));
        free(t1->pData);
        t1->pData = newData;
        t1->allocated = estimatedWords;
        
        // s = a - b - c_in
        // ( (s < a) || (c_in && (s == a)) ) then underflow
        // 0 <= a <= UINT_MAX
        // 0 <= b <= UINT_MAX
        // 0 <= c_in <= 1
        // a와 b의 대소관계 정해지지 않음.
        //

        // substraction:
        for(int32_t i=0; i<(ABS(tSmall->allocated)); i++){
            tempIdx = i;
            s = *((tBig->pData) + tempIdx) - *((tSmall->pData) + tempIdx) - b;
            if((s > *((tBig->pData) + tempIdx)) || (b && (s == *((tBig->pData) + tempIdx)))){
                // underflow detected:
                b = 1;
            }else{
                b = 0;
            }
            *((t1->pData) + tempIdx) = s;
        }

        // substract borrow to rest of bigger one:
        for(int32_t i=0; i<((ABS(tBig->allocated)) - (ABS(tSmall->allocated))); i++){
            tempIdx = i + (ABS(tSmall->allocated));
            s = *((tBig->pData) + tempIdx) - b;
            if(s > *((tBig->pData) + tempIdx)){
                // underflow detected:
                b = 1;
            }else{
                b = 0;
            }
            *((t1->pData) + tempIdx) = s;
        }

        // remove zero filled word:
        for(int32_t i=(ABS(t1->allocated) - 1); i>=0; i--){
            if(*((t1->pData) + i)){
                nonZeroIdx = i;
                break;
            }
        }
        if(nonZeroIdx != -1){
            newData = (marsword_t *)malloc((sizeof(marsword_t) * (nonZeroIdx + 1)));
            Marsh_Zeroize((void *)(newData), (sizeof(marsword_t) * (nonZeroIdx + 1)));
            Marsh_Memcpy((void *)(newData), (void *)(t1->pData), (sizeof(marsword_t) * (nonZeroIdx + 1)));
            free(t1->pData);
            t1->pData = newData;
            t1->allocated = (nonZeroIdx + 1);
        }

        if(pSmall == pIn1){
            t1->allocated = (CONST_SIGN_NEGATIVE) * (t1->allocated);
        }

        // t1 to pOut:
        Marsz_Assign(pOut, t1); // pOut = t1
        Marsz_Final(tBig);
        Marsz_Final(tSmall);
        Marsz_Final(t1);
    }else{
        // case 2. pIn1.sign == pIn2.sign: 절댓값 덧셈 수행
        b = Marsz_Add(pOut, pIn1, pIn2);
    } 
    // return:
    return b;
}

MARS_API_EXPORT int32_t Marsz_Mul(
    marszptr_t pOut,
    marszptr_t pIn1,
    marszptr_t pIn2
)
{
    /*
    int32_t Marsz_Mul(
        marszptr_t pOut,
        marszptr_t pIn1,
        marszptr_t pIn2
    )

    Arg:
    - pOut: target (marsz_t) object output pointer
    - pIn1: target (marsz_t) object input 1 pointer
    - pIn2: target (marsz_t) object input 2 pointer

    Do:
    - Do (pOut) = (pIn1) * (pIn2),

    Return:
    - (NO RETURN)

    Other info:
    - nope
    */
    // check exception condition:
    if((!pOut) || (!pIn1) || (!pIn2)){
        // exception: pOut is NULL OR pIn1 is NULL OR pIn2 is NULL
        return 0;
    }
    // else:


    ///*
    // 구현 2: naive 구현 2: 참을만함
    marsz_t marsn_temp;
    marsz_t marsn_result;

    Marsz_Init(marsn_temp);
    Marsz_Init(marsn_result);

    //
    Marsz_Assign(marsn_temp, pIn2); // marsn_temp = pIn2
    for(int32_t i=0; i<(sizeof(marsword_t) * ABS(pIn1->allocated) << 3); i++){
        if(((*((pIn1->pData) + (i / (sizeof(marsword_t) << 3))) >> (i % (sizeof(marsword_t) << 3))) & 1) == 1){
            Marsz_Add(marsn_result, marsn_result, marsn_temp); // marsn_result += marsn_temp
        }
        if(i < ((sizeof(marsword_t) * ABS(pIn1->allocated) << 3) - 1)){
            // 마지막 루프시 (marsn_temp *= 2) 생략
            Marsz_Add(marsn_temp, marsn_temp, marsn_temp); // marsn_temp *= 2
        }
    }
    Marsz_Assign(pOut, marsn_result); // pOut = marsn_result
    
    // clear:
    Marsz_Final(marsn_temp);
    Marsz_Final(marsn_result);
    //*/
    //

    /*
    // 구현 1: naive 구현 1: 너무 느림
    marsz_t marsn_i;
    marsz_t marsn_result;

    Marsz_Init(marsn_i);
    Marsz_Init(marsn_result);

    // (pIn2) times: marsn_result += (pIn1)
    for(Marsz_Assign(marsn_i, mars_zero); (Marsz_Compare(pIn2, marsn_i) == 1); (Marsz_Add(marsn_i, marsn_i, mars_one))){
        Marsz_Add(marsn_result, marsn_result, pIn1); // marsn_result += pIn1
    }
    Marsz_Assign(pOut, marsn_result); // pOut = marsn_result
    // clear:
    Marsz_Final(marsn_i);
    Marsz_Final(marsn_result);
    */
    //

    // end:
    return 0;
}

MARS_API_EXPORT int32_t Marsz_Div(
    marszptr_t pOut1,
    marszptr_t pOut2,
    marszptr_t pIn1,
    marszptr_t pIn2
)
{
    /*
    int32_t Marsz_Div(
        marszptr_t pOut1,
        marszptr_t pOut2,
        marszptr_t pIn1,
        marszptr_t pIn2
    )

    Arg:
    - pOut1: target (marsz_t) object output 1 pointer
    - pOut2: target (marsz_t) object output 2 pointer
    - pIn1: target (marsz_t) object input 1 pointer
    - pIn2: target (marsz_t) object input 2 pointer

    Do:
    - Do (pIn1) / (pIn2), q = (pOut1), r = (pOut2)

    Return:
    - (NO RETURN)

    Other info:
    - nope
    */
    // check exception condition:
    if((!pOut1) || (!pOut2) || (!pIn1) || (!pIn2)){
        // exception: pOut1 is NULL OR pOut2 is NULL OR pIn1 is NULL OR pIn2 is NULL
        return 0;
    }
    // else:
    if(Marsz_Compare(pIn2, mars_zero) == 0){
        // exception: pIn2 == 0, division by zero
        return 0;
    }

    //
    ///*
    // 구현 2: restoring division

    //R := N
    //D := D << n            -- R and D need twice the word width of N and Q
    //for i := n − 1 .. 0 do  -- For example 31..0 for 32 bits
    //R := 2 * R − D          -- Trial subtraction from shifted value (multiplication by 2 is a shift in binary representation)
    //if R >= 0 then
    //    q(i) := 1          -- Result-bit 1
    //else
    //    q(i) := 0          -- Result-bit 0
    //    R := R + D         -- New partial remainder is (restored) shifted value
    //end
    //end
    //
    //-- Where: N = numerator, D = denominator, n = #bits, R = partial remainder, q(i) = bit #i of quotient
    //

    marsz_t marsn_q, marsn_r, marsn_d, marsn_t1;
    int32_t n = Marsh_GetDigitsInBits_LE(pIn1->pData, (sizeof(marsword_t) * ABS(pIn1->allocated)));

    Marsz_Init(marsn_q);
    Marsz_Init(marsn_r);
    Marsz_Init(marsn_d);
    Marsz_Init(marsn_t1);
    
    Marsz_Assign(marsn_t1, mars_one); // marsn_t1 = 1
    Marsz_BitwiseLeftShift(marsn_t1, marsn_t1, n-1); // marsn_t1 = 2 ^ (n-1)

    Marsz_Assign(marsn_r, pIn1); // marsn_r = pIn1
    Marsz_BitwiseLeftShift(marsn_d, pIn2, n); // marsn_d = pIn2 << n
    for(int32_t i=(n-1); i>=0; i--){
        // marsn_r = 2 * marsn_r - marsn_d :
        Marsz_Add(marsn_r, marsn_r, marsn_r); // marsn_r = 2 * marsn_r
        Marsz_Sub(marsn_r, marsn_r, marsn_d); // marsn_r = marsn_r - marsn_d

        if(Marsz_Compare(marsn_r, mars_zero) >= 0){
            //Marsz_BitwiseOr(marsn_q, marsn_q, marsn_t1); // q[bit i] = 1
            Marsz_Add(marsn_q, marsn_q, marsn_t1); // q[bit i] = 1
        }else{
            Marsz_Add(marsn_r, marsn_r, marsn_d); // marsn_r = marsn_r + marsn_d
        }
        Marsz_BitwiseRightShift(marsn_t1, marsn_t1, 1); // marsn_t1 = marsn_t1 >> 1
    }
    Marsz_BitwiseRightShift(marsn_r, marsn_r, n); // marsn_r = marsn_r >> n
    Marsz_Assign(pOut1, marsn_q); // pOut1 = marsn_q
    Marsz_Assign(pOut2, marsn_r); // pOut2 = marsn_r
    
    Marsz_Final(marsn_q);
    Marsz_Final(marsn_r);
    Marsz_Final(marsn_d);
    Marsz_Final(marsn_t1);
    //*/
    //

    /*
    // 구현 1: naive 구현 1: 너무 느림
    marsz_t marsn_temp;
    marsz_t marsn_q, marsn_r;

    Marsz_Init(marsn_temp);
    Marsz_Init(marsn_q);
    Marsz_Init(marsn_r);

    Marsz_Assign(marsn_temp, pIn1); // marsn_temp = pIn1
    while(Marsz_Compare(marsn_temp, pIn2) > 0){
        //
        Marsz_Sub(marsn_temp, marsn_temp, pIn2); // marsn_temp -= pIn2
        Marsz_Add(marsn_q, marsn_q, mars_one); // marsn_q += 1
    }
    Marsz_Assign(marsn_r, marsn_temp); // marsn_r = marsn_temp

    Marsz_Assign(pOut1, marsn_q); // pOut1 = marsn_q
    Marsz_Assign(pOut2, marsn_r); // pOut2 = marsn_r

    Marsz_Final(marsn_temp);
    Marsz_Final(marsn_q);
    Marsz_Final(marsn_r);    
    */
    //

    // end:
    return 0;
}
// end code