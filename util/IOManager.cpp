#include "IOManager.h"

MatrixXu IOManager::train_data;
MatrixXu IOManager::train_label;
MatrixXu IOManager::train_data_delta;
MatrixXu IOManager::train_label_delta;
MatrixXu IOManager::test_data;
MatrixXu IOManager::test_label;
MatrixXu IOManager::test_data_delta;
MatrixXu IOManager::test_label_delta;

void IOManager::load_data(ifstream &in, MatrixXu &data, MatrixXu &label, int size)
{
    int i = 0;
    while (in)
    {
        string s;
        if (!getline(in, s))
            break;
        char *ch;
        ch = const_cast<char *>(s.c_str());
        int temp;
        // char c;
        temp = Constant::Util::getu64(ch);

        // two class for mnist and fashion_mnist
        // if (temp > 1)
        //     temp = 1;
        // else
        //     temp = 0;

        // two class for svhn
        // if (temp == 10)
        //     temp = 0;
        // else
        //     temp = 1;

        // label(i, 0) = temp * Config::config->IE;

        // one_hot for svhn
        // if (temp == 10)
        //     label(i, 0) = Config::config->IE;
        // else
        //     label(i, temp) = Config::config->IE;

        // one_hot for mnist and fashion_mnist
        label(i, temp) = Config::config->IE;

        for (int j = 0; j < Config::config->D; j++)
        {
            temp = Constant::Util::getint(ch);
            data(i, j) = temp * Config::config->IE / 255;
        }
        i++;
        if (i >= size)
            break;
    }
    // cout << label << endl;
    // DBGprint("n=%d\n", i);
}

MatrixXu IOManager::secret_share(MatrixXu &truth)
{
    int row = truth.rows(), col = truth.cols();
    MatrixXu A_plus_mat = Mat::toMatrix(Mat::A_plus);
    MatrixXu share(row, col), temp(row, col);
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
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
    return share;
}

MatrixXu IOManager::secret_share(MatrixXu &truth, MatrixXu &gamma)
{
    int row = truth.rows(), col = truth.cols();
    MatrixXu A_plus_mat = Mat::toMatrix(Mat::A_plus);
    MatrixXu share(row, col), temp(row, col);
    MatrixXu delta = gamma + truth;
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            MatrixXu temp_vector(Config::config->M, 1);
            temp_vector(0, 0) = gamma(i, j);
            for (int k = 1; k < Config::config->M; k++)
            {
                temp_vector(k, 0) = Constant::Util::random_u64();
            }
            MatrixXu temp1 = A_plus_mat.row(party) * temp_vector;
            temp(i, j) = temp1(0, 0);
        }
    }
    truth = temp;
    return delta;
}

void IOManager::secret_share(MatrixXu &data, MatrixXu &label, string category)
{
    MatrixXu A_plus_mat = Mat::toMatrix(Mat::A_plus);
    vector<ofstream> out_files(Config::config->M + 1);
    ofstream out_delta_file;
    for (int i = 0; i < Config::config->M + 1; ++i)
    {
        out_files[i].open("data/mnist/mnist_" + category + "_" + to_string(i) + ".csv", ios::out);
        // out_files[i].open("data/fashion_mnist/fashion_mnist_" + category + "_" + to_string(i) + ".csv", ios::out);
        // out_files[i].open("data/svhn/svhn_" + category + "_" + to_string(i) + ".csv", ios::out);
    }
    out_delta_file.open("data/mnist/mnist_" + category + "_delta" + ".csv", ios::out);
    // out_delta_file.open("data/fashion_mnist/fashion_mnist_" + category + "_delta" + ".csv", ios::out);
    // out_delta_file.open("data/svhn/svhn_" + category + "_delta" + ".csv", ios::out);
    srand(time(NULL));

    int r_x = data.rows();
    int c_x = data.cols();
    int r_y = label.rows();
    int c_y = label.cols();
    MatrixXu delta_x(r_x, c_x), gamma_x(r_x, c_x), delta_y(r_y, c_y), gamma_y(r_y, c_y);
    // gamma_x = Mat::randomMatrixXu(r_x, c_x);
    // gamma_y = Mat::randomMatrixXu(r_y, c_y);
    gamma_x.setZero();
    gamma_y.setZero();
    delta_x = gamma_x + data;
    delta_y = gamma_y + label;
    vector<MatrixXu> out_Matrix(Config::config->M + 1);
    MatrixXu out_delta(r_x, c_x + Config::config->numClass);
    for (int i = 0; i < Config::config->M + 1; ++i)
    {
        out_Matrix[i].resize(r_x, c_x + Config::config->numClass);
    }
    MatrixXu temp_vec(Config::config->M, 1);
    cout << r_x << " : " << c_x << endl;
    for (int i = 0; i < r_x; ++i)
    {
        for (int j = 0; j < Config::config->numClass; j++)
        {
            temp_vec(0, 0) = gamma_y(i, j);
            for (int k = 1; k < Config::config->M; k++)
            {
                temp_vec(k, 0) = Constant::Util::random_u64();
            }
            for (int k = 0; k < Config::config->M + 1; k++)
            {
                MatrixXu temp = A_plus_mat.row(k) * temp_vec;
                out_Matrix[k](i, j) = temp(0, 0);
            }
            out_delta(i, j) = delta_y(i, j);
        }
    }
    for (int i = 0; i < r_x; i++)
    {
        for (int j = 0; j < c_x; j++)
        {
            temp_vec(0, 0) = gamma_x(i, j);
            for (int k = 1; k < Config::config->M; k++)
            {
                temp_vec(k, 0) = Constant::Util::random_u64();
            }
            for (int k = 0; k < Config::config->M + 1; k++)
            {
                MatrixXu temp = A_plus_mat.row(k) * temp_vec;
                out_Matrix[k](i, j + Config::config->numClass) = temp(0, 0);
            }
            out_delta(i, j + Config::config->numClass) = delta_x(i, j);
        }
    }
    for (int i = 0; i < r_x; ++i)
    {
        for (int j = 0; j < c_x + Config::config->numClass; ++j)
        {
            for (int k = 0; k < Config::config->M + 1; ++k)
            {
                if (j == (c_x + Config::config->numClass - 1))
                {
                    out_files[k] << out_Matrix[k](i, j) << "\n";
                }
                else
                {
                    out_files[k] << out_Matrix[k](i, j) << ",";
                }
            }
            if (j == (c_x + Config::config->numClass - 1))
            {
                out_delta_file << out_delta(i, j) << "\n";
            }
            else
            {
                out_delta_file << out_delta(i, j) << ",";
            }
        }
    }
    for (int k = 0; k < Config::config->M + 1; ++k)
    {
        out_files[k].close();
    }
    out_delta_file.close();
}
void IOManager::load_ss(ifstream &in, MatrixXu &data, MatrixXu &label, ifstream &in_delta, MatrixXu &data_delta, MatrixXu &label_delta, int size)
{
    int i = 0;
    while (in && in_delta)
    {
        string s, s_delta;
        if (!getline(in, s))
            break;
        if (!getline(in_delta, s_delta))
            break;
        char *ch, *ch_delta;
        ch = const_cast<char *>(s.c_str());
        ch_delta = const_cast<char *>(s_delta.c_str());
        u64 temp, temp_delta;
        for (int j = 0; j < Config::config->numClass; j++)
        {
            temp = Constant::Util::getu64(ch);
            temp_delta = Constant::Util::getu64(ch_delta);
            label(i, j) = temp;
            label_delta(i, j) = temp_delta;
        }

        for (int j = 0; j < Config::config->D; j++)
        {
            temp = Constant::Util::getu64(ch);
            temp_delta = Constant::Util::getu64(ch_delta);
            data(i, j) = temp;
            data_delta(i, j) = temp_delta;
        }
        i++;
        if (i >= size)
            break;
    }
    DBGprint("n=%d\n", i);
}

void IOManager::init()
{
    train_data = MatrixXu(Config::config->N, Config::config->D);
    train_label = MatrixXu(Config::config->N, Config::config->numClass);
    train_data_delta = MatrixXu(Config::config->N, Config::config->D);
    train_label_delta = MatrixXu(Config::config->N, Config::config->numClass);
    test_data = MatrixXu(Config::config->testN, Config::config->D);
    test_label = MatrixXu(Config::config->testN, Config::config->numClass);
    test_data_delta = MatrixXu(Config::config->testN, Config::config->D);
    test_label_delta = MatrixXu(Config::config->testN, Config::config->numClass);

    train_data.setZero();
    train_label.setZero();
    train_data_delta.setZero();
    train_label_delta.setZero();
    test_data.setZero();
    test_label.setZero();
    test_data_delta.setZero();
    test_label_delta.setZero();
    DBGprint("load training data......\n");

    // ifstream infile("data/mnist/mnist_train.csv");
    // // ifstream infile("data/fashion_mnist/fashion_mnist_train.csv");
    // // ifstream infile("data/svhn/svhn_train.csv");
    // load_data(infile, train_data, train_label, Config::config->N);
    // secret_share(train_data, train_label, "train");
    // infile.close();

    ifstream infile("data/mnist/mnist_train_" + to_string(party) + ".csv");
    ifstream infile_delta("data/mnist/mnist_train_delta.csv");
    // ifstream infile("data/fashion_mnist/fashion_mnist_train_" + to_string(party) + ".csv");
    // ifstream infile_delta("data/fashion_mnist/fashion_mnist_train_delta.csv");
    // ifstream infile("data/svhn/svhn_train_" + to_string(party) + ".csv");
    // ifstream infile_delta("data/svhn/svhn_train_delta.csv");
    load_ss(infile, train_data, train_label, infile_delta, train_data_delta, train_label_delta, Config::config->N);
    infile.close();
    infile_delta.close();

    ifstream intest("data/mnist/mnist_test.csv");
    // ifstream intest("data/fashion_mnist/fashion_mnist_test.csv");
    // ifstream intest("data/svhn/svhn_test.csv");
    load_data(intest, test_data, test_label, Config::config->testN);
    // secret_share(test_data, test_label, "test");
    intest.close();

    // ifstream intest("data/mnist/mnist_test_" + to_string(party) + ".csv");
    // ifstream intest("data/fashion_mnist/fashion_mnist_test_" + to_string(party) + ".csv");
    // load_ss(intest, test_data, test_label, testN);
    // intest.close();
}