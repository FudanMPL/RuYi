#include "Secret_Mul.h"
#include "Secret_Cmp.h"

MatrixXu Boolean_Share::secret_share(MatrixXu &share)
{
    vector<MatrixXu> sec_share;
    MatrixXu truth(Config::config->B, Config::config->D);
    truth << Mat::randomMatrixXu(Config::config->B, Config::config->D);
    MatrixXu temp = truth;
    // cout << "The truth:" << endl
    //      << temp << endl;
    for (int i = 0; i < Config::config->M - 1; i++)
    {
        MatrixXu noise = Mat::randomMatrixXu(Config::config->B, Config::config->D);
        sec_share.push_back(noise);
        temp = Mat::op_Xor(temp, noise);
    }
    sec_share.push_back(temp);
    share = sec_share[party];
    // cout << "share:" << endl
    //      << share << endl;
    return truth;
}

MatrixXu Boolean_Share::reveal(MatrixXu &share)
{
    vector<MatrixXu> shares(Config::config->M);
    MatrixXu result;
    for (int i = 0; i < Config::config->M; i++)
    {
        if (i == party)
        {
            shares[i] = share;
        }
        else
        {
            SocketUtil::send(share, i);
        }
    }
    for (int i = 0; i < Config::config->M; i++)
    {
        if (i != party)
            SocketUtil::receive(shares[i], i);
    }
    result = shares[0];
    for (int i = 1; i < Config::config->M; i++)
    {
        result = Mat::op_Xor(result, shares[i]);
    }
    return result;
    // MatrixXu result;
    // if (party == 0)
    // {
    //     vector<MatrixXu> reveal_data(Config::config->M);
    //     reveal_data[0] = share;
    //     result = reveal_data[0];
    //     for (int i = 1; i < Config::config->M; i++)
    //     {
    //         SocketUtil::receive(reveal_data[i], i);
    //         result = Mat::op_Xor(reveal_data[i], result);
    //     }

    //     for (int i = 1; i < Config::config->M; i++)
    //     {
    //         SocketUtil::send(result, i);
    //     }
    // }
    // else
    // {
    //     SocketUtil::send(share, 0);
    //     SocketUtil::receive(result, 0);
    // }
    // return result;
}

Matrix8 Boolean_Share::reveal(Matrix8 &share)
{
    vector<Matrix8> shares(Config::config->M);
    Matrix8 result;
    for (int i = 0; i < Config::config->M; i++)
    {
        if (i == party)
        {
            shares[i] = share;
        }
        else
        {
            SocketUtil::send(share, i);
        }
    }
    for (int i = 0; i < Config::config->M; i++)
    {
        if (i != party)
            SocketUtil::receive(shares[i], i);
    }
    result = shares[0];
    for (int i = 1; i < Config::config->M; i++)
    {
        result = Mat::op_Xor(result, shares[i]);
    }
    return result;
    // Matrix8 result;
    // if (party == 0)
    // {
    //     vector<Matrix8> reveal_data(Config::config->M);
    //     reveal_data[0] = share;
    //     result = reveal_data[0];
    //     for (int i = 1; i < Config::config->M; i++)
    //     {
    //         SocketUtil::receive(reveal_data[i], i);
    //         result = Mat::op_Xor(reveal_data[i], result);
    //     }

    //     for (int i = 1; i < Config::config->M; i++)
    //     {
    //         SocketUtil::send(result, i);
    //     }
    // }
    // else
    // {
    //     SocketUtil::send(share, 0);
    //     SocketUtil::receive(result, 0);
    // }
    // return result;
}

MatrixXu Boolean_Share::add_reveal(MatrixXu &share)
{
    vector<MatrixXu> shares(Config::config->M);
    MatrixXu result;
    for (int i = 0; i < Config::config->M; i++)
    {
        if (i == party)
        {
            shares[i] = share;
        }
        else
        {
            SocketUtil::send(share, i);
        }
    }
    for (int i = 0; i < Config::config->M; i++)
    {
        if (i != party)
            SocketUtil::receive(shares[i], i);
    }
    result = shares[0];
    for (int i = 1; i < Config::config->M; i++)
    {
        result = result + shares[i];
    }
    return result;
    // MatrixXu result;
    // if (party == 0)
    // {
    //     vector<MatrixXu> reveal_data(Config::config->M);
    //     reveal_data[0] = share;
    //     result = reveal_data[0];
    //     for (int i = 1; i < Config::config->M; i++)
    //     {
    //         SocketUtil::receive(reveal_data[i], i);
    //         result = reveal_data[i] + result;
    //     }

    //     for (int i = 1; i < Config::config->M; i++)
    //     {
    //         SocketUtil::send(result, i);
    //     }
    // }
    // else
    // {
    //     SocketUtil::send(share, 0);
    //     SocketUtil::receive(result, 0);
    // }
    // return result;
}

Matrix8 Boolean_Share::byte_and_byte(Matrix8 &x, Matrix8 &y)
{
    int row = x.rows(), col = x.cols();
    Matrix8 a(row, col), b(row, col), c(row, col);
    a.setZero();
    b.setZero();
    c.setZero();
    Matrix8 s_share = Mat::op_Xor(x, a);
    Matrix8 t_share = Mat::op_Xor(y, b);
    Matrix8 s = Boolean_Share::reveal(s_share);
    Matrix8 t = Boolean_Share::reveal(t_share);
    if (party == 0)
    {
        Matrix8 temp1 = Mat::op_And(s, t);
        Matrix8 temp2 = Mat::op_And(s, b);
        Matrix8 temp3 = Mat::op_And(t, a);
        Matrix8 res1 = Mat::op_Xor(temp1, temp2);
        Matrix8 res2 = Mat::op_Xor(temp3, c);
        return Mat::op_Xor(res1, res2);
    }
    else
    {
        Matrix8 temp1 = Mat::op_And(s, b);
        Matrix8 temp2 = Mat::op_And(t, a);
        Matrix8 temp3 = Mat::op_Xor(temp1, temp2);
        return Mat::op_Xor(temp3, c);
    }
}

MatrixXu Boolean_Share::bool_and(MatrixXu &x, MatrixXu &y)
{
    int row = x.rows(), col = x.cols();
    MatrixXu a(row, col), b(row, col), c(row, col);
    a.setZero();
    b.setZero();
    c.setZero();
    MatrixXu s_share = Mat::op_Xor(x, a);
    MatrixXu t_share = Mat::op_Xor(y, b);
    // vector<MatrixXu> s_shares(Config::config->M), t_shares(Config::config->M);
    // MatrixXu s, t;
    // for (int i = 0; i < Config::config->M; i++)
    // {
    //     if (i == party)
    //     {
    //         s_shares[i] = s_share;
    //         t_shares[i] = t_share;
    //     }
    //     else
    //     {
    //         SocketUtil::send(s_share, i);
    //         SocketUtil::send(t_share, i);
    //         SocketUtil::receive(s_shares[i], i);
    //         SocketUtil::receive(t_shares[i], i);
    //     }
    // }
    // s = s_shares[0];
    // t = t_shares[0];
    // for (int i = 1; i < Config::config->M; i++)
    // {
    //     s = Mat::op_Xor(s, s_shares[i]);
    //     t = Mat::op_Xor(t, t_shares[i]);
    // }
    // MatrixXu com_share(row, col + col);
    // com_share << s_share, t_share;
    // MatrixXu com = Boolean_Share::reveal(com_share);
    // MatrixXu s = com.leftCols(col);
    // MatrixXu t = com.rightCols(col);
    MatrixXu s = Boolean_Share::reveal(s_share);
    MatrixXu t = Boolean_Share::reveal(t_share);
    if (party == 0)
    {
        MatrixXu temp1 = Mat::op_And(s, t);
        MatrixXu temp2 = Mat::op_And(s, b);
        MatrixXu temp3 = Mat::op_And(t, a);
        MatrixXu res1 = Mat::op_Xor(temp1, temp2);
        MatrixXu res2 = Mat::op_Xor(temp3, c);
        return Mat::op_Xor(res1, res2);
    }
    else
    {
        MatrixXu temp1 = Mat::op_And(s, b);
        MatrixXu temp2 = Mat::op_And(t, a);
        MatrixXu temp3 = Mat::op_Xor(temp1, temp2);
        return Mat::op_Xor(temp3, c);
    }
}

MatrixXu Boolean_Share::add(MatrixXu &x, MatrixXu &y)
{
    // cnt1++;
    int row = x.rows();
    int col = x.cols();
    MatrixXu keep_masks(6, 1);
    keep_masks << 0x0000000000000001,
        0x0000000000000003,
        0x000000000000000f,
        0x00000000000000ff,
        0x000000000000ffff,
        0x00000000ffffffff;
    MatrixXu g = Boolean_Share::bool_and(x, y);
    MatrixXu p = Mat::op_Xor(x, y);
    for (int i = 0; i < 6; i++)
    {
        MatrixXu g1 = Mat::op_shift_left(g, int(pow(2, i)));
        MatrixXu p1 = Mat::op_shift_left(p, int(pow(2, i)));
        p1 = Mat::op_Xor(keep_masks(i), p1);
        MatrixXu com0(row, col + col), com1(row, col + col);
        com0 << p, p;
        com1 << g1, p1;
        MatrixXu com = Boolean_Share::bool_and(com0, com1);
        // g = Mat::op_Xor(g, Boolean_Share::bool_and(p, g1));
        // p = Boolean_Share::bool_and(p, p1);
        g = Mat::op_Xor(g, com.leftCols(col));
        p = com.rightCols(col);
    }
    MatrixXu c = Mat::op_shift_left(g, 1);
    p = Mat::op_Xor(x, y);
    MatrixXu z = Mat::op_Xor(c, p);
    return z;
    // MatrixXu G(row, col), P(row, col), temp(row, col), c_mask(row, col), k_mask(row, col), G1(row, col), P1(row, col), C(row, col);
    // temp.setOnes();
    // MatrixXu keep_masks(6, 1);
    // keep_masks << 0x5555555555555555,
    //     0x3333333333333333,
    //     0x0F0F0F0F0F0F0F0F,
    //     0x00FF00FF00FF00FF,
    //     0x0000FFFF0000FFFF,
    //     0x00000000FFFFFFFF;
    // MatrixXu copy_masks(6, 1);
    // copy_masks << 0x5555555555555555,
    //     0x2222222222222222,
    //     0x0808080808080808,
    //     0x0080008000800080,
    //     0x0000800000008000,
    //     0x0000000080000000;
    // G = Boolean_Share::bool_and(x, y);
    // P = Mat::op_Xor(x, y);
    // for (int i = 0; i < 6; i++)
    // {
    //     c_mask = temp * copy_masks(i);
    //     k_mask = temp * keep_masks(i);
    //     G1 = Mat::op_shift_left(Mat::op_And(G, c_mask), 1);
    //     P1 = Mat::op_shift_left(Mat::op_And(P, c_mask), 1);
    //     for (int j = 0; j < i; j++)
    //     {
    //         G1 = Mat::op_Xor(Mat::op_shift_left(G1, int(pow(2, j))), G1);
    //         P1 = Mat::op_Xor(Mat::op_shift_left(P1, int(pow(2, j))), P1);
    //     }
    //     P1 = Mat::op_Xor(P1, k_mask);
    //     G = Mat::op_Xor(G, Boolean_Share::bool_and(P, G1));
    //     P = Boolean_Share::bool_and(P, P1);
    // }
    // C = Mat::op_shift_left(G, 1);
    // P = Mat::op_Xor(x, y);
    // return Mat::op_Xor(C, P);
}

// 采用New Primitives for Actively-Secure MPC over Rings with Applications to Private Machine Learning论文中的b2a的方法
// [x]  = c + [r] + 2 * c * [r]
// 为了减少通信使用int8(只转换符号位)

MatrixXu Boolean_Share::to_additive_share(MatrixXu &x)
{
    int row = x.rows();
    int col = x.cols();
    Matrix8 r_add_share(row, col), r_bool_share(row, col);
    r_add_share.setZero();
    r_bool_share.setZero();
    Matrix8 x_byte = x.cast<u8>();
    Matrix8 c = Mat::op_Xor(r_bool_share, x_byte);
    c = Boolean_Share::reveal(c);
    Matrix8 add_val_byte(row, col);
    MatrixXu add_val(row, col);
    add_val_byte.setZero();
    Matrix8 ci = Mat::op_And(1, c);
    if (party == 0)
    {
        Matrix8 temp = ci + r_add_share - 2 * ci.cwiseProduct(r_add_share);
        add_val_byte = add_val_byte + temp;
    }
    else
    {
        Matrix8 temp = r_add_share - 2 * ci.cwiseProduct(r_add_share);
        add_val_byte = add_val_byte + temp;
    }
    add_val = add_val_byte.cast<u64>();
    return add_val;
}

MatrixXu Boolean_Share::secret_share_for_test(MatrixXu &share)
{
    MatrixXu A_plus_mat = Mat::toMatrix(Mat::A_plus);
    MatrixXu truth = Mat::randomMatrixXu(Config::config->B, Config::config->hiddenDim);
    // MatrixXu truth(Config::config->B, Config::config->D);
    // truth << 18446744073709551611ull;
    MatrixXu temp(Config::config->B, Config::config->hiddenDim);
    for (int i = 0; i < Config::config->B; i++)
    {
        for (int j = 0; j < Config::config->hiddenDim; j++)
        {
            MatrixXu temp_vector(Config::config->M, 1);
            temp_vector(0, 0) = truth(i, j);
            for (int k = 1; k < Config::config->M; k++)
            {
                temp_vector(k, 0) = Constant::Util::random_u64();
            }
            MatrixXu temp1 = A_plus_mat.row(party) * temp_vector;
            temp(i, j) = temp1(0, 0);
        }
    }
    share = temp;
    // if (party == 0)
    // {
    //     share(0, 0) = 6475874102933811596ull;
    // }
    // else if (party == 1)
    // {
    //     share(0, 0) = 10656984297989353986ull;
    // }
    // else if (party == 2)
    // {
    //     share(0, 0) = 4181110195055542390ull;
    // }
    return truth;
}

MatrixXu add_secret_share(MatrixXu &share)
{
    vector<MatrixXu> sec_share;
    MatrixXu temp = Mat::randomMatrixXu(Config::config->B, Config::config->D);
    MatrixXu truth = temp;
    // cout << "The truth:" << endl
    //      << temp << endl;
    for (int i = 0; i < Config::config->M - 1; i++)
    {
        MatrixXu noise = Mat::randomMatrixXu(Config::config->B, Config::config->D);
        sec_share.push_back(noise);
        temp -= noise;
    }
    sec_share.push_back(temp);
    share = sec_share[party];
    // cout << "share:" << endl
    //      << share << endl;
    return truth;
}

void Boolean_Share::test()
{
    // // test for byte and byte

    // Matrix8 share1(B, D), share2(B, D);
    // Matrix8 truth1_u8 = Boolean_Share::secret_share(share1);
    // Matrix8 truth2_u8 = Boolean_Share::secret_share(share2);
    // MatrixXu truth1 = truth1_u8.cast<u64>();
    // MatrixXu truth2 = truth2_u8.cast<u64>();
    // cout << "The truth1:" << endl
    //      << truth1 << endl;
    // cout << "The truth2:" << endl
    //      << truth2 << endl;
    // Matrix8 truth_u8 = Mat::op_And(truth1_u8, truth2_u8);
    // MatrixXu truth = truth_u8.cast<u64>();
    // Matrix8 share = Boolean_Share::byte_and_byte(share1, share2);
    // Matrix8 result_u8 = Boolean_Share::reveal(share);
    // MatrixXu result = result_u8.cast<u64>();
    // cout << "The truth result:" << endl
    //      << truth << endl;
    // cout << "The reveal result:" << endl
    //      << result << endl;
    // if (truth == result)
    // {
    //     cout << "True" << endl;
    // }
    // else
    // {
    //     cout << "False" << endl;
    // }

    // test for add
    // MatrixXu share1(Config::config->B, Config::config->D), share2(Config::config->B, Config::config->D);
    // MatrixXu truth1 = Boolean_Share::secret_share(share1);
    // MatrixXu truth2 = Boolean_Share::secret_share(share2);
    // MatrixXu truth = truth1 + truth2;
    // MatrixXu share = Boolean_Share::add(share1, share2);
    // MatrixXu result = Boolean_Share::reveal(share);
    // cout << "The truth result:" << endl
    //      << truth << endl;
    // cout << "The reveal result:" << endl
    //      << result << endl;
    // if (truth == result)
    // {
    //     cout << "True" << endl;
    // }
    // else
    // {
    //     cout << "False" << endl;
    // }

    // test for B2A(由于只写了bool_share为1bit时的转换 因此秘密分享时真实值只能为0或1)
    // MatrixXu bool_share(Config::config->B, Config::config->hiddenDim);
    // MatrixXu truth = Boolean_Share::secret_share(bool_share);
    // cout << "The truth result:" << endl
    //      << truth << endl;
    // // cout << bool_share <<endl;
    // MatrixXu add_share = Boolean_Share::to_additive_share(bool_share);
    // MatrixXu result = Boolean_Share::add_reveal(add_share);
    // cout << "The reveal result:" << endl
    //      << result << endl;
    // if (truth == result)
    // {
    //     cout << "True" << endl;
    // }
    // else
    // {
    //     cout << "False" << endl;
    // }

    // test for A2B
    // MatrixXu vector_share = Mat::randomMatrixXu(Config::config->B, Config::config->hiddenDim);
    // MatrixXu m = Offline::m2;
    // MatrixXu truth = vector_share;
    // MatrixXu delta_vector_share = vector_share + m;
    // MatrixXu x = Offline::trun_2[party];
    // MatrixXu bool_share = Secret_Cmp::get_bool_share_bit(x, delta_vector_share);
    // MatrixXu result = Boolean_Share::reveal(bool_share);
    // if (truth == result)
    // {
    //     cout << "True" << endl;
    // }
    // else
    // {
    //     cout << "False" << endl;
    // }

    // test for v_share to v2.0_share
    // MatrixXu vector_truth = Mat::randomMatrixXu(Config::config->B, Config::config->D);
    // MatrixXu vector_share = IOManager::secret_share(vector_truth);
    // MatrixXu x(Config::config->B, Config::config->D);
    // MatrixXu delta_x = Secret_Cmp::secret_share(vector_share, x);
    // MatrixXu res = Secret_Mul::reveal(delta_x, x);
    // if(vector_truth == res)
    // {
    //     cout << "True" << endl;
    // }
    // else
    // {
    //     cout << "False" << endl;
    // }

    // test for comparison
    // MatrixXu vector_share(Config::config->B, Config::config->hiddenDim);
    // MatrixXu truth = Boolean_Share::secret_share_for_test(vector_share);
    // cout << "The truth result:" << endl
    //      << truth << endl;
    // MatrixXu sign_share = Secret_Cmp::get_sign(vector_share);
    // // cout << "sign_share:" << sign_share << endl;
    // MatrixXu sign = Secret_Mul::Mul_reveal(sign_share);
    // cout << "The reveal result:" << endl
    //      << sign << endl;

    // test for relu
    // MatrixXu vector_share = Mat::randomMatrixXu(Config::config->B, Config::config->hiddenDim);
    // MatrixXu delta_x = vector_share + Offline::m2;
    // MatrixXu x = Offline::trun_2[party];
    // Secret_Cmp::Relu(x, delta_x);
    // MatrixXu truth(Config::config->B, Config::config->hiddenDim);
    // for (int i = 0; i < vector_share.size(); i++)
    // {
    //     if (vector_share(i) >= 0 && vector_share(i) <= 9223372036854775808)
    //         truth(i) = vector_share(i);
    //     else
    //         truth(i) = 0;
    // }
    // MatrixXu res = Secret_Mul::reveal(delta_x, x);
    // if (res == truth)
    // {
    //     cout << "True" << endl;
    // }
    // else
    // {
    //     cout << "False" << endl;
    //     cout << "res: " << res << endl;
    //     cout << "truth: " << truth << endl;
    // }

    // test for sigmoid
    // MatrixXu vector_share = Mat::randomMatrixXu(Config::config->B, Config::config->numClass);
    // vector_share << 18446744073709027328, 18446744073708503040, 18446744073709289472, 262144, 1048576,104857600, 524288;
    // // cout << "vector_share:" << vector_share << endl;
    // MatrixXu delta_x = vector_share + Offline::m0;
    // MatrixXu x = Offline::trun_0[party];
    // Secret_Cmp::Sigmoid(x, delta_x);
    // MatrixXu truth(Config::config->B, Config::config->numClass);
    // for (int i = 0; i < vector_share.size(); i++)
    // {
    //     if (vector_share(i) >= 9223372036854775808 && vector_share(i) <= 18446744073709027328)
    //         truth(i) = 0;
    //     else if (vector_share(i) >= 524288 && vector_share(i) < 9223372036854775808)
    //         truth(i) = 1048576;
    //     else
    //         truth(i) = vector_share(i) + 524288;
    // }
    // MatrixXu res = Secret_Mul::reveal(delta_x, x);
    // if (res == truth)
    // {
    //     cout << "True" << endl;
    // }
    // else
    // {
    //     cout << "False" << endl;
    // }
    // cout << "res: " << res << endl;
    // cout << "truth: " << truth << endl;
}