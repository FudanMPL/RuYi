#include "Mat.h"

TensorRu Mat::A = TensorRu(3, 3);
TensorRu Mat::A_plus = TensorRu(4, 3);
MatrixXu Mat::A_inv;
vector<u64> Mat::inv_for_mul;
vector<u64> Mat::inv_for_div;
vector<bool> Mat::div_flag;

TensorRu Mat::getInverse(const MatrixXu &origin)
{
    TensorRu inv(3, 3);
    // int row = inv.rows(), col = inv.cols();
    int row = inv.dimension(0), col = inv.dimension(1);
    MatrixXd temp(row, col);
    // temp << origin.cast<double>();
    //  for (int i = 0; i < origin.rows(); i++)
    //  {
    //      for (int j = 0; j < origin.cols(); j++)
    //      {
    //          temp(i, j) = Constant::Util::u64_to_double(origin(i, j));
    //      }
    //  }
    temp = temp.inverse();
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            inv(i, j) = Constant::Util::double_to_u64(temp(i, j));
        }
    }
    return inv;
}

MatrixXd Mat::get_binary_matrix()
{
    MatrixXd binary_matrix(Config::config->nd, Config::config->na);
    binary_matrix.setZero();
    for (int i = 0; i < Config::config->nd; i++)
    {
        int k = i;
        for (int j = Config::config->na - 1; k > 0; j--)
        {
            binary_matrix(i, j) = k % 2;
            k = k / 2;
        }
    }
    return binary_matrix;
}

void Mat::init_public_vector()
{
A_inv = MatrixXu(Config::config->M, Config::config->M);
    inv_for_mul.resize(Config::config->M);
    inv_for_div.resize(Config::config->M);
    div_flag.resize(Config::config->M);
    MatrixXu A_plus_temp(Config::config->M + Config::config->nd, Config::config->M);
    // A_plus_temp << 1, 1, UINT64_MAX,
    //     1, 2, UINT64_MAX,
    //     1, 1, 0,
    //     2, 3, UINT64_MAX;
    // A_plus.setValues({{1, 0, 1}, {2, 2, UINT64_MAX - 2}, {3, 3, UINT64_MAX - 3}, {1, 1, UINT64_MAX}});
    // srand(time(0));
    MatrixXd identity_matrix;
    MatrixXd public_matrix(Config::config->M + Config::config->nd, Config::config->M);
    identity_matrix.setIdentity(Config::config->M, Config::config->M);
    public_matrix.setZero();
    MatrixXd combine_matrix_for_np1(1, Config::config->M), combine_matrix_for_np2(1, Config::config->M);
    // MatrixXd combine_matrix_for_na(1, Config::config->M), combine_matrix_for_nr(1, Config::config->M);
    combine_matrix_for_np1.setZero();
    combine_matrix_for_np2.setZero();
    // combine_matrix_for_na.setZero();
    // combine_matrix_for_nr.setZero();
    // if (Config::config->na % 2 == 0)
    // {
    //     for (int i = Config::config->np; i < Config::config->M; i++)
    //     {
    //         combine_matrix_for_np1.row(0) = combine_matrix_for_np1.row(0) + pow(-1, i + 1) * identity_matrix.row(i);
    //     }
    // }
    // else
    // {
    //     for (int i = Config::config->np; i < Config::config->M - 3; i++)
    //     {
    //         combine_matrix_for_np1.row(0) = combine_matrix_for_np1.row(0) + pow(-1, i + 1) * identity_matrix.row(i);
    //     }
    //     combine_matrix_for_np1.row(0) = combine_matrix_for_np1.row(0) + pow(-1, Config::config->M - 2) * 2 * identity_matrix.row(Config::config->M - 3) + pow(-1, Config::config->M - 1) * (identity_matrix.row(Config::config->M - 2) + identity_matrix.row(Config::config->M - 1));
    // }

    // for (int i = Config::config->np; i < Config::config->M; i++)
    // {
    //     combine_matrix_for_np2.row(0) = combine_matrix_for_np2.row(0) + identity_matrix.row(i);
    // }

    // for (int j = 0; j < Config::config->np; j++)
    // {
    //     if (j == 0)
    //         identity_matrix.row(j) = identity_matrix.row(j) + combine_matrix_for_np1.row(0);
    //     else
    //         identity_matrix.row(j) = identity_matrix.row(j) + combine_matrix_for_np2.row(0);

    //     // combine_matrix_for_na.row(0) = combine_matrix_for_na.row(0) + identity_matrix.row(j);
    // }

    // // combine_matrix_for_na.row(0) = combine_matrix_for_na.row(0) + identity_matrix.row(0);

    // for (int i = Config::config->np; i < Config::config->M; i++)
    // {
    //     for (int j = 0; j < Config::config->np; j++)
    //     {
    //         if (j == 0)
    //             identity_matrix.row(i) = identity_matrix.row(i) + identity_matrix.row(0);
    //         else
    //             identity_matrix.row(i) = identity_matrix.row(i) + (i - Config::config->np + 1) * identity_matrix.row(j);
    //     }
    // }

    // public_matrix.topRows(Config::config->M) = identity_matrix.topRows(Config::config->M);

    for (int i = Config::config->np; i < Config::config->M; i++)
    {
        if (i != (Config::config->M - 1))
            combine_matrix_for_np1.row(0) = combine_matrix_for_np1.row(0) + (i - Config::config->np + 1) * identity_matrix.row(i);
        else
            combine_matrix_for_np1.row(0) = combine_matrix_for_np1.row(0) + -(Config::config->na - 1) * Config::config->na / 2 * identity_matrix.row(i);
    }

    for (int i = Config::config->np; i < Config::config->M; i++)
    {
        combine_matrix_for_np2.row(0) += identity_matrix.row(i);
    }

    for (int j = 0; j < Config::config->np; j++)
    {
        if (j == 0)
            identity_matrix.row(j) += combine_matrix_for_np1.row(0);
        else
            identity_matrix.row(j) += combine_matrix_for_np2.row(0);
    }

    for (int i = Config::config->np; i < Config::config->M; i++)
    {
        identity_matrix.row(i) += identity_matrix.row(0);
        for (int j = 1; j < Config::config->np; j++)
        {
            // to ensure the reconstruction coefficents held by priviliged parties are not zeros
            if (i == Config::config->np)
                identity_matrix.row(i) += -identity_matrix.row(j);
            else
                identity_matrix.row(i) += identity_matrix.row(j);
            // identity_matrix.row(i) += identity_matrix.row(j);
        }
        public_matrix.row(Config::config->M) += identity_matrix.row(i);
    }

    public_matrix.topRows(Config::config->M) = identity_matrix.topRows(Config::config->M);

    cout << "public_matrix:" << public_matrix << endl;
    A_plus_temp = public_matrix.cast<u64>();
    A_plus = Mat::toTensor(A_plus_temp);
    MatrixXd temp = public_matrix.topRows(Config::config->M);
    MatrixXd temp_inv = temp.inverse();
    Matrixint64 int_temp(Config::config->M, Config::config->M);
    MatrixXu A_inv1(Config::config->M, Config::config->M);
    A_inv = temp_inv.array().round().cast<u64>();
    int_temp = temp_inv.array().round().cast<int64>();
    // cout << "A_inv1" << A_inv1 << endl;
    for (int i = 0; i < Config::config->M; i++)
    {
        if (temp_inv(0, i) < 0)
        {
            inv_for_div[i] = UINT64_MAX / (-1 * int_temp(0, i));
            div_flag[i] = false;
        }
        else
        {
            inv_for_div[i] = A_inv(0, i);
            div_flag[i] = true;
        }
    }
    for (int i = 0; i < Config::config->M; i++)
    {
        inv_for_mul[i] = A_inv(0, i);
    }
}

MatrixXd Mat::u642Double(const MatrixXu &a)
{
    MatrixXu temp(a.rows(), a.cols());
    MatrixXd res(a.rows(), a.cols());
    int l = a.rows() * a.cols();
    for (int i = 0; i < l; i++)
    {
        temp(i) = (u64)a(i);
    }
    for (int i = 0; i < l; i++)
    {
        res(i) = (long)temp(i) / (double)Config::config->IE;
    }
    return res;
}

TensorRu Mat::randomTensorRu(int rows, int cols)
{
    TensorRu res(rows, cols);
    res.setZero();
    u64 *data = res.data();
    for (int i = 0; i < res.size(); i++)
    {
        *(data + i) = Constant::Util::random_u64();
    }
    return res;
}

TensorRu3 Mat::randomTensorRu3(int rows, int cols)
{
    TensorRu3 res(3, rows, cols);
    // res.setZero();
    u64 *data = res.data();
    for (int i = 0; i < res.size(); i++)
    {
        *(data + i) = Constant::Util::random_u64();
    }
    return res;
}

MatrixXu Mat::randomMatrixXu(int rows, int cols)
{
    MatrixXu res(rows, cols);
    res.setZero();
    u64 *data = res.data();
    for (int i = 0; i < res.size(); i++)
    {
        *(data + i) = Constant::Util::random_u64();
    }
    return res;
}

Matrix8 Mat::randomMatrix8(int rows, int cols)
{
    Matrix8 res(rows, cols);
    res.setZero();
    u8 *data = res.data();
    for (int i = 0; i < res.size(); i++)
    {
        *(data + i) = Constant::Util::random_u8();
    }
    return res;
}

TensorRu Mat::initFromVector(vector<u64> &data, long rows, long cols)
{
    assert(data.size() == (ulong)(rows * cols));
    TensorRu res(rows, cols);
    res.setZero();
    u64 *pt = res.data();
    for (long i = 0; i < res.size(); i++)
    {
        *(pt + i) = data[i];
    }
    return res;
}

void Mat::truncateTensorRu(TensorRu &x)
{
    for (long i = 0; i < x.size(); i++)
    {
        x.data()[i] = Constant::Util::truncate(x.data()[i]);
    }
}

void Mat::truncateMatrixXu(MatrixXu &x)
{
    for (long i = 0; i < x.size(); i++)
    {
        x.data()[i] = Constant::Util::truncate(x.data()[i]);
    }
}

void Mat::reshape(TensorRu &x, long nrows, long ncols)
{
    int row = x.dimension(0);
    int col = x.dimension(1);
    if (row == nrows && col == ncols)
    {
        return;
    }
    else if (row > nrows && col > ncols)
    {
        MatrixXu temp = toMatrix(x);
        temp = temp.topLeftCorner(nrows, ncols);
        x = toTensor(temp);
    }
    else
    {
        MatrixXu nx = MatrixXu::Zero(nrows, ncols);
        MatrixXu temp = toMatrix(x);
        nx.topLeftCorner(row, col) = temp;
        TensorRu temp1 = toTensor(nx);
        x = temp1;
    }
}

void Mat::randomFill(TensorRu &x)
{
    for (long i = 0; i < x.size(); i++)
    {
        x.data()[i] = Constant::Util::random_u64();
    }
}

MatrixXu Mat::constant_multiply(MatrixXu &x, double d)
{
    MatrixXu res(x.rows(), x.cols());
    for (int i = 0; i < x.size(); i++)
    {
        res.data()[i] = (u64)(d * x.data()[i]);
    }
    // Mat::truncateMatrixXu(res);
    return res;
}

TensorRu Mat::cwiseProduct(TensorRu &a, TensorRu &b)
{
    int a_row = a.dimension(0);
    int a_col = a.dimension(1);
    int b_row = b.dimension(0);
    int b_col = b.dimension(1);
    assert(a_row == b_row);
    assert(a_col == b_col);
    TensorRu res(a_row, a_col);
    for (int i = 0; i < a.size(); i++)
    {
        res.data()[i] = a.data()[i] * b.data()[i];
    }
    return res;
}

MatrixXu Mat::getFrom_pos(char *&p)
{
    vector<u64> val;
    int r = Constant::Util::char_to_int(p);
    int c = Constant::Util::char_to_int(p);
    val.resize(r * c);
    int l = r * c;
    for (int i = 0; i < l; i++)
    {
        val[i] = Constant::Util::char_to_u64(p);
    }
    return Mat::fromVector(val, r, c);
}

int Mat::toString_pos(char *p, int r, int c, vector<u64> val)
{
    Constant::Util::int_to_char(p, r);
    Constant::Util::int_to_char(p, c);
    int l = r * c;
    for (int i = 0; i < l; i++)
    {
        Constant::Util::u64_to_char(p, val[i]);
    }
    *p = 0;
    return 2 * 4 + r * c * 8;
}

vector<u64> Mat::toVector(MatrixXu &a)
{
    vector<u64> val;
    int r = a.rows();
    int c = a.cols();
    int l = r * c;
    val.resize(r * c);
    // cout<<"toVector:"<<endl;
    for (int i = 0; i < c; i++)
        for (int j = 0; j < r; j++)
        {
            val[(i * r + j) % l] = a(j, i);
            // printf("%lld ", (long long)val[(i * c + j) % l]);
        }
    // printf("\n");
    return val;
}

MatrixXu Mat::fromVector(vector<u64> val, int r, int c)
{
    MatrixXu a(r, c);
    int l = r * c;
    // cout<<"fromVector:"<<endl;
    for (int i = 0; i < c; i++)
        for (int j = 0; j < r; j++)
        {
            a(j, i) = val[(i * r + j) % l];
            // printf("%lld ", (long long)val[(i * c + j) % l]);
        }
    // printf("\n");
    return a;
}

MatrixXu Mat::toMatrix(const TensorRu &a)
{
    int row = a.dimension(0);
    int col = a.dimension(1);
    MatrixXu res(row, col);
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            res(i, j) = a(i, j);
        }
    }
    return res;
}

TensorRu Mat::toTensor(const MatrixXu &a)
{
    int row = a.rows();
    int col = a.cols();
    TensorRu res(row, col);
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            res(i, j) = a(i, j);
        }
    }
    return res;
}

MatrixXu Mat::op_Xor(const MatrixXu &a, const MatrixXu &b)
{
    if (a.rows() == b.rows() && a.cols() == b.cols())
    {
        int row = a.rows();
        int col = a.cols();
        MatrixXu c(row, col);
        for (int i = 0; i < a.size(); i++)
        {
            c(i) = a(i) ^ b(i);
        }
        return c;
    }
    else
    {
        cout << "The Matrices can not do operator xor" << endl;
    }
}

Matrix8 Mat::op_Xor(const Matrix8 &a, const Matrix8 &b)
{
    if (a.rows() == b.rows() && a.cols() == b.cols())
    {
        int row = a.rows();
        int col = a.cols();
        Matrix8 c(row, col);
        for (int i = 0; i < a.size(); i++)
        {
            c(i) = a(i) ^ b(i);
        }
        return c;
    }
    else
    {
        cout << "The Matrices can not do operator xor" << endl;
    }
}

MatrixXu Mat::op_Xor(const u64 &a, const MatrixXu &b)
{
    int row = b.rows();
    int col = b.cols();
    MatrixXu c(row, col);
    for (int i = 0; i < b.size(); i++)
    {
        c(i) = a ^ b(i);
    }
    return c;
}

Matrix8 Mat::op_Xor(const u64 &a, const Matrix8 &b)
{
    int row = b.rows();
    int col = b.cols();
    Matrix8 c(row, col);
    for (int i = 0; i < b.size(); i++)
    {
        c(i) = a ^ b(i);
    }
    return c;
}

MatrixXu Mat::op_And(const MatrixXu &a, const MatrixXu &b)
{
    if (a.rows() == b.rows() && a.cols() == b.cols())
    {
        int row = a.rows();
        int col = a.cols();
        MatrixXu c(row, col);
        for (int i = 0; i < a.size(); i++)
        {
            c(i) = a(i) & b(i);
        }
        return c;
    }
    else
    {
        cout << "The Matrices can not do operator and" << endl;
    }
}

Matrix8 Mat::op_And(const Matrix8 &a, const Matrix8 &b)
{
    if (a.rows() == b.rows() && a.cols() == b.cols())
    {
        int row = a.rows();
        int col = a.cols();
        Matrix8 c(row, col);
        for (int i = 0; i < a.size(); i++)
        {
            c(i) = a(i) & b(i);
        }
        return c;
    }
    else
    {
        cout << "The Matrices can not do operator and" << endl;
    }
}

MatrixXu Mat::op_And(const u64 &a, const MatrixXu &b)
{
    int row = b.rows();
    int col = b.cols();
    MatrixXu c(row, col);
    for (int i = 0; i < b.size(); i++)
    {
        c(i) = a & b(i);
    }
    return c;
}

Matrix8 Mat::op_And(const u64 &a, const Matrix8 &b)
{
    int row = b.rows();
    int col = b.cols();
    Matrix8 c(row, col);
    for (int i = 0; i < b.size(); i++)
    {
        c(i) = a & b(i);
    }
    return c;
}

MatrixXu Mat::op_Or(const MatrixXu &a, const MatrixXu &b)
{
    if (a.rows() == b.rows() && a.cols() == b.cols())
    {
        int row = a.rows();
        int col = a.cols();
        MatrixXu c(row, col);
        for (int i = 0; i < a.size(); i++)
        {
            c(i) = a(i) | b(i);
        }
        return c;
    }
    else
    {
        cout << "The Matrices can not do operator and" << endl;
    }
}

Matrix8 Mat::op_Or(const Matrix8 &a, const Matrix8 &b)
{
    if (a.rows() == b.rows() && a.cols() == b.cols())
    {
        int row = a.rows();
        int col = a.cols();
        Matrix8 c(row, col);
        for (int i = 0; i < a.size(); i++)
        {
            c(i) = a(i) | b(i);
        }
        return c;
    }
    else
    {
        cout << "The Matrices can not do operator and" << endl;
    }
}

MatrixXu Mat::op_Or(const u64 &a, const MatrixXu &b)
{
    int row = b.rows();
    int col = b.cols();
    MatrixXu c(row, col);
    for (int i = 0; i < b.size(); i++)
    {
        c(i) = a | b(i);
    }
    return c;
}

Matrix8 Mat::op_Or(const u64 &a, const Matrix8 &b)
{
    int row = b.rows();
    int col = b.cols();
    Matrix8 c(row, col);
    for (int i = 0; i < b.size(); i++)
    {
        c(i) = a | b(i);
    }
    return c;
}

MatrixXu Mat::op_shift_left(const MatrixXu &b, const int &a)
{
    int row = b.rows();
    int col = b.cols();
    MatrixXu c(row, col);
    for (int i = 0; i < b.size(); i++)
    {
        c(i) = b(i) << a;
    }
    return c;
}

Matrix8 Mat::op_shift_left(const Matrix8 &b, const int &a)
{
    int row = b.rows();
    int col = b.cols();
    Matrix8 c(row, col);
    for (int i = 0; i < b.size(); i++)
    {
        c(i) = b(i) << a;
    }
    return c;
}

MatrixXu Mat::op_shift_right(const MatrixXu &b, const int &a)
{
    int row = b.rows();
    int col = b.cols();
    MatrixXu c(row, col);
    for (int i = 0; i < b.size(); i++)
    {
        c(i) = b(i) >> a;
    }
    return c;
}

Matrix8 Mat::op_shift_right(const Matrix8 &b, const int &a)
{
    int row = b.rows();
    int col = b.cols();
    Matrix8 c(row, col);
    for (int i = 0; i < b.size(); i++)
    {
        c(i) = b(i) >> a;
    }
    return c;
}