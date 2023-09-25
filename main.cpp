#include <iostream>
#include "json/json.h"
#include "ml/linear.h"
#include "ml/logistic.h"
#include "ml/nn.h"

using namespace std;
using namespace Eigen;

u64 send_size = 0;
u64 receive_size = 0;

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        DBGprint("Please enter party index:\n");
        scanf("%d", &party);
    }
    else
    {
        party = argv[1][0] - '0';
    }
    DBGprint("party index: %d\n", party);
    Config::config = Config::init("constant.json");
    Mat::init_public_vector();
    // cout << "A_plus:" << endl
    //      << Mat::A_plus << endl;
    // cout << "A_inv:" << endl
    //      << Mat::A_inv << endl;
    // cout << "inv_for_mul" << endl;
    // for (int i = 0; i < Config::config->M; i++)
    // {
    //     cout << Mat::inv_for_mul[i] << " ";
    // }
    // cout << endl;
    // cout << "inv_for_div" << endl;
    // for (int i = 0; i < Config::config->M; i++)
    // {
    //     cout << Mat::inv_for_div[i] << " ";
    // }
    // cout << endl;
    // cout << "div_flag" << endl;
    // for (int i = 0; i < Config::config->M; i++)
    // {
    //     cout << Mat::div_flag[i] << " ";
    // }
    // cout << endl;
    Offline::init();
    // Secret_Mul::init();
    // Boolean_Share::init();
    // SocketManager::pMPL tel;
    // tel.init();
    SocketUtil::socket_init();
    IOManager::init();
    Secret_Mul::get_Triplets();
    Boolean_Share::test();
    if (Config::config->ML == 0)
        Linear::train_model();
    else if (Config::config->ML == 1)
    {
        Secret_Cmp::get_Share(2 * Config::config->B, Config::config->numClass);
        Logistic::train_model();
    }
    else if (Config::config->ML == 2)
    {
        Secret_Cmp::get_Share(Config::config->B, Config::config->hiddenDim);
        NN::train_model();
    }
    // Secret_Cmp::get_Share(2 * Config::config->B, Config::config->numClass);
    // MatrixXu a = Mat::randomMatrixXu(Config::config->B, Config::config->numClass);
    // MatrixXu delta_a = Mat::randomMatrixXu(Config::config->B, Config::config->numClass);;
    // Constant::Clock *clock_train;
    // clock_train = new Constant::Clock(1);
    // for (int i = 0; i < Config::config->Ep; i++)
    // {
    //     MatrixXu z = Secret_Cmp::Sigmoid(a, delta_a);
    // }
    // cout << "次/秒：" << Config::config->Ep / clock_train->get() << endl;
    // SocketManager::print();
    return 0;
}