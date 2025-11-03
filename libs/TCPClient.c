#include "TCPClient.h"

// Initializes a TCPClient with an existing file descriptor.
int TCPClient_Initiate(TCPClient* c, int fd) {
    c->fd = fd;
    return 0;
}

// Connects to a server.
int TCPClient_Connect(TCPClient* c, const char* host, const char* port) {
    if (c->fd >= 0)
        return -1;

    struct addrinfo hints = {0};
    struct addrinfo* res = NULL;

    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    if (getaddrinfo(host, port, &hints, &res) != 0)
        return -1;

    int fd = -1;
    for (struct addrinfo* rp = res; rp; rp = rp->ai_next) {
        fd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (fd < 0)
            continue;

        if (connect(fd, rp->ai_addr, rp->ai_addrlen) == 0)
            break;

        close(fd);
        fd = -1;
    }

    freeaddrinfo(res);
    if (fd < 0)
        return -1;

    c->fd = fd;
    return 0;
}

// Writes data to the connection.
int TCPClient_Write(TCPClient* c, const uint8_t* buf, int len) {
    return send(c->fd, buf, len, MSG_NOSIGNAL);
}

// Reads data from the connection.
int TCPClient_Read(TCPClient* c, uint8_t* buf, int len) {
    // MSG_DONTWAIT makes the read non-blocking.
    return recv(c->fd, buf, len, MSG_DONTWAIT);
}

// Disconnects from the server.
void TCPClient_Disconnect(TCPClient* c) {
    if (c->fd >= 0)
        close(c->fd);

    c->fd = -1;
}

// Disposes a TCPClient.
void TCPClient_Dispose(TCPClient* c) {
    TCPClient_Disconnect(c);
}