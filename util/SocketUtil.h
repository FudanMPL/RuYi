#ifndef SOCKETUTIL_H
#define SOCKETUTIL_H

#include "unistd.h"
#include "Mat.h"

class SocketUtil
{
public:
    // SocketUtil ();
    // ~SocketUtil();
    static vector<string> ips;

    static void socket_init();
    static void wait_for_connect(int port);
    static void connect_others(string ip, int port, int myport);
    static void greet();

    static void send_n(int fd, char *data, int len);
    static void recv_n(int sock, char *data, int len);

    // void send_array(int pid, const Int_128_Array &mat);
    static void send(const MatrixXu &mat, int pid);
    static void send(const Matrix8 &mat, int pid);

    // void send_ring_array(int pid, const Ring_Array &mat);
    // void send_128_array(int pid, const Int_128_Array &mat);

    // void recv_array(int pid, Int_128_Array &mat);
    static void receive(MatrixXu &mat, int pid);
    static void receive(Matrix8 &mat, int pid);

    // static void print();


    // Int_128_Array recv_matrix(int pid);
};

#endif //SOCKETUTIL_H
