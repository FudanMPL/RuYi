
# RuYi
A Configurable and Efficient Secure Multi-Party Learning Framework with Privileged Parties

This is a code for experimentation.

## Configure

**constant.json**

Configure the number of parties, training parameters (e.g. batch size), and machine learning models. 

"M" refers to the number of total parties, "np" refers to the number of privileged parties, "na" refers to the number of assistant parties, and "nd" refers to the number of assistant parties allowed to drop out. 
Note that, $np + na = M$ and $nd < na$.


## Dataset

Download the MNIST dataset to the ./data folder and delete the first line of the dataset.

Then secret share the MNIST dataset before training. (Ruyi/util/IOManager.cpp line 266)

```c++
ifstream infile("data/mnist/mnist_train.csv");
// // ifstream infile("data/fashion_mnist/fashion_mnist_train.csv");
// // ifstream infile("data/svhn/svhn_train.csv");
// load_data(infile, train_data, train_label, Config::config->N);
if (party == 0)
    secret_share(train_data, train_label, "train");
infile.close();
```
