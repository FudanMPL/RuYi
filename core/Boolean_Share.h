#ifndef BOOLEAN_SHARE_H
#define BOOLEAN_SHARE_H

// #include "../util/SocketManager.h"
#include "../util/SocketUtil.h"
#include "../util/Mat.h"

extern int party;

class Boolean_Share
{
public:
    static void init();
    static MatrixXu secret_share(MatrixXu &share);
    static Matrix8 secret_share(Matrix8 &share);
    static MatrixXu reveal(MatrixXu &share);
    static Matrix8 reveal(Matrix8 &share);
    static MatrixXu add_reveal(MatrixXu &share);
    static void get_byte_and_triplets();                  // offline阶段生成三元组 目前全部设置为0
    static void get_n_dabits();                           // used for B2A
    static Matrix8 byte_and_byte(Matrix8 &x, Matrix8 &y); // 原本只需要对位操作 但最小操作单位为字节
    static MatrixXu add(MatrixXu &x, MatrixXu &y);
    static MatrixXu bool_and(MatrixXu &x, MatrixXu &y);
    static MatrixXu to_additive_share(MatrixXu &x);
    static MatrixXu secret_share_for_test(MatrixXu &x);
    static void test(); // test
};

#endif