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
    memset((pIn->pData), 0, (CONST_SIZE_DEFAULT_BNZ_WORDS * sizeof(bnword_t)));
    pIn->allocated = CONST_SIZE_DEFAULT_BNZ_WORDS;
    pIn->used = 0;
    

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
        free(pIn->pData);
    }
    pIn->pData = NULL;
    pIn->allocated = 0;
    pIn->used = 0;

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
    digitsInBytes = BnhGetDigitsInBytes_BE(pBaIn, lenBaIn); // get digits in bytes
    neededWords = (int32_t)(ceil(((double)digitsInBytes) / sizeof(bnword_t))); // get needed words from needed bytes
    neededWords = MAX(neededWords, 1);
    pOut->pData = realloc((void *)(pOut->pData), (sizeof(bnword_t) * neededWords)); // reallocate words
    pOut->allocated = neededWords;
    pOut->used = sign * (digitsInBytes);
    memset((pOut->pData), 0, (sizeof(bnword_t) * neededWords));
    
    if(!(pOut->pData)){
        // exception: failed to realloc()
        return;
    }
    // else:
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
    for(int32_t i=0; ((i<lenBaOut) && (i<ABS(pIn->used))); i++){
        *(pBaOut + lenBaOut - 1 - i) = *(((uint8_t *)(pIn->pData)) + i);
    }
    sign = (((pIn->used) < 0) ? -1 : 1);

    // return:
    return sign; 
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

    for(int32_t i=0; i<(MIN(pIn1->allocated, pIn2->allocated)); i++){
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

MARS_API_EXPORT void BnzAssign(
    bnzptr_t pOut,
    bnzptr_t pIn
){
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
    pOut->pData = realloc((void *)(pOut->pData), (sizeof(bnword_t) * (pIn->allocated)));
    memcpy((void *)(pOut->pData), (void *)(pIn->pData), (sizeof(bnword_t) * (pIn->allocated)));
    pOut->allocated = pIn->allocated;
    pOut->used = pIn->used;

    // return:
    return;
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
    int32_t estimatedWords = 0;
    bnz_t t1;
    bnzptr_t t2 = NULL;
    int32_t tempIdx = 0;
    int32_t resultSign = 0;
    if(((pIn1->used) < 0) == ((pIn2->used) < 0)){
        // case 1. pIn1.sign == pIn2.sign: 그대로 더하기
        if((pIn1->used) < 0){
            // 음수 + 음수:
            resultSign = -1; 
        }else{
            // 양수 + 양수:
            resultSign = 1;
        }
        
        BnzInit(t1);
        estimatedWords = MAX((pIn1->allocated), (pIn2->allocated));

        t1->pData = realloc((void *)(t1->pData), (sizeof(bnword_t) * estimatedWords)); // reallocate words
        t1->allocated = estimatedWords;
        t1->used = MAX(ABS(pIn1->used), ABS(pIn2->used));
        memset((t1->pData), 0, (sizeof(bnword_t) * estimatedWords)); // clear to zero
        
        // s = a + b + c_in
        // ( (s < a) || (c_in && (s == a)) ) then overflow
        // 0 <= a <= UINT_MAX
        // 0 <= b <= UINT_MAX
        // 0 <= c_in <= 1
        // a와 b의 대소관계 정해지지 않음.
        //

        // addition:
        for(int32_t i=0; i<(MIN(pIn1->allocated, pIn2->allocated)); i++){
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
        if((pIn1->allocated) > (pIn2->allocated)){
            t2 = pIn1;
        }else if((pIn1->allocated) < (pIn2->allocated)){
            t2 = pIn2;
        }
        if(t2){
            for(int32_t i=0; i<((MAX(pIn1->allocated, pIn2->allocated)) - (MIN(pIn1->allocated, pIn2->allocated))); i++){
                tempIdx = i + (MIN(pIn1->allocated, pIn2->allocated));
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
        t1->used = resultSign * (BnhGetDigitsInBytes_LE((uint8_t *)(t1->pData), (sizeof(bnword_t) * t1->allocated)));

        if(c){
            t1->pData = realloc((void *)(t1->pData), (sizeof(bnword_t) * (estimatedWords + 1))); // reallocate words
            t1->allocated = estimatedWords + 1;
            *((t1->pData) + tempIdx + 1) = 1;
            t1->used = resultSign * ((sizeof(bnword_t) * ((t1->allocated) - 1)) + 1);
        }

        // t1 to pOut:
        bnword_t * oldData = pOut->pData;
        if(oldData){
            free(oldData);
        }
        pOut->pData = t1->pData;
        pOut->allocated = t1->allocated;
        pOut->used = t1->used;
    }else{
        // case2. pIn1.sign != pIn2.sign: 뺄셈 수행
        c = BnzSub(pOut, pIn1, pIn2);
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
    int32_t estimatedWords = 0;
    bnz_t t1;
    bnzptr_t t2 = NULL;
    int32_t tempIdx = 0;


    bnz_t tBig;
    bnz_t tSmall;
    if(((pIn1->used) < 0) == ((pIn2->used) < 0)){
        // case1. pIn1.sign == pIn2.sign: 뺄셈 수행
        BnzInit(tBig);
        BnzInit(tSmall);

        if(BnzCompare(pIn1, pIn2) == 1){
            // ABS(pIn1) > ABS(pIn2):
            BnzAssign(tBig, pIn1);
            BnzAssign(tSmall, pIn2);
        }else{
            // ABS(pIn1) <= ABS(pIn2):
            BnzAssign(tBig, pIn2);
            BnzAssign(tSmall, pIn1);  
        }
        
        BnzInit(t1);
        estimatedWords = MAX((tBig->allocated), (tSmall->allocated));

        t1->pData = realloc((void *)(t1->pData), (sizeof(bnword_t) * estimatedWords)); // reallocate words
        t1->allocated = estimatedWords;
        t1->used = MAX(ABS(tBig->used), ABS(tSmall->used));
        memset((t1->pData), 0, (sizeof(bnword_t) * estimatedWords)); // clear to zero

        
        // s = a - b - c_in
        // ( (s > a) || (c_in && (s == a)) ) then overflow
        // 0 <= a <= UINT_MAX
        // 0 <= b <= UINT_MAX
        // 0 <= c_in <= 1
        // a와 b의 대소관계 정해지지 않음.
        //

        // substraction:
        for(int32_t i=0; i<(MIN(tBig->allocated, tSmall->allocated)); i++){
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
        if((tBig->allocated) > (tSmall->allocated)){
            t2 = tBig;
        }else if((tBig->allocated) < (tSmall->allocated)){
            t2 = tSmall;
        }
        if(t2){
            for(int32_t i=0; i<((MAX(tBig->allocated, tSmall->allocated)) - (MIN(tBig->allocated, tSmall->allocated))); i++){
                tempIdx = i + (MIN(tBig->allocated, tSmall->allocated));
                s = *((t2->pData) + tempIdx) - b;
                if((s > *((t2->pData) + tempIdx)) || (b && (s == *((t2->pData) + tempIdx)))){
                    // underflow detected:
                    b = 1;
                }else{
                    b = 0;
                }
                *((t1->pData) + tempIdx) = s;
            }
        }
        t1->used = BnhGetDigitsInBytes_LE((uint8_t *)(t1->pData), (sizeof(bnword_t) * t1->allocated));

        if(b){
            t1->pData = realloc((void *)(t1->pData), (sizeof(bnword_t) * (estimatedWords + 1))); // reallocate words
            t1->allocated = estimatedWords + 1;
            *((t1->pData) + tempIdx + 1) = 1;
            t1->used = (sizeof(bnword_t) * ((t1->allocated) - 1)) + 1;
        }

        if(ABS(pIn1->used) < ABS(pIn2->used)){
            t1->used = (-1) * (t1->used);
        }

        //
        BnzFinal(tBig);
        BnzFinal(tSmall);

        // t1 to pOut:
        bnword_t * oldData = pOut->pData;
        if(oldData){
            free(oldData);
        }
        pOut->pData = t1->pData;
        pOut->allocated = t1->allocated;
        pOut->used = t1->used;
    }else{
        // case 2. pIn1.sign != pIn2.sign: 덧셈 수행
        BnzInit(t1);

        if(((pIn1->used) < 0) < ((pIn2->used) < 0)){
            // pIn1: 양수 -> pIn2를 양수로 바꾸어 덧셈:
            BnzAssign(t1, pIn2);
            t1->used = (-1) * (t1->used);
            b = BnzAdd(pOut, pIn1, t1);
        }else{
            // pIn2: 양수 -> pIn1를 양수로 바꾸어 덧셈:
            BnzAssign(t1, pIn1);
            t1->used = (-1) * (t1->used);
            b = BnzAdd(pOut, t1, pIn2);
        }
        BnzFinal(t1);
    }
    // return:
    return b;
}
// end code