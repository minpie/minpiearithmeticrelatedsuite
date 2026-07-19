/*
example7.c

created: 2026.07.07
last modified: 2026.07.19
author: minpie
last modify: minpie
version: 0.0.1

*/
// start code:
// include:
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <stdlib.h>
#include <gmp.h>
#include "mars.h"

// define:
#define MAX_N_BYTES 256

// function:
void TestPrintHex(void * pData, uint32_t len){
    for(uint32_t i=0; i<len; i++){
        if(i && (!(i % 8))){
            printf(" ");
        }
        printf("%02x", ((uint8_t *)pData)[i]);
    }
    return;
}

void GetRandom(uint8_t *pOut, uint32_t nOfBytes){
    for(uint32_t i=0; i<nOfBytes; i++){
        *(pOut + i) = (uint8_t)(rand() & 0xff);
    }
    return;
}

void Func_Bn2Gn(mpz_t gnOut, bnz_t bnIn){
    // bnz_t -> mpz_t
    uint8_t tempArr[MAX_N_BYTES] = {0,};
    BnzBn2Ba(tempArr, MAX_N_BYTES, bnIn); // tempArr = bnIn
    mpz_import(gnOut, MAX_N_BYTES, 1, 1, 1, 0, tempArr); // gnOut = tempArr    
    //
    return;
}

void Func_Gn2Bn(bnz_t bnOut, mpz_t gnIn){
    // mpz_t -> bnz_t
    uint8_t tempArr[MAX_N_BYTES] = {0,};
    int32_t tempSiz = 0;
    int32_t tempSign = 0;
    tempSiz = mpz_sizeinbase(gnIn, 256);
    tempSign = ((mpz_sgn(gnIn) < 0) ? -1 : 1);
    mpz_export((tempArr + (MAX_N_BYTES - tempSiz)), &tempSiz, 1, 1, 1, 0, gnIn); // tempArr = gnIn
    BnzBa2Bn(bnOut, tempArr, MAX_N_BYTES, tempSign); // bnOut = tempArr
    //
    return;
}

void Test_CrossValidation(uint64_t n){
    /*
    검증 대상:
    case 1. bn_a + bn_b
    case 3. bn_a - bn_b
    case 5. bn_a * bn_b
    
    검증용:
    case 2. gn_a + gn_b
    case 4. gn_a - gn_b
    case 6. gn_a * gn_b

    */
    //
    uint64_t errCase1 = 0;
    uint64_t errCase3 = 0;
    uint64_t errCase5 = 0;
    int flg = 0;

    //
    mpz_t gn_a, gn_b;
    mpz_t gn_case2, gn_case4, gn_case6;
    bnz_t bn_a, bn_b;
    bnz_t bn_case1, bn_case3, bn_case5;
    uint8_t a[MAX_N_BYTES] = {0, };
    uint8_t b[MAX_N_BYTES] = {0, };
    uint8_t ba_case1[MAX_N_BYTES] = {0, };
    uint8_t ba_case3[MAX_N_BYTES] = {0, };
    uint8_t ba_case2[MAX_N_BYTES] = {0, };
    uint8_t ba_case4[MAX_N_BYTES] = {0, };
    uint8_t ba_case5[MAX_N_BYTES] = {0, };
    uint8_t ba_case6[MAX_N_BYTES] = {0, };
    int ba_case2_siz = 0;
    int ba_case4_siz = 0;
    int ba_case6_siz = 0;

    // init:
    mpz_inits(gn_a, gn_b, gn_case2, gn_case4, gn_case6, NULL);
    BnzInit(bn_a);
    BnzInit(bn_b);
    BnzInit(bn_case1);
    BnzInit(bn_case3);
    BnzInit(bn_case5);

    // operation:
    for(uint64_t i=0; i<n; i++){
        // clear:
        memset(ba_case1, 0, MAX_N_BYTES); // ba_case1 = 0
        memset(ba_case3, 0, MAX_N_BYTES); // ba_case3 = 0
        memset(ba_case2, 0, MAX_N_BYTES); // ba_case2 = 0
        memset(ba_case4, 0, MAX_N_BYTES); // ba_case4 = 0
        memset(ba_case5, 0, MAX_N_BYTES); // ba_case5 = 0
        memset(ba_case6, 0, MAX_N_BYTES); // ba_case6 = 0
        memset(a, 0, MAX_N_BYTES);
        memset(b, 0, MAX_N_BYTES);
        flg = 0;

        // get digits:
        uint64_t siz_a = 0;
        uint64_t siz_b = 0;
        siz_a = (rand() % MAX_N_BYTES);
        siz_b = (rand() % MAX_N_BYTES);
        GetRandom((a + (MAX_N_BYTES - siz_a)), siz_a);
        GetRandom((b + (MAX_N_BYTES - siz_b)), siz_b);

        //
        BnzBa2Bn(bn_a, a, MAX_N_BYTES, 1);
        BnzBa2Bn(bn_b, b, MAX_N_BYTES, 1);
        mpz_import(gn_a, MAX_N_BYTES, 1, 1, 1, 0, a); // gn_a = a
        mpz_import(gn_b, MAX_N_BYTES, 1, 1, 1, 0, b); // gn_b = b


        // calc case1:
        BnzAdd(bn_case1, bn_a, bn_b); // bn_case1 = bn_a + bn_b

        // calc case3:
        BnzSub(bn_case3, bn_a, bn_b); // bn_case3 = bn_a - bn_b

        // calc case5:
        BnzMul(bn_case5, bn_a, bn_b); // bn_case5 = bn_a * bn_b

        // calc case2:
        mpz_add(gn_case2, gn_a, gn_b); // gn_case2 = gn_a + gn_b

        // calc case4:
        mpz_sub(gn_case4, gn_a, gn_b); // gn_case4 = gn_a - gn_b

        // calc case6:
        mpz_mul(gn_case6, gn_a, gn_b); // gn_case6 = gn_a * gn_b

        // convert:
        BnzBn2Ba(ba_case1, MAX_N_BYTES, bn_case1);
        BnzBn2Ba(ba_case3, MAX_N_BYTES, bn_case3);
        BnzBn2Ba(ba_case5, MAX_N_BYTES, bn_case5);
        ba_case2_siz = mpz_sizeinbase(gn_case2, 256);
        ba_case4_siz = mpz_sizeinbase(gn_case4, 256);
        ba_case6_siz = mpz_sizeinbase(gn_case6, 256);
        mpz_export((ba_case2 + (MAX_N_BYTES - ba_case2_siz)), NULL, 1, 1, 1, 0, gn_case2); // ba_case2 = gn_case2
        mpz_export((ba_case4 + (MAX_N_BYTES - ba_case4_siz)), NULL, 1, 1, 1, 0, gn_case4); // ba_case4 = gn_case4
        mpz_export((ba_case6 + (MAX_N_BYTES - ba_case6_siz)), NULL, 1, 1, 1, 0, gn_case6); // ba_case6 = gn_case6

        // compare:
        if(memcmp(ba_case1, ba_case2, MAX_N_BYTES) != 0){
            // ba_case1 != ba_case2:
            errCase1++;
            //printf("error in addition    : i=%lu, j=%lu\n", i, j);
            printf("ba_case1 = "); TestPrintHex(ba_case1, MAX_N_BYTES); printf("\n");
            printf("ba_case2 = "); TestPrintHex(ba_case2, MAX_N_BYTES); printf("\n");
            flg = 1;
        }
        if(memcmp(ba_case3, ba_case4, MAX_N_BYTES) != 0){
            // ba_case3 != ba_case4:
            errCase3++;
            //printf("error in substitution: i=%lu, j=%lu\n", i, j);
            printf("ba_case3 = "); TestPrintHex(ba_case3, MAX_N_BYTES); printf("\n");
            printf("ba_case4 = "); TestPrintHex(ba_case4, MAX_N_BYTES); printf("\n");
            flg = 1;
        }
        if(memcmp(ba_case5, ba_case6, MAX_N_BYTES) != 0){
            // ba_case5 != ba_case6:
            errCase3++;
            //printf("error in multiplication: i=%lu, j=%lu\n", i, j);
            printf("ba_case5 = "); TestPrintHex(ba_case5, MAX_N_BYTES); printf("\n");
            printf("ba_case6 = "); TestPrintHex(ba_case6, MAX_N_BYTES); printf("\n");
            flg = 1;
        }
        if(flg){
            printf("\n");
        }
    }

    // clear:
    mpz_clears(gn_a, gn_b, gn_case2, gn_case4, gn_case6, NULL);
    BnzFinal(bn_a);
    BnzFinal(bn_b);
    BnzFinal(bn_case1);
    BnzFinal(bn_case3);
    BnzFinal(bn_case5);

    // print result:
    printf("Total case                 = %lu\n", (n));
    printf("Total addition error       = %lu\n", errCase1);
    printf("Total substitution error   = %lu\n", errCase3);
    printf("Total multiplication error = %lu\n", errCase5);
    printf("Addition error rate        = %.4lf\n", ((double)errCase1 / (n)));
    printf("Substitution error rate    = %.4lf\n", ((double)errCase3 / (n)));
    printf("Multiplication error rate  = %.4lf\n", ((double)errCase5 / (n)));

    // return:
    return;
}


// main():
int main(void){
    //Test_CrossValidation(10000000);
    Test_CrossValidation(1000);
    return 0;
}
// end code