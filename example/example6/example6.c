/*
example6.c

created: 2026.07.07
last modified: 2026.07.07
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

/*
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
            BnzSub(bn_c, bn_a, bn_b); // bn_c = bn_a - bn_b
            BnzBn2Ba(c1, MAX_N_BYTES, bn_c); // c = bn_c
            
            // calculate with GNU GMP
            uint32_t c2_siz = 0;
            // operation:
            mpz_import(gn_a, MAX_N_BYTES, 1, 1, 1, 0, a); // gn_a = a
            mpz_import(gn_b, MAX_N_BYTES, 1, 1, 1, 0, b); // gn_b = b
            mpz_sub(gn_c, gn_a, gn_b); // gn_c = gn_a - gn_b
            c2_siz = mpz_sizeinbase(gn_c, 256);
            mpz_export((c2 + (MAX_N_BYTES - c2_siz)), &c2_siz, 1, 1, 1, 0, gn_c); // c2 = gn_c

            // compare:
            if(memcmp(c1, c2, MAX_N_BYTES)){
                // found mismatch:
                errCnt++;
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

void Test_CrossValidation_Strict(uint64_t n) {
    int errAddCnt = 0;
    int errSubCnt = 0;

    // GNU GMP 타입 변수
    mpz_t gn_a, gn_b;
    
    // 4가지 케이스 및 검증을 위한 GMP 타입 결과 변수들
    mpz_t res_case1;  // 1) gn_a + gn_b (순수 GMP)
    mpz_t res_case2;  // 2) gn_a + bn_b (혼합 연산 검증용)
    mpz_t res_case3;  // 3) gn_a - gn_b (순수 GMP)
    mpz_t res_case4;  // 4) gn_a - bn_b (혼합 연산 검증용)
    
    // 내 라이브러리 자체 연산 결과 및 변환 검증용 변수
    bnz_t bn_a, bn_b;
    bnz_t bn_add_res, bn_sub_res; 
    mpz_t gn_bn_add_chk, gn_bn_sub_chk; // bn_ 연산 결과를 GMP로 바꾼 값
    
    // 임시 변수 (bn_b를 GMP로 변환하여 혼합 연산에 사용)
    mpz_t gn_b_converted;

    // 1. 메모리 할당 및 초기화
    mpz_inits(gn_a, gn_b, gn_b_converted, NULL);
    mpz_inits(res_case1, res_case2, res_case3, res_case4, NULL);
    mpz_inits(gn_bn_add_chk, gn_bn_sub_chk, NULL);
    
    BnzInit(bn_a);
    BnzInit(bn_b);
    BnzInit(bn_add_res);
    BnzInit(bn_sub_res);

    for (uint64_t i = 0; i < n; i++) {
        for (uint64_t j = 0; j < n; j++) {
            
            // 기본 값 세팅 (GMP)
            mpz_set_ui(gn_a, i);
            mpz_set_ui(gn_b, j);

            // 변환 함수를 사용해 내 라이브러리 변수(bn_)에 동기화
            Func_Gn2Bn(bn_a, gn_a);
            Func_Gn2Bn(bn_b, gn_b);

            // 혼합 연산(Case 2, 4) 검증을 위해 bn_b를 GMP 변수로 다시 역변환
            Func_Bn2Gn(gn_b_converted, bn_b);

            // ==========================================
            // [ADDITION] 덧셈 관련 케이스 처리 및 검증
            // ==========================================
            
            // Case 1) gn_a + gn_b
            mpz_add(res_case1, gn_a, gn_b);

            // Case 2) gn_a + bn_b (bn_b를 변환한 값과 연산)
            mpz_add(res_case2, gn_a, gn_b_converted);

            // [추가 검증] 내 라이브러리 자체 덧셈 연산 수행 (bn_a + bn_b)
            BnzAdd(bn_add_res, bn_a, bn_b);
            Func_Bn2Gn(gn_bn_add_chk, bn_add_res);

            // 3가지 결과(Case 1, Case 2, 내 라이브러리 결과)가 모두 일치하는지 raw 값 비교
            if ((mpz_cmp(res_case1, res_case2) != 0) || (mpz_cmp(res_case1, gn_bn_add_chk) != 0)) {
                errAddCnt++;
                gmp_printf("[Add Mismatch] i=%lu, j=%lu\n", i, j);
                gmp_printf("Case 1 (gn_a + gn_b) : %Zx\n", res_case1);
                gmp_printf("Case 2 (gn_a + bn_b) : %Zx\n", res_case2);
                gmp_printf("Mine   (bn_a + bn_b) : %Zx\n\n", gn_bn_add_chk);
            }

            // ==========================================
            // [SUBTRACTION] 뺄셈 관련 케이스 처리 및 검증
            // ==========================================
            
            // Case 3) gn_a - gn_b
            mpz_sub(res_case3, gn_a, gn_b);

            // Case 4) gn_a - bn_b (bn_b를 변환한 값과 연산)
            mpz_sub(res_case4, gn_a, gn_b_converted);

            // [추가 검증] 내 라이브러리 자체 뺄셈 연산 수행 (bn_a - bn_b)
            BnzSub(bn_sub_res, bn_a, bn_b);
            Func_Bn2Gn(gn_bn_sub_chk, bn_sub_res);

            // 3가지 결과(Case 3, Case 4, 내 라이브러리 결과)가 모두 일치하는지 raw 값 비교
            if ((mpz_cmp(res_case3, res_case4) != 0) || (mpz_cmp(res_case3, gn_bn_sub_chk) != 0)) {
                errSubCnt++;
                gmp_printf("[Sub Mismatch] i=%lu, j=%lu\n", i, j);
                gmp_printf("Case 3 (gn_a - gn_b) : %Zx\n", res_case3);
                gmp_printf("Case 4 (gn_a - bn_b) : %Zx\n", res_case4);
                gmp_printf("Mine   (bn_a - bn_b) : %Zx\n\n", gn_bn_sub_chk);
            }
        }
    }

    // 결과 출력
    uint64_t total_cases = n * n;
    printf("--- Strict Multi-Case Validation Results ---\n");
    printf("Total n            = %lu\n", n);
    printf("Total combinations = %lu\n", total_cases);
    printf("Add Errors (Case 1 vs 2 vs Mine) = %d\n", errAddCnt);
    printf("Sub Errors (Case 3 vs 4 vs Mine) = %d\n", errSubCnt);

    // 메모리 해제
    mpz_clears(gn_a, gn_b, gn_b_converted, NULL);
    mpz_clears(res_case1, res_case2, res_case3, res_case4, NULL);
    mpz_clears(gn_bn_add_chk, gn_bn_sub_chk, NULL);
    
    BnzFinal(bn_a);
    BnzFinal(bn_b);
    BnzFinal(bn_add_res);
    BnzFinal(bn_sub_res);
    
    return;
}

*/
//

void Test_CrossValidation(uint64_t n, uint64_t offset){
    /*
    검증 대상:
    case 1. bn_a + bn_b
    case 2. bn_a - bn_b
    
    검증용:
    case 3. gn_a + gn_b
    case 4. gn_a - gn_b

    */
    //
    uint64_t errCase1 = 0;
    uint64_t errCase2 = 0;
    int flg = 0;

    //
    mpz_t gn_a, gn_b;
    mpz_t gn_case3, gn_case4;
    bnz_t bn_a, bn_b;
    bnz_t bn_case1, bn_case2;
    uint8_t ba_case1[MAX_N_BYTES] = {0, };
    uint8_t ba_case2[MAX_N_BYTES] = {0, };
    uint8_t ba_case3[MAX_N_BYTES] = {0, };
    uint8_t ba_case4[MAX_N_BYTES] = {0, };
    int ba_case3_siz = 0;
    int ba_case4_siz = 0;

    // init:
    mpz_inits(gn_a, gn_b, gn_case3, gn_case4, NULL);
    BnzInit(bn_a);
    BnzInit(bn_b);
    BnzInit(bn_case1);
    BnzInit(bn_case2);

    // operation:
    for(uint64_t i=0; i<n; i++){
        for(uint64_t j=0; j<n; j++){
            // clear:
            memset(ba_case1, 0, MAX_N_BYTES); // ba_case1 = 0
            memset(ba_case2, 0, MAX_N_BYTES); // ba_case2 = 0
            memset(ba_case3, 0, MAX_N_BYTES); // ba_case3 = 0
            memset(ba_case4, 0, MAX_N_BYTES); // ba_case4 = 0
            flg = 0;

            // set gn_a, gn_b:
            mpz_set_ui(gn_a, (offset + i)); // gn_a = offset + i
            mpz_set_ui(gn_b, (offset + j)); // gn_b = offset + j

            // set bn_a, bn_b:
            Func_Gn2Bn(bn_a, gn_a); // bn_a = gn_a
            Func_Gn2Bn(bn_b, gn_b); // bn_b = gn_b

            // calc case1:
            BnzAdd(bn_case1, bn_a, bn_b); // bn_case1 = bn_a + bn_b

            // calc case2:
            BnzSub(bn_case2, bn_a, bn_b); // bn_case2 = bn_a - bn_b

            // calc case3:
            mpz_add(gn_case3, gn_a, gn_b); // gn_case3 = gn_a + gn_b

            // calc case4:
            mpz_sub(gn_case4, gn_a, gn_b); // gn_case4 = gn_a - gn_b

            // convert:
            BnzBn2Ba(ba_case1, MAX_N_BYTES, bn_case1);
            BnzBn2Ba(ba_case2, MAX_N_BYTES, bn_case2);
            ba_case3_siz = mpz_sizeinbase(gn_case3, 256);
            ba_case4_siz = mpz_sizeinbase(gn_case4, 256);
            mpz_export((ba_case3 + (MAX_N_BYTES - ba_case3_siz)), NULL, 1, 1, 1, 0, gn_case3); // ba_case3 = gn_case3
            mpz_export((ba_case4 + (MAX_N_BYTES - ba_case4_siz)), NULL, 1, 1, 1, 0, gn_case4); // ba_case4 = gn_case4

            // compare:
            if(memcmp(ba_case1, ba_case3, MAX_N_BYTES) != 0){
                // ba_case1 != ba_case3:
                errCase1++;
                printf("error in addition    : i=%lu, j=%lu\n", i, j);
                printf("ba_case1 = "); TestPrintHex(ba_case1, MAX_N_BYTES); printf("\n");
                printf("ba_case3 = "); TestPrintHex(ba_case3, MAX_N_BYTES); printf("\n");
                flg = 1;
            }
            if(memcmp(ba_case2, ba_case4, MAX_N_BYTES) != 0){
                // ba_case2 != ba_case4:
                errCase2++;
                printf("error in substitution: i=%lu, j=%lu\n", i, j);
                printf("ba_case2 = "); TestPrintHex(ba_case2, MAX_N_BYTES); printf("\n");
                printf("ba_case4 = "); TestPrintHex(ba_case4, MAX_N_BYTES); printf("\n");
                flg = 1;
                //gmp_printf("ba_case4 = %0256Zx\n", gn_case4);
                //printf("ba_case4_siz = %d\n", ba_case4_siz);
                //printf("(MAX_N_BYTES - ba_case4_siz) = %d\n", (MAX_N_BYTES - ba_case4_siz));
            }
            if(flg){
                printf("\n");
            }
        }
    }

    // clear:
    mpz_clears(gn_a, gn_b, gn_case3, gn_case4, NULL);
    BnzFinal(bn_a);
    BnzFinal(bn_b);
    BnzFinal(bn_case1);
    BnzFinal(bn_case2);

    // print result:
    printf("Total case               = %lu\n", (n * n));
    printf("Total addition error     = %lu\n", errCase1);
    printf("Total substitution error = %lu\n", errCase2);
    printf("Addition error rate      = %.4lu\n", ((double)errCase1 / (n * n)));
    printf("Substitution error rate  = %.4lu\n", ((double)errCase2 / (n * n)));

    // return:
    return;
}


// main():
int main(void){
    //Test3(10000);
    //Test_CrossValidation_Strict(10000);
    Test_CrossValidation(10000, 0);
    return 0;
}
// end code