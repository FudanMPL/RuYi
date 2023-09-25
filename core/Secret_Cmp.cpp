#include "Secret_Cmp.h"
#include "Secret_Mul.h"
 
MatrixXu Secret_Cmp::offline_delta_x;
MatrixXu Secret_Cmp::offline_temp;


void Secret_Cmp::get_Share(int row, int col)
{ 
    Secret_Cmp::offline_delta_x = MatrixXu::Zero(row, col);
    Secret_Cmp::offline_temp = MatrixXu(row, col);
    vector<MatrixXu> recv(Config::config->M);
    vector<MatrixXu> delta_x_share(Config::config->M);
    for (int l = 0; l < Config::config->M; l++)
    {
        delta_x_share[l].resize(row, col);
    }
    MatrixXu A_plus_mat = Mat::toMatrix(Mat::A_plus);
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            MatrixXu temp_vector(Config::config->M, 1);
            temp_vector(0, 0) = Secret_Cmp::offline_delta_x(i, j);
            for (int k = 1; k < Config::config->M; k++)
            {
                temp_vector(k, 0) = Constant::Util::random_u64();
            }
            for (int l = 0; l < Config::config->M; l++)
            {
                MatrixXu temp1 = A_plus_mat.row(l) * temp_vector;
                delta_x_share[l](i, j) = temp1(0, 0);
            }
        }
    }
    for (int i = 0; i < Config::config->M; i++)
    {
        if (i == party)
        {
            recv[i] = delta_x_share[i];
        }
        else
        {
            SocketUtil::send(delta_x_share[i], i);
            SocketUtil::receive(recv[i], i);
        }
    } 
    Secret_Cmp::offline_temp.setZero();
    for (int i = 0; i < Config::config->M; i++)
    {
        Secret_Cmp::offline_temp += recv[i] * Mat::inv_for_mul[i];
    }
}

MatrixXu Secret_Cmp::to_Boolean_Share(MatrixXu &x)
{
    int row = x.rows();
    int col = x.cols();
    // 先把空间秘密分享转为additive share
    MatrixXu share(row, col);

    share = x * Mat::inv_for_mul[party];

    // A2B
    vector<MatrixXu> temp_shares;
    for (int i = 0; i < Config::config->M; i++)
    {
        if (party == i)
        {
            temp_shares.push_back(share);
        }
        else
        {
            MatrixXu temp(row, col);
            temp.setZero();
            temp_shares.push_back(temp);
        }
    }
    MatrixXu boolean_share = temp_shares[0];
    for (int i = 1; i < Config::config->M; i++)
    {
        boolean_share = Boolean_Share::add(boolean_share, temp_shares[i]);
    }
    return boolean_share;
}

MatrixXu Secret_Cmp::get_bool_share_bit(MatrixXu &x)
{
    int row = x.rows();
    int col = x.cols();
    MatrixXu r_bool(row, col), r_add(row, col);
    if (col == Config::config->numClass)
    {
        r_add = Offline::boolean_r_add0[party];
        r_bool = Offline::boolean_r_bool0[party];
    }
    else if (col == Config::config->hiddenDim)
    {
        r_add = Offline::boolean_r_add1[party];
        r_bool = Offline::boolean_r_bool1[party];
    }

    // 先把空间秘密分享转为additive share
    MatrixXu share(row, col);

    share = x * Mat::inv_for_mul[party];

    MatrixXu a = share + r_add;
    MatrixXu rev_a = Boolean_Share::add_reveal(a);
    MatrixXu max(row, col), mat_IE(row, col);
    max.setConstant(UINT64_MAX);
    mat_IE.setConstant(1);
    MatrixXu a_plus_one(row, col), t(row, col);
    a_plus_one = rev_a + mat_IE;
    t = max - r_bool;
    // if (party != 0)
    // {
        t = r_bool;
        a_plus_one.setZero();
    // }
    MatrixXu boolean_share = Boolean_Share::add(a_plus_one, t);
    MatrixXu bool_share_bit = Mat::op_shift_right(boolean_share, Config::config->BIT_LENGTH - 1);
    return bool_share_bit;
}

MatrixXu Secret_Cmp::get_bool_share_bit(MatrixXu &x, MatrixXu &delta_x)
{
    int row = x.rows();
    int col = x.cols();
    MatrixXu r_bool(row, col), r_add(row, col);
    if (col == Config::config->numClass)
    {
        r_add = Offline::boolean_r_add0[party];
        r_bool = Offline::boolean_r_bool0[party];
    }
    else if (col == Config::config->hiddenDim)
    {
        r_add = Offline::boolean_r_add1[party];
        r_bool = Offline::boolean_r_bool1[party];
    }

    // 先把空间秘密分享转为additive share

    MatrixXu max(row, col), mat_IE(row, col);
    max.setConstant(UINT64_MAX);
    mat_IE.setConstant(1);
    MatrixXu delta_x_plus_one(row, col), t(row, col);
    delta_x_plus_one = delta_x + mat_IE;
    t = max - r_bool;
    if (party != 0)
    {
        t = r_bool;
        delta_x_plus_one.setZero();
    }
    MatrixXu boolean_share = Boolean_Share::add(delta_x_plus_one, t);
    MatrixXu bool_share_bit = Mat::op_shift_right(boolean_share, Config::config->BIT_LENGTH - 1);
    return bool_share_bit;
}

// used for v_share to v2.0_share
// MatrixXu Secret_Cmp::secret_share(MatrixXu &sign, MatrixXu &x)
// {
//     int row = sign.rows(), col = sign.cols();
//     // MatrixXu delta_x_truth = Mat::randomMatrixXu(row, col);
//     MatrixXu delta_x_truth = sign;
//     vector<MatrixXu> shares(Config::config->M);
//     vector<MatrixXu> temp(Config::config->M);
//     vector<MatrixXu> recv(Config::config->M);
//     MatrixXu truth = delta_x_truth - sign;
//     MatrixXu A_plus_mat = Mat::toMatrix(Mat::A_plus);
//     for (int l = 0; l < Config::config->M; l++)
//     {
//         temp[l].resize(row, col);
//     }
//     for (int i = 0; i < row; i++)
//     {
//         for (int j = 0; j < col; j++)
//         {
//             MatrixXu temp_vector(Config::config->M, 1);
//             temp_vector(0, 0) = truth(i, j);
//             for (int k = 1; k < Config::config->M; k++)
//             {
//                 temp_vector(k, 0) = Constant::Util::random_u64();
//             }
//             for (int l = 0; l < Config::config->M; l++)
//             {
//                 MatrixXu temp1 = A_plus_mat.row(l) * temp_vector;
//                 temp[l](i, j) = temp1(0, 0);
//             }
//         }
//     }
//     for (int i = 0; i < Config::config->M; i++)
//     {
//         MatrixXu share(row, col + col);
//         share << temp[i], delta_x_truth;
//         shares[i] = share;
//     }
//     for (int i = 0; i < Config::config->M; i++)
//     {
//         if (i == party)
//         {
//             recv[i] = shares[i];
//         }
//         else
//         {
//             SocketUtil::send(shares[i], i);
//             SocketUtil::receive(recv[i], i);
//         }
//     }
//     MatrixXu delta_x(row, col);
//     delta_x.setZero();
//     x.setZero();
//     for (int i = 0; i < Config::config->M; i++)
//     {
//         x += recv[i].leftCols(col) * Mat::inv_for_mul[i];
//         delta_x += recv[i].rightCols(col) * Mat::inv_for_mul[i];
//     }
//     return delta_x;
// }

MatrixXu Secret_Cmp::secret_share(MatrixXu &sign, MatrixXu &x)
{
    // cout << "in" << endl;
    int row = sign.rows(), col = sign.cols();
    MatrixXu offline_delta_x, offline_temp;
    if(row == Config::config->B && col == Config::config->numClass)
    {
        offline_delta_x = Secret_Cmp::offline_delta_x.bottomRows(Config::config->B);
        offline_temp = Secret_Cmp::offline_temp.bottomRows(Config::config->B);
    }
    else
    {
        offline_delta_x = Secret_Cmp::offline_delta_x;
        offline_temp = Secret_Cmp::offline_temp;
    }
    vector<MatrixXu> recv(Config::config->M);
    MatrixXu Deltax_share(row, col), Deltax(row, col);
    Deltax_share = sign + offline_delta_x;
    for (int i = 0; i < Config::config->M; i++)
    {
        if (i == party)
        {
            recv[i] = Deltax_share;
        }
        else
        {
            SocketUtil::send(Deltax_share, i);
        }
    }
    for (int i = 0; i < Config::config->M; i++)
    {
        if(i != party)
            SocketUtil::receive(recv[i], i);
    }
    Deltax.setZero();
    for (int i = 0; i < Config::config->M; i++)
    {
        Deltax += recv[i] * Mat::inv_for_mul[i];
    }
    x = offline_temp;
    return Deltax;
}

MatrixXu Secret_Cmp::get_sign(MatrixXu &x)
{
    MatrixXu add_share_bit = Boolean_Share::to_additive_share(x);
    MatrixXu sign = Mat::op_shift_left(add_share_bit, Config::config->DECIMAL_LENGTH);
    if (Mat::div_flag[party])
    {
        sign = sign / Mat::inv_for_div[party];
    }
    else
    {
        sign = sign * Mat::inv_for_div[party];
    }

    return sign;
}

MatrixXu Secret_Cmp::get_sign(MatrixXu &bool_bit, MatrixXu &x)
{
    MatrixXu add_share_bit = Boolean_Share::to_additive_share(bool_bit);
    MatrixXu sign = Mat::op_shift_left(add_share_bit, Config::config->DECIMAL_LENGTH);
    if (Mat::div_flag[party])
    {
        sign = sign / Mat::inv_for_div[party];
    }
    else
    {
        sign = sign * Mat::inv_for_div[party];
    }
    MatrixXu delta_x = Secret_Cmp::secret_share(sign, x);
    return delta_x;
}

MatrixXu Secret_Cmp::get_sign_xor_1(MatrixXu &x)
{
    MatrixXu bool_share_bit_xor_1 = x;
    if (party == 0)
    {
        bool_share_bit_xor_1 = Mat::op_Xor(1, x);
    }
    MatrixXu add_share_bit = Boolean_Share::to_additive_share(bool_share_bit_xor_1);
    MatrixXu sign = Mat::op_shift_left(add_share_bit, Config::config->DECIMAL_LENGTH);
    if (Mat::div_flag[party])
    {
        sign = sign / Mat::inv_for_div[party];
    }
    else
    {
        sign = sign * Mat::inv_for_div[party];
    }
    return sign;
}

MatrixXu Secret_Cmp::get_sign_xor_1(MatrixXu &bool_bit, MatrixXu &x)
{
    MatrixXu bool_share_bit_xor_1 = bool_bit;
    if (party == 0)
    {
        bool_share_bit_xor_1 = Mat::op_Xor(1, bool_bit);
    }
    MatrixXu add_share_bit = Boolean_Share::to_additive_share(bool_share_bit_xor_1);
    MatrixXu sign = Mat::op_shift_left(add_share_bit, Config::config->DECIMAL_LENGTH);
    if (Mat::div_flag[party])
    {
        sign = sign / Mat::inv_for_div[party];
    }
    else
    {
        sign = sign * Mat::inv_for_div[party];
    }
    MatrixXu delta_x = Secret_Cmp::secret_share(sign, x);
    return delta_x;
}

MatrixXu Secret_Cmp::Relu(MatrixXu x)
{
    MatrixXu bool_share_bit = Secret_Cmp::get_bool_share_bit(x);
    MatrixXu b = Secret_Cmp::get_sign_xor_1(bool_share_bit);
    // cout << "get_sign_xor_1: " << Secret_Mul::Mul_reveal(b) << endl;
    int row = x.rows(), col = x.cols();
    MatrixXu r(row, col), q(row, col), t(row, col);
    MatrixXu result = Secret_Mul::CwiseProduct(x, b, r, q, t);
    return result;
}

MatrixXu Secret_Cmp::Relu(MatrixXu &x, MatrixXu &delta_x)
{
    int row = x.rows(), col = x.cols();
    MatrixXu bool_share_bit = Secret_Cmp::get_bool_share_bit(x, delta_x);
    MatrixXu sign(row, col);
    MatrixXu delta_sign = Secret_Cmp::get_sign_xor_1(bool_share_bit, sign);
    
    // MatrixXu a = Secret_Mul::Mul_reveal(x);
    // MatrixXu b = Secret_Mul::Mul_reveal(sign);
    // MatrixXu c = a.cwiseProduct(b);
    // MatrixXu ab = IOManager::secret_share(c);
    MatrixXu ab = MatrixXu::Zero(row, col);
    MatrixXu res(row, col);

    MatrixXu delta_res = Secret_Mul::Element_Wise(x, delta_x, sign, delta_sign, res, ab);
    x = res;
    delta_x = delta_res;
    return delta_res;
}

MatrixXu Secret_Cmp::Sigmoid(MatrixXu x)
{
    int row = x.rows();
    int col = x.cols();
    MatrixXu result;
    // 第0方的秘密值用于形成真实值的(x+1/2)或(x-1/2)，其他方秘密值保持不变
    if (party == 0)
    {
        MatrixXu Mat_IE(row, col);
        Mat_IE.setConstant(Config::config->IE / 2);
        MatrixXu x_plus_ie = x + Mat_IE;
        MatrixXu x_sub_ie = x - Mat_IE;
        MatrixXu combination(row * 2, col);
        combination << x_plus_ie,
            x_sub_ie;
        MatrixXu com_bool_share_bit = Secret_Cmp::get_bool_share_bit(combination);
        MatrixXu r(row, col), q(row, col), t(row, col);
        MatrixXu b2_bool_share_bit = com_bool_share_bit.bottomRows(row);
        MatrixXu com_xor_1 = Secret_Cmp::get_sign_xor_1(com_bool_share_bit);
        MatrixXu b1_xor_1 = com_xor_1.topRows(row);
        MatrixXu b2_xor_1 = com_xor_1.bottomRows(row);
        MatrixXu b2 = Secret_Cmp::get_sign(b2_bool_share_bit);

        MatrixXu temp = Secret_Mul::CwiseProduct(b1_xor_1, b2, r, q, t);
        MatrixXu result1 = Secret_Mul::CwiseProduct(temp, x_plus_ie, r, q, t);
        result = result1 + b2_xor_1;
    }
    else
    {
        MatrixXu combination(row * 2, col);
        combination << x,
            x;
        MatrixXu com_bool_share_bit = Secret_Cmp::get_bool_share_bit(combination);
        MatrixXu r(row, col), q(row, col), t(row, col);
        MatrixXu b2_bool_share_bit = com_bool_share_bit.bottomRows(row);
        MatrixXu com_xor_1 = Secret_Cmp::get_sign_xor_1(com_bool_share_bit);
        MatrixXu b1_xor_1 = com_xor_1.topRows(row);
        MatrixXu b2_xor_1 = com_xor_1.bottomRows(row);
        MatrixXu b2 = Secret_Cmp::get_sign(b2_bool_share_bit);

        MatrixXu temp = Secret_Mul::CwiseProduct(b1_xor_1, b2, r, q, t);
        MatrixXu result1 = Secret_Mul::CwiseProduct(temp, x, r, q, t);
        result = result1 + b2_xor_1;
    }
    return result;
}

MatrixXu Secret_Cmp::Sigmoid(MatrixXu &x, MatrixXu &delta_x)
{
    int row = x.rows();
    int col = x.cols();
    // 第0方的秘密值用于形成真实值的(x+1/2)或(x-1/2)，其他方秘密值保持不变
    MatrixXu delta_combination(row * 2, col), combination(row * 2, col);
    MatrixXu Mat_IE(row, col);
    Mat_IE.setConstant(Config::config->IE / 2);
    MatrixXu x_plus_ie = delta_x + Mat_IE;
    MatrixXu x_sub_ie = delta_x - Mat_IE;
    delta_combination << x_plus_ie,
        x_sub_ie;
    combination << x,
        x;
    MatrixXu com_bool_share_bit = Secret_Cmp::get_bool_share_bit(combination, delta_combination);
    MatrixXu b2_bool_share_bit = com_bool_share_bit.bottomRows(row);
    MatrixXu com_xor_1(row * 2, col);
    MatrixXu delta_com_xor_1 = Secret_Cmp::get_sign_xor_1(com_bool_share_bit, com_xor_1);
    MatrixXu b1_xor_1 = com_xor_1.topRows(row), delta_b1_xor_1 = delta_com_xor_1.topRows(row);
    MatrixXu b2_xor_1 = com_xor_1.bottomRows(row), delta_b2_xor_1 = delta_com_xor_1.bottomRows(row);
    MatrixXu b2(row, col);
    MatrixXu delta_b2 = Secret_Cmp::get_sign(b2_bool_share_bit, b2);


    // MatrixXu a = Secret_Mul::Mul_reveal(b1_xor_1);
    // MatrixXu b = Secret_Mul::Mul_reveal(b2);
    // MatrixXu c = a.cwiseProduct(b);
    // MatrixXu ab = IOManager::secret_share(c);
    MatrixXu ab = MatrixXu::Zero(row, col);
    MatrixXu temp(row, col);

    MatrixXu delta_temp = Secret_Mul::Element_Wise(b1_xor_1, delta_b1_xor_1, b2, delta_b2, temp, ab);

    // MatrixXu u = Secret_Mul::Mul_reveal(temp);
    // MatrixXu v = Secret_Mul::Mul_reveal(x);
    // MatrixXu h = u.cwiseProduct(v);
    // MatrixXu uv = IOManager::secret_share(h);
    MatrixXu uv = MatrixXu::Zero(row, col);
    MatrixXu result1(row, col);

    MatrixXu delta_result1 = Secret_Mul::Element_Wise(temp, delta_temp, x, x_plus_ie, result1, uv);
    x = result1 + b2_xor_1;
    delta_x = delta_result1 + delta_b2_xor_1;
    return delta_x;
}