/*
example3.c

created: 2026.07.03
last modified: 2026.07.03
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
void TestPrintHex(uint8_t * pData, uint32_t len){
    for(uint32_t i=0; i<len; i++){
        if(i && (!(i % 8))){
            printf(" ");
        }
        printf("%02x", pData[i]);
    }
    return;
}


void Test3(uint64_t n){
    int errCnt = 0;
    uint8_t a[MAX_N_BYTES] = {0, };
    uint8_t b[MAX_N_BYTES] = {0, };
    uint8_t c1[MAX_N_BYTES] = {0, };
    uint8_t c2[MAX_N_BYTES] = {0, };
    bnz_t bn_a;
    bnz_t bn_b;
    bnz_t bn_c;
    mpz_t gn_a;
    mpz_t gn_b;
    mpz_t gn_c;

    // init:
    BnzInit(bn_a);
    BnzInit(bn_b);
    BnzInit(bn_c);
    mpz_init(gn_a);
    mpz_init(gn_b);
    mpz_init(gn_c);

    for(uint64_t i=0; i<n; i++){
        for(uint64_t j=0; j<n; j++){
            // reset:
            memset(a, 0, MAX_N_BYTES);
            memset(b, 0, MAX_N_BYTES);
            memset(c1, 0, MAX_N_BYTES);
            memset(c2, 0, MAX_N_BYTES);

            // get digits

            // get (a, b):
            *(((uint64_t *)a) + ((MAX_N_BYTES / sizeof(bnword_t)) - 1)) = i;
            *(((uint64_t *)b) + ((MAX_N_BYTES / sizeof(bnword_t)) - 1)) = j;

            // calculate with M.A.R.S.
            // operation:
            BnzBa2Bn(bn_a, a, MAX_N_BYTES, 1); // bn_a = a
            BnzBa2Bn(bn_b, b, MAX_N_BYTES, 1); // bn_b = b
            BnzAdd(bn_c, bn_a, bn_b); // bn_c = bn_a + bn_b
            BnzBn2Ba(c1, MAX_N_BYTES, bn_c); // c = bn_c
            
            // calculate with GNU GMP
            uint32_t c2_siz = 0;
            // operation:
            mpz_import(gn_a, MAX_N_BYTES, 1, 1, 1, 0, a); // gn_a = a
            mpz_import(gn_b, MAX_N_BYTES, 1, 1, 1, 0, b); // gn_b = b
            mpz_add(gn_c, gn_a, gn_b); // gn_c = gn_a + gn_b
            c2_siz = mpz_sizeinbase(gn_c, 256);
            mpz_export((c2 + (MAX_N_BYTES - c2_siz)), NULL, 1, 1, 1, 0, gn_c); // c2 = gn_c

            // compare:
            if(memcmp(c1, c2, MAX_N_BYTES)){
                // found mismatch:
                errCnt++;
                ///*
                // print error case:
                printf("a  = "); TestPrintHex(a, MAX_N_BYTES); printf("\n");
                //gmp_printf("a2 = %Zx\n", gn_a);
                printf("\n");

                printf("b  = "); TestPrintHex(b, MAX_N_BYTES); printf("\n");
                //gmp_printf("b2 = %Zx\n", gn_b);
                printf("\n");

                printf("c1 = "); TestPrintHex(c1, MAX_N_BYTES); printf("\n");
                printf("c2 = "); TestPrintHex(c2, MAX_N_BYTES); printf("\n");
                //gmp_printf("c2 = %Zx\n", gn_c);
                printf("\n");
                //*/
                //
            }
        }

    }
    // print result:
    printf("total n     = %lu\n", n);
    printf("total error = %d\n", errCnt);
    printf("error rate  = %.4lf\n", (((double)errCnt) / n));

    // final:
    BnzFinal(bn_a);
    BnzFinal(bn_b);
    BnzFinal(bn_c);
    mpz_clear(gn_a);
    mpz_clear(gn_b);
    mpz_clear(gn_c);
    return;
}

// main():
int main(void){
    Test3(10000);
    return 0;
}
// end code