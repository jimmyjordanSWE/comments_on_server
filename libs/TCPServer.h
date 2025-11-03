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
#define MAX_CLIENTS 10

// Callback function that is called when a new connection is accepted.
// The client_fd is the file descriptor of the new connection.
// The context is the context that was passed to the TCPServer_Initiate function.
typedef int (*TCPServer_OnAccept)(int client_fd, void* context);

// The TCPServer struct.
// It contains the listen file descriptor, a callback for new connections, and a task for the smw worker.
typedef struct {
    // The file descriptor that the server is listening on.
    int listen_fd;
    // The callback that will be called when a new connection is accepted.
    TCPServer_OnAccept onAccept;
    // The context that will be passed to the onAccept callback.
    void* context;
    // The task that will be executed by the smw_work function.
    smw_task* task;
} TCPServer;

// Initializes a TCPServer.
int TCPServer_Initiate(TCPServer* _Server, const char* _Port, TCPServer_OnAccept _OnAccept, void* _Context);
// Initializes a TCPServer and allocates memory for it.
int TCPServer_InitiatePtr(const char* _Port, TCPServer_OnAccept _OnAccept, void* _Context, TCPServer** _ServerPtr);

// Disposes a TCPServer.
void TCPServer_Dispose(TCPServer* _Server);
// Disposes a TCPServer and frees the allocated memory.
void TCPServer_DisposePtr(TCPServer** _ServerPtr);

// Sets a file descriptor to non-blocking mode.
static inline int TCPServer_Nonblocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags < 0)
        return -1;

    return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

#endif //__TCPServer_h_
