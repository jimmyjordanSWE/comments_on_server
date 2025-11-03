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

// The TCPClient struct.
// It contains the file descriptor of the connection.
typedef struct TCPClient {
    // The file descriptor of the connection.
    int fd;
} TCPClient;

// Initializes a TCPClient with an existing file descriptor.
int TCPClient_Initiate(TCPClient* c, int _FD);

// Connects to a server.
int TCPClient_Connect(TCPClient* c, const char* host, const char* port);

// Writes data to the connection.
int TCPClient_Write(TCPClient* c, const uint8_t* buf, int len);
// Reads data from the connection.
int TCPClient_Read(TCPClient* c, uint8_t* buf, int len);

// Disconnects from the server.
void TCPClient_Disconnect(TCPClient* c);

// Disposes a TCPClient.
void TCPClient_Dispose(TCPClient* c);

#endif // __TCPClient_h_