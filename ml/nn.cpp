#include "nn.h"

int NN::myrandom(int i)
{
    return rand() % i;
}

vector<int> NN::random_perm()
{
    vector<int> temp, perm;
    for (int i = 0; i < Config::config->N - Config::config->B; i++)
        temp.push_back(i);

    for (int i = 0; i < Config::config->Ep + 1; i++)
    {                                                       // construct the random shuffled indices of every epoch
        random_shuffle(temp.begin(), temp.end(), myrandom); // random permutations of indices between 0 to N-1
        perm.insert(perm.end(), temp.begin(), temp.end());  // append the random permutated indicies
    }
    return perm;
}

void NN::next_batch(MatrixXu &batch, MatrixXu &delta_batch, int start, vector<int> &perm, MatrixXu &data, MatrixXu &delta_data)
{
    int col = data.cols();
    int begin = perm[start];
    batch = data.block(begin, 0, Config::config->B, col);
    delta_batch = delta_data.block(begin, 0, Config::config->B, col);
}

MatrixXu relu(MatrixXu x)
{
    for (int i = 0; i < x.size(); i++)
    {
        if (x(i) >= 0 && x(i) <= 9223372036854775808)
            x(i) = x(i);
        else
            x(i) = 0;
    }
    return x;
}

MatrixXu NN::argmax(MatrixXu &x)
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

        res(i, 0) = index % 10;
    }
    return res;
}

void NN::forward(MatrixXu &x, MatrixXu &delta_x)
{
    int l = this->layers.size();
    for (int i = 0; i < l; i++)
    {
        // Constant::Clock *clock_train;
        // clock_train = new Constant::Clock(3);
        this->layers[i]->forward(x, delta_x);
        // this->time[i] += clock_train->get();
    }
}

void NN::backward(MatrixXu &delta, MatrixXu &delta_delta)
{
    int l = this->layers.size();
    for (int i = l - 1; i >= 0; i--)
    {
        this->layers[i]->backward(delta, delta_delta);
    }
}

void NN::fit(MatrixXu &train_data, MatrixXu &delta_train_data, MatrixXu &test_data, MatrixXu &delta_test_data, int epoch)
{
    vector<int> perm = random_perm();
    int start = 0;
    int iterations = 0;
    MatrixXu x_batch(Config::config->B, Config::config->D), y_batch(Config::config->B, Config::config->numClass);
    MatrixXu delta_x_batch(Config::config->B, Config::config->D), delta_y_batch(Config::config->B, Config::config->numClass);
    Constant::Clock *clock_train;
    clock_train = new Constant::Clock(2);
    for (int i = 0; i < epoch; i++)
    {
        double error = 0;
        for (int j = 0; j < Config::config->N / Config::config->B; j++)
        {
            if(iterations % 100 == 0)
            {
                cout << "第" << iterations << "迭代："<<endl;
                this->test_model();
            }
            iterations++;
            next_batch(x_batch, delta_x_batch, start, perm, train_data, delta_train_data);
            next_batch(y_batch, delta_y_batch, start, perm, test_data, delta_test_data);
            
            start += Config::config->B;
            MatrixXu output = x_batch;
            MatrixXu delta_output = delta_x_batch;
            this->forward(output, delta_output);
            MatrixXu delta = output - y_batch;
            MatrixXu delta_delta = delta_output - delta_y_batch;
            // MatrixXd temp = Mat::u642Double(Secret_Mul::reveal(delta_delta, delta));
            // error = error + (temp.array() * temp.array()).sum();
            this->backward(delta, delta_delta);
        }
        // cout << "square error" << endl;
        // cout << error / Config::config->N << endl;
        // this->test_model();
    }
    // cout << "online time:" << clock_train->get() << endl;
    // cout << "IT/s:" << epoch / clock_train->get() << endl;
    // cout << "forward time:" << t1 << endl;
    // cout << "backward time" << t2 << endl;
    // for (int i = 0; i < this->layers.size(); i++)
    // {
    //     cout << "Forward:" << i << " :" << this->time[i] << endl;
    // }
    // this->layers[1]->print();
    // this->test_model();
    // cout << "IT/s:" << 10/clock_train->get() << endl;
}

void NN::add_layer(Layer *layer)
{
    this->layers.push_back(layer);
}

LinearLayer::LinearLayer(int input_size, int output_size, bool first_layer)
{
    this->input_size = input_size;
    this->output_size = output_size;
    this->first_layer = first_layer;
    this->weight = MatrixXu(input_size, output_size);
    this->delta_weight = MatrixXu(input_size, output_size);
    MatrixXu w = MatrixXu(input_size, output_size);
    static default_random_engine e(time(0));
    static normal_distribution<double> n(0, 0.05);
    MatrixXd m = MatrixXd::Zero(input_size, output_size).unaryExpr([](double dummy)
                                                                   { return n(e); });
    for (int i = 0; i < m.size(); i++)
        w(i) = Constant::Util::double_to_u64(m(i));
    MatrixXu gamma = Mat::randomMatrixXu(input_size, output_size);
    this->delta_weight = IOManager::secret_share(w, gamma);
    this->weight = w;
    // this->weight.setZero();
}

void LinearLayer::forward(MatrixXu &x, MatrixXu &delta_x)
{
    int row = x.rows(), col = this->weight.cols();
    this->input = x;
    this->delta_input = delta_x;

    // MatrixXu a = Secret_Mul::Mul_reveal(x);
    // MatrixXu b = Secret_Mul::Mul_reveal(this->weight);
    // MatrixXu c = a * b;
    // MatrixXu ab = IOManager::secret_share(c);
    MatrixXu ab = MatrixXu::Zero(row, col);
    MatrixXu output(row, col);

    delta_x = Secret_Mul::Mul(x, delta_x, this->weight, this->delta_weight, output, ab);
    x = output;
}

void LinearLayer::backward(MatrixXu &delta, MatrixXu &delta_delta)
{
    MatrixXu input_trans = this->input.transpose();
    MatrixXu delta_input_trans = this->delta_input.transpose();

    int row = input_trans.rows(), col = delta.cols();
    // MatrixXu a = Secret_Mul::Mul_reveal(input_trans);
    // MatrixXu b = Secret_Mul::Mul_reveal(delta);
    // MatrixXu c = a * b;
    // MatrixXu ab = IOManager::secret_share(c);
    MatrixXu ab = MatrixXu::Zero(row, col);
    MatrixXu w_gradients(row, col);

    MatrixXu delta_w_gradients = Secret_Mul::Mul(input_trans, delta_input_trans, delta, delta_delta, w_gradients, ab);
    this->delta_weight = this->delta_weight - Secret_Mul::Constant_Mul(delta_w_gradients, w_gradients, Config::config->R / Config::config->B);
    this->weight = this->weight - w_gradients;

    MatrixXu weight_trans = this->weight.transpose();
    MatrixXu delta_weight_trans = this->delta_weight.transpose();
    if (!first_layer)
    {
        int row = delta.rows(), col = weight_trans.cols();
        // MatrixXu u = Secret_Mul::Mul_reveal(delta);
        // MatrixXu v = Secret_Mul::Mul_reveal(weight_trans);
        // MatrixXu h = u * v;
        // MatrixXu uv = IOManager::secret_share(h);
        MatrixXu uv = MatrixXu::Zero(row, col);
        MatrixXu next_delta(row, col);

        delta_delta = Secret_Mul::Mul(delta, delta_delta, weight_trans, delta_weight_trans, next_delta, uv);
        delta = next_delta;
    }
}

void ReluLayer::forward(MatrixXu &x, MatrixXu &delta_x)
{
    int row = x.rows(), col = x.cols();
    this->sign = MatrixXu(row, col);
    // Constant::Clock *clock_train;
    // clock_train = new Constant::Clock(3);
    MatrixXu bool_share_bit = Secret_Cmp::get_bool_share_bit(x, delta_x);
    // this->time0 += clock_train->get();
    // Constant::Clock *clock_train1;
    // clock_train1 = new Constant::Clock(3);
    this->delta_sign = Secret_Cmp::get_sign_xor_1(bool_share_bit, this->sign);
    // this->time1 += clock_train1->get();

    // MatrixXu a = Secret_Mul::Mul_reveal(x);
    // MatrixXu b = Secret_Mul::Mul_reveal(this->sign);
    // MatrixXu c = a.cwiseProduct(b);
    // MatrixXu ab = IOManager::secret_share(c);
    MatrixXu ab = MatrixXu::Zero(row, col);
    MatrixXu result(row, col);

    delta_x = Secret_Mul::Element_Wise(x, delta_x, this->sign, this->delta_sign, result, ab);
    x = result;
}

void ReluLayer::backward(MatrixXu &delta, MatrixXu &delta_delta)
{
    int row = delta.rows(), col = delta.cols();

    // MatrixXu a = Secret_Mul::Mul_reveal(delta);
    // MatrixXu b = Secret_Mul::Mul_reveal(this->sign);
    // MatrixXu c = a.cwiseProduct(b);
    // MatrixXu ab = IOManager::secret_share(c);
    MatrixXu ab = MatrixXu::Zero(row, col);
    MatrixXu result(row, col);

    delta_delta = Secret_Mul::Element_Wise(delta, delta_delta, this->sign, this->delta_sign, result, ab);
    delta = result;
}

void NN::train_model()
{
    MatrixXu train_data = IOManager::train_data;
    MatrixXu train_label = IOManager::train_label;
    MatrixXu delta_train_data = IOManager::train_data_delta;
    MatrixXu delta_train_label = IOManager::train_label_delta;

    MatrixXu test_data = IOManager::test_data;
    MatrixXu test_label = IOManager::test_label;

    NN net;
    Layer *linearLayer0 = new LinearLayer(Config::config->D, Config::config->hiddenDim, true);
    net.add_layer(linearLayer0);
    Layer *reluLayer0 = new ReluLayer();
    net.add_layer(reluLayer0);
    Layer *linearLayer1 = new LinearLayer(Config::config->hiddenDim, Config::config->hiddenDim);
    net.add_layer(linearLayer1);
    Layer *reluLayer1 = new ReluLayer();
    net.add_layer(reluLayer1);
    Layer *linearLayer2 = new LinearLayer(Config::config->hiddenDim, Config::config->numClass);
    net.add_layer(linearLayer2);
    int size = net.layers.size();
    net.time.resize(size);
    net.fit(train_data, delta_train_data, train_label, delta_train_label, Config::config->Ep);
    // net.inference(test_data, test_label);
    // cout << "time_0:" << time_0 << endl;
    // cout << "time_1:" << time_1 << endl;
    // cout << "cnt:" << cnt << endl;
    // cout << "cnt1:" << cnt1 << endl;
    // cout << "size_:" << size_ << endl;
}

void NN::test_model()
{
    double count = 0;
    MatrixXu test_data = IOManager::test_data;
    MatrixXu test_label = IOManager::test_label;

    vector<MatrixXu> W_(static_cast<unsigned long>(Config::config->nLayer + 1));
    vector<MatrixXu> A(static_cast<unsigned long>(Config::config->nLayer + 1));
    vector<MatrixXu> Z(static_cast<unsigned long>(Config::config->nLayer + 1));
    A[0] = test_data;
    MatrixXu result;

    // if (party == 0)
    // {
        W_[1] = Secret_Mul::reveal(this->layers[0]->delta_weight, this->layers[0]->weight);
        W_[2] = Secret_Mul::reveal(this->layers[2]->delta_weight, this->layers[2]->weight);
        W_[3] = Secret_Mul::reveal(this->layers[4]->delta_weight, this->layers[4]->weight);

        for (int l = 1; l <= Config::config->nLayer; l++)
        {
            Z[l] = A[l - 1] * W_[l];
            Mat::truncateMatrixXu(Z[l]);
            if (l == Config::config->nLayer)
                A[l] = Z[l];
            else
                A[l] = relu(Z[l]);
        }
        result = A[Config::config->nLayer];
        MatrixXu res = argmax(result);
        MatrixXu label = argmax(test_label);
        for (int i = 0; i < Config::config->testN; i++)
        {
            // double resulttt = Constant::Util::u64_to_double(result(i, 0));
            // if (resulttt > 0.5 && test_label(i, 0) == 1048576)
            //     count++;
            // else if (resulttt < 0.5 && test_label(i, 0) == 0)
            //     count++;
            if (res(i, 0) == label(i, 0))
            {
                count++;
            }
        }
        cout << "accuracy:" << count / Config::config->testN << endl;
    // }
    // else
    // {
    //     W_[1] = Secret_Mul::reveal(this->layers[0]->delta_weight, this->layers[0]->weight);
    //     W_[2] = Secret_Mul::reveal(this->layers[2]->delta_weight, this->layers[2]->weight);
    //     W_[3] = Secret_Mul::reveal(this->layers[4]->delta_weight, this->layers[4]->weight);
    // }
}

// void NN::inference(MatrixXu &x, MatrixXu &y)
// {
//     int l = y.rows();
//     int batch_len = ceil(l / Config::config->B);
//     MatrixXu x_batch, y_batch, y_batch_plaintext;
//     int count = 0;

//     for (int j = 0; j < batch_len; j++)
//     {
//         int temp = min(Config::config->B, l - j * Config::config->B);
//         x_batch = x.middleRows(j * Config::config->B, temp);
//         y_batch = y.middleRows(j * Config::config->B, temp);
//         MatrixXu predict = this->forward(x_batch);
//         predict = Secret_Mul::Mul_reveal(predict);
//         y_batch_plaintext = Secret_Mul::Mul_reveal(y_batch);
//         MatrixXu res = argmax(predict);
//         MatrixXu label = argmax(y_batch_plaintext);
//         for (int i = 0; i < temp; i++)
//         {
//             // if (predict(i) > IE / 2)
//             //     predict(i) = IE;
//             // else
//             //     predict(i) = 0;
//             // count = count + (predict(i) == y_batch_plaintext(i));
//             if (res(i, 0) == label(i, 0))
//             {
//                 count++;
//             }
//         }
//     }
//     cout << "accuracy of inference:" << count * 1.0 / l << endl;
// }