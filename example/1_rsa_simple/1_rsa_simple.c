/*
1_rsa_simple.c

created: 2026.09.30
last modified: 2026.10.08
author: minpie
last modify: minpie
version: 1.0.0

*/
// start code:
// include:
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "mars.h"

// define:
#define MAX_N_BYTES 256


// for dev:
void Marsh_DbgPrintHex_BE(void * pData, int32_t len){
    // print as big endian.
    for(int32_t i=len-1; i>=0; i--){
        if((i != len-1) && (!((i + 1) % 8))){
            printf(" ");
        }
        printf("%02x", ((uint8_t *)pData)[i]);
    }
    return;
}

void Marsh_DbgPrintHex_LE(void * pData, int32_t len){
    // print as little endian.
    for(int32_t i=0; i<len; i++){
        if(i && (!(i % 8))){
            printf(" ");
        }
        printf("%02x", ((uint8_t *)pData)[i]);
    }
    return;
}

#define DbgPrintMarsz(pIn) {\
    printf("%s", (((pIn)->allocated < 0) ? "-" : "+"));\
    Marsh_DbgPrintHex_BE((pIn)->pData, (sizeof(marsword_t) * ABS((pIn)->allocated))); \
}
//

#define TestPrintHex(pData, len) Marsh_DbgPrintHex_LE((void *)(pData), (uint32_t)(len))
// end for dev

// function:
void GetRandom(uint8_t *pOut, uint32_t nOfBytes){
    for(uint32_t i=0; i<nOfBytes; i++){
        *(pOut + i) = (uint8_t)(rand() & 0xff);
    }
    return;
}

int32_t decimal_string_to_bytes(unsigned char* bytes, const char* dec_str, int32_t max_bytes) {
    int32_t len = strlen(dec_str);
    
    // Make a mutable copy of the string to perform in-place division
    char* copy = malloc(len + 1);
    strcpy(copy, dec_str);

    int32_t byte_count = 0;
    int32_t start_idx = 0;

    // Skip leading zeros
    while (start_idx < len && copy[start_idx] == '0') {
        start_idx++;
    }

    // Process the string until it is completely divided down to 0
    while (start_idx < len && byte_count < max_bytes) {
        unsigned int remainder = 0;

        // Perform standard base-10 to base-256 long division
        for (int32_t i = start_idx; i < len; i++) {
            unsigned int current = remainder * 10 + (copy[i] - '0');
            copy[i] = (current / 256) + '0';
            remainder = current % 256;
        }

        // Store the remainder as the current byte value
        bytes[byte_count++] = (unsigned char)remainder;

        // Advance start index if the leading characters became '0'
        while (start_idx < len && copy[start_idx] == '0') {
            start_idx++;
        }
    }

    free(copy);

    // Reverse the array to convert it to standard Big-Endian format
    for (int32_t i = 0; i < byte_count / 2; i++) {
        unsigned char temp = bytes[i];
        bytes[i] = bytes[byte_count - 1 - i];
        bytes[byte_count - 1 - i] = temp;
    }

    return byte_count;
}

void GetGcd(marszptr_t result, marszptr_t a, marszptr_t b)
{
    // ## a, b의 GCD(최대공약수) 구하는 함수
    // 유클리드 알고리즘 사용
    marsz_t r, r1, r2, q, tmp1, tmp2;
    Marsz_Inits(r, r1, r2, q, tmp1, tmp2, NULL);

    Marsz_Assign(r1, a);
    Marsz_Assign(r2, b);
    while (Marsz_Compare(r2, (marszptr_t)mars_zero) > 0)
    {
        Marsz_Div(q, tmp2, r1, r2); // q = r1 / r2
        Marsz_Mul(tmp1, q, r2);  // tmp1 = q * r2
        Marsz_Sub(r, r1, tmp1);  // r = r1 - tmp1 = r1 - (q * r2)
        Marsz_Assign(r1, r2);       // r1 = r2
        Marsz_Assign(r2, r);        // r2 = r
    }
    Marsz_Assign(result, r1); // result = r1

    Marsz_Finals(r, r1, r2, q, tmp1, tmp2, NULL);
}
void GetModuloMultiplicativeInverse(marszptr_t a_1, marszptr_t n, marszptr_t a)
{
    // ## 모듈로 곱셈 역 구하기
    // 확장 유클리드 알고리즘 사용
    marsz_t q, r1, r2, r, t, t1, t2, tmp1, tmp2, tmp3;
    Marsz_Inits(q, r1, r2, r, t, t1, t2, tmp1, tmp2, tmp3, NULL);
    Marsz_Assign(r1, n);    // r1 = n;
    Marsz_Assign(r2, a);    // r2 = a
    Marsz_Assign(t1, (marszptr_t)mars_zero); // t1 = 0
    Marsz_Assign(t2, (marszptr_t)mars_one); // t2 = 1

    while (Marsz_Compare(r2, (marszptr_t)mars_zero))
    {
        Marsz_Div(q, tmp3, r1, r2); // q = r1 / r2
        Marsz_Mul(tmp1, q, r2);  // tmp1 = q * r2
        Marsz_Sub(r, r1, tmp1);  // r = r1 - tmp1 = r1 - (q * r2)
        Marsz_Assign(r1, r2);       // r1 = r2
        Marsz_Assign(r2, r);        // r2 = r

        Marsz_Mul(tmp2, q, t2); // tmp2 = q * t2
        Marsz_Sub(t, t1, tmp2); // t = t1 - tmp2 = t1 - (q * t2)
        Marsz_Assign(t1, t2);      // t1 = t2
        Marsz_Assign(t2, t);       // t2 = t
        Marsz_Mod(t2, t2, n);
    }
    Marsz_Assign(a_1, t1);
    if (Marsz_Sgn(a_1) < 0)
    {
        Marsz_Add(a_1, n, a_1);
    }
    Marsz_Finals(q, r1, r2, r, t, t1, t2, tmp1, tmp2, tmp3, NULL);
}
void KeyGeneration(marszptr_t e, marszptr_t d, marszptr_t p, marszptr_t q)
{
    marsz_t n, phi_n, i, tmp1, tmp2, n65537;
    Marsz_Inits(n, phi_n, i, tmp1, tmp2, n65537, NULL);

    // set i=2:
    Marsz_Assign(i, (marszptr_t)mars_one);
    Marsz_Add(i, i, (marszptr_t)mars_one);

    // number generation: 65537
    Marsz_Assign(n65537, (marszptr_t)mars_one);
    Marsz_BitwiseLeftShift(n65537, n65537, 16);
    Marsz_Add(n65537, n65537, (marszptr_t)mars_one);

    // get n , phi(n)
    Marsz_Mul(n, p, q);           // n = p * q;
    Marsz_Sub(tmp1, p, (marszptr_t)mars_one);     // tmp1 = p - 1
    Marsz_Sub(tmp2, q, (marszptr_t)mars_one);     // tmp2 = q - 1
    Marsz_Mul(phi_n, tmp1, tmp2); // phi_n = tmp1 * tmp2 = (p - 1) * (q - 1)
    printf("Done: Get n, phi_n\n");

    // get e
    Marsz_Assign(tmp1, (marszptr_t)mars_zero);
    Marsz_Assign(tmp2, (marszptr_t)mars_zero);
    while (Marsz_Compare(phi_n, i))
    {
        // loop 조건: i < phi_n
        GetGcd(tmp1, phi_n, i); // tmp1 = gcd(phi_n, i)
        if (!Marsz_Compare(tmp1, (marszptr_t)mars_one))
        {
            // gcd(phi_n, i) == 1
            Marsz_Assign(e, i); // e = i
            break;
        }
        Marsz_Add(i, i, (marszptr_t)mars_one); // i++
    }
    Marsz_Assign(e, n65537);
    printf("Done: Get e\n");

    // get d
    GetModuloMultiplicativeInverse(d, phi_n, e);
    printf("Done: Get d\n");

    // end:
    Marsz_Finals(n, phi_n, i, tmp1, tmp2, n65537, NULL);
}

void Encryption(marszptr_t cipher, marszptr_t plain, marszptr_t n, marszptr_t e)
{
    marsz_t t;
    Marsz_Init(t);
    Marsz_ModExp(t, plain, e, n);
    Marsz_Assign(cipher, t);
    Marsz_Final(t);
}

void Decryption(marszptr_t plain, marszptr_t cipher, marszptr_t n, marszptr_t d)
{
    marsz_t t;
    Marsz_Init(t);
    Marsz_ModExp(t, cipher, d, n);
    Marsz_Assign(plain, t);
    Marsz_Final(t);
}

void SimpleRsa(void){
    marsz_t p, q, n, e, d, plain, cipher, plain2;
    uint8_t ba_p[256] = {0, };
    uint8_t ba_q[256] = {0, };
    uint8_t ba_plain[256] = {0, };
    int32_t siz_p, siz_q, siz_plain;
    Marsz_Inits(p, q, n, e, d, plain, cipher, plain2, NULL);


    /*
    // small (p, q):
    // set (p, q, plain):
    siz_p = decimal_string_to_bytes(
        ba_p, "7",
        256
    );
    //
    siz_q = decimal_string_to_bytes(
        ba_q,
        "11",
        256
    );
    //
    siz_plain = decimal_string_to_bytes(ba_plain, "8", 256);
    */
    //

    /*
    // RSA-1024
    // set (p, q, plain):
    siz_p = decimal_string_to_bytes(
        ba_p, "9613034531358350457419158128061542790930984559499621582258315087964794045505647063849125716018034750312098666606492420191808780667421096063354219926661209",
        256
    );
    //
    siz_q = decimal_string_to_bytes(
        ba_q,
        "12060191957231446918276794204450896001555925054637033936061798321731482148483764659215389453209175225273226830107120695604602513887145524969000359660045617",
        256
    );
    //
    siz_plain = decimal_string_to_bytes(ba_plain, "1907081826081826002619041819", 256);
    */
    //

    ///*
    // RSA-2048
    // set (p, q, plain):
    siz_p = decimal_string_to_bytes(
        ba_p, "162767522504398096700608603303914433434285313162180266043101535901146710207856631259333917077282579901902838218297910729355005714361109839042536261726920048798834178891150594198971426540220155487831599961920043561527980827220211072839096273332284801329293267386003947055994744489902248503660351138917763528831",
        256
    );
    //
    siz_q = decimal_string_to_bytes(
        ba_q,
        "144900539479446165890173866061202627272766709570537894773015747666063954074853423752365796080354346798143120877731515283232248233975644249381224455309178721632388864989220142177032256901911582822669278026615177929862018597685876490721410383966755229262347060103463569732575139601234848220110923351792142034863",
        256
    );
    //
    siz_plain = decimal_string_to_bytes(ba_plain, "1907081826081826002619041819", 256);
    //*/
    //

    Marsz_Ba2Bn(p, ba_p, siz_p, 1);
    Marsz_Ba2Bn(q, ba_q, siz_q, 1);
    Marsz_Ba2Bn(plain, ba_plain, siz_plain, 1);

    printf("(p, q):\n");
    printf("p="); DbgPrintMarsz(p); printf("\n");
    printf("q="); DbgPrintMarsz(q); printf("\n");
    printf("\n");

    // get n, e, d
    Marsz_Mul(n, p, q); // n = p = q;
    KeyGeneration(e, d, p, q);


    printf("Public Key (n, e):\n");
    printf("n="); DbgPrintMarsz(n); printf("\n");
    printf("e="); DbgPrintMarsz(e); printf("\n");
    printf("\n");

    printf("Private Key (d):\n");
    printf("d="); DbgPrintMarsz(d); printf("\n");
    printf("\n");

    Encryption(cipher, plain, n, e);
    Decryption(plain2, cipher, n, d);


    printf("Encryption:\n");
    printf("P="); DbgPrintMarsz(plain); printf("\n");
    printf("C="); DbgPrintMarsz(cipher); printf("\n");

    printf("Decryption:\n");
    printf("C="); DbgPrintMarsz(cipher); printf("\n");
    printf("P="); DbgPrintMarsz(plain2); printf("\n");

    printf("\n");
    printf("original plain == decrypted plain: %d\n", !Marsz_Compare(plain, plain2));

    // end:
    Marsz_Inits(p, q, n, e, d, plain, cipher, plain2, NULL);

    // return:
    return;
}

// main():
int main(void){
    SimpleRsa();
    return 0;
}
// end code