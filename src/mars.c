/*
mars.c

created: 2026.02.16
last modified: 2026.09.21
author: minpie
last modify: minpie
version: 1.0.0

*/
// start code:
// include:
#include "mars.h"

// global variable:
const bnword_t _bnword_zero[CONST_SIZE_DEFAULT_BNZ_WORDS] = {0, };
const bnword_t _bnword_one[CONST_SIZE_DEFAULT_BNZ_WORDS] = {1, };
const bnz_t bn_zero = {
    // pData:
    _bnword_zero,
    // allocated:
    (CONST_SIGN_POSITIVE * CONST_SIZE_DEFAULT_BNZ_WORDS)
}; // constant for 0
const bnz_t bn_one = {
    // pData:
    _bnword_one,
    // allocated:
    (CONST_SIGN_POSITIVE * CONST_SIZE_DEFAULT_BNZ_WORDS)
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

#define DbgPrintBnz(pIn) {\
    printf("%s", (((pIn)->allocated < 0) ? "-" : "+"));\
    DbgPrintHex_BE((pIn)->pData, (sizeof(bnword_t) * ABS((pIn)->allocated))); \
}
// end for dev


// function:
// Bnh: utility function:
MARS_API_EXPORT void BnhMemcpy(
    void * pOut,
    void * pIn,
    int32_t len
)
{
    /*
    void BnhMemcpy(
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

    //
    if((!pOut) || (!pIn) || (!len)){
        // exception: pOut is NULL OR pIn is NULL OR len is 0
        return;
    }
    // else:
    memmove(pOut, pIn, (size_t)(ABS(len))); // copy

    // return:
    return;
}

MARS_API_EXPORT void BnhMemset(
    void * pOut,
    uint8_t val,
    int32_t len
)
{
    /*
    void BnhMemset(
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
    //
    if((!pOut) || (!len)){
        // exception: pOut is NULL OR len is 0
        return;
    }
    // else:
    memset(pOut, val, (size_t)(ABS(len))); // fill

    // return:
    return;
}

MARS_API_EXPORT int32_t BnhMemcmp(
    void * pIn1,
    void * pIn2,
    int32_t len
)
{
    /*
    void BnhMemcmp(
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
    //
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

MARS_API_EXPORT void BnhZeroize(
    void * pOut,
    int32_t len
)
{
    /*
    void BnhZeroize(
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
    //
    if((!pOut) || (!len)){
        // exception: pOut is NULL OR len is 0
        return;
    }
    // else:
    memset(pOut, 0, (size_t)(ABS(len))); // zeroize

    // return:
    return;
}

MARS_API_EXPORT int32_t BnhGetDigitsInBytes_LE(
    uint8_t * pBaIn,
    int32_t lenBaIn
)
{
    /*
    int32_t BnhGetDigitsInBytes_LE(
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
    //
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

MARS_API_EXPORT int32_t BnhGetDigitsInBytes_BE(
    uint8_t * pBaIn,
    int32_t lenBaIn
)
{
    /*
    int32_t BnhGetDigitsInBytes_BE(
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
    //
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

MARS_API_EXPORT int32_t BnhGetDigitsInBits_LE(
    uint8_t * pBaIn,
    int32_t lenBaIn
)
{
    /*
    int32_t BnhGetDigitsInBits_LE(
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
    //
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
MARS_API_EXPORT int32_t BnhGetDigitsInBits_BE(
    uint8_t * pBaIn,
    int32_t lenBaIn
)
{
    /*
    int32_t BnhGetDigitsInBits_BE(
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
    //
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

// Bnz: signed integer related function:
MARS_API_EXPORT void BnzInit(
    bnzptr_t pIn
)
{
    /*
    void BnzInit(
        bnzptr_t pIn
    );

    Arg:
    - pIn: target (bnz_t) object pointer

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
    pIn->pData = (bnword_t *)malloc(CONST_SIZE_DEFAULT_BNZ_WORDS * sizeof(bnword_t)); // allocate
    if(!(pIn->pData)){
        // exception: pData is NULL = failed to malloc()
        return;
    }
    // else:
    BnhZeroize((void *)(pIn->pData), (CONST_SIZE_DEFAULT_BNZ_WORDS * sizeof(bnword_t))); // reset to 0
    pIn->allocated = CONST_SIGN_POSITIVE * CONST_SIZE_DEFAULT_BNZ_WORDS;
    

    // return:
    return; 
}

MARS_API_EXPORT void BnzFinal(
    bnzptr_t pIn
)
{
    /*
    void BnzFinal(
        bnzptr_t pIn
    );

    Arg:
    - pIn: target (bnz_t) object pointer

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
        BnhZeroize((void *)(pIn->pData), (ABS(pIn->allocated) * sizeof(bnword_t))); // reset to 0
        free(pIn->pData);
    }
    pIn->pData = NULL;
    pIn->allocated = 0;

    // return:
    return; 
}

MARS_API_EXPORT void BnzBa2Bn(
    bnzptr_t pOut,
    uint8_t * pBaIn,
    int32_t lenBaIn,
    int32_t sign
)
{
    /*
    void BnzBa2Bn(
        bnzptr_t pOut,
        uint8_t * pBaIn,
        int32_t lenBaIn,
        int32_t sign
    );

    Arg:
    - pOut: target (bnz_t) object pointer
    - pBaIn: source (uint8_t) byte array pointer
    - lenBaIn: length of (uint8_t) source array (pBaIn) refers
    - sign: sign of the number, 1 to positive, -1 to negative

    Do:
    - Convert (lenBaIn) bytes in (pBaIn) to target big number (bnz_t) object (pOut) refers.

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
    int32_t digitsInBytes = 0;
    int32_t neededWords = 0;
    bnword_t * newData = NULL;
    digitsInBytes = BnhGetDigitsInBytes_BE(pBaIn, lenBaIn); // get digits in bytes
    neededWords = (digitsInBytes + (sizeof(bnword_t) - 1)) / sizeof(bnword_t); // get needed words from needed bytes
    neededWords = MAX(neededWords, 1);

    newData = (bnword_t *)malloc((sizeof(bnword_t) * neededWords));
    free(pOut->pData);
    pOut->pData = newData;
    if(!(pOut->pData)){
        // exception: failed to realloc()
        return;
    }
    // else:
    pOut->allocated = sign * neededWords;
    BnhZeroize((void *)(pOut->pData), (sizeof(bnword_t) * neededWords)); // reset to 0
    for(int32_t i=0; i<digitsInBytes; i++){
        *(((uint8_t *)(pOut->pData)) + i) = (uint8_t)(*(pBaIn + lenBaIn - 1 - i));
    }

    // return:
    return; 
}

MARS_API_EXPORT int32_t BnzBn2Ba(
    uint8_t * pBaOut,
    int32_t lenBaOut,
    bnzptr_t pIn
)
{
    /*
    int32_t BnzBn2Ba(
        uint8_t * pBaOut,
        int32_t lenBaOut,
        bnzptr_t pIn
    )

    Arg:
    - pBaOut: target (uint8_t) byte array pointer
    - lenBaOut: length of (uint8_t) target array (pBaOut) refers
    - pBaIn: source (bnz_t) object pointer

    Do:
    - Convert (lenBaOut) bytes of big number in (pIn) to target (uint8_t) bytes array (pBaOut) refers.

    Return:
    - sign of the number, 1 to positive, -1 to negative

    Other info:
    - the number data will be stored to (pBaOut) as big-endian.
    */
    //
    if((!pIn) || (!pBaOut) || (!lenBaOut)){
        // exception: pIn is NULL OR pBaOut is NULL OR lenBaOut is zero
        return 0;
    }
    // else:
    int32_t sign = 0;
    for(int32_t i=0; ((i<lenBaOut) && (i<(ABS(pIn->allocated) * sizeof(bnword_t)))); i++){
        *(pBaOut + lenBaOut - 1 - i) = *(((uint8_t *)(pIn->pData)) + i);
    }
    sign = (((pIn->allocated) < 0) ? CONST_SIGN_NEGATIVE : CONST_SIGN_POSITIVE);

    // return:
    return sign; 
}

MARS_API_EXPORT int32_t BnzCompareAbs(
    bnzptr_t pIn1,
    bnzptr_t pIn2
)
{
    /*
    int32_t BnzCompareAbs(
        bnzptr_t pIn1,
        bnzptr_t pIn2
    )

    Arg:
    - pIn1: target (bnz_t) object 1 pointer
    - pIn2: target (bnz_t) object 2 pointer

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
    //
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
        // 워드 수 같음:
        // else:
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

MARS_API_EXPORT int32_t BnzCompare(
    bnzptr_t pIn1,
    bnzptr_t pIn2
)
{
    /*
    int32_t BnzCompare(
        bnzptr_t pIn1,
        bnzptr_t pIn2
    )

    Arg:
    - pIn1: target (bnz_t) object 1 pointer
    - pIn2: target (bnz_t) object 2 pointer

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
    if((pIn1->allocated) > (pIn2->allocated)){
        return CONST_SIGN_POSITIVE;
    }else if((pIn1->allocated) < (pIn2->allocated)){
        return CONST_SIGN_NEGATIVE;
    }
    // else:
    // 부호, 워드 수 같음:
    int absCompared = 0;
    absCompared = BnzCompareAbs(pIn1, pIn2);
    if((pIn1->allocated) < 0){
        // 음수이므로 결과 반전
        if(absCompared == CONST_SIGN_POSITIVE){
            return CONST_SIGN_NEGATIVE;
        }else if(absCompared == CONST_SIGN_NEGATIVE){
            return CONST_SIGN_POSITIVE;
        }
    }
    return absCompared;
}

MARS_API_EXPORT void BnzAssign(
    bnzptr_t pOut,
    bnzptr_t pIn
)
{
    /*
    void BnzAssign(
        bnzptr_t pOut,
        bnzptr_t pIn
    )

    Arg:
    - pOut: target (bnz_t) object output pointer
    - pIn: target (bnz_t) object input pointer

    Do:
    - Do (pOut) = (pIn)

    Return:
    - (NO RETURN)

    Other info:
    - nope
    */
    //

    // return:
    if((!pOut) || (!pIn)){
        // exception: pOut is NULL OR pIn is NULL
        return;
    }
    if(pOut == pIn){
        // exception: pOut == pIn
        return;
    }
    // else:
    bnword_t * newData = NULL;
    newData = (bnword_t *)malloc((sizeof(bnword_t) * ABS(pIn->allocated)));
    BnhZeroize((void *)(newData), (sizeof(bnword_t) * ABS(pIn->allocated)));
    free(pOut->pData);
    pOut->pData = newData;
    BnhMemcpy((void *)(pOut->pData), (void *)(pIn->pData), (sizeof(bnword_t) * ABS(pIn->allocated)));
    pOut->allocated = pIn->allocated;

    // return:
    return;
}

MARS_API_EXPORT int32_t BnzSgn(
    bnzptr_t pIn
)
{
    /*
    int32_t BnzSgn(
        bnzptr_t pIn
    )

    Arg:
    - pIn: target (bnz_t) object pointer

    Do:
    - Check sign of (pIn)
    return 1 if (pIn) >= 0
    return -1 if (pIn) < 0

    Return:
    - (int32_t) value: -1 OR 1

    Other info:
    - nope
    */
    //
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

MARS_API_EXPORT int32_t BnzBitwiseAnd(
    bnzptr_t pOut,
    bnzptr_t pIn1,
    bnzptr_t pIn2
)
{
    /*
    int32_t BnzBitwiseAnd(
        bnzptr_t pOut,
        bnzptr_t pIn1,
        bnzptr_t pIn2
    )

    Arg:
    - pOut: target (bnz_t) object output pointer
    - pIn1: target (bnz_t) object input 1 pointer
    - pIn2: target (bnz_t) object input 2 pointer

    Do:
    - Do (pOut) = (pIn1) & (pIn2)

    Return:
    - (TODO)

    Other info:
    - nope
    */
    //
    if((!pOut) || (!pIn1) || (!pIn2)){
        // exception: pOut is NULL OR pIn1 is NULL OR pIn2 is NULL
        return 0;
    }

    // else:
    for(int32_t i=0; i<(MIN(ABS(pIn1->allocated), ABS(pIn2->allocated))); i++){
        *((pOut->pData) + i) = *((pIn1->pData) + i) & *((pIn2->pData) + i);
    }
    for(int32_t i=(MIN(ABS(pIn1->allocated), ABS(pIn2->allocated))); i<(MAX(ABS(pIn1->allocated), ABS(pIn2->allocated))); i++){
        *((pOut->pData) + i) = 0;
    }

    // return:
    return 0;
}

MARS_API_EXPORT int32_t BnzBitwiseOr(
    bnzptr_t pOut,
    bnzptr_t pIn1,
    bnzptr_t pIn2
)
{
    /*
    int32_t BnzBitwiseOr(
        bnzptr_t pOut,
        bnzptr_t pIn1,
        bnzptr_t pIn2
    )

    Arg:
    - pOut: target (bnz_t) object output pointer
    - pIn1: target (bnz_t) object input 1 pointer
    - pIn2: target (bnz_t) object input 2 pointer

    Do:
    - Do (pOut) = (pIn1) | (pIn2)

    Return:
    - (TODO)

    Other info:
    - nope
    */
    //
    if((!pOut) || (!pIn1) || (!pIn2)){
        // exception: pOut is NULL OR pIn1 is NULL OR pIn2 is NULL
        return 0;
    }

    // else:
    bnzptr_t t1 = NULL;
    bnzptr_t t2 = NULL;
    bnz_t t3;
    BnzInit(t3);
    if(ABS(pIn1->allocated) > ABS(pIn2->allocated)){
        t1 = pIn1;
        t2 = pIn2;
    }else{
        t1 = pIn2;
        t2 = pIn1;
    }

    BnzAssign(t3, t1);
    for(int32_t i=0; i<ABS(t2->allocated); i++){
        *((t3->pData) + i) = *((t1->pData) + i) | *((t2->pData) + i);
    }
    BnzAssign(pOut, t3);

    // return:
    BnzFinal(t3);
    return 0;
}

MARS_API_EXPORT int32_t BnzBitwiseXor(
    bnzptr_t pOut,
    bnzptr_t pIn1,
    bnzptr_t pIn2
)
{
    /*
    int32_t BnzBitwiseXor(
        bnzptr_t pOut,
        bnzptr_t pIn1,
        bnzptr_t pIn2
    )

    Arg:
    - pOut: target (bnz_t) object output pointer
    - pIn1: target (bnz_t) object input 1 pointer
    - pIn2: target (bnz_t) object input 2 pointer

    Do:
    - Do (pOut) = (pIn1) ^ (pIn2)

    Return:
    - (TODO)

    Other info:
    - nope
    */
    //
    if((!pOut) || (!pIn1) || (!pIn2)){
        // exception: pOut is NULL OR pIn1 is NULL OR pIn2 is NULL
        return 0;
    }

    // else:
    bnzptr_t t1 = NULL;
    bnzptr_t t2 = NULL;
    bnz_t t3;
    BnzInit(t3);
    if(ABS(pIn1->allocated) > ABS(pIn2->allocated)){
        t1 = pIn1;
        t2 = pIn2;
    }else{
        t1 = pIn2;
        t2 = pIn1;
    }

    BnzAssign(t3, t1);
    for(int32_t i=0; i<ABS(t2->allocated); i++){
        *((t3->pData) + i) = *((t1->pData) + i) ^ *((t2->pData) + i);
    }
    BnzAssign(pOut, t3);


    // return:
    BnzFinal(t3);
    return 0;
}

MARS_API_EXPORT int32_t BnzBitwiseNot(
    bnzptr_t pOut,
    bnzptr_t pIn
)
{
    /*
    int32_t BnzBitwiseNot(
        bnzptr_t pOut,
        bnzptr_t pIn
    )

    Arg:
    - pOut: target (bnz_t) object output pointer
    - pIn: target (bnz_t) object input pointer

    Do:
    - Do (pOut) = ~(pIn)

    Return:
    - (TODO)

    Other info:
    - nope
    */
    //
    if((!pOut) || (!pIn)){
        // exception: pOut is NULL OR pIn is NULL
        return 0;
    }

    // else:
    bnz_t t1;
    BnzInit(t1);
    for(int32_t i=0; i<(ABS(pIn->allocated)); i++){
        *((t1->pData) + i) = ~(*((pIn->pData) + i));
    }
    BnzAssign(pOut, t1);

    // return:
    BnzFinal(t1);
    return 0;
}

MARS_API_EXPORT int32_t BnzBitwiseLeftShift(
    bnzptr_t pOut,
    bnzptr_t pIn,
    int32_t shift
)
{
    /*
    int32_t BnzBitwiseLeftShift(
        bnzptr_t pOut,
        bnzptr_t pIn,
        int32_t shift
    )

    Arg:
    - pOut: target (bnz_t) object output pointer
    - pIn: target (bnz_t) object input pointer
    - shift: number of positions to shift

    Do:
    - Do (pOut) = (pIn) << (shift)

    Return:
    - (TODO)

    Other info:
    - nope
    */
    //
    if((!pOut) || (!pIn)){
        // exception: pOut is NULL OR pIn is NULL
        return 0;
    }

    // else:
    bnz_t t1;
    BnzInit(t1);

    BnzAssign(t1, pIn);
    for(int32_t i=0; i<shift; i++){
        BnzAdd(t1, t1, t1); // t1 = t1 << 1
    }
    BnzAssign(pOut, t1);
    
    BnzFinal(t1);
    return 0;
}

MARS_API_EXPORT int32_t BnzBitwiseRightShift(
    bnzptr_t pOut,
    bnzptr_t pIn,
    int32_t shift
)
{
    /*
    int32_t BnzBitwiseRightShift(
        bnzptr_t pOut,
        bnzptr_t pIn,
        int32_t shift
    )

    Arg:
    - pOut: target (bnz_t) object output pointer
    - pIn: target (bnz_t) object input pointer
    - shift: number of positions to shift

    Do:
    - Do (pOut) = (pIn) >> (shift)

    Return:
    - (TODO)

    Other info:
    - nope
    */
    //
    if((!pOut) || (!pIn)){
        // exception: pOut is NULL OR pIn is NULL
        return 0;
    }

    if(!shift){
        BnzAssign(pOut, pIn);
        return 0;
    }

    // else:
    bnz_t t1;
    bnword_t * newData = NULL;
    int32_t orgSign = 0;
    int32_t nonZeroIdx = -1;

    BnzInit(t1);
    orgSign = BnzSgn(pIn);
    newData = (bnword_t *)malloc((sizeof(bnword_t) * ABS(pIn->allocated)));
    BnhZeroize((void *)(newData), (sizeof(bnword_t) * ABS(pIn->allocated)));
    free(t1->pData);
    t1->pData = newData;
    t1->allocated = ABS(pIn->allocated);

    for(int32_t i=(ABS(pIn->allocated) - 1); i>=0; i--){
        int32_t srcidx = i;
        int32_t destidx = srcidx - (shift / (sizeof(bnword_t) << 3));

        if(destidx < 0){
            break;
        }
        *((t1->pData) + destidx) |= *((pIn->pData) + srcidx) >> (shift % (sizeof(bnword_t) << 3));
        if(destidx){
            if((shift % (sizeof(bnword_t) << 3))){
                *((t1->pData) + destidx - 1) |= *((pIn->pData) + srcidx) << ((sizeof(bnword_t) << 3) - (shift % (sizeof(bnword_t) << 3)));
            }
        }
    }

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
    if(nonZeroIdx == -2){
        t1->allocated = orgSign * (t1->allocated);
        BnzAssign(pOut, t1);
    }else if(nonZeroIdx != -1){
        newData = (bnword_t *)malloc((sizeof(bnword_t) * (nonZeroIdx + 1)));
        BnhZeroize((void *)(newData), (sizeof(bnword_t) * (nonZeroIdx + 1)));
        BnhMemcpy((void *)(newData), (void *)(t1->pData), (sizeof(bnword_t) * (nonZeroIdx + 1)));
        free(t1->pData);
        t1->pData = newData;
        t1->allocated = orgSign * (nonZeroIdx + 1);
        BnzAssign(pOut, t1);
    }else{
        BnzAssign(pOut, bn_zero);
    }

    BnzFinal(t1);
    return 0;
}

MARS_API_EXPORT int32_t BnzAdd(
    bnzptr_t pOut,
    bnzptr_t pIn1,
    bnzptr_t pIn2
)
{
    /*
    int32_t BnzAdd(
        bnzptr_t pOut,
        bnzptr_t pIn1,
        bnzptr_t pIn2
    )

    Arg:
    - pOut: target (bnz_t) object output pointer
    - pIn1: target (bnz_t) object input 1 pointer
    - pIn2: target (bnz_t) object input 2 pointer

    Do:
    - Do (pOut) = (pIn1) + (pIn2),

    Return:
    - return 1 if last carry was 1, else return 0

    Other info:
    - nope
    */
    //
    if((!pOut) || (!pIn1) || (!pIn2)){
        // exception: pOut is NULL OR pIn1 is NULL OR pIn2 is NULL
        return 0;
    }
    // else:
    bnword_t s = 0; // sum
    bnword_t c = 0; // carry
    bnword_t * oldData = NULL;
    bnword_t * newData = NULL;
    int32_t tempIdx = 0;
    int32_t resultSign = 0;
    int32_t estimatedWords = 0;
    bnz_t t1;
    bnzptr_t t2 = NULL;

    if(BnzSgn(pIn1) == BnzSgn(pIn2)){
        // case 1. pIn1.sign == pIn2.sign: 그대로 더하기
        if((pIn1->allocated) < 0){
            // 음수 + 음수:
            resultSign = CONST_SIGN_NEGATIVE; 
        }else{
            // 양수 + 양수:
            resultSign = CONST_SIGN_POSITIVE;
        }
        
        BnzInit(t1);
        estimatedWords = MAX(ABS(pIn1->allocated), ABS(pIn2->allocated));

        // reallocate t1->pData to estimatedWords:
        newData = (bnword_t *)malloc((sizeof(bnword_t) * estimatedWords));
        BnhZeroize((void *)(newData), (sizeof(bnword_t) * estimatedWords));
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

        // add rest of bigger one:
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
            newData = (bnword_t *)malloc((sizeof(bnword_t) * (estimatedWords + 1)));
            BnhZeroize((void *)(newData), (sizeof(bnword_t) * (estimatedWords + 1)));
            BnhMemcpy((void *)(newData), (void *)(t1->pData), (sizeof(bnword_t) * estimatedWords));
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
        BnzInit(t1);

        if(BnzSgn(pIn1) > BnzSgn(pIn2)){
            // pIn1: 양수 -> pIn2를 양수로 바꾸어 뺄셈:
            BnzAssign(t1, pIn2);
            t1->allocated = (CONST_SIGN_NEGATIVE) * (t1->allocated);
            c = BnzSub(pOut, pIn1, t1);
        }else{
            // pIn2: 양수 -> pIn1를 양수로 바꾸어 뺄셈:
            BnzAssign(t1, pIn1);
            t1->allocated = (CONST_SIGN_NEGATIVE) * (t1->allocated);
            c = BnzSub(pOut, pIn2, t1);
        }
        BnzFinal(t1);
    }

    // return:
    return c;
}

MARS_API_EXPORT int32_t BnzSub(
    bnzptr_t pOut,
    bnzptr_t pIn1,
    bnzptr_t pIn2
)
{
    /*
    int32_t BnzSub(
        bnzptr_t pOut,
        bnzptr_t pIn1,
        bnzptr_t pIn2
    )

    Arg:
    - pOut: target (bnz_t) object output pointer
    - pIn1: target (bnz_t) object input 1 pointer
    - pIn2: target (bnz_t) object input 2 pointer

    Do:
    - Do (pOut) = (pIn1) - (pIn2),

    Return:
    - return 1 if last carry was 1, else return 0

    Other info:
    - nope
    */
    //
    if((!pOut) || (!pIn1) || (!pIn2)){
        // exception: pOut is NULL OR pIn1 is NULL OR pIn2 is NULL
        return 0;
    }
    // else:
    bnword_t s = 0; // sum
    bnword_t b = 0; // borrow
    bnword_t * oldData = NULL;
    bnword_t * newData = NULL;
    int32_t tempIdx = 0;
    int32_t compareResult = 0;
    int32_t estimatedWords = 0;
    int32_t nonZeroIdx = -1;
    bnz_t t1;
    bnz_t tBig;
    bnz_t tSmall;
    bnzptr_t pSmall = NULL;

    if(BnzSgn(pIn1) == BnzSgn(pIn2)){
        // case1. pIn1.sign == pIn2.sign: 절댓값 뺄셈 수행
        BnzInit(tBig);
        BnzInit(tSmall);

        compareResult = BnzCompareAbs(pIn1, pIn2);
        if(compareResult == CONST_SIGN_POSITIVE){
            // ABS(pIn1) > ABS(pIn2):
            BnzAssign(tBig, pIn1);
            BnzAssign(tSmall, pIn2);
            pSmall = pIn2;
        }else if(compareResult == CONST_SIGN_NEGATIVE){
            // ABS(pIn1) < ABS(pIn2):
            BnzAssign(tBig, pIn2);
            BnzAssign(tSmall, pIn1);  
            pSmall = pIn1;
        }else{
            // ABS(pIn1) == ABS(pIn2):
            BnzAssign(tBig, pIn1);
            BnzAssign(tSmall, pIn2);  
            pSmall = NULL;
        }
        
        BnzInit(t1);
        estimatedWords = ABS(tBig->allocated);

        // reallocate t1->pData to estimatedWords:
        newData = (bnword_t *)malloc((sizeof(bnword_t) * estimatedWords));
        BnhZeroize((void *)(newData), (sizeof(bnword_t) * estimatedWords));
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

        // add rest of bigger one:
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
            newData = (bnword_t *)malloc((sizeof(bnword_t) * (nonZeroIdx + 1)));
            BnhZeroize((void *)(newData), (sizeof(bnword_t) * (nonZeroIdx + 1)));
            BnhMemcpy((void *)(newData), (void *)(t1->pData), (sizeof(bnword_t) * (nonZeroIdx + 1)));
            free(t1->pData);
            t1->pData = newData;
            t1->allocated = (nonZeroIdx + 1);
        }

        if(pSmall == pIn1){
            t1->allocated = (CONST_SIGN_NEGATIVE) * (t1->allocated);
        }

        // t1 to pOut:
        BnzAssign(pOut, t1);
        BnzFinal(tBig);
        BnzFinal(tSmall);
        BnzFinal(t1);
    }else{
        // case 2. pIn1.sign == pIn2.sign: 절댓값 덧셈 수행
        b = BnzAdd(pOut, pIn1, pIn2);
    } 
    // return:
    return b;
}

MARS_API_EXPORT int32_t BnzMul(
    bnzptr_t pOut,
    bnzptr_t pIn1,
    bnzptr_t pIn2
)
{
    /*
    int32_t BnzMul(
        bnzptr_t pOut,
        bnzptr_t pIn1,
        bnzptr_t pIn2
    )

    Arg:
    - pOut: target (bnz_t) object output pointer
    - pIn1: target (bnz_t) object input 1 pointer
    - pIn2: target (bnz_t) object input 2 pointer

    Do:
    - Do (pOut) = (pIn1) * (pIn2),

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


    ///*
    // 구현 2: naive 구현 2: 참을만함
    bnz_t bn_temp;
    bnz_t bn_result;

    BnzInit(bn_temp);
    BnzInit(bn_result);

    //
    BnzAssign(bn_temp, pIn2); // bn_temp = pIn2
    for(int32_t i=0; i<(sizeof(bnword_t) * ABS(pIn1->allocated) << 3); i++){
        if(((*((pIn1->pData) + (i / (sizeof(bnword_t) << 3))) >> (i % (sizeof(bnword_t) << 3))) & 1) == 1){
            BnzAdd(bn_result, bn_result, bn_temp); // bn_result += bn_temp
        }
        if(i < ((sizeof(bnword_t) * ABS(pIn1->allocated) << 3) - 1)){
            // 마지막 루프시 (bn_temp *= 2) 생략 위함
            BnzAdd(bn_temp, bn_temp, bn_temp); // bn_temp *= 2
        }
    }
    BnzAssign(pOut, bn_result); // pOut = bn_result
    
    // clear:
    BnzFinal(bn_temp);
    BnzFinal(bn_result);
    //*/
    //

    /*
    // 구현 1: naive 구현 1: 너무 느림
    bnz_t bn_i;
    bnz_t bn_result;

    BnzInit(bn_i);
    BnzInit(bn_result);

    // (pIn2) times: bn_result += (pIn1)
    for(BnzAssign(bn_i, bn_zero); (BnzCompare(pIn2, bn_i) == 1); (BnzAdd(bn_i, bn_i, bn_one))){
        BnzAdd(bn_result, bn_result, pIn1); // bn_result += pIn1
    }
    BnzAssign(pOut, bn_result); // pOut = bn_result
    // clear:
    BnzFinal(bn_i);
    BnzFinal(bn_result);
    */
    //

    // end:
    return 0;
}

MARS_API_EXPORT int32_t BnzDiv(
    bnzptr_t pOut1,
    bnzptr_t pOut2,
    bnzptr_t pIn1,
    bnzptr_t pIn2
)
{
    /*
    int32_t BnzDiv(
        bnzptr_t pOut1,
        bnzptr_t pOut2,
        bnzptr_t pIn1,
        bnzptr_t pIn2
    )

    Arg:
    - pOut1: target (bnz_t) object output 1 pointer
    - pOut2: target (bnz_t) object output 2 pointer
    - pIn1: target (bnz_t) object input 1 pointer
    - pIn2: target (bnz_t) object input 2 pointer

    Do:
    - Do (pIn1) / (pIn2), q = (pOut1), r = (pOut2)

    Return:
    - (NO RETURN)

    Other info:
    - nope
    */
    //
    if((!pOut1) || (!pOut2) || (!pIn1) || (!pIn2)){
        // exception: pOut1 is NULL OR pOut2 is NULL OR pIn1 is NULL OR pIn2 is NULL
        return 0;
    }
    // else:
    if(BnzCompare(pIn2, bn_zero) == 0){
        // exception: pIn2 == 0
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

    bnz_t bn_q, bn_r, bn_d, bn_t1;
    int32_t n = BnhGetDigitsInBits_LE(pIn1->pData, (sizeof(bnword_t) * ABS(pIn1->allocated)));

    BnzInit(bn_q);
    BnzInit(bn_r);
    BnzInit(bn_d);
    BnzInit(bn_t1);
    
    BnzAssign(bn_t1, bn_one); // bn_t1 = 1
    BnzBitwiseLeftShift(bn_t1, bn_t1, n-1); // bn_t1 = 2 ^ (n-1)

    BnzAssign(bn_r, pIn1); // bn_r = pIn1
    BnzBitwiseLeftShift(bn_d, pIn2, n); // bn_d = pIn2 << n
    for(int32_t i=(n-1); i>=0; i--){
        // bn_r = 2 * bn_r - bn_d :
        BnzAdd(bn_r, bn_r, bn_r); // bn_r = 2 * bn_r
        BnzSub(bn_r, bn_r, bn_d); // bn_r = bn_r - bn_d

        if(BnzCompare(bn_r, bn_zero) >= 0){
            //BnzBitwiseOr(bn_q, bn_q, bn_t1); // q[bit i] = 1
            BnzAdd(bn_q, bn_q, bn_t1); // q[bit i] = 1
        }else{
            BnzAdd(bn_r, bn_r, bn_d); // bn_r = bn_r + bn_d
        }
        BnzBitwiseRightShift(bn_t1, bn_t1, 1); // bn_t1 = bn_t1 >> 1
    }
    BnzBitwiseRightShift(bn_r, bn_r, n);
    BnzAssign(pOut1, bn_q); // pOut1 = bn_q
    BnzAssign(pOut2, bn_r); // pOut2 = bn_r
    
    BnzFinal(bn_q);
    BnzFinal(bn_r);
    BnzFinal(bn_d);
    BnzFinal(bn_t1);
    //*/
    //

    /*
    // 구현 1: naive 구현 1: 너무 느림
    bnz_t bn_temp;
    bnz_t bn_q, bn_r;

    BnzInit(bn_temp);
    BnzInit(bn_q);
    BnzInit(bn_r);

    BnzAssign(bn_temp, pIn1); // bn_temp = pIn1
    while(BnzCompare(bn_temp, pIn2) > 0){
        //
        BnzSub(bn_temp, bn_temp, pIn2); // bn_temp -= pIn2
        BnzAdd(bn_q, bn_q, bn_one); // bn_q += 1
    }
    BnzAssign(bn_r, bn_temp); // bn_r = bn_temp

    BnzAssign(pOut1, bn_q); // pOut1 = bn_q
    BnzAssign(pOut2, bn_r); // pOut2 = bn_r

    BnzFinal(bn_temp);
    BnzFinal(bn_q);
    BnzFinal(bn_r);    
    */
    //

    // end:
    return 0;
}

// end code