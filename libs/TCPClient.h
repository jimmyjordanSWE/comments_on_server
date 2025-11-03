#ifndef __TCPClient_h_
#define __TCPClient_h_

#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define _POSIX_C_SOURCE 200809L

// The tcp_client_t struct.
// It contains the file descriptor of the connection.
typedef struct tcp_client_t {
    // The file descriptor of the connection.
    int fd;
} tcp_client_t;

// Initializes a tcp_client_t with an existing file descriptor.
int tcp_client_init(tcp_client_t* c, int _FD);

// Connects to a server.
int tcp_client_connect(tcp_client_t* c, const char* host, const char* port);

// Writes data to the connection.
int tcp_client_write(tcp_client_t* c, const uint8_t* buf, int len);
// Reads data from the connection.
int tcp_client_read(tcp_client_t* c, uint8_t* buf, int len);

// Disconnects from the server.
void tcp_client_disconnect(tcp_client_t* c);

// Disposes a tcp_client_t.
void tcp_client_dispose(tcp_client_t* c);

#endif // __TCPClient_h_