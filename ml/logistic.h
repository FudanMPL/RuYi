#ifndef LOGISTIC_H
#define LOGISTIC_H

#include "../util/Mat.h"
#include "../util/IOManager.h"
#include "../core/Secret_Mul.h"
#include "../core/Secret_Cmp.h"

extern int party;

class Logistic
{
public:
    static MatrixXu w, y_inf, delta_w;

    static int myrandom(int i);
    static vector<int> random_perm();

    static void sigmoid(MatrixXu &x);

    static MatrixXu argmax(MatrixXu &x);

    static void next_batch(MatrixXu &batch, MatrixXu &delta_batch, int start, vector<int> &perm, MatrixXu &data, MatrixXu &delta_data);
    static void train_model();
    static void test_model();
    static void inference();
};

#endif // LOGISTIC_H