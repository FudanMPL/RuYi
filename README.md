
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

Then secret share the MNIST dataset before training.
