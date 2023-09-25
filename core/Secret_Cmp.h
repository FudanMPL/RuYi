#ifndef SECRET_CMP_H
#define SECRET_CMP_H

#include "../util/Mat.h"
#include "Boolean_Share.h"

extern int party;
extern double time_0;
extern double time_1;
extern int cnt;
extern int cnt1;
extern int size_;

class Secret_Cmp
{
public:
    static MatrixXu offline_delta_x;
    static MatrixXu offline_temp;
    static void get_Share(int row, int col);
    static MatrixXu to_Boolean_Share(MatrixXu &x);
    static MatrixXu get_bool_share_bit(MatrixXu &x);
    static MatrixXu get_bool_share_bit(MatrixXu &x, MatrixXu &delta_x);
    static MatrixXu secret_share(MatrixXu& sign, MatrixXu& x);
    // static MatrixXu secret_share2(MatrixXu& sign, MatrixXu& x);
    static MatrixXu get_sign(MatrixXu &x);
    static MatrixXu get_sign(MatrixXu &bool_bit, MatrixXu &x);
    static MatrixXu get_sign_xor_1(MatrixXu& x);
    static MatrixXu get_sign_xor_1(MatrixXu& bool_bit, MatrixXu& x);
    static MatrixXu Relu(MatrixXu x);
    static MatrixXu Relu(MatrixXu& x, MatrixXu& delta_x);
    static MatrixXu Sigmoid(MatrixXu x);
    static MatrixXu Sigmoid(MatrixXu &x, MatrixXu &delta_x);
};

#endif