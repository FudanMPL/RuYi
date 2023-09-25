#ifndef SOCKETMANAGER_H
#define SOCKETMANAGER_H

#include "SocketOnline.h"

#ifndef MULTI_THREAD

extern int party;

class SocketManager
{
public:
    static vector<vector<SocketOnline *>> socket_io;
    static void init_windows_socket();
    static void exit_windows_socket();
    static void server_init(SOCK &sock, string ip, u64 port);
    static void client_init(SOCK &sock, string ip, u64 port, u64 myport);
    static void socket_close(SOCK sock);
    static SOCK accept_sock(SOCK sock);
    static void print_socket();
    static void print();
    static void send(MatrixXu *a, int target);
    static void send(Matrix8 *a, int target);
    static void send(Matrixint128 *a, int target);
    static void receive(MatrixXu *a, int from);
    static void receive(Matrix8 *a, int from);
    static void receive(Matrixint128 *a, int from);
    class pMPL
    {
        vector<string> ip;
        MatrixXu port;
        SOCK serv_sock;
        SOCK *clnt_sock;
        SOCK sock;
        int epoch;

    public:
        pMPL();
        void init();
        void init(vector<string> &ip, MatrixXu &port);
        void server();
        void client();
        void server_exit();
        void client_exit();
        void exit_all();
    };
};

#endif // MULTI_THREAD_SOCKET_H

#endif // SOCKETMANAGER_H
