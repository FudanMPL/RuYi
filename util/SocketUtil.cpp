#include "SocketUtil.h"

// const char ips[Config::config->M][MAX_PROT_LENGTH] =  {"0.0.0.0", "0.0.0.0"};
// const int server_port[Config::config->M] = {20001, 20002};
// const int client_port[3][3] = {{0,0,0},{30001,0},{40001 ,30002}};

const int socket_buffer_size = 4194304;
vector<int> socket_fds;

int64 *buffer = new int64[100];
int buffer_size = 100 * sizeof(int64);

vector<string> SocketUtil::ips;

extern u64 send_size;
extern u64 receive_size;

void SocketUtil::socket_init()
{
    ips.resize(Config::config->M);
    socket_fds.resize(Config::config->M);
    for (int i = 0; i < Config::config->M; i++)
    {
        ips[i] = {"10.176.34.173"};
    }
    int server_port[Config::config->M - 1];
    vector<vector<int>> client_port(Config::config->M, vector<int>(Config::config->M, 0));
    int s = 20001;
    for (int i = 0; i < Config::config->M - 1; i++)
    {
        server_port[i] = s++;
        cout << server_port[i] << endl;
    }
    int c = 30001;
    for (int i = 1; i < Config::config->M; i++)
    {
        for (int j = 0; j < i; j++)
        {
            client_port[i][j] = c++;
        }
    }

    for (int i = 0; i < Config::config->M; i++)
        socket_fds[i] = 0;
    if (party != Config::config->M - 1)
        wait_for_connect(server_port[party]);
    for (int i = 0; i < party; i++)
    {
        connect_others(ips[i], server_port[i], client_port[party][i]);
    }
    greet();
}

void SocketUtil::wait_for_connect(int port)
{
    int serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    int opt = 1;
    setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, (const void *)&opt, sizeof(opt));
    // setsockopt(serv_sock, SOL_SOCKET, SO_RCVBUF, (const char *)&socket_buffer_size, sizeof(int));
    // setsockopt(serv_sock, SOL_SOCKET, SO_SNDBUF, (const char *)&socket_buffer_size, sizeof(int));
    setsockopt(serv_sock, IPPROTO_TCP, TCP_NODELAY, (char *)&opt, sizeof(int));
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("0.0.0.0");
    serv_addr.sin_port = htons(port);
    ::bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    //    bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    listen(serv_sock, 20);
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size = sizeof(clnt_addr);
    for (int i = party + 1; i < Config::config->M; i++)
    {
        socket_fds[i] = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_size);
        cout << socket_fds[i] << endl;
        // setsockopt(socket_fds[i], SOL_SOCKET, SO_RCVBUF, (const char *)&socket_buffer_size, sizeof(int));
        // setsockopt(socket_fds[i], SOL_SOCKET, SO_SNDBUF, (const char *)&socket_buffer_size, sizeof(int));
        setsockopt(socket_fds[i], IPPROTO_TCP, TCP_NODELAY, (char *)&opt, sizeof(int));
    }
}

void SocketUtil::connect_others(string ip, int port, int myport)
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in my_addr;
    my_addr.sin_family = AF_INET;
    my_addr.sin_addr.s_addr = inet_addr("0.0.0.0");
    my_addr.sin_port = htons(myport);
    ::bind(sock, (struct sockaddr *)&my_addr, sizeof(my_addr));

    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(ip.c_str());
    serv_addr.sin_port = htons(port);
    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    int opt = 1;
    for (int i = 0; i < Config::config->M; i++)
    {
        if (socket_fds[i] == 0)
        {
            socket_fds[i] = sock;
            // setsockopt(socket_fds[i], SOL_SOCKET, SO_RCVBUF, (const char *)&socket_buffer_size, sizeof(int));
            // setsockopt(socket_fds[i], SOL_SOCKET, SO_SNDBUF, (const char *)&socket_buffer_size, sizeof(int));
            setsockopt(socket_fds[i], IPPROTO_TCP, TCP_NODELAY, (char *)&opt, sizeof(int));
            break;
        }
    }
}

void SocketUtil::greet()
{
    int *temp_pid = new int[Config::config->M];
    temp_pid[party] = 0;
    for (int i = 0; i < Config::config->M; i++)
    {
        if (socket_fds[i] != 0)
            send_n(socket_fds[i], (char *)&party, 4);
    }
    for (int i = 0; i < Config::config->M; i++)
    {
        int id;
        if (socket_fds[i] != 0)
        {
            recv_n(socket_fds[i], (char *)&id, 4);
            cout << "id " << id << endl;
            temp_pid[id] = socket_fds[i];
        }
    }
    for (int i = 0; i < Config::config->M; i++)
        socket_fds[i] = temp_pid[i];
    delete[] temp_pid;
}

void SocketUtil::send_n(int fd, char *data, int len)
{
    // communication_size = communication_size + len;
    send_size = send_size + static_cast<u64>(len);
    int temp = 0;
    while (temp < len)
    {
        temp = temp + write(fd, data + temp, len - temp);
    }
}

void SocketUtil::recv_n(int fd, char *data, int len)
{
    receive_size = receive_size + static_cast<u64>(len);
    int temp = 0;
    while (temp < len)
    {
        temp = temp + read(fd, data + temp, len - temp);
    }
}

void SocketUtil::send(const MatrixXu &mat, int pid)
{
    int r = mat.rows();
    int c = mat.cols();
    send_n(socket_fds[pid], (char *)&r, 4);
    send_n(socket_fds[pid], (char *)&c, 4);
    send_size -= 8;
    send_n(socket_fds[pid], (char *)mat.data(), r * c * sizeof(u64));
}

void SocketUtil::send(const Matrix8 &mat, int pid)
{
    int r = mat.rows();
    int c = mat.cols();
    send_n(socket_fds[pid], (char *)&r, 4);
    send_n(socket_fds[pid], (char *)&c, 4);
    send_size -= 8;
    send_n(socket_fds[pid], (char *)mat.data(), r * c * sizeof(u8));
}

void SocketUtil::receive(MatrixXu &mat, int pid)
{
    int r;
    int c;
    recv_n(socket_fds[pid], (char *)&r, 4);
    recv_n(socket_fds[pid], (char *)&c, 4);
    receive_size -= 8;
    mat.resize(r, c);
    recv_n(socket_fds[pid], (char *)mat.data(), r * c * sizeof(u64));
}

void SocketUtil::receive(Matrix8 &mat, int pid)
{
    int r;
    int c;
    recv_n(socket_fds[pid], (char *)&r, 4);
    recv_n(socket_fds[pid], (char *)&c, 4);
    receive_size -= 8;
    mat.resize(r, c);
    recv_n(socket_fds[pid], (char *)mat.data(), r * c * sizeof(u8));
}

//void send_array(int pid, const Int_128_Array& mat){
//    int r = mat.rows();
//    int c = mat.cols();
//    send_n(socket_fds[pid], (char *) &r, 4);
//    send_n(socket_fds[pid], (char *) &c, 4);
//    send_n(socket_fds[pid], (char *) mat.data(), r*c*sizeof(int128));
//}
//Int_128_Array  recv_matrix(int pid){
//    int r;
//    int c;
//    recv_n(socket_fds[pid], (char *)&r, 4);
//    recv_n(socket_fds[pid], (char *)&c, 4);
//    Int_128_Array res(r, c);
//    recv_n(socket_fds[pid], (char *) res.data(), r*c*sizeof(int128));
//    return res;
//}

// void send_array(int pid, const Int_128_Array& mat){
//     int r = mat.rows();
//     int c = mat.cols();
//     int64 * buffer = new int64[r*c];
//     for (int i = 0; i < r * c; i++){
//         buffer[i] = mat(i);
//     }
//     send_n(socket_fds[pid], (char *) &r, 4);
//     send_n(socket_fds[pid], (char *) &c, 4);
//     send_n(socket_fds[pid], (char *) buffer, r*c*sizeof(int64));
//     delete buffer;
// }

// void send_128_array(int pid, const Int_128_Array& mat){
//     int r = mat.rows();
//     int c = mat.cols();
//     int64 * buffer = new int64[r*c];
//     for (int i = 0; i < r * c; i++){
//         buffer[i] = mat(i);
//     }
//     send_n(socket_fds[pid], (char *) &r, 4);
//     send_n(socket_fds[pid], (char *) &c, 4);
//     send_n(socket_fds[pid], (char *) buffer, r*c*sizeof(int64));
//     delete buffer;
// }

// void send_ring_array(int pid, const Ring_Array& mat){
//     int r = mat.rows();
//     int c = mat.cols();
//     send_n(socket_fds[pid], (char *) &r, 4);
//     send_n(socket_fds[pid], (char *) &c, 4);
//     send_n(socket_fds[pid], (char *) mat.data(), r*c*sizeof(ringint));
// }

//void send_array(int pid, const Int_128_Array* mat){
//    int r = mat->rows();
//    int c = mat->cols();
//    if (buffer_size < r * c * sizeof(int64)){
//        delete buffer;
//        buffer = new int64[r*c];
//        buffer_size = r * c * sizeof(int64);
//    }
//    for (int i = 0; i < r * c; i++){
//        buffer[i] = (*mat)(i);
//    }
//    send_n(socket_fds[pid], (char *) &r, 4);
//    send_n(socket_fds[pid], (char *) &c, 4);
//    send_n(socket_fds[pid], (char *) buffer, r*c*sizeof(int64));
//}
//void send_array(int pid, const Ring_Array* mat){
//    int r = mat->rows();
//    int c = mat->cols();
//    send_n(socket_fds[pid], (char *) &r, 4);
//    send_n(socket_fds[pid], (char *) &c, 4);
//    send_n(socket_fds[pid], (char *) mat->data(), r*c*sizeof(ringint));
//}

// Int_128_Array recv_matrix(int pid){
//     int r;
//     int c;
//     recv_n(socket_fds[pid], (char *)&r, 4);
//     recv_n(socket_fds[pid], (char *)&c, 4);
//     if (buffer_size < r * c * sizeof(int64)){
//         delete buffer;
//         buffer = new int64[r*c];
//         buffer_size = r * c * sizeof(int64);
//     }
//     Int_128_Array res(r, c);
//     recv_n(socket_fds[pid], (char *) buffer, r*c*sizeof(int64));
//     for (int i =0; i < r *c; i++){
//         res(i) = buffer[i];
//     }
//     return res;
// }