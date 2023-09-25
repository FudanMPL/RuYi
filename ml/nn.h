#ifndef NN_H
#define NN_H

#include "../util/Mat.h"
#include "../util/IOManager.h"
#include "../core/Secret_Mul.h"
#include "../core/Secret_Cmp.h"

extern int party;

class Layer
{
public:
    MatrixXu input, delta_input;
    MatrixXu weight, delta_weight;
    virtual void forward(MatrixXu &x, MatrixXu &delta_x) = 0;
    virtual void backward(MatrixXu &delta, MatrixXu &delta_delta) = 0;
    virtual void print() = 0;
};

class LinearLayer : public Layer
{
public:
    LinearLayer(int input_size, int output_size, bool first_layer = false);
    int input_size;
    int output_size;
    bool first_layer;
    void forward(MatrixXu &x, MatrixXu &delta_x);
    void backward(MatrixXu &delta, MatrixXu &delta_delta);
    void print(){}
};

class ReluLayer : public Layer
{
public:
    double time0 = 0, time1 = 0;
    MatrixXu sign, delta_sign;
    void forward(MatrixXu &x, MatrixXu &delta_x);
    void backward(MatrixXu &delta, MatrixXu &delta_delta);
    void print(){
        cout <<"time0 :" << time0 << endl;
        cout <<"time1 :" << time1 << endl;
    }
};

class NN
{
public:
    vector<Layer *> layers;
    vector<double> time;
    static int myrandom(int i);
    static vector<int> random_perm();
    void next_batch(MatrixXu &batch, MatrixXu &delta_batch, int start, vector<int> &perm, MatrixXu &data, MatrixXu &delta_data);
    MatrixXu argmax(MatrixXu &x);
    void add_layer(Layer *layer);
    void forward(MatrixXu &x, MatrixXu &delta_x);
    void backward(MatrixXu &delta, MatrixXu &delta_delta);
    void fit(MatrixXu &x, MatrixXu &delta_x, MatrixXu &y, MatrixXu &delta_y, int epoch);
    static void train_model();
    void test_model();
    void inference(MatrixXu &x, MatrixXu &y);
};

// class NN
// {
// public:

//     static MatrixXu w, y_inf;

//     static int myrandom(int i);
//     static vector<int> random_perm();
//     static void next_batch(MatrixXu &batch, int start, vector<int> &perm, MatrixXu &data);
//     static MatrixXu argmax(MatrixXu &x);
//     static void train_model();
//     static void test_model();
//     static void inference();
// };

#endif // NN_H