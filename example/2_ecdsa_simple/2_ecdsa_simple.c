/*
2_ecdsa_simple.c

created: 2026.10.07
last modified: 2026.10.08
author: minpie
last modify: minpie
version: 1.0.0

- ECDSA C언어 구현
- GF(p)상의 타원곡선에서 함.
*/
// start code:
// include:
#include <stdio.h>
#include <malloc.h>
#include "mars.h"


// #### struct:
// ### GF(p)상의 타원곡선을 정의하는 구조체:
typedef struct EllipticCurveType{
    /*
    사용하는 타원곡선:
    y^2 = x^3 + ax + b
    */
    marsz_t a; // x의 계수 a
    marsz_t b; // 상수항 b
    marsz_t p; // 모듈로 p
}EllipticCurve;


// ### GF(p)상의 점을 정의하는 구조체:
typedef struct PointType{
    EllipticCurve * pCurve; // 사용하는 타원곡선의 포인터
    marsz_t x; // x좌표
    marsz_t y; // y좌표
}Point;


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

//
void mpz_set_str(marszptr_t out, char * in){
    uint8_t temp_ba[512] = {0, };
    decimal_string_to_bytes(temp_ba, (in), 512);
    Marsz_Ba2Bn((out), temp_ba, 512, 1);
}



void mod_neg(marszptr_t out, marszptr_t a, marszptr_t n){
    if (Marsz_Sgn(a) < 0)
    {
        Marsz_Add(out, n, a);
    }
    Marsz_Mod(out, out, n);
    return;
}


// #### 함수 정의:
// ### 연산 함수:
// ##GF(p)상에서 덧셈의 역원 구하는 함수
int GetAdditiveInverse(marszptr_t inv, marszptr_t target, marszptr_t p){
    Marsz_Sub(inv, p, target); // inv = p - target
    mod_neg(inv, inv, p);
    return 1; // 함수 종료
}
// ## GF(p)상에서 곱셈의 역원 구하는 함수
int GetMultiplicativeInverse(marszptr_t inv, marszptr_t target, marszptr_t p){
    // 확장 유클리드 알고리즘 사용
    marsz_t q, r1, r2, r, t, t1, t2, tmp1, tmp2, tmp3;
    Marsz_Inits(q, r1, r2, r, t, t1, t2, tmp1, tmp2, tmp3, NULL);
    Marsz_Assign(r1, p);    // r1 = p;
    Marsz_Assign(r2, target);    // r2 = target
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
        Marsz_Mod(t2, t2, p);
        //mod_neg(t2, t2, p);
    }
    Marsz_Assign(inv, t1);
    Marsz_Mod(inv, inv, p);
    //mod_neg(inv, inv, p);
    Marsz_Finals(q, r1, r2, r, t, t1, t2, tmp1, tmp2, tmp3, NULL); // gmp 변수 지우기
    return 1; // 함수 종료
}
// ### 타원곡선 관련 함수:
// ## 타원곡선 만드는 함수
EllipticCurve * CreateEllipticCurve(marszptr_t a, marszptr_t b, marszptr_t p){
    EllipticCurve * pCurve = NULL;
    pCurve = (EllipticCurve *)malloc(sizeof(EllipticCurve)); // 동적할당
    if(!pCurve){
        // 예외처리: 동적할당 실패
        return NULL; // 함수 종료
    }
    Marsz_Inits(pCurve->a, pCurve->b, pCurve->p, NULL); // gmp 변수 초기화
    Marsz_Assign(pCurve->a, a); // pOut->a = a;
    Marsz_Assign(pCurve->b, b); // pOut->b = b;
    Marsz_Assign(pCurve->p, p); // pOut->p = p;
    return pCurve; // 함수 종료
}
// ## 타원곡선 지우는 함수
int DeleteEllipticCurve(EllipticCurve * ec1){
    if(!ec1){
        // 예외처리: NULL 타원곡선
        return 0; // 함수 종료
    }
    Marsz_Finals(ec1->a, ec1->b, ec1->p, NULL); // gmp 변수 지우기
    free(ec1); // 메모리 반환
    return 1; // 함수 종료
}
// ## 두 타원곡선이 같은지 비교하는 함수
int IsEqualEllipticCurve(EllipticCurve * ec1, EllipticCurve * ec2){
    if((!ec1) || (!ec2)){
        // 예외처리: NULL 타원곡선
        return 0; // 함수 종료
    }
    if((!Marsz_Compare(ec1->a, ec2->a)) && (!Marsz_Compare(ec1->a, ec2->a)) && (!Marsz_Compare(ec1->a, ec2->a))){
        // ec1과 ec2의 모든 요소의 값이 각각 같음:
        return 1; // 함수 종료
    }
    // 같지 않음:
    return 0; // 함수 종료
}
// ### 타원곡선 상의 점 관련 함수:
// ## 점 만드는 함수
Point * CreatePoint(marszptr_t x, marszptr_t y, EllipticCurve * ec1){
    if(!ec1){
        // 예외처리: NULL 타원곡선
        return NULL; // 함수 종료
    }
    Point * pPoint = NULL; 
    pPoint = (Point *)malloc(sizeof(Point)); // 동적할당
    if(!pPoint){
        // 예외처리: 동적할당 실패
        return NULL; // 함수 종료
    }
    Marsz_Inits(pPoint->x, pPoint->y, NULL); // gmp 변수 초기화
    Marsz_Assign(pPoint->x, x); // pPoint->x = x;
    Marsz_Assign(pPoint->y, y); // pPoint->y = y;
    pPoint->pCurve = ec1; // pPoint->pCurve = pCurve
    return pPoint; // 함수 종료
}
// ## 점 지우는 함수
int DeletePoint(Point * p1){
    if(!p1){
        // 예외처리: NULL 점
        return 0; // 함수 종료
    }
    Marsz_Finals(p1->x, p1->y, NULL); // gmp 변수 지우기
    free(p1); // 메모리 반환
    return 1; // 함수 종료
}
// 항등원 점 구하는 함수
Point * GetIdentityPoint(EllipticCurve * ec1){
    if(!ec1){
        // 예외처리: NULL 타원곡선
        return NULL; // 함수 종료
    }
    marsz_t zero, p;
    Point * pPoint = NULL;
    Marsz_Inits(zero, p, NULL); // gmp 변수 초기화
    Marsz_Assign(zero, (marszptr_t)mars_zero); // zero = 0
    Marsz_Assign(p, ec1->p); // p = ec->p
    pPoint = CreatePoint(zero, p, ec1); // 항등원 점 생성
    Marsz_Finals(zero, p, NULL); // gmp 변수 지우기
    return pPoint; // 함수 종료
}
// ## 점 좌표 수정하는 함수
int SetPointCoordinate(marszptr_t x, marszptr_t y, Point * p1){
    if(!p1){
        // 예외처리: NULL 점
        return 0; // 함수 종료
    }
    Marsz_Assign(p1->x, x); // pPoint->x = x;
    Marsz_Assign(p1->y, y); // pPoint->y = y;
    return 1; // 함수 종료
}
// ## 두 점이 같은지 비교하는 함수
int IsEqualPoint(Point * p1, Point * p2){
    if((!p1) || (!p2)){
        // 예외처리: NULL 점
        return 0; // 함수 종료
    }
    if((IsEqualEllipticCurve(p1->pCurve, p2->pCurve)) && (!Marsz_Compare(p1->x, p2->x)) && (!Marsz_Compare(p1->y, p2->y))){
        // p1과 p2의 모든 요소의 값이 각각 같음:
        return 1; // 함수 종료
    }
    // 같지 않음:
    return 0; // 함수 종료
}
// 해당 점이 항등원 점인지 비교하는 함수
int IsIdentityPoint(Point * p1){
    if(!p1){
        // 예외처리: NULL 점
        return 0; // 함수 종료
    }
    Point * identity = GetIdentityPoint(p1->pCurve); // 항등원 점 구하기
    int result = IsEqualPoint(identity, p1);
    DeletePoint(identity); // 점 삭제
    return result; // 함수 종료
}
// 한 점의 덧셈 역을 구하는 함수
Point * GetAdditiveInversePoint(Point * p1){
    if(!p1){
        // 예외처리: NULL 점
        return NULL; // 함수 종료
    }
    marsz_t yInverse;
    Point * pPoint = NULL;
    Marsz_Inits(yInverse, NULL); // gmp 변수 초기화
    GetAdditiveInverse(yInverse, p1->y, p1->pCurve->p); // -y 구하기
    pPoint = CreatePoint(p1->x, yInverse, p1->pCurve); // 덧셈의 역원 점 생성
    Marsz_Finals(yInverse, NULL); // gmp 변수 지우기
    return pPoint; // 함수 종료
}
// 두 점을 더하는 함수
Point * AddPoint(Point * p1, Point * p2){
    // 예외처리:
    if((!p1) || (!p1)){
        // 예외처리: NULL 점
        return NULL; // 함수 종료
    }
    if(!IsEqualEllipticCurve(p1->pCurve, p2->pCurve)){
        // 예외처리: 두 점이 속한 타원곡선이 서로 다름
        return NULL; // 함수 종료
    }


    // 변수 준비:
    Point * p3 = NULL; // NULL 초기화
    Point * identity = GetIdentityPoint(p1->pCurve); // 항등원 점 구해두기
    marsz_t x3, y3, gradient;
    marsz_t tmp1, tmp2, tmp3, tmp4, tmp5;
    Marsz_Inits(x3, y3, gradient, NULL);
    Marsz_Inits(tmp1, tmp2, tmp3, tmp4, tmp5, NULL);
    if(!identity){
        // 예외처리: 항등원 점 계산 실패
        return NULL; // 함수 종료
    }

    // 경우에 따른 처리:
    if(IsEqualPoint(identity, p1) && IsEqualPoint(identity, p2)){
        // case 1: O + O = O
        p3 = CreatePoint(identity->x, identity->y, identity->pCurve);
    }else if((!IsEqualPoint(identity, p1)) && IsEqualPoint(identity, p2)){
        // case 2: P + O = P
        p3 = CreatePoint(p1->x, p1->y, p1->pCurve);
    }else if(IsEqualPoint(identity, p1) && (!IsEqualPoint(identity, p2))){
        // case 3: O + P = P
        p3 = CreatePoint(p2->x, p2->y, p2->pCurve);
    }else if(IsEqualPoint(p1, p2)){
        // case 4: P + P = 2P
        // 기울기 구하기:
        Marsz_Mul(tmp1, p1->x, p1->x); // tmp1 = p1.x * p1.x

        Marsz_Add(tmp2, tmp1, tmp1);
        Marsz_Add(tmp2, tmp2, tmp1);
        //Marsz_Mul_ui(tmp2, tmp1, 3); // tmp2 = tmp1 * 3

        Marsz_Add(tmp3, tmp2, p1->pCurve->a); // tmp3 = tmp2 + a
        Marsz_Add(tmp4, p1->y, p1->y); // tmp4 = p1.y * 2
        GetMultiplicativeInverse(tmp5, tmp4, p1->pCurve->p); // tmp5 = tmp4 ^ -1
        Marsz_Mul(gradient, tmp3, tmp5); // gradient = tmp3 * tmp5
        mod_neg(gradient, gradient, p1->pCurve->p); // gradient = gradient mod p

        // x3 구하기:
        Marsz_Mul(tmp1, gradient, gradient); // tmp1 = gradient * gradient
        GetAdditiveInverse(tmp2, p1->x, p1->pCurve->p); // tmp2 = -(p1.x)
        GetAdditiveInverse(tmp3, p2->x, p1->pCurve->p); // tmp3 = -(p2.x)
        Marsz_Add(x3, tmp1, tmp2); // x3 = tmp1 + tmp2
        Marsz_Add(x3, x3, tmp3); // x3 = x3 + tmp3
        mod_neg(x3, x3, p1->pCurve->p); // x3 = x3 mod p

        // y3 구하기:
        GetAdditiveInverse(tmp1, x3, p1->pCurve->p); // tmp1 = -(x3)
        Marsz_Add(tmp2, p1->x, tmp1); // tmp2 = p1.x + tmp1
        Marsz_Mul(tmp3, gradient, tmp2); // tmp3 = gradient * tmp2
        GetAdditiveInverse(tmp4, p1->y, p1->pCurve->p); // tmp4 = -(p1.y)
        Marsz_Add(y3, tmp3, tmp4); // y3 = tmp3 + tmp4
        mod_neg(y3, y3, p1->pCurve->p); // y3 = y3 mod p

        // 점 생성:
        p3 = CreatePoint(x3, y3, p1->pCurve);
    }else if(Marsz_Compare(p1->x, p2->x) && Marsz_Compare(p1->y, p2->y)){
        // case 5: P + Q = R

        // 기울기 구하기:
        GetAdditiveInverse(tmp1, p1->y, p1->pCurve->p); // tmp1 = -(p1.y)
        Marsz_Add(tmp2, p2->y, tmp1); // tmp2 = (p2.y + tmp1) = (p2.y - p1.y)
        GetAdditiveInverse(tmp3, p1->x, p1->pCurve->p); // tmp3 = -(p1.x)
        Marsz_Add(tmp4, p2->x, tmp3); // tmp4 = (p2.x + tmp3) = (p2.x - p1.x)
        GetMultiplicativeInverse(tmp5, tmp4, p1->pCurve->p); // tmp5 = tmp4^-1 = (p2.x - p1.x) ^ -1
        Marsz_Mul(gradient, tmp2, tmp5); // gradient = tmp2 * tmp5
        mod_neg(gradient, gradient, p1->pCurve->p); // gradient = gradient mod p

        // x3 구하기:
        Marsz_Mul(tmp1, gradient, gradient); // tmp1 = gradient * gradient
        GetAdditiveInverse(tmp2, p1->x, p1->pCurve->p); // tmp2 = -(p1.x)
        GetAdditiveInverse(tmp3, p2->x, p1->pCurve->p); // tmp3 = -(p2.x)
        Marsz_Add(x3, tmp1, tmp2); // x3 = tmp1 + tmp2
        Marsz_Add(x3, x3, tmp3); // x3 = x3 + tmp3
        mod_neg(x3, x3, p1->pCurve->p); // x3 = x3 mod p

        // y3 구하기:
        GetAdditiveInverse(tmp1, x3, p1->pCurve->p); // tmp1 = -(x3)
        Marsz_Add(tmp2, p1->x, tmp1); // tmp2 = p1.x + tmp1
        Marsz_Mul(tmp3, gradient, tmp2); // tmp3 = gradient * tmp2
        GetAdditiveInverse(tmp4, p1->y, p1->pCurve->p); // tmp4 = -(p1.y)
        Marsz_Add(y3, tmp3, tmp4); // y3 = tmp3 + tmp4
        mod_neg(y3, y3, p1->pCurve->p); // y3 = y3 mod p

        // 점 생성:
        p3 = CreatePoint(x3, y3, p1->pCurve);
    }else{
        // case 6: P + (-P) = O
        p3 = CreatePoint(identity->x, identity->y, identity->pCurve);
    }



    // 종료 처리:
    DeletePoint(identity); // 항등원 점 지우기
    Marsz_Finals(x3, y3, gradient, NULL); // gmp 변수 지우기
    Marsz_Finals(tmp1, tmp2, tmp3, tmp4, tmp5, NULL); // gmp 변수 지우기
    return p3; // 함수 종료
}
// ## 한 점을 k번 더하는 함수
Point * MultiplicatePoint(Point * p1, marszptr_t k){
    if(!p1){
        // 예외처리: NULL 점
        return NULL; // 함수 종료
    }
    //
    /*
    // method 1: 정직하게 여러번 더하기

    marszptr_t i;
    Point * p2 = GetIdentityPoint(p1->pCurve); // 항등원 점으로 초기화
    Point * pTemp = NULL;
    Marsz_Init(i);
    mpz_set_ui(i, 0); // i = 0

    while(mpz_cmp(k, i) != 0){
        pTemp = AddPoint(p2, p1);
        DeletePoint(p2);
        p2 = pTemp;
        Marsz_Add_ui(i, i, 1); // i++
    }
    Marsz_Final(i);
    return p2;
    */
    //
    ///*
    // method 2:
    int32_t bits_k;
    bits_k = Marsh_GetDigitsInBits_LE((uint8_t *)(k->pData), (ABS(k->allocated) * sizeof(marsword_t)));
    Point * identity = GetIdentityPoint(p1->pCurve);
    Point * res = AddPoint(identity, p1);
    Point * temp1 = NULL;
    for(int32_t i = (bits_k-2); i>=0; i--){
        temp1 = AddPoint(res, res);
        DeletePoint(res);
        res = temp1;
        printf("i = %d\n", i);
        //if(((*((k->pData) + (i / (sizeof(marsword_t) << 3))) >> (i % (sizeof(marsword_t) << 3))) & 1) == 1){
        if(((*((k->pData) + ((bits_k - 2 - i) / (sizeof(marsword_t) << 3))) >> ((bits_k - 2 - i) % (sizeof(marsword_t) << 3))) & 1) == 1){
            temp1 = AddPoint(res, p1);
            DeletePoint(res);
            res = temp1;
        }
    }
    DeletePoint(identity);
    return res;
    //*/
}


// #### main():
int main(void) {
    // Temporary variables
    marsz_t temp1, temp2, temp3, temp4;
    Marsz_Inits(temp1, temp2, temp3, temp4, NULL);

    // ### ECDSA Parameters:
    marsz_t a, b, p;           // Elliptic curve parameters
    marsz_t gx, gy;            // Generator point (G)
    marsz_t qx, qy;            // Public key (Q)
    marsz_t x;                 // Private key
    marsz_t n;                 // Order of the curve
    marsz_t h;                 // Hashed message
    marsz_t k;                 // Deterministic nonce
    marsz_t qx_test, qy_test;  //

    Marsz_Inits(a, b, p, NULL);
    Marsz_Inits(gx, gy, NULL);
    Marsz_Inits(qx, qy, NULL);
    Marsz_Inits(x, n, h, k, NULL);
    Marsz_Inits(qx_test, qy_test, NULL);

    // # Set curve parameters for P-192:
    mpz_set_str(p, "6277101735386680763835789423207666416083908700390324961279");
    //mpz_set_str(a, "-3", 10);
    //mod_neg(a, a, p);
    Marsz_Sub(a, p, (marszptr_t)mars_one);
    Marsz_Sub(a, a, (marszptr_t)mars_one);
    Marsz_Sub(a, a, (marszptr_t)mars_one);
    
    
    mpz_set_str(b, "2455155546008943817740293915197451784769108058161191238065");
    
    // # Set curve order (n):
    mpz_set_str(n, "6277101735386680763835789423176059013767194773182842284081");

    // # Set generator point (G):
    mpz_set_str(gx, "602046282375688656758213480587526111916698976636884684818");
    mpz_set_str(gy, "174050332293622031404857552280219410364023488927386650641");

    // # Set private key (x):
    mpz_set_str(x, "2738091856095668696411541285481651538517235492429819322324");

    // # Set message hash (h):
    mpz_set_str(h, "738270557331375240739390660544819254563247600137");
    // # Deterministic nonce:
    mpz_set_str(k, "1369264563168378552698902663065244063007168579396219633697");
    // 테스트벡터에 있는 Q의 (x, y)값:
    mpz_set_str(qx_test, "4221686972693711597846017334586518767782741265666324032854");
    mpz_set_str(qy_test, "1465749634281639091955516932500199697567738736585249397827");

    // Create elliptic curve
    EllipticCurve *curve = NULL;
    curve = CreateEllipticCurve(a, b, p);

    // Create generator point G
    Point *G = NULL;
    G = CreatePoint(gx, gy, curve);

    // ### Calculate Public Key Q:
    Point *Q = NULL;
    Q = MultiplicatePoint(G, x); // Q = xG

    // Extract calculated Qx and Qy
    Marsz_Assign(qx, Q->x);
    Marsz_Assign(qy, Q->y);

    // Print calculated public key
    printf("[INFO] Calculated public key (Q):\n");
    printf("Qx = "); DbgPrintMarsz(qx); printf("\n");
    printf("Qy = "); DbgPrintMarsz(qy); printf("\n");

    // Validate against test vector (RFC 6979 Appendix A.2.3):
    int qx_valid = !Marsz_Compare(qx, qx_test);
    int qy_valid = !Marsz_Compare(qy, qy_test);
    printf("[INFO] Public key validation:\n");
    printf("Qx valid: %s\n", qx_valid ? "YES" : "NO");
    printf("Qy valid: %s\n", qy_valid ? "YES" : "NO");

    // ### Sign:
    marsz_t r, s; // Signature values
    Marsz_Inits(r, s, NULL);

    // Calculate r = (kG).x mod n
    Point *kG = MultiplicatePoint(G, k);
    mod_neg(r, kG->x, n);

    // Calculate s = k^(-1) * (h + d * r) mod n
    GetMultiplicativeInverse(temp1, k, n); // temp1 = k^(-1) mod n
    Marsz_Mul(temp2, x, r);                  // temp2 = x * r
    Marsz_Add(temp3, h, temp2);              // temp3 = h + temp2
    Marsz_Mul(temp4, temp3, temp1);          // temp4 = temp3 * temp1
    mod_neg(s, temp4, n);                  // s = temp4 mod n

    // Print signature
    printf("[INFO] Signature:\n");
    printf("r = "); DbgPrintMarsz(r); printf("\n");
    printf("s = "); DbgPrintMarsz(s); printf("\n");

    // ### Signature Validation:
    printf("\n[INFO] Starting signature validation...\n");

    // Calculate w = s^(-1) mod n
    marsz_t w, u1, u2;
    Marsz_Inits(w, u1, u2, NULL);
    GetMultiplicativeInverse(w, s, n); // w = s^(-1) mod n

    // Calculate u1 = (h * w) mod n
    Marsz_Mul(u1, h, w);
    mod_neg(u1, u1, n);

    // Calculate u2 = (r * w) mod n
    Marsz_Mul(u2, r, w);
    mod_neg(u2, u2, n);

    // Calculate u1G + u2Q
    Point *u1G = MultiplicatePoint(G, u1); // u1G
    Point *u2Q = MultiplicatePoint(Q, u2); // u2Q
    Point *R = AddPoint(u1G, u2Q);       // R = u1G + u2Q

    // Extract x-coordinate of R (Rx)
    marsz_t Rx;
    Marsz_Init(Rx);
    Marsz_Assign(Rx, R->x);

    // Validate: r == Rx mod n
    mod_neg(Rx, Rx, n); // Rx = Rx mod n
    int is_valid = !Marsz_Compare(Rx, r);

    // Print validation result
    printf("[INFO] Signature validation result:\n");
    printf("Signature valid: %s\n", is_valid ? "YES" : "NO");

    // ## Cleanup
    DeletePoint(u1G);
    DeletePoint(u2Q);
    DeletePoint(R);
    DeletePoint(kG);
    DeletePoint(Q);
    DeletePoint(G);
    DeleteEllipticCurve(curve);
    Marsz_Finals(w, u1, u2, Rx, NULL);
    Marsz_Finals(a, b, p, gx, gy, qx, qy, x, n, h, k, r, s, temp1, temp2, temp3, temp4, qx_test, qy_test, NULL);

    return 0;
}
// #### END