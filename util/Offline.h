#ifndef OFFLINE_H
#define OFFLINE_H

#include "Mat.h"

extern int party;

class Offline
{
public:
    static vector<MatrixXu> b0, c0, d0;
    static MatrixXu r0, q0, t0;
    // Multiply_Triplets(for (B, D)X(D, numClass))

    static vector<MatrixXu> b1, c1, d1;
    static MatrixXu r1, q1, t1;
    // Multiply_Triplets(for(D, B)X(B, numClass))

    static vector<MatrixXu> b2, c2, d2;
    static MatrixXu r2, q2, t2;
    // Multiply_Triplets(for(B, D)X(D, hiddenDim))

    static vector<MatrixXu> b3, c3, d3;
    static MatrixXu r3, q3, t3;
    // Multiply_Triplets(for(B, hiddenDim)X(hiddenDim, numClass))

    static vector<MatrixXu> b4, c4, d4;
    static MatrixXu r4, q4, t4;
    // Multiply_Triplets(for(B, numClass)X(numClass, hiddenDim))

    static vector<MatrixXu> b5, c5, d5;
    static MatrixXu r5, q5, t5;
    // Multiply_Triplets(for(B, hiddenDim)X(hiddenDim, hiddenDim))

    static vector<MatrixXu> b6, c6, d6;
    static MatrixXu r6, q6, t6;
    // Multiply_Triplets(for(D, B)X(B, hiddenDim))

    static vector<MatrixXu> b7, c7, d7;
    static MatrixXu r7, q7, t7;
    // Multiply_Triplets(for(hiddenDim, B)X(B, numClass))

    static vector<MatrixXu> b8, c8, d8;
    static MatrixXu r8, q8, t8;
    // Multiply_Triplets(for(hiddenDim, B)X(B, hiddenDim))

    static vector<MatrixXu> trun0, trun_0;
    // truncation for (B, numClass)

    static vector<MatrixXu> trun1, trun_1;
    // truncation for (D, numClass)

    static vector<MatrixXu> trun2, trun_2;
    // truncation for (B, hiddenDim)

    static vector<MatrixXu> trun3, trun_3;
    // truncation for (D, hiddenDim)

    static vector<MatrixXu> trun4, trun_4;
    // truncation for (hiddenDim, numClass)

    static vector<MatrixXu> trun5, trun_5;
    // truncation for (hiddenDim, hiddenDim)

    static vector<MatrixXu> boolean_r_add0, boolean_r_bool0;
    // boolean random (add share and bool share) for (B + B, numClass)

    static vector<MatrixXu> boolean_r_add1, boolean_r_bool1;
    // boolean random (add share and bool share) for (B, hiddenDim)
    static MatrixXu m0, m2;
    static void init();
};
#endif