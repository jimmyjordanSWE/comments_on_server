#ifndef __TCPServer_h_
#define __TCPServer_h_

#include "smw.h"
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define _POSIX_C_SOURCE 200809L
#define TCP_MAX_CLIENTS 10

// Callback function that is called when a new connection is accepted.
// The client_fd is the file descriptor of the new connection.
// The context is the context that was passed to the tcp_server_init function.
typedef int (*tcp_server_on_accept_cb)(int client_fd, void* context);

// The tcp_server_t struct.
// It contains the listen file descriptor, a callback for new connections, and a task for the smw worker.
typedef struct {
    // The file descriptor that the server is listening on.
    int listen_fd;
    // The callback that will be called when a new connection is accepted.
    tcp_server_on_accept_cb on_accept;
    // The context that will be passed to the on_accept callback.
    void* context;
    // The task that will be executed by the smw_work function.
    smw_task_t* task;
} tcp_server_t;

// Initializes a tcp_server_t.
int tcp_server_init(tcp_server_t* _Server, const char* _Port, tcp_server_on_accept_cb _OnAccept, void* _Context);
// Initializes a tcp_server_t and allocates memory for it.
int tcp_server_new(const char* _Port, tcp_server_on_accept_cb _OnAccept, void* _Context, tcp_server_t** _ServerPtr);

// Disposes a tcp_server_t.
void tcp_server_dispose(tcp_server_t* _Server);
// Disposes a tcp_server_t and frees the allocated memory.
void tcp_server_free(tcp_server_t** _ServerPtr);

// Sets a file descriptor to non-blocking mode.
static inline int tcp_server_set_nonblocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags < 0)
        return -1;

    return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

#endif //__TCPServer_h_
