#include "logistic.h"

MatrixXu Logistic::w;
MatrixXu Logistic::delta_w;

int Logistic::myrandom(int i) { return rand() % i; }

vector<int> Logistic::random_perm()
{
    vector<int> temp, perm;
    for (int i = 0; i < Config::config->N - Config::config->B; i++)
        temp.push_back(i);

    for (int i = 0; i < Config::config->Ep; i++)
    {                                                       // construct the random shuffled indices of every epoch
        random_shuffle(temp.begin(), temp.end(), myrandom); // random permutations of indices between 0 to N-1
        perm.insert(perm.end(), temp.begin(), temp.end());  // append the random permutated indicies
        // cout << "perm:" << perm.size() <<endl;
    }
    return perm;
}

void Logistic::next_batch(MatrixXu &batch, MatrixXu &delta_batch, int start, vector<int> &perm, MatrixXu &data, MatrixXu &delta_data)
{
    int col = data.cols();
    batch = data.block(perm[start], 0, Config::config->B, col);
    delta_batch = delta_data.block(perm[start], 0, Config::config->B, col);
}

void Logistic::sigmoid(MatrixXu &x)
{
    for (int i = 0; i < x.size(); i++)
    {
        if (x(i) >= 9223372036854775808 && x(i) <= 18446744073709027328)
            x(i) = 0;
        else if (x(i) >= 524288 && x(i) < 9223372036854775808)
            x(i) = 1048576;
        else
            x(i) = x(i) + 524288;
    }
}

MatrixXu Logistic::argmax(MatrixXu &x)
{
    int row = x.rows(), col = x.cols();
    MatrixXu res(row, 1);
    Matrixint64 temp = x.cast<int64>();
    for (int i = 0; i < row; i++)
    {
        int index = 0;
        int64 max = temp(i, 0);
        for (int j = 1; j < col; j++)
        {
            if (temp(i, j) > max)
            {
                max = temp(i, j);
                index = j;
            }
        }

        // SVHN
        //  res(i, 0) = index % 10;

        // mnist and fashion_mnist
        res(i, 0) = index;
    }
    return res;
}

// void Logistic::train_model()
// {

//     MatrixXu x_batch(Config::config->B, Config::config->D), y_batch(Config::config->B, Config::config->numClass);
//     MatrixXu train_data = IOManager::train_data;
//     MatrixXu train_label = IOManager::train_label;

//     // static default_random_engine e(time(0));
//     // static normal_distribution<double> n(0, 0.05);
//     // MatrixXd m = MatrixXd::Zero(Config::config->D, 1).unaryExpr([](double dummy) { return n(e); });
//     // for (int i = 0; i < Config::config->D; i++)
//     // {
//     //     w(i, 0) = Constant::Util::double_to_u64(m(i, 0));
//     // }
//     w = MatrixXu(Config::config->D, Config::config->numClass);
//     w.setZero();

//     cout << "weights initialized" << endl;
//     vector<int> perm = random_perm();

//     int start = 0;
//     MatrixXu r0(Config::config->B, Config::config->D), q0(Config::config->D, Config::config->numClass), t0(Config::config->B, Config::config->numClass), r1(Config::config->D, Config::config->B), q1(Config::config->B, Config::config->numClass), t1(Config::config->D, Config::config->numClass);
//     MatrixXu wx(Config::config->B, Config::config->numClass), wx_y(Config::config->B, Config::config->numClass);
//     Constant::Clock *clock_train;
//     clock_train = new Constant::Clock(2);
//     for (int j = 0; j < Config::config->Ep; j++)
//     {
//         cout << "epoch:" << j << endl;
//         double error = 0;
//         for (int i = 0; i < Config::config->N / Config::config->B; i++)
//         {
//             // cout << "第" << i << "个batch" << endl;

//             next_batch(x_batch, start, perm, train_data);
//             next_batch(y_batch, start, perm, train_label); //选出mini batch

//             start += Config::config->B;

//             r0 = Secret_Mul::r0;
//             r1 = Secret_Mul::r1;
//             q0 = Secret_Mul::q0;
//             q1 = Secret_Mul::q1;
//             t0 = Secret_Mul::t0;
//             t1 = Secret_Mul::t1;

//             wx = Secret_Mul::Multiply(x_batch, w, r0, q0, t0);
//             wx = Secret_Cmp::Sigmoid(wx); // 加入一层sigmoid函数即可
//             wx_y = wx - y_batch;

//             MatrixXd temp = Mat::u642Double(Secret_Mul::Mul_reveal(wx_y));
//             error = error + (temp.array() * temp.array()).sum();

//             MatrixXu x_batch_trans = x_batch.transpose();
//             MatrixXu delta;
//             delta = Secret_Mul::Multiply(x_batch_trans, wx_y, r1, q1, t1);

//             // w = w - Mat::constant_multiply(delta, 0.01 / Config::config->B);
//             w = w - Secret_Mul::constant_Mul(delta, Config::config->R / Config::config->B);
//         }
//         cout << "square error" << endl;
//         cout << error / Config::config->N << endl;
//         // test_model();
//         // inference();
//     }
//     cout << "online time:" << clock_train->get() << endl;
//     test_model();
//     // cout << "it/s:" << 10 / clock_train->get() << endl;
//     // cout << Secret_Mul::reveal(w) << endl;
// }

void Logistic::train_model()
{
    MatrixXu x_batch(Config::config->B, Config::config->D), y_batch(Config::config->B, Config::config->numClass);
    MatrixXu delta_x_batch(Config::config->B, Config::config->D), delta_y_batch(Config::config->B, Config::config->numClass);
    MatrixXu train_data = IOManager::train_data;
    MatrixXu train_label = IOManager::train_label;
    MatrixXu delta_train_data = IOManager::train_data_delta;
    MatrixXu delta_train_label = IOManager::train_label_delta;
    w = MatrixXu(Config::config->D, Config::config->numClass);
    delta_w = MatrixXu(Config::config->D, Config::config->numClass);
    w.setZero();
    delta_w.setZero();
    cout << "weights initialized" << endl;
    vector<int> perm = random_perm();

    int start = 0;
    int iterations = 0;

    MatrixXu wx(Config::config->B, Config::config->numClass), wx_y(Config::config->B, Config::config->numClass);
    MatrixXu delta_wx(Config::config->B, Config::config->numClass), delta_wx_y(Config::config->B, Config::config->numClass);

    Constant::Clock *clock_train;
    clock_train = new Constant::Clock(2);

    for (int j = 0; j < Config::config->Ep; j++)
    {
        // double error = 0;
        // cout << "第" << j << "个epoch" << endl;
        for (int i = 0; i < Config::config->N / Config::config->B; i++)
        {
            if(iterations % 100 == 0)
            {
                cout << "第" << iterations << "迭代："<<endl;
                test_model();
            }
            iterations++;
            // next_batch(x_batch, start, perm, train_data);
            // next_batch(delta_x_batch, start, perm, delta_train_data);
            // next_batch(y_batch, start, perm, train_label);
            // next_batch(delta_y_batch, start, perm, delta_train_label); // 选出mini batch
            next_batch(x_batch, delta_x_batch, start, perm, train_data, delta_train_data);
            next_batch(y_batch, delta_y_batch, start, perm, train_label, delta_train_label);
            start += Config::config->B;

            // Constant::Clock *clock_train1;
            // clock_train1 = new Constant::Clock(2);
            // MatrixXu a = Secret_Mul::Mul_reveal(x_batch);
            // MatrixXu b = Secret_Mul::Mul_reveal(w);
            // MatrixXu c = a * b;
            // MatrixXu ab = IOManager::secret_share(c);
            // double time1 = clock_train1->get();
            // t1 += time1;
            MatrixXu ab = MatrixXu::Zero(Config::config->B, Config::config->numClass);

            delta_wx = Secret_Mul::Mul(x_batch, delta_x_batch, w, delta_w, wx, ab);
            delta_wx = Secret_Cmp::Sigmoid(wx, delta_wx);
            wx_y = wx - y_batch;
            delta_wx_y = delta_wx - delta_y_batch;

            // MatrixXd temp = Mat::u642Double(Secret_Mul::reveal(delta_wx_y, wx_y));
            // error = error + (temp.array() * temp.array()).sum();

            MatrixXu x_batch_trans = x_batch.transpose();
            MatrixXu delta_x_batch_trans = delta_x_batch.transpose();

            MatrixXu delta(Config::config->D, Config::config->numClass), delta_delta(Config::config->D, Config::config->numClass);
            // Constant::Clock *clock_train2;
            // clock_train2 = new Constant::Clock(2);
            // MatrixXu u = a.transpose();
            // MatrixXu v = Secret_Mul::Mul_reveal(wx_y);
            // MatrixXu h = u * v;
            // MatrixXu uv = IOManager::secret_share(h);
            // double time2 = clock_train2->get();
            // t2 += time2;

            MatrixXu uv = MatrixXu::Zero(Config::config->D, Config::config->numClass);

            delta_delta = Secret_Mul::Mul(x_batch_trans, delta_x_batch_trans, wx_y, delta_wx_y, delta, uv);
            delta_w = delta_w - Secret_Mul::Constant_Mul(delta_delta, delta, 0.01 / Config::config->B);
            w = w - delta;
        }
        // cout << "square error" << endl;
        // cout << error / Config::config->N << endl;
        // test_model();
    }
    // cout << "online time:" << clock_train->get() << endl;
    // cout << "it/s:" << Config::config->Ep / clock_train->get() << endl;
    // inference();
    // test_model();
}

void Logistic::test_model()
{
    double count = 0;
    MatrixXu w_(Config::config->D, Config::config->numClass);
    MatrixXu test_data = IOManager::test_data;
    MatrixXu test_label = IOManager::test_label;

    // if (party == 0)
    // {
        w_ = Secret_Mul::reveal(delta_w, w);
        MatrixXu y_ = test_data * w_;

        Mat::truncateMatrixXu(y_);
        Logistic::sigmoid(y_);
        // MatrixXu res = argmax(y_);
        // MatrixXu label = argmax(test_label);
        for (int i = 0; i < Config::config->testN; i++)
        {
            double yyy = Constant::Util::u64_to_double(y_(i, 0));
            if (yyy > 0.5 && test_label(i, 0) == 1048576)
            {
                count++;
            }
            else if (yyy < 0.5 && test_label(i, 0) == 0)
            {
                count++;
            }
            // if (res(i, 0) == label(i, 0))
            // {
            //     count++;
            // }
        }
        cout << "accuracy:" << count / Config::config->testN << endl;
    // }
    // else
    // {
    //     Secret_Mul::reveal(delta_w, w);
    // }
}

void Logistic::inference()
{
    MatrixXu test_data = IOManager::test_data;
    MatrixXu test_label = IOManager::test_label;
    MatrixXu wx;
    double count = 0;
    int it = ceil(Config::config->testN / Config::config->B);
    MatrixXu x_batch, y_batch, y_infer;

    MatrixXu r0(Config::config->B, Config::config->D), q0(Config::config->D, Config::config->numClass), t0(Config::config->B, Config::config->numClass);
    r0 = Secret_Mul::r0;
    q0 = Secret_Mul::q0;
    t0 = Secret_Mul::t0;
    for (int i = 0; i < it; i++)
    {
        int temp = min(Config::config->B, Config::config->testN - i * Config::config->B);
        x_batch = test_data.middleRows(i * Config::config->B, temp);
        y_batch = test_label.middleRows(i * Config::config->B, temp);
        wx = Secret_Mul::Multiply(x_batch, w, r0, q0, t0);
        y_infer = Secret_Cmp::Sigmoid(wx);
        MatrixXu y_predict = Secret_Mul::Mul_reveal(y_infer);
        MatrixXu y_plaintext = Secret_Mul::Mul_reveal(y_batch);
        MatrixXu res = argmax(y_predict);
        MatrixXu label = argmax(y_plaintext);
        for (int j = 0; j < temp; j++)
        {
            double yyy = Constant::Util::u64_to_double(y_predict(j));
            if (yyy > 0.5)
                y_predict(j) = Config::config->IE;
            else
                y_predict(j) = 0;
            count = count + (y_predict(j) == y_plaintext(j));
            // if (res(i, 0) == label(i, 0))
            // {
            //     count++;
            // }
        }
    }
    cout << "accuracy of inference:" << count * 1.0 / Config::config->testN << endl;
}
