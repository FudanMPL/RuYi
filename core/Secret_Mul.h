#ifndef SECRET_MUL_H
#define SECRET_MUL_H

// #include "../util/SocketManager.h"
#include "../util/SocketUtil.h"
#include "../util/Mat.h"
#include "../util/Offline.h"
#include "../util/IOManager.h"

extern int party;
extern double time_1;
extern int cnt1;
extern u64 send_size;
extern u64 receive_size;

class Secret_Mul
{
public:
    // Multiply_Triplets(for(B, D)X(D, numclass))
    static MatrixXu r0, q0, t0;
    // Multiply_Triplets(for(D, B)X(B, numclass))
    static MatrixXu r1, q1, t1;
    // Multiply_Triplets(for(B, D)X(D, hiddenDim))
    static MatrixXu r2, q2, t2;
    // Multiply_Triplets(for(B, hiddenDim)X(hiddenDim, numclass))
    static MatrixXu r3, q3, t3;
    // Multiply_Triplets(for(B, numclass)X(numclass, hiddenDim))
    static MatrixXu r4, q4, t4;
    // Multiply_Triplets(for(B, hiddenDim)X(hiddenDim, hiddenDim))
    static MatrixXu r5, q5, t5;
    // Multiply_Triplets(for(D, B)X(B, hiddenDim))
    static MatrixXu r6, q6, t6;
    // Multiply_Triplets(for(hiddenDim, B)X(B, numclass))
    static MatrixXu r7, q7, t7;
    // Multiply_Triplets(for(hiddenDim, B)X(B, hiddenDim))
    static MatrixXu r8, q8, t8;

    static void get_Triplets();                                                                // get Triplets
    static void Mul_truncation(MatrixXu &z);                                                   // for Truncation
    static MatrixXu Multiply(MatrixXu &x, MatrixXu &y, MatrixXu &r, MatrixXu &q, MatrixXu &t); // element-wise multiply
    static MatrixXu Mul(MatrixXu &x, MatrixXu &delta_x, MatrixXu &y, MatrixXu &delta_y, MatrixXu &z, MatrixXu &xy);
    static MatrixXu reveal(MatrixXu &delta, MatrixXu &data);
    static MatrixXu Mul_reveal(MatrixXu &data);
    static MatrixXu CwiseProduct(MatrixXu &x, MatrixXu &y, MatrixXu &r, MatrixXu &q, MatrixXu &t);
    static MatrixXu Element_Wise(MatrixXu &x, MatrixXu &delta_x, MatrixXu &y, MatrixXu &delta_y, MatrixXu &z, MatrixXu &xy);
    static MatrixXu constant_Mul(MatrixXu &x, double d);
    static MatrixXu Constant_Mul(MatrixXu &delta_x, MatrixXu &x, double d);
};

#endif