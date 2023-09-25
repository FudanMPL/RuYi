#include "Offline.h"

vector<MatrixXu> Offline::b0, Offline::c0, Offline::d0;
MatrixXu r0, q0, t0;

vector<MatrixXu> Offline::b1, Offline::c1, Offline::d1;
MatrixXu r1, q1, t1;

vector<MatrixXu> Offline::b2, Offline::c2, Offline::d2;
MatrixXu r2, q2, t2;

vector<MatrixXu> Offline::b3, Offline::c3, Offline::d3;
MatrixXu r3, q3, t3;

vector<MatrixXu> Offline::b4, Offline::c4, Offline::d4;
MatrixXu r4, q4, t4;

vector<MatrixXu> Offline::b5, Offline::c5, Offline::d5;
MatrixXu r5, q5, t5;

vector<MatrixXu> Offline::b6, Offline::c6, Offline::d6;
MatrixXu r6, q6, t6;

vector<MatrixXu> Offline::b7, Offline::c7, Offline::d7;
MatrixXu r7, q7, t7;

vector<MatrixXu> Offline::b8, Offline::c8, Offline::d8;
MatrixXu r8, q8, t8;

vector<MatrixXu> Offline::trun0, Offline::trun_0;
vector<MatrixXu> Offline::trun1, Offline::trun_1;
vector<MatrixXu> Offline::trun2, Offline::trun_2;
vector<MatrixXu> Offline::trun3, Offline::trun_3;
vector<MatrixXu> Offline::trun4, Offline::trun_4;
vector<MatrixXu> Offline::trun5, Offline::trun_5;

vector<MatrixXu> Offline::boolean_r_add0, Offline::boolean_r_bool0;
vector<MatrixXu> Offline::boolean_r_add1, Offline::boolean_r_bool1;

MatrixXu Offline::m2;

MatrixXu Offline::m0;

void Offline::init()
{
    // for (B, D)X(D, numClass)

    b0.resize(Config::config->M);
    c0.resize(Config::config->M);
    d0.resize(Config::config->M);
    for (int i = 0; i < Config::config->M; i++)
    {
        b0[i] = Mat::randomMatrixXu(Config::config->B, Config::config->D);
        c0[i] = Mat::randomMatrixXu(Config::config->D, Config::config->numClass);
        if (i == 0)
        {
            d0[i] = b0[i] * c0[i];
        }
        else
        {
            d0[i] = Mat::randomMatrixXu(Config::config->B, Config::config->numClass);
        }
    }
    Tensor<u64, 3> b_0(Config::config->M, Config::config->B, Config::config->D);
    for (int i = 0; i < Config::config->M; i++)
    {
        for (int j = 0; j < Config::config->B; j++)
        {
            for (int k = 0; k < Config::config->D; k++)
            {
                b_0(i, j, k) = b0[i](j, k);
            }
        }
    }
    Tensor<u64, 3> c_0(Config::config->M, Config::config->D, Config::config->numClass);
    for (int i = 0; i < Config::config->M; i++)
    {
        for (int j = 0; j < Config::config->D; j++)
        {
            for (int k = 0; k < Config::config->numClass; k++)
            {
                c_0(i, j, k) = c0[i](j, k);
            }
        }
    }
    Tensor<u64, 3> d_0(Config::config->M, Config::config->B, Config::config->numClass);
    for (int i = 0; i < Config::config->M; i++)
    {
        for (int j = 0; j < Config::config->B; j++)
        {
            for (int k = 0; k < Config::config->numClass; k++)
            {
                d_0(i, j, k) = d0[i](j, k);
            }
        }
    }

    Tensor<u64, 3> r0, q0, t0;
    Eigen::array<Eigen::IndexPair<u64>, 1> product_dims = {Eigen::IndexPair<u64>(1, 0)};
    r0 = Mat::A_plus.contract(b_0, product_dims);
    q0 = Mat::A_plus.contract(c_0, product_dims);
    t0 = Mat::A_plus.contract(d_0, product_dims);

    vector<ofstream> F0(Config::config->M + Config::config->nd);
    for (int i = 0; i < Config::config->M + Config::config->nd; i++)
    {
        F0[i].open("Mul_Triplets/MT0_" + to_string(i) + ".txt", ios::out);
    }
    for (int i = 0; i < Config::config->M + Config::config->nd; i++)
    {
        for (int j = 0; j < Config::config->B; j++)
        {
            for (int k = 0; k < Config::config->D; k++)
            {
                F0[i] << r0(i, j, k) << ",";
            }
            F0[i] << std::endl;
        }
    }
    for (int i = 0; i < Config::config->M + Config::config->nd; i++)
    {
        for (int j = 0; j < Config::config->D; j++)
        {
            for (int k = 0; k < Config::config->numClass; k++)
            {
                F0[i] << q0(i, j, k) << ",";
            }
            F0[i] << std::endl;
        }
    }
    for (int i = 0; i < Config::config->M + Config::config->nd; i++)
    {
        for (int j = 0; j < Config::config->B; j++)
        {
            for (int k = 0; k < Config::config->numClass; k++)
            {
                F0[i] << t0(i, j, k) << ",";
            }
            F0[i] << std::endl;
        }
    }
    for (int i = 0; i < Config::config->M + Config::config->nd; i++)
    {
        F0[i].close();
    }

    // for (D, B)X(B, numClass)

    b1.resize(Config::config->M);
    c1.resize(Config::config->M);
    d1.resize(Config::config->M);
    for (int i = 0; i < Config::config->M; i++)
    {
        b1[i] = Mat::randomMatrixXu(Config::config->D, Config::config->B);
        c1[i] = Mat::randomMatrixXu(Config::config->B, Config::config->numClass);
        if (i == 0)
        {
            d1[i] = b1[i] * c1[i];
        }
        else
        {
            d1[i] = Mat::randomMatrixXu(Config::config->D, Config::config->numClass);
        }
    }
    Tensor<u64, 3> b_1(Config::config->M, Config::config->D, Config::config->B);
    for (int i = 0; i < Config::config->M; i++)
    {
        for (int j = 0; j < Config::config->D; j++)
        {
            for (int k = 0; k < Config::config->B; k++)
            {
                b_1(i, j, k) = b1[i](j, k);
            }
        }
    }
    Tensor<u64, 3> c_1(Config::config->M, Config::config->B, Config::config->numClass);
    for (int i = 0; i < Config::config->M; i++)
    {
        for (int j = 0; j < Config::config->B; j++)
        {
            for (int k = 0; k < Config::config->numClass; k++)
            {
                c_1(i, j, k) = c1[i](j, k);
            }
        }
    }
    Tensor<u64, 3> d_1(Config::config->M, Config::config->D, Config::config->numClass);
    for (int i = 0; i < Config::config->M; i++)
    {
        for (int j = 0; j < Config::config->D; j++)
        {
            for (int k = 0; k < Config::config->numClass; k++)
            {
                d_1(i, j, k) = d1[i](j, k);
            }
        }
    }

    Tensor<u64, 3> r1, q1, t1;
    r1 = Mat::A_plus.contract(b_1, product_dims);
    q1 = Mat::A_plus.contract(c_1, product_dims);
    t1 = Mat::A_plus.contract(d_1, product_dims);

    vector<ofstream> F1(Config::config->M + Config::config->nd);
    for (int i = 0; i < Config::config->M + Config::config->nd; i++)
    {
        F1[i].open("Mul_Triplets/MT1_" + to_string(i) + ".txt", ios::out);
    }
    for (int i = 0; i < Config::config->M + Config::config->nd; i++)
    {
        for (int j = 0; j < Config::config->D; j++)
        {
            for (int k = 0; k < Config::config->B; k++)
            {
                F1[i] << r1(i, j, k) << ",";
            }
            F1[i] << std::endl;
        }
    }
    for (int i = 0; i < Config::config->M + Config::config->nd; i++)
    {
        for (int j = 0; j < Config::config->B; j++)
        {
            for (int k = 0; k < Config::config->numClass; k++)
            {
                F1[i] << q1(i, j, k) << ",";
            }
            F1[i] << std::endl;
        }
    }
    for (int i = 0; i < Config::config->M + Config::config->nd; i++)
    {
        for (int j = 0; j < Config::config->D; j++)
        {
            for (int k = 0; k < Config::config->numClass; k++)
            {
                F1[i] << t1(i, j, k) << ",";
            }
            F1[i] << std::endl;
        }
    }
    for (int i = 0; i < Config::config->M + Config::config->nd; i++)
    {
        F1[i].close();
    }

    // for(B, D)X(D, hiddenDim)

    b2.resize(Config::config->M);
    c2.resize(Config::config->M);
    d2.resize(Config::config->M);
    for (int i = 0; i < Config::config->M; i++)
    {
        b2[i] = Mat::randomMatrixXu(Config::config->B, Config::config->D);
        c2[i] = Mat::randomMatrixXu(Config::config->D, Config::config->hiddenDim);
        if (i == 0)
        {
            d2[i] = b2[i] * c2[i];
        }
        else
        {
            d2[i] = Mat::randomMatrixXu(Config::config->B, Config::config->hiddenDim);
        }
    }
    Tensor<u64, 3> b_2(Config::config->M, Config::config->B, Config::config->D);
    for (int i = 0; i < Config::config->M; i++)
    {
        for (int j = 0; j < Config::config->B; j++)
        {
            for (int k = 0; k < Config::config->D; k++)
            {
                b_2(i, j, k) = b2[i](j, k);
            }
        }
    }
    Tensor<u64, 3> c_2(Config::config->M, Config::config->D, Config::config->hiddenDim);
    for (int i = 0; i < Config::config->M; i++)
    {
        for (int j = 0; j < Config::config->D; j++)
        {
            for (int k = 0; k < Config::config->hiddenDim; k++)
            {
                c_2(i, j, k) = c2[i](j, k);
            }
        }
    }
    Tensor<u64, 3> d_2(Config::config->M, Config::config->B, Config::config->hiddenDim);
    for (int i = 0; i < Config::config->M; i++)
    {
        for (int j = 0; j < Config::config->B; j++)
        {
            for (int k = 0; k < Config::config->hiddenDim; k++)
            {
                d_2(i, j, k) = d2[i](j, k);
            }
        }
    }

    Tensor<u64, 3> r2, q2, t2;
    r2 = Mat::A_plus.contract(b_2, product_dims);
    q2 = Mat::A_plus.contract(c_2, product_dims);
    t2 = Mat::A_plus.contract(d_2, product_dims);

    vector<ofstream> F2(Config::config->M + Config::config->nd);
    for (int i = 0; i < Config::config->M + Config::config->nd; i++)
    {
        F2[i].open("Mul_Triplets/MT2_" + to_string(i) + ".txt", ios::out);
    }
    for (int i = 0; i < Config::config->M + Config::config->nd; i++)
    {
        for (int j = 0; j < Config::config->B; j++)
        {
            for (int k = 0; k < Config::config->D; k++)
            {
                F2[i] << r2(i, j, k) << ",";
            }
            F2[i] << std::endl;
        }
    }
    for (int i = 0; i < Config::config->M + Config::config->nd; i++)
    {
        for (int j = 0; j < Config::config->D; j++)
        {
            for (int k = 0; k < Config::config->hiddenDim; k++)
            {
                F2[i] << q2(i, j, k) << ",";
            }
            F2[i] << std::endl;
        }
    }
    for (int i = 0; i < Config::config->M + Config::config->nd; i++)
    {
        for (int j = 0; j < Config::config->B; j++)
        {
            for (int k = 0; k < Config::config->hiddenDim; k++)
            {
                F2[i] << t2(i, j, k) << ",";
            }
            F2[i] << std::endl;
        }
    }
    for (int i = 0; i < Config::config->M + Config::config->nd; i++)
    {
        F2[i].close();
    }

    // for(B, hiddenDim)X(hiddenDim, numClass)
    b3.resize(Config::config->M);
    c3.resize(Config::config->M);
    d3.resize(Config::config->M);
    for (int i = 0; i < Config::config->M; i++)
    {
        b3[i] = Mat::randomMatrixXu(Config::config->B, Config::config->hiddenDim);
        c3[i] = Mat::randomMatrixXu(Config::config->hiddenDim, Config::config->numClass);
        if (i == 0)
        {
            d3[i] = b3[i] * c3[i];
        }
        else
        {
            d3[i] = Mat::randomMatrixXu(Config::config->B, Config::config->numClass);
        }
    }
    Tensor<u64, 3> b_3(Config::config->M, Config::config->B, Config::config->hiddenDim);
    for (int i = 0; i < Config::config->M; i++)
    {
        for (int j = 0; j < Config::config->B; j++)
        {
            for (int k = 0; k < Config::config->hiddenDim; k++)
            {
                b_3(i, j, k) = b3[i](j, k);
            }
        }
    }
    Tensor<u64, 3> c_3(Config::config->M, Config::config->hiddenDim, Config::config->numClass);
    for (int i = 0; i < Config::config->M; i++)
    {
        for (int j = 0; j < Config::config->hiddenDim; j++)
        {
            for (int k = 0; k < Config::config->numClass; k++)
            {
                c_3(i, j, k) = c3[i](j, k);
            }
        }
    }
    Tensor<u64, 3> d_3(Config::config->M, Config::config->B, Config::config->numClass);
    for (int i = 0; i < Config::config->M; i++)
    {
        for (int j = 0; j < Config::config->B; j++)
        {
            for (int k = 0; k < Config::config->numClass; k++)
            {
                d_3(i, j, k) = d3[i](j, k);
            }
        }
    }

    Tensor<u64, 3> r3, q3, t3;
    r3 = Mat::A_plus.contract(b_3, product_dims);
    q3 = Mat::A_plus.contract(c_3, product_dims);
    t3 = Mat::A_plus.contract(d_3, product_dims);

    vector<ofstream> F3(Config::config->M + Config::config->nd);
    for (int i = 0; i < Config::config->M + Config::config->nd; i++)
    {
        F3[i].open("Mul_Triplets/MT3_" + to_string(i) + ".txt", ios::out);
    }
    for (int i = 0; i < Config::config->M + Config::config->nd; i++)
    {
        for (int j = 0; j < Config::config->B; j++)
        {
            for (int k = 0; k < Config::config->hiddenDim; k++)
            {
                F3[i] << r3(i, j, k) << ",";
            }
            F3[i] << std::endl;
        }
    }
    for (int i = 0; i < Config::config->M + Config::config->nd; i++)
    {
        for (int j = 0; j < Config::config->hiddenDim; j++)
        {
            for (int k = 0; k < Config::config->numClass; k++)
            {
                F3[i] << q3(i, j, k) << ",";
            }
            F3[i] << std::endl;
        }
    }
    for (int i = 0; i < Config::config->M + Config::config->nd; i++)
    {
        for (int j = 0; j < Config::config->B; j++)
        {
            for (int k = 0; k < Config::config->numClass; k++)
            {
                F3[i] << t3(i, j, k) << ",";
            }
            F3[i] << std::endl;
        }
    }
    for (int i = 0; i < Config::config->M + Config::config->nd; i++)
    {
        F3[i].close();
    }

    // for(B, numClass)X(numClass, hiddenDim)
    b4.resize(Config::config->M);
    c4.resize(Config::config->M);
    d4.resize(Config::config->M);
    for (int i = 0; i < Config::config->M; i++)
    {
        b4[i] = Mat::randomMatrixXu(Config::config->B, Config::config->numClass);
        c4[i] = Mat::randomMatrixXu(Config::config->numClass, Config::config->hiddenDim);
        if (i == 0)
        {
            d4[i] = b4[i] * c4[i];
        }
        else
        {
            d4[i] = Mat::randomMatrixXu(Config::config->B, Config::config->hiddenDim);
        }
    }
    Tensor<u64, 3> b_4(Config::config->M, Config::config->B, Config::config->numClass);
    for (int i = 0; i < Config::config->M; i++)
    {
        for (int j = 0; j < Config::config->B; j++)
        {
            for (int k = 0; k < Config::config->numClass; k++)
            {
                b_4(i, j, k) = b4[i](j, k);
            }
        }
    }
    Tensor<u64, 3> c_4(Config::config->M, Config::config->numClass, Config::config->hiddenDim);
    for (int i = 0; i < Config::config->M; i++)
    {
        for (int j = 0; j < Config::config->numClass; j++)
        {
            for (int k = 0; k < Config::config->hiddenDim; k++)
            {
                c_4(i, j, k) = c4[i](j, k);
            }
        }
    }
    Tensor<u64, 3> d_4(Config::config->M, Config::config->B, Config::config->hiddenDim);
    for (int i = 0; i < Config::config->M; i++)
    {
        for (int j = 0; j < Config::config->B; j++)
        {
            for (int k = 0; k < Config::config->hiddenDim; k++)
            {
                d_4(i, j, k) = d4[i](j, k);
            }
        }
    }

    Tensor<u64, 3> r4, q4, t4;
    r4 = Mat::A_plus.contract(b_4, product_dims);
    q4 = Mat::A_plus.contract(c_4, product_dims);
    t4 = Mat::A_plus.contract(d_4, product_dims);

    vector<ofstream> F4(Config::config->M + Config::config->nd);
    for (int i = 0; i < Config::config->M + Config::config->nd; i++)
    {
        F4[i].open("Mul_Triplets/MT4_" + to_string(i) + ".txt", ios::out);
    }
    for (int i = 0; i < Config::config->M + Config::config->nd; i++)
    {
        for (int j = 0; j < Config::config->B; j++)
        {
            for (int k = 0; k < Config::config->numClass; k++)
            {
                F4[i] << r4(i, j, k) << ",";
            }
            F4[i] << std::endl;
        }
    }
    for (int i = 0; i < Config::config->M + Config::config->nd; i++)
    {
        for (int j = 0; j < Config::config->numClass; j++)
        {
            for (int k = 0; k < Config::config->hiddenDim; k++)
            {
                F4[i] << q4(i, j, k) << ",";
            }
            F4[i] << std::endl;
        }
    }
    for (int i = 0; i < Config::config->M + Config::config->nd; i++)
    {
        for (int j = 0; j < Config::config->B; j++)
        {
            for (int k = 0; k < Config::config->hiddenDim; k++)
            {
                F4[i] << t4(i, j, k) << ",";
            }
            F4[i] << std::endl;
        }
    }
    for (int i = 0; i < Config::config->M + Config::config->nd; i++)
    {
        F4[i].close();
    }

    // for(B, hiddenDim)X(hiddenDim, hiddenDim)
    b5.resize(Config::config->M);
    c5.resize(Config::config->M);
    d5.resize(Config::config->M);
    for (int i = 0; i < Config::config->M; i++)
    {
        b5[i] = Mat::randomMatrixXu(Config::config->B, Config::config->hiddenDim);
        c5[i] = Mat::randomMatrixXu(Config::config->hiddenDim, Config::config->hiddenDim);
        if (i == 0)
        {
            d5[i] = b5[i] * c5[i];
        }
        else
        {
            d5[i] = Mat::randomMatrixXu(Config::config->B, Config::config->hiddenDim);
        }
    }
    Tensor<u64, 3> b_5(Config::config->M, Config::config->B, Config::config->hiddenDim);
    for (int i = 0; i < Config::config->M; i++)
    {
        for (int j = 0; j < Config::config->B; j++)
        {
            for (int k = 0; k < Config::config->hiddenDim; k++)
            {
                b_5(i, j, k) = b5[i](j, k);
            }
        }
    }
    Tensor<u64, 3> c_5(Config::config->M, Config::config->hiddenDim, Config::config->hiddenDim);
    for (int i = 0; i < Config::config->M; i++)
    {
        for (int j = 0; j < Config::config->hiddenDim; j++)
        {
            for (int k = 0; k < Config::config->hiddenDim; k++)
            {
                c_5(i, j, k) = c5[i](j, k);
            }
        }
    }
    Tensor<u64, 3> d_5(Config::config->M, Config::config->B, Config::config->hiddenDim);
    for (int i = 0; i < Config::config->M; i++)
    {
        for (int j = 0; j < Config::config->B; j++)
        {
            for (int k = 0; k < Config::config->hiddenDim; k++)
            {
                d_5(i, j, k) = d5[i](j, k);
            }
        }
    }

    Tensor<u64, 3> r5, q5, t5;
    r5 = Mat::A_plus.contract(b_5, product_dims);
    q5 = Mat::A_plus.contract(c_5, product_dims);
    t5 = Mat::A_plus.contract(d_5, product_dims);

    vector<ofstream> F5(Config::config->M + Config::config->nd);
    for (int i = 0; i < Config::config->M + Config::config->nd; i++)
    {
        F5[i].open("Mul_Triplets/MT5_" + to_string(i) + ".txt", ios::out);
    }
    for (int i = 0; i < Config::config->M + Config::config->nd; i++)
    {
        for (int j = 0; j < Config::config->B; j++)
        {
            for (int k = 0; k < Config::config->hiddenDim; k++)
            {
                F5[i] << r5(i, j, k) << ",";
            }
            F5[i] << std::endl;
        }
    }
    for (int i = 0; i < Config::config->M + Config::config->nd; i++)
    {
        for (int j = 0; j < Config::config->hiddenDim; j++)
        {
            for (int k = 0; k < Config::config->hiddenDim; k++)
            {
                F5[i] << q5(i, j, k) << ",";
            }
            F5[i] << std::endl;
        }
    }
    for (int i = 0; i < Config::config->M + Config::config->nd; i++)
    {
        for (int j = 0; j < Config::config->B; j++)
        {
            for (int k = 0; k < Config::config->hiddenDim; k++)
            {
                F5[i] << t5(i, j, k) << ",";
            }
            F5[i] << std::endl;
        }
    }
    for (int i = 0; i < Config::config->M + Config::config->nd; i++)
    {
        F5[i].close();
    }

    // for(D, B)X(B, hiddenDim)
    b6.resize(Config::config->M);
    c6.resize(Config::config->M);
    d6.resize(Config::config->M);
    for (int i = 0; i < Config::config->M; i++)
    {
        b6[i] = Mat::randomMatrixXu(Config::config->D, Config::config->B);
        c6[i] = Mat::randomMatrixXu(Config::config->B, Config::config->hiddenDim);
        if (i == 0)
        {
            d6[i] = b6[i] * c6[i];
        }
        else
        {
            d6[i] = Mat::randomMatrixXu(Config::config->D, Config::config->hiddenDim);
        }
    }
    Tensor<u64, 3> b_6(Config::config->M, Config::config->D, Config::config->B);
    for (int i = 0; i < Config::config->M; i++)
    {
        for (int j = 0; j < Config::config->D; j++)
        {
            for (int k = 0; k < Config::config->B; k++)
            {
                b_6(i, j, k) = b6[i](j, k);
            }
        }
    }
    Tensor<u64, 3> c_6(Config::config->M, Config::config->B, Config::config->hiddenDim);
    for (int i = 0; i < Config::config->M; i++)
    {
        for (int j = 0; j < Config::config->B; j++)
        {
            for (int k = 0; k < Config::config->hiddenDim; k++)
            {
                c_6(i, j, k) = c6[i](j, k);
            }
        }
    }
    Tensor<u64, 3> d_6(Config::config->M, Config::config->D, Config::config->hiddenDim);
    for (int i = 0; i < Config::config->M; i++)
    {
        for (int j = 0; j < Config::config->D; j++)
        {
            for (int k = 0; k < Config::config->hiddenDim; k++)
            {
                d_6(i, j, k) = d6[i](j, k);
            }
        }
    }

    Tensor<u64, 3> r6, q6, t6;
    r6 = Mat::A_plus.contract(b_6, product_dims);
    q6 = Mat::A_plus.contract(c_6, product_dims);
    t6 = Mat::A_plus.contract(d_6, product_dims);

    vector<ofstream> F6(Config::config->M + Config::config->nd);
    for (int i = 0; i < Config::config->M + Config::config->nd; i++)
    {
        F6[i].open("Mul_Triplets/MT6_" + to_string(i) + ".txt", ios::out);
    }
    for (int i = 0; i < Config::config->M + Config::config->nd; i++)
    {
        for (int j = 0; j < Config::config->D; j++)
        {
            for (int k = 0; k < Config::config->B; k++)
            {
                F6[i] << r6(i, j, k) << ",";
            }
            F6[i] << std::endl;
        }
    }
    for (int i = 0; i < Config::config->M + Config::config->nd; i++)
    {
        for (int j = 0; j < Config::config->B; j++)
        {
            for (int k = 0; k < Config::config->hiddenDim; k++)
            {
                F6[i] << q6(i, j, k) << ",";
            }
            F6[i] << std::endl;
        }
    }
    for (int i = 0; i < Config::config->M + Config::config->nd; i++)
    {
        for (int j = 0; j < Config::config->D; j++)
        {
            for (int k = 0; k < Config::config->hiddenDim; k++)
            {
                F6[i] << t6(i, j, k) << ",";
            }
            F6[i] << std::endl;
        }
    }
    for (int i = 0; i < Config::config->M + Config::config->nd; i++)
    {
        F6[i].close();
    }

    // for(hiddenDim，B)X(B, numClass)
    b7.resize(Config::config->M);
    c7.resize(Config::config->M);
    d7.resize(Config::config->M);
    for (int i = 0; i < Config::config->M; i++)
    {
        b7[i] = Mat::randomMatrixXu(Config::config->hiddenDim, Config::config->B);
        c7[i] = Mat::randomMatrixXu(Config::config->B, Config::config->numClass);
        if (i == 0)
        {
            d7[i] = b7[i] * c7[i];
        }
        else
        {
            d7[i] = Mat::randomMatrixXu(Config::config->hiddenDim, Config::config->numClass);
        }
    }
    Tensor<u64, 3> b_7(Config::config->M, Config::config->hiddenDim, Config::config->B);
    for (int i = 0; i < Config::config->M; i++)
    {
        for (int j = 0; j < Config::config->hiddenDim; j++)
        {
            for (int k = 0; k < Config::config->B; k++)
            {
                b_7(i, j, k) = b7[i](j, k);
            }
        }
    }
    Tensor<u64, 3> c_7(Config::config->M, Config::config->B, Config::config->numClass);
    for (int i = 0; i < Config::config->M; i++)
    {
        for (int j = 0; j < Config::config->B; j++)
        {
            for (int k = 0; k < Config::config->numClass; k++)
            {
                c_7(i, j, k) = c7[i](j, k);
            }
        }
    }
    Tensor<u64, 3> d_7(Config::config->M, Config::config->hiddenDim, Config::config->numClass);
    for (int i = 0; i < Config::config->M; i++)
    {
        for (int j = 0; j < Config::config->hiddenDim; j++)
        {
            for (int k = 0; k < Config::config->numClass; k++)
            {
                d_7(i, j, k) = d7[i](j, k);
            }
        }
    }

    Tensor<u64, 3> r7, q7, t7;
    r7 = Mat::A_plus.contract(b_7, product_dims);
    q7 = Mat::A_plus.contract(c_7, product_dims);
    t7 = Mat::A_plus.contract(d_7, product_dims);

    vector<ofstream> F7(Config::config->M + Config::config->nd);
    for (int i = 0; i < Config::config->M + Config::config->nd; i++)
    {
        F7[i].open("Mul_Triplets/MT7_" + to_string(i) + ".txt", ios::out);
    }
    for (int i = 0; i < Config::config->M + Config::config->nd; i++)
    {
        for (int j = 0; j < Config::config->hiddenDim; j++)
        {
            for (int k = 0; k < Config::config->B; k++)
            {
                F7[i] << r7(i, j, k) << ",";
            }
            F7[i] << std::endl;
        }
    }
    for (int i = 0; i < Config::config->M + Config::config->nd; i++)
    {
        for (int j = 0; j < Config::config->B; j++)
        {
            for (int k = 0; k < Config::config->numClass; k++)
            {
                F7[i] << q7(i, j, k) << ",";
            }
            F7[i] << std::endl;
        }
    }
    for (int i = 0; i < Config::config->M + Config::config->nd; i++)
    {
        for (int j = 0; j < Config::config->hiddenDim; j++)
        {
            for (int k = 0; k < Config::config->numClass; k++)
            {
                F7[i] << t7(i, j, k) << ",";
            }
            F7[i] << std::endl;
        }
    }
    for (int i = 0; i < Config::config->M + Config::config->nd; i++)
    {
        F7[i].close();
    }

    // for(hiddenDim, B)X(B, hiddenDim)
    b8.resize(Config::config->M);
    c8.resize(Config::config->M);
    d8.resize(Config::config->M);
    for (int i = 0; i < Config::config->M; i++)
    {
        b8[i] = Mat::randomMatrixXu(Config::config->hiddenDim, Config::config->B);
        c8[i] = Mat::randomMatrixXu(Config::config->B, Config::config->hiddenDim);
        if (i == 0)
        {
            d8[i] = b8[i] * c8[i];
        }
        else
        {
            d8[i] = Mat::randomMatrixXu(Config::config->hiddenDim, Config::config->hiddenDim);
        }
    }
    Tensor<u64, 3> b_8(Config::config->M, Config::config->hiddenDim, Config::config->B);
    for (int i = 0; i < Config::config->M; i++)
    {
        for (int j = 0; j < Config::config->hiddenDim; j++)
        {
            for (int k = 0; k < Config::config->B; k++)
            {
                b_8(i, j, k) = b8[i](j, k);
            }
        }
    }
    Tensor<u64, 3> c_8(Config::config->M, Config::config->B, Config::config->hiddenDim);
    for (int i = 0; i < Config::config->M; i++)
    {
        for (int j = 0; j < Config::config->B; j++)
        {
            for (int k = 0; k < Config::config->hiddenDim; k++)
            {
                c_8(i, j, k) = c8[i](j, k);
            }
        }
    }
    Tensor<u64, 3> d_8(Config::config->M, Config::config->hiddenDim, Config::config->hiddenDim);
    for (int i = 0; i < Config::config->M; i++)
    {
        for (int j = 0; j < Config::config->hiddenDim; j++)
        {
            for (int k = 0; k < Config::config->hiddenDim; k++)
            {
                d_8(i, j, k) = d8[i](j, k);
            }
        }
    }

    Tensor<u64, 3> r8, q8, t8;
    r8 = Mat::A_plus.contract(b_8, product_dims);
    q8 = Mat::A_plus.contract(c_8, product_dims);
    t8 = Mat::A_plus.contract(d_8, product_dims);

    vector<ofstream> F8(Config::config->M + Config::config->nd);
    for (int i = 0; i < Config::config->M + Config::config->nd; i++)
    {
        F8[i].open("Mul_Triplets/MT8_" + to_string(i) + ".txt", ios::out);
    }
    for (int i = 0; i < Config::config->M + Config::config->nd; i++)
    {
        for (int j = 0; j < Config::config->hiddenDim; j++)
        {
            for (int k = 0; k < Config::config->B; k++)
            {
                F8[i] << r8(i, j, k) << ",";
            }
            F8[i] << std::endl;
        }
    }
    for (int i = 0; i < Config::config->M + Config::config->nd; i++)
    {
        for (int j = 0; j < Config::config->B; j++)
        {
            for (int k = 0; k < Config::config->hiddenDim; k++)
            {
                F8[i] << q8(i, j, k) << ",";
            }
            F8[i] << std::endl;
        }
    }
    for (int i = 0; i < Config::config->M + Config::config->nd; i++)
    {
        for (int j = 0; j < Config::config->hiddenDim; j++)
        {
            for (int k = 0; k < Config::config->hiddenDim; k++)
            {
                F8[i] << t8(i, j, k) << ",";
            }
            F8[i] << std::endl;
        }
    }
    for (int i = 0; i < Config::config->M + Config::config->nd; i++)
    {
        F8[i].close();
    }

    // truncation for (B, numClass)
    MatrixXu A_plus_mat = Mat::toMatrix(Mat::A_plus);
    m0 = Mat::randomMatrixXu(Config::config->B, Config::config->numClass);
    m0.setZero();
    trun0.resize(Config::config->M + Config::config->nd);
    for (int i = 0; i < Config::config->M + Config::config->nd; i++)
    {
        trun0[i].resize(Config::config->B, Config::config->numClass);
    }
    for (int i = 0; i < Config::config->B; i++)
    {
        for (int j = 0; j < Config::config->numClass; j++)
        {
            MatrixXu temp_vec(Config::config->M, 1);
            temp_vec(0, 0) = m0(i, j);
            for (int k = 1; k < Config::config->M; k++)
            {
                temp_vec(k, 0) = Constant::Util::random_u64();
            }
            MatrixXu temp = A_plus_mat * temp_vec;
            for (int k = 0; k < Config::config->M + Config::config->nd; k++)
            {
                trun0[k](i, j) = temp(k, 0);
            }
        }
    }
    Mat::truncateMatrixXu(m0);
    trun_0.resize(Config::config->M + Config::config->nd);
    for (int i = 0; i < Config::config->M + Config::config->nd; i++)
    {
        trun_0[i].resize(Config::config->B, Config::config->numClass);
    }
    for (int i = 0; i < Config::config->B; i++)
    {
        for (int j = 0; j < Config::config->numClass; j++)
        {
            MatrixXu temp_vec(Config::config->M, 1);
            temp_vec(0, 0) = m0(i, j);
            for (int k = 1; k < Config::config->M; k++)
            {
                temp_vec(k, 0) = Constant::Util::random_u64();
            }
            MatrixXu temp = A_plus_mat * temp_vec;
            for (int k = 0; k < Config::config->M + Config::config->nd; k++)
            {
                trun_0[k](i, j) = temp(k, 0);
            }
        }
    }

    // truncation for (D, numClass)
    MatrixXu m1 = Mat::randomMatrixXu(Config::config->D, Config::config->numClass);
    m1.setZero();
    trun1.resize(Config::config->M + Config::config->nd);
    for (int i = 0; i < Config::config->M + Config::config->nd; i++)
    {
        trun1[i].resize(Config::config->D, Config::config->numClass);
    }
    for (int i = 0; i < Config::config->D; i++)
    {
        for (int j = 0; j < Config::config->numClass; j++)
        {
            MatrixXu temp_vec(Config::config->M, 1);
            temp_vec(0, 0) = m1(i, j);
            for (int k = 1; k < Config::config->M; k++)
            {
                temp_vec(k, 0) = Constant::Util::random_u64();
            }
            MatrixXu temp = A_plus_mat * temp_vec;
            for (int k = 0; k < Config::config->M + Config::config->nd; k++)
            {
                trun1[k](i, j) = temp(k, 0);
            }
        }
    }
    Mat::truncateMatrixXu(m1);
    trun_1.resize(Config::config->M + Config::config->nd);
    for (int i = 0; i < Config::config->M + Config::config->nd; i++)
    {
        trun_1[i].resize(Config::config->D, Config::config->numClass);
    }
    for (int i = 0; i < Config::config->D; i++)
    {
        for (int j = 0; j < Config::config->numClass; j++)
        {
            MatrixXu temp_vec(Config::config->M, 1);
            temp_vec(0, 0) = m1(i, j);
            for (int k = 1; k < Config::config->M; k++)
            {
                temp_vec(k, 0) = Constant::Util::random_u64();
            }
            MatrixXu temp = A_plus_mat * temp_vec;
            for (int k = 0; k < Config::config->M + Config::config->nd; k++)
            {
                trun_1[k](i, j) = temp(k, 0);
            }
        }
    }

    // truncation for (B, hiddenDim)
    m2 = Mat::randomMatrixXu(Config::config->B, Config::config->hiddenDim);
    m2.setZero();
    trun2.resize(Config::config->M + Config::config->nd);
    for (int i = 0; i < Config::config->M + Config::config->nd; i++)
    {
        trun2[i].resize(Config::config->B, Config::config->hiddenDim);
    }
    for (int i = 0; i < Config::config->B; i++)
    {
        for (int j = 0; j < Config::config->hiddenDim; j++)
        {
            MatrixXu temp_vec(Config::config->M, 1);
            temp_vec(0, 0) = m2(i, j);
            for (int k = 1; k < Config::config->M; k++)
            {
                temp_vec(k, 0) = Constant::Util::random_u64();
            }
            MatrixXu temp = A_plus_mat * temp_vec;
            for (int k = 0; k < Config::config->M + Config::config->nd; k++)
            {
                trun2[k](i, j) = temp(k, 0);
            }
        }
    }
    Mat::truncateMatrixXu(m2);
    trun_2.resize(Config::config->M + Config::config->nd);
    for (int i = 0; i < Config::config->M + Config::config->nd; i++)
    {
        trun_2[i].resize(Config::config->B, Config::config->hiddenDim);
    }
    for (int i = 0; i < Config::config->B; i++)
    {
        for (int j = 0; j < Config::config->hiddenDim; j++)
        {
            MatrixXu temp_vec(Config::config->M, 1);
            temp_vec(0, 0) = m2(i, j);
            for (int k = 1; k < Config::config->M; k++)
            {
                temp_vec(k, 0) = Constant::Util::random_u64();
            }
            MatrixXu temp = A_plus_mat * temp_vec;
            for (int k = 0; k < Config::config->M + Config::config->nd; k++)
            {
                trun_2[k](i, j) = temp(k, 0);
            }
        }
    }

    // truncation for (D, hiddenDim)
    MatrixXu m3 = Mat::randomMatrixXu(Config::config->D, Config::config->hiddenDim);
    m3.setZero();
    trun3.resize(Config::config->M + Config::config->nd);
    for (int i = 0; i < Config::config->M + Config::config->nd; i++)
    {
        trun3[i].resize(Config::config->D, Config::config->hiddenDim);
    }
    for (int i = 0; i < Config::config->D; i++)
    {
        for (int j = 0; j < Config::config->hiddenDim; j++)
        {
            MatrixXu temp_vec(Config::config->M, 1);
            temp_vec(0, 0) = m3(i, j);
            for (int k = 1; k < Config::config->M; k++)
            {
                temp_vec(k, 0) = Constant::Util::random_u64();
            }
            MatrixXu temp = A_plus_mat * temp_vec;
            for (int k = 0; k < Config::config->M + Config::config->nd; k++)
            {
                trun3[k](i, j) = temp(k, 0);
            }
        }
    }
    Mat::truncateMatrixXu(m3);
    trun_3.resize(Config::config->M + Config::config->nd);
    for (int i = 0; i < Config::config->M + Config::config->nd; i++)
    {
        trun_3[i].resize(Config::config->D, Config::config->hiddenDim);
    }
    for (int i = 0; i < Config::config->D; i++)
    {
        for (int j = 0; j < Config::config->hiddenDim; j++)
        {
            MatrixXu temp_vec(Config::config->M, 1);
            temp_vec(0, 0) = m3(i, j);
            for (int k = 1; k < Config::config->M; k++)
            {
                temp_vec(k, 0) = Constant::Util::random_u64();
            }
            MatrixXu temp = A_plus_mat * temp_vec;
            for (int k = 0; k < Config::config->M + Config::config->nd; k++)
            {
                trun_3[k](i, j) = temp(k, 0);
            }
        }
    }

    // truncation for (hiddenDim, numClass)
    MatrixXu m4 = Mat::randomMatrixXu(Config::config->hiddenDim, Config::config->numClass);
    m4.setZero();
    trun4.resize(Config::config->M + Config::config->nd);
    for (int i = 0; i < Config::config->M + Config::config->nd; i++)
    {
        trun4[i].resize(Config::config->hiddenDim, Config::config->numClass);
    }
    for (int i = 0; i < Config::config->hiddenDim; i++)
    {
        for (int j = 0; j < Config::config->numClass; j++)
        {
            MatrixXu temp_vec(Config::config->M, 1);
            temp_vec(0, 0) = m4(i, j);
            for (int k = 1; k < Config::config->M; k++)
            {
                temp_vec(k, 0) = Constant::Util::random_u64();
            }
            MatrixXu temp = A_plus_mat * temp_vec;
            for (int k = 0; k < Config::config->M + Config::config->nd; k++)
            {
                trun4[k](i, j) = temp(k, 0);
            }
        }
    }
    Mat::truncateMatrixXu(m4);
    trun_4.resize(Config::config->M + Config::config->nd);
    for (int i = 0; i < Config::config->M + Config::config->nd; i++)
    {
        trun_4[i].resize(Config::config->hiddenDim, Config::config->numClass);
    }
    for (int i = 0; i < Config::config->hiddenDim; i++)
    {
        for (int j = 0; j < Config::config->numClass; j++)
        {
            MatrixXu temp_vec(Config::config->M, 1);
            temp_vec(0, 0) = m4(i, j);
            for (int k = 1; k < Config::config->M; k++)
            {
                temp_vec(k, 0) = Constant::Util::random_u64();
            }
            MatrixXu temp = A_plus_mat * temp_vec;
            for (int k = 0; k < Config::config->M + Config::config->nd; k++)
            {
                trun_4[k](i, j) = temp(k, 0);
            }
        }
    }

    // truncation for (hiddenDim, hiddenDim)
    MatrixXu m5 = Mat::randomMatrixXu(Config::config->hiddenDim, Config::config->hiddenDim);
    m5.setZero();
    trun5.resize(Config::config->M + Config::config->nd);
    for (int i = 0; i < Config::config->M + Config::config->nd; i++)
    {
        trun5[i].resize(Config::config->hiddenDim, Config::config->hiddenDim);
    }
    for (int i = 0; i < Config::config->hiddenDim; i++)
    {
        for (int j = 0; j < Config::config->hiddenDim; j++)
        {
            MatrixXu temp_vec(Config::config->M, 1);
            temp_vec(0, 0) = m5(i, j);
            for (int k = 1; k < Config::config->M; k++)
            {
                temp_vec(k, 0) = Constant::Util::random_u64();
            }
            MatrixXu temp = A_plus_mat * temp_vec;
            for (int k = 0; k < Config::config->M + Config::config->nd; k++)
            {
                trun5[k](i, j) = temp(k, 0);
            }
        }
    }
    Mat::truncateMatrixXu(m5);
    trun_5.resize(Config::config->M + Config::config->nd);
    for (int i = 0; i < Config::config->M + Config::config->nd; i++)
    {
        trun_5[i].resize(Config::config->hiddenDim, Config::config->hiddenDim);
    }
    for (int i = 0; i < Config::config->hiddenDim; i++)
    {
        for (int j = 0; j < Config::config->hiddenDim; j++)
        {
            MatrixXu temp_vec(Config::config->M, 1);
            temp_vec(0, 0) = m5(i, j);
            for (int k = 1; k < Config::config->M; k++)
            {
                temp_vec(k, 0) = Constant::Util::random_u64();
            }
            MatrixXu temp = A_plus_mat * temp_vec;
            for (int k = 0; k < Config::config->M + Config::config->nd; k++)
            {
                trun_5[k](i, j) = temp(k, 0);
            }
        }
    }

    // boolean random (add share and bool share) for (B + B,numClass)
    MatrixXu temp_r0(Config::config->B + Config::config->B, Config::config->numClass);
    temp_r0 << m0,
        m0;
    MatrixXu temp0 = temp_r0;
    for (int i = 0; i < Config::config->M - 1; i++)
    {
        MatrixXu noise0 = Mat::randomMatrixXu(Config::config->B + Config::config->B, Config::config->numClass);
        boolean_r_add0.push_back(noise0);
        temp_r0 = temp_r0 - noise0;
        MatrixXu noise1 = Mat::randomMatrixXu(Config::config->B + Config::config->B, Config::config->numClass);
        boolean_r_bool0.push_back(noise1);
        temp0 = Mat::op_Xor(temp0, noise1);
    }
    boolean_r_add0.push_back(temp_r0);
    boolean_r_bool0.push_back(temp0);

    // boolean random (add share and bool share) for (B, hiddenDim)
    MatrixXu temp_r1 = m2;
    MatrixXu temp1 = temp_r1;
    for (int i = 0; i < Config::config->M - 1; i++)
    {
        MatrixXu noise0 = Mat::randomMatrixXu(Config::config->B, Config::config->hiddenDim);
        boolean_r_add1.push_back(noise0);
        temp_r1 = temp_r1 - noise0;
        MatrixXu noise1 = Mat::randomMatrixXu(Config::config->B, Config::config->hiddenDim);
        boolean_r_bool1.push_back(noise1);
        temp1 = Mat::op_Xor(temp1, noise1);
    }
    boolean_r_add1.push_back(temp_r1);
    boolean_r_bool1.push_back(temp1);
}