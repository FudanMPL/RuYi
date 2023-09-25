#include "Secret_Mul.h"
// Multiply_Triplets(for(B, D)X(D, numClass))
MatrixXu Secret_Mul::r0;
MatrixXu Secret_Mul::q0;
MatrixXu Secret_Mul::t0;

// Multiply_Triplets(for(D, B)X(B, numClass))
MatrixXu Secret_Mul::r1;
MatrixXu Secret_Mul::q1;
MatrixXu Secret_Mul::t1;

// Multiply_Triplets(for(B, D)X(D, hiddenDim))
MatrixXu Secret_Mul::r2;
MatrixXu Secret_Mul::q2;
MatrixXu Secret_Mul::t2;

// Multiply_Triplets(for(B, hiddenDim)X(hiddenDim, numClass))
MatrixXu Secret_Mul::r3;
MatrixXu Secret_Mul::q3;
MatrixXu Secret_Mul::t3;

// Multiply_Triplets(for(B, 1)X(1, hiddenDim))
MatrixXu Secret_Mul::r4;
MatrixXu Secret_Mul::q4;
MatrixXu Secret_Mul::t4;

// Multiply_Triplets(for(B, hiddenDim)X(hiddenDim, hiddenDim))
MatrixXu Secret_Mul::r5;
MatrixXu Secret_Mul::q5;
MatrixXu Secret_Mul::t5;

// Multiply_Triplets(for(D, B)X(B, hiddenDim))
MatrixXu Secret_Mul::r6;
MatrixXu Secret_Mul::q6;
MatrixXu Secret_Mul::t6;

// Multiply_Triplets(for(hiddenDim, B)X(B, 1))
MatrixXu Secret_Mul::r7;
MatrixXu Secret_Mul::q7;
MatrixXu Secret_Mul::t7;

// Multiply_Triplets(for(hiddenDim, B)X(B, hiddenDim))
MatrixXu Secret_Mul::r8;
MatrixXu Secret_Mul::q8;
MatrixXu Secret_Mul::t8;

void Secret_Mul::get_Triplets()
{
    ifstream infile0("Mul_Triplets/MT0_" + to_string(party) + ".txt");
    r0 = MatrixXu(Config::config->B, Config::config->D);
    q0 = MatrixXu(Config::config->D, Config::config->numClass);
    t0 = MatrixXu(Config::config->B, Config::config->numClass);
    for (int i = 0; i < Config::config->B; i++)
    {
        string s;
        getline(infile0, s);
        char *ch;
        ch = const_cast<char *>(s.c_str());
        for (int j = 0; j < Config::config->D; j++)
        {
            r0(i, j) = Constant::Util::getu64(ch);
        }
    }
    for (int i = 0; i < Config::config->D; i++)
    {
        string s;
        getline(infile0, s);
        char *ch;
        ch = const_cast<char *>(s.c_str());
        for (int j = 0; j < Config::config->numClass; j++)
        {
            q0(i, j) = Constant::Util::getu64(ch);
        }
    }
    for (int i = 0; i < Config::config->B; i++)
    {
        string s;
        getline(infile0, s);
        char *ch;
        ch = const_cast<char *>(s.c_str());
        for (int j = 0; j < Config::config->numClass; j++)
        {
            t0(i, j) = Constant::Util::getu64(ch);
        }
    }

    ifstream infile1("Mul_Triplets/MT1_" + to_string(party) + ".txt");
    r1 = MatrixXu(Config::config->D, Config::config->B);
    q1 = MatrixXu(Config::config->B, Config::config->numClass);
    t1 = MatrixXu(Config::config->D, Config::config->numClass);
    for (int i = 0; i < Config::config->D; i++)
    {
        string s;
        getline(infile1, s);
        char *ch;
        ch = const_cast<char *>(s.c_str());
        for (int j = 0; j < Config::config->B; j++)
        {
            r1(i, j) = Constant::Util::getu64(ch);
        }
    }
    for (int i = 0; i < Config::config->B; i++)
    {
        string s;
        getline(infile1, s);
        char *ch;
        ch = const_cast<char *>(s.c_str());
        for (int j = 0; j < Config::config->numClass; j++)
        {
            q1(i, j) = Constant::Util::getu64(ch);
        }
    }
    for (int i = 0; i < Config::config->D; i++)
    {
        string s;
        getline(infile1, s);
        char *ch;
        ch = const_cast<char *>(s.c_str());
        for (int j = 0; j < Config::config->numClass; j++)
        {
            t1(i, j) = Constant::Util::getu64(ch);
        }
    }
    ifstream infile2("Mul_Triplets/MT2_" + to_string(party) + ".txt");
    r2 = MatrixXu(Config::config->B, Config::config->D);
    q2 = MatrixXu(Config::config->D, Config::config->hiddenDim);
    t2 = MatrixXu(Config::config->B, Config::config->hiddenDim);
    for (int i = 0; i < Config::config->B; i++)
    {
        string s;
        getline(infile2, s);
        char *ch;
        ch = const_cast<char *>(s.c_str());
        for (int j = 0; j < Config::config->D; j++)
        {
            r2(i, j) = Constant::Util::getu64(ch);
        }
    }
    for (int i = 0; i < Config::config->D; i++)
    {
        string s;
        getline(infile2, s);
        char *ch;
        ch = const_cast<char *>(s.c_str());
        for (int j = 0; j < Config::config->hiddenDim; j++)
        {
            q2(i, j) = Constant::Util::getu64(ch);
        }
    }
    for (int i = 0; i < Config::config->B; i++)
    {
        string s;
        getline(infile2, s);
        char *ch;
        ch = const_cast<char *>(s.c_str());
        for (int j = 0; j < Config::config->hiddenDim; j++)
        {
            t2(i, j) = Constant::Util::getu64(ch);
        }
    }
    ifstream infile3("Mul_Triplets/MT3_" + to_string(party) + ".txt");
    r3 = MatrixXu(Config::config->B, Config::config->hiddenDim);
    q3 = MatrixXu(Config::config->hiddenDim, Config::config->numClass);
    t3 = MatrixXu(Config::config->B, Config::config->numClass);
    for (int i = 0; i < Config::config->B; i++)
    {
        string s;
        getline(infile3, s);
        char *ch;
        ch = const_cast<char *>(s.c_str());
        for (int j = 0; j < Config::config->hiddenDim; j++)
        {
            r3(i, j) = Constant::Util::getu64(ch);
        }
    }
    for (int i = 0; i < Config::config->hiddenDim; i++)
    {
        string s;
        getline(infile3, s);
        char *ch;
        ch = const_cast<char *>(s.c_str());
        for (int j = 0; j < Config::config->numClass; j++)
        {
            q3(i, j) = Constant::Util::getu64(ch);
        }
    }
    for (int i = 0; i < Config::config->B; i++)
    {
        string s;
        getline(infile3, s);
        char *ch;
        ch = const_cast<char *>(s.c_str());
        for (int j = 0; j < Config::config->numClass; j++)
        {
            t3(i, j) = Constant::Util::getu64(ch);
        }
    }
    ifstream infile4("Mul_Triplets/MT4_" + to_string(party) + ".txt");
    r4 = MatrixXu(Config::config->B, Config::config->numClass);
    q4 = MatrixXu(Config::config->numClass, Config::config->hiddenDim);
    t4 = MatrixXu(Config::config->B, Config::config->hiddenDim);
    for (int i = 0; i < Config::config->B; i++)
    {
        string s;
        getline(infile4, s);
        char *ch;
        ch = const_cast<char *>(s.c_str());
        for (int j = 0; j < Config::config->numClass; j++)
        {
            r4(i, j) = Constant::Util::getu64(ch);
        }
    }
    for (int i = 0; i < Config::config->numClass; i++)
    {
        string s;
        getline(infile4, s);
        char *ch;
        ch = const_cast<char *>(s.c_str());
        for (int j = 0; j < Config::config->hiddenDim; j++)
        {
            q4(i, j) = Constant::Util::getu64(ch);
        }
    }
    for (int i = 0; i < Config::config->B; i++)
    {
        string s;
        getline(infile4, s);
        char *ch;
        ch = const_cast<char *>(s.c_str());
        for (int j = 0; j < Config::config->hiddenDim; j++)
        {
            t4(i, j) = Constant::Util::getu64(ch);
        }
    }
    ifstream infile5("Mul_Triplets/MT5_" + to_string(party) + ".txt");
    r5 = MatrixXu(Config::config->B, Config::config->hiddenDim);
    q5 = MatrixXu(Config::config->hiddenDim, Config::config->hiddenDim);
    t5 = MatrixXu(Config::config->B, Config::config->hiddenDim);
    for (int i = 0; i < Config::config->B; i++)
    {
        string s;
        getline(infile5, s);
        char *ch;
        ch = const_cast<char *>(s.c_str());
        for (int j = 0; j < Config::config->hiddenDim; j++)
        {
            r5(i, j) = Constant::Util::getu64(ch);
        }
    }
    for (int i = 0; i < Config::config->hiddenDim; i++)
    {
        string s;
        getline(infile5, s);
        char *ch;
        ch = const_cast<char *>(s.c_str());
        for (int j = 0; j < Config::config->hiddenDim; j++)
        {
            q5(i, j) = Constant::Util::getu64(ch);
        }
    }
    for (int i = 0; i < Config::config->B; i++)
    {
        string s;
        getline(infile5, s);
        char *ch;
        ch = const_cast<char *>(s.c_str());
        for (int j = 0; j < Config::config->hiddenDim; j++)
        {
            t5(i, j) = Constant::Util::getu64(ch);
        }
    }
    ifstream infile6("Mul_Triplets/MT6_" + to_string(party) + ".txt");
    r6 = MatrixXu(Config::config->D, Config::config->B);
    q6 = MatrixXu(Config::config->B, Config::config->hiddenDim);
    t6 = MatrixXu(Config::config->D, Config::config->hiddenDim);
    for (int i = 0; i < Config::config->D; i++)
    {
        string s;
        getline(infile6, s);
        char *ch;
        ch = const_cast<char *>(s.c_str());
        for (int j = 0; j < Config::config->B; j++)
        {
            r6(i, j) = Constant::Util::getu64(ch);
        }
    }
    for (int i = 0; i < Config::config->B; i++)
    {
        string s;
        getline(infile6, s);
        char *ch;
        ch = const_cast<char *>(s.c_str());
        for (int j = 0; j < Config::config->hiddenDim; j++)
        {
            q6(i, j) = Constant::Util::getu64(ch);
        }
    }
    for (int i = 0; i < Config::config->D; i++)
    {
        string s;
        getline(infile6, s);
        char *ch;
        ch = const_cast<char *>(s.c_str());
        for (int j = 0; j < Config::config->hiddenDim; j++)
        {
            t6(i, j) = Constant::Util::getu64(ch);
        }
    }
    ifstream infile7("Mul_Triplets/MT7_" + to_string(party) + ".txt");
    r7 = MatrixXu(Config::config->hiddenDim, Config::config->B);
    q7 = MatrixXu(Config::config->B, Config::config->numClass);
    t7 = MatrixXu(Config::config->hiddenDim, Config::config->numClass);
    for (int i = 0; i < Config::config->hiddenDim; i++)
    {
        string s;
        getline(infile7, s);
        char *ch;
        ch = const_cast<char *>(s.c_str());
        for (int j = 0; j < Config::config->B; j++)
        {
            r7(i, j) = Constant::Util::getu64(ch);
        }
    }
    for (int i = 0; i < Config::config->B; i++)
    {
        string s;
        getline(infile7, s);
        char *ch;
        ch = const_cast<char *>(s.c_str());
        for (int j = 0; j < Config::config->numClass; j++)
        {
            q7(i, j) = Constant::Util::getu64(ch);
        }
    }
    for (int i = 0; i < Config::config->hiddenDim; i++)
    {
        string s;
        getline(infile7, s);
        char *ch;
        ch = const_cast<char *>(s.c_str());
        for (int j = 0; j < Config::config->numClass; j++)
        {
            t7(i, j) = Constant::Util::getu64(ch);
        }
    }

    ifstream infile8("Mul_Triplets/MT8_" + to_string(party) + ".txt");
    r8 = MatrixXu(Config::config->hiddenDim, Config::config->B);
    q8 = MatrixXu(Config::config->B, Config::config->hiddenDim);
    t8 = MatrixXu(Config::config->hiddenDim, Config::config->hiddenDim);
    for (int i = 0; i < Config::config->hiddenDim; i++)
    {
        string s;
        getline(infile8, s);
        char *ch;
        ch = const_cast<char *>(s.c_str());
        for (int j = 0; j < Config::config->B; j++)
        {
            r8(i, j) = Constant::Util::getu64(ch);
        }
    }
    for (int i = 0; i < Config::config->B; i++)
    {
        string s;
        getline(infile8, s);
        char *ch;
        ch = const_cast<char *>(s.c_str());
        for (int j = 0; j < Config::config->hiddenDim; j++)
        {
            q8(i, j) = Constant::Util::getu64(ch);
        }
    }
    for (int i = 0; i < Config::config->hiddenDim; i++)
    {
        string s;
        getline(infile8, s);
        char *ch;
        ch = const_cast<char *>(s.c_str());
        for (int j = 0; j < Config::config->hiddenDim; j++)
        {
            t8(i, j) = Constant::Util::getu64(ch);
        }
    }

    cout << "get triplets" << endl;
}

void Secret_Mul::Mul_truncation(MatrixXu &z)
{
    int row = z.rows();
    int col = z.cols();
    MatrixXu trun, trun_;
    if (row == Config::config->B && col == Config::config->numClass)
    {
        trun = Offline::trun0[party];
        trun_ = Offline::trun_0[party];
    }
    else if (row == Config::config->Config::config->D && col == Config::config->numClass)
    {
        trun = Offline::trun1[party];
        trun_ = Offline::trun_1[party];
    }
    else if (row == Config::config->B && col == Config::config->hiddenDim)
    {
        trun = Offline::trun2[party];
        trun_ = Offline::trun_2[party];
    }
    else if (row == Config::config->Config::config->D && col == Config::config->hiddenDim)
    {
        trun = Offline::trun3[party];
        trun_ = Offline::trun_3[party];
    }
    else if (row == Config::config->hiddenDim && col == Config::config->numClass)
    {
        trun = Offline::trun4[party];
        trun_ = Offline::trun_4[party];
    }
    else if (row == Config::config->hiddenDim && col == Config::config->hiddenDim)
    {
        trun = Offline::trun5[party];
        trun_ = Offline::trun_5[party];
    }
    if (party == 0)
    {
        MatrixXu z0_sub_r0 = z - trun;
        MatrixXu reveal = Secret_Mul::Mul_reveal(z0_sub_r0);
        MatrixXu z_sub_r;
        if (Mat::div_flag[party])
        {
            z_sub_r = reveal / Mat::inv_for_div[party];
        }
        else
        {
            z_sub_r = reveal * Mat::inv_for_div[party];
        }
        Mat::truncateMatrixXu(z_sub_r);
        z = trun_ + z_sub_r;
    }
    else
    {
        MatrixXu z_sub_r = z - trun;
        Secret_Mul::Mul_reveal(z_sub_r);
        z = trun_;
    }
}

MatrixXu Secret_Mul::Multiply(MatrixXu &x, MatrixXu &y, MatrixXu &r, MatrixXu &q, MatrixXu &t)
{
    MatrixXu result;
    MatrixXu y_plus_q = y + q;
    MatrixXu x_plus_r = x + r;

    MatrixXu Y_plus_Q, X_plus_R, combination, rev_combine;
    // (B, D)*(D, numClass)
    if (x.rows() == Config::config->B && x.cols() == Config::config->Config::config->D && y.rows() == Config::config->D && y.cols() == Config::config->numClass)
    {
        combination.resize(Config::config->B + Config::config->numClass, Config::config->D);
        MatrixXu y_plus_q_trans = y_plus_q.transpose();
        combination << x_plus_r,
            y_plus_q_trans;
        MatrixXu rev_combine = Secret_Mul::Mul_reveal(combination);
        Y_plus_Q = rev_combine.bottomRows(Config::config->numClass).transpose();
        X_plus_R = rev_combine.topRows(Config::config->B);
    }
    // (D, B)*(B, numClass)
    else if (x.rows() == Config::config->D && x.cols() == Config::config->B && y.rows() == Config::config->B && y.cols() == Config::config->numClass)
    {
        combination.resize(Config::config->D + Config::config->numClass, Config::config->B);
        MatrixXu y_plus_q_trans = y_plus_q.transpose();
        combination << x_plus_r,
            y_plus_q_trans;
        MatrixXu rev_combine = Secret_Mul::Mul_reveal(combination);
        Y_plus_Q = rev_combine.bottomRows(Config::config->numClass).transpose();
        X_plus_R = rev_combine.topRows(Config::config->D);
    }
    // (B, D)*(D, hiddenDim)
    else if (x.rows() == Config::config->B && x.cols() == Config::config->D && y.rows() == Config::config->D && y.cols() == Config::config->hiddenDim)
    {
        combination.resize(Config::config->B + Config::config->hiddenDim, Config::config->D);
        MatrixXu y_plus_q_trans = y_plus_q.transpose();
        combination << x_plus_r,
            y_plus_q_trans;
        MatrixXu rev_combine = Secret_Mul::Mul_reveal(combination);
        Y_plus_Q = rev_combine.bottomRows(Config::config->hiddenDim).transpose();
        X_plus_R = rev_combine.topRows(Config::config->B);
    }
    // (B, hiddenDim)*(hiddenDim, numClass)
    else if (x.rows() == Config::config->B && x.cols() == Config::config->hiddenDim && y.rows() == Config::config->hiddenDim && y.cols() == Config::config->numClass)
    {
        combination.resize(Config::config->B + Config::config->numClass, Config::config->hiddenDim);
        MatrixXu y_plus_q_trans = y_plus_q.transpose();
        combination << x_plus_r,
            y_plus_q_trans;
        MatrixXu rev_combine = Secret_Mul::Mul_reveal(combination);
        Y_plus_Q = rev_combine.bottomRows(Config::config->numClass).transpose();
        X_plus_R = rev_combine.topRows(Config::config->B);
    }
    // (B, numClass)*(numClass, hiddenDim)
    else if (x.rows() == Config::config->B && x.cols() == Config::config->numClass && y.rows() == Config::config->numClass && y.cols() == Config::config->hiddenDim)
    {
        combination.resize(Config::config->B + Config::config->hiddenDim, Config::config->numClass);
        MatrixXu y_plus_q_trans = y_plus_q.transpose();
        combination << x_plus_r,
            y_plus_q_trans;
        MatrixXu rev_combine = Secret_Mul::Mul_reveal(combination);
        Y_plus_Q = rev_combine.bottomRows(Config::config->hiddenDim).transpose();
        X_plus_R = rev_combine.topRows(Config::config->B);
    }
    // (B, hiddenDim)*(hiddenDim, hiddenDim)
    else if (x.rows() == Config::config->B && x.cols() == Config::config->hiddenDim && y.rows() == Config::config->hiddenDim && y.cols() == Config::config->hiddenDim)
    {
        combination.resize(Config::config->B + Config::config->hiddenDim, Config::config->hiddenDim);
        combination << x_plus_r,
            y_plus_q;
        MatrixXu rev_combine = Secret_Mul::Mul_reveal(combination);
        Y_plus_Q = rev_combine.bottomRows(Config::config->hiddenDim);
        X_plus_R = rev_combine.topRows(Config::config->B);
    }
    // (hiddenDim, B)*(B, numClass)
    else if (x.rows() == Config::config->hiddenDim && x.cols() == Config::config->B && y.rows() == Config::config->B && y.cols() == Config::config->numClass)
    {
        combination.resize(Config::config->hiddenDim + Config::config->numClass, Config::config->B);
        MatrixXu y_plus_q_trans = y_plus_q.transpose();
        combination << x_plus_r,
            y_plus_q_trans;
        MatrixXu rev_combine = Secret_Mul::Mul_reveal(combination);
        Y_plus_Q = rev_combine.bottomRows(Config::config->numClass).transpose();
        X_plus_R = rev_combine.topRows(Config::config->hiddenDim);
    }
    // (D, B)*(B, hiddenDim)
    else if (x.rows() == Config::config->D && x.cols() == Config::config->B && y.rows() == Config::config->B && y.cols() == Config::config->hiddenDim)
    {
        combination.resize(Config::config->D + Config::config->hiddenDim, Config::config->B);
        MatrixXu y_plus_q_trans = y_plus_q.transpose();
        combination << x_plus_r,
            y_plus_q_trans;
        MatrixXu rev_combine = Secret_Mul::Mul_reveal(combination);
        Y_plus_Q = rev_combine.bottomRows(Config::config->hiddenDim).transpose();
        X_plus_R = rev_combine.topRows(Config::config->D);
    }
    // (hiddenDim, B)*(B, hiddenDim)
    else if (x.rows() == Config::config->hiddenDim && x.cols() == Config::config->B && y.rows() == Config::config->B && y.cols() == Config::config->hiddenDim)
    {
        combination.resize(Config::config->hiddenDim + Config::config->hiddenDim, Config::config->B);
        MatrixXu y_plus_q_trans = y_plus_q.transpose();
        combination << x_plus_r,
            y_plus_q_trans;
        MatrixXu rev_combine = Secret_Mul::Mul_reveal(combination);
        Y_plus_Q = rev_combine.bottomRows(Config::config->hiddenDim).transpose();
        X_plus_R = rev_combine.topRows(Config::config->hiddenDim);
    }
    else
    {
        cout << "no multiply for " << x.rows() << "*" << x.cols() << "  " << y.rows() << " " << y.cols() << endl;
    }
    result = x * Y_plus_Q - X_plus_R * q + t;
    Secret_Mul::Mul_truncation(result);
    return result;
}

MatrixXu Secret_Mul::Mul(MatrixXu &x, MatrixXu &delta_x, MatrixXu &y, MatrixXu &delta_y, MatrixXu &z, MatrixXu &xy)
{
    int row = z.rows();
    int col = z.cols();
    MatrixXu z_(row, col);
    if (row == Config::config->B && col == Config::config->numClass)
    {
        z = Offline::trun0[party];
        z_ = Offline::trun_0[party];
    }
    else if (row == Config::config->Config::config->D && col == Config::config->numClass)
    {
        z = Offline::trun1[party];
        z_ = Offline::trun_1[party];
    }
    else if (row == Config::config->B && col == Config::config->hiddenDim)
    {
        z = Offline::trun2[party];
        z_ = Offline::trun_2[party];
    }
    else if (row == Config::config->Config::config->D && col == Config::config->hiddenDim)
    {
        z = Offline::trun3[party];
        z_ = Offline::trun_3[party];
    }
    else if (row == Config::config->hiddenDim && col == Config::config->numClass)
    {
        z = Offline::trun4[party];
        z_ = Offline::trun_4[party];
    }
    else if (row == Config::config->hiddenDim && col == Config::config->hiddenDim)
    {
        z = Offline::trun5[party];
        z_ = Offline::trun_5[party];
    }
    MatrixXu delta_z_share = xy + z - delta_x * y - x * delta_y;
    if (party == 0)
    {
        delta_z_share = delta_z_share + delta_x * delta_y;
    }
    MatrixXu delta_z = Secret_Mul::Mul_reveal(delta_z_share);
    Mat::truncateMatrixXu(delta_z);
    z = z_;
    return delta_z;
}

// todo:三元组的生成 目前全部设置为0
MatrixXu Secret_Mul::CwiseProduct(MatrixXu &x, MatrixXu &y, MatrixXu &r, MatrixXu &q, MatrixXu &t)
{
    r.setZero();
    q.setZero();
    t.setZero();
    MatrixXu result;
    MatrixXu y_plus_q = y + q;
    MatrixXu x_plus_r = x + r;

    MatrixXu Y_plus_Q, X_plus_R, combination, rev_combine;
    if (x.rows() == Config::config->B && x.cols() == Config::config->numClass)
    {
        combination.resize(Config::config->B, Config::config->numClass * 2);
        combination << y_plus_q, x_plus_r;
        MatrixXu rev_combine = Secret_Mul::Mul_reveal(combination);
        Y_plus_Q = rev_combine.leftCols(Config::config->numClass);
        X_plus_R = rev_combine.rightCols(Config::config->numClass);
    }
    else if (x.rows() == Config::config->B && x.cols() == Config::config->hiddenDim)
    {
        combination.resize(Config::config->B, Config::config->hiddenDim + Config::config->hiddenDim);
        combination << y_plus_q, x_plus_r;
        MatrixXu rev_combine = Secret_Mul::Mul_reveal(combination);
        Y_plus_Q = rev_combine.leftCols(Config::config->hiddenDim);
        X_plus_R = rev_combine.rightCols(Config::config->hiddenDim);
    }
    result = x.cwiseProduct(Y_plus_Q) - X_plus_R.cwiseProduct(q) + t;
    Secret_Mul::Mul_truncation(result);
    return result;
}

MatrixXu Secret_Mul::Element_Wise(MatrixXu &x, MatrixXu &delta_x, MatrixXu &y, MatrixXu &delta_y, MatrixXu &z, MatrixXu &xy)
{
    int row = z.rows();
    int col = z.cols();
    MatrixXu z_(row, col);
    if (row == Config::config->B && col == Config::config->numClass)
    {
        z = Offline::trun0[party];
        z_ = Offline::trun_0[party];
    }
    else if (row == Config::config->B && col == Config::config->hiddenDim)
    {
        z = Offline::trun2[party];
        z_ = Offline::trun_2[party];
    }
    MatrixXu delta_z_share = xy + z - delta_x.cwiseProduct(y) - x.cwiseProduct(delta_y);
    if (party == 0)
    {
        delta_z_share = delta_z_share + delta_x.cwiseProduct(delta_y);
    }
    MatrixXu delta_z = Secret_Mul::Mul_reveal(delta_z_share);
    Mat::truncateMatrixXu(delta_z);
    z = z_;
    return delta_z;
}

MatrixXu Secret_Mul::constant_Mul(MatrixXu &x, double d)
{
    MatrixXu result = x * (Constant::Util::double_to_u64(d));
    Secret_Mul::Mul_truncation(result);
    return result;
}

MatrixXu Secret_Mul::Constant_Mul(MatrixXu &delta_x, MatrixXu &x, double d)
{
    MatrixXu delta_res = delta_x * (Constant::Util::double_to_u64(d));
    x = x * (Constant::Util::double_to_u64(d));
    Mat::truncateMatrixXu(delta_res);
    Secret_Mul::Mul_truncation(x);
    return delta_res;
}

MatrixXu Secret_Mul::reveal(MatrixXu &delta, MatrixXu &data)
{
    int row = data.rows();
    int col = data.cols();
    MatrixXu res;
    if (party == 0)
    {
        vector<MatrixXu> reveal_data(Config::config->M);
        reveal_data[0] = data;
        res = data * Mat::inv_for_mul[0];
        for (int i = 1; i < Config::config->M; i++)
        {
            SocketUtil::receive(reveal_data[i], i);
            res += reveal_data[i] * Mat::inv_for_mul[i];
        }
        res = delta - res;
        for (int i = 1; i < Config::config->M; i++)
        {
            SocketUtil::send(res, i);
        }
    }
    else
    {
        SocketUtil::send(data, 0);
        SocketUtil::receive(res, 0);
    }
    return res;
}

MatrixXu Secret_Mul::Mul_reveal(MatrixXu &data)
{
    vector<MatrixXu> shares(Config::config->M);
    int row = data.rows();
    int col = data.cols();
    MatrixXu res(row, col);
    res.setZero();
    for (int i = 0; i < Config::config->M; i++)
    {
        if (i == party)
        {
            shares[i] = data;
        }
        else
        {
            SocketUtil::send(data, i);
        }
    }
    for (int i = 0; i < Config::config->M; i++)
    {
        if (i != party)
            SocketUtil::receive(shares[i], i);
    }
    for (int i = 0; i < Config::config->M; i++)
    {
        res += shares[i] * Mat::inv_for_mul[i];
    }
    return res;
    // int row = data.rows();
    // int col = data.cols();
    // MatrixXu res;
    // if (party == 0)
    // {
    //     vector<MatrixXu> reveal_data(Config::config->M);
    //     reveal_data[0] = data;
    //     res = data * Mat::inv_for_mul[0];
    //     for (int i = 1; i < Config::config->M; i++)
    //     {
    //         SocketUtil::receive(reveal_data[i], i);
    //         res += reveal_data[i] * Mat::inv_for_mul[i];
    //     }
    //     for (int i = 1; i < Config::config->M; i++)
    //     {
    //         SocketUtil::send(res, i);
    //     }
    // }
    // else
    // {
    //     SocketUtil::send(data, 0);
    //     SocketUtil::receive(res, 0);
    // }
    // return res;
}
