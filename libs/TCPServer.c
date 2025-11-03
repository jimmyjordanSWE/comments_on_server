#include "TCPServer.h"
#include <stdlib.h>

//-----------------Internal Functions-----------------
void tcp_server_work(void* _Context, uint64_t _MonTime);
//----------------------------------------------------

int tcp_server_init(tcp_server_t* _Server, const char* _Port, tcp_server_on_accept_cb _OnAccept, void* _Context) {
    _Server->on_accept = _OnAccept;
    _Server->context = _Context;

    struct addrinfo hints = {0}, *res = NULL;
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if (getaddrinfo(NULL, _Port, &hints, &res) != 0)
        return -1;

    int fd = -1;
    for (struct addrinfo* rp = res; rp; rp = rp->ai_next) {
        fd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (fd < 0)
            continue;

        int yes = 1;
        setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
        if (bind(fd, rp->ai_addr, rp->ai_addrlen) == 0)
            break;

        close(fd);
        fd = -1;
    }

    freeaddrinfo(res);
    if (fd < 0)
        return -1;

    if (listen(fd, TCP_MAX_CLIENTS) < 0) {
        close(fd);
        return -1;
    }

    // Set the socket to non-blocking mode.
    tcp_server_set_nonblocking(fd);

    _Server->listen_fd = fd;

    // Create a task for the smw worker.
    _Server->task = smw_create_task(_Server, tcp_server_work);

    return 0;
}

int tcp_server_new(const char* _Port, tcp_server_on_accept_cb _OnAccept, void* _Context, tcp_server_t** _ServerPtr) {
    if (_ServerPtr == NULL)
        return -1;

    tcp_server_t* _Server = (tcp_server_t*)malloc(sizeof(tcp_server_t));
    if (_Server == NULL)
        return -2;

    int result = tcp_server_init(_Server, _Port, _OnAccept, _Context);
    if (result != 0) {
        free(_Server);
        return result;
    }

    *(_ServerPtr) = _Server;

    return 0;
}

// This function accepts a new connection and calls the on_accept callback.
int tcp_server_accept(tcp_server_t* _Server) {
    int socket_fd = accept(_Server->listen_fd, NULL, NULL);
    if (socket_fd < 0) {
        if (errno == EAGAIN || errno == EWOULDBLOCK)
            return 0; // no new connection

        perror("accept");
        return -1;
    }

    // Set the new connection to non-blocking mode.
    tcp_server_set_nonblocking(socket_fd);

    // Call the on_accept callback to notify the user of the new connection.
    int result = _Server->on_accept(socket_fd, _Server->context);
    if (result != 0)
        close(socket_fd);

    return 0;
}

// This function is called by the smw_work function.
void tcp_server_work(void* _Context, uint64_t _MonTime) {
    // The context is the tcp_server_t itself.
    tcp_server_t* _Server = (tcp_server_t*)_Context;

    // Accept new connections.
    tcp_server_accept(_Server);
}

void tcp_server_dispose(tcp_server_t* _Server) {
    smw_destroy_task(_Server->task);
}

void tcp_server_free(tcp_server_t** _ServerPtr) {
    if (_ServerPtr == NULL || *(_ServerPtr) == NULL)
        return;

    tcp_server_dispose(*(_ServerPtr));
    free(*(_ServerPtr));
    *(_ServerPtr) = NULL;
}
