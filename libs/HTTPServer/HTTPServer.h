#ifndef __HTTPServer_h_
#define __HTTPServer_h_

#include "../TCPServer.h"
#include "HTTPServerConnection.h"
#include "smw.h"

// Callback function that is called when a new connection is accepted.
// The context is the context that was passed to the HTTPServer_Initiate function.
// The connection is the new connection that was accepted.
typedef int (*HTTPServer_OnConnection)(void* _Context, HTTPServerConnection* _Connection);

// The HTTPServer struct.
// It contains the TCPServer, a callback for new connections, and a task for the smw worker.
typedef struct {
    // The context that will be passed to the onConnection callback.
    void* context;
    // The callback that will be called when a new connection is accepted.
    HTTPServer_OnConnection onConnection;
    // The underlying TCPServer.
    TCPServer tcpServer;
    // The task that will be executed by the smw_work function.
    smw_task* task;
} HTTPServer;

// Initializes a HTTPServer.
int HTTPServer_Initiate(HTTPServer* _Server, void* _Context, HTTPServer_OnConnection _OnConnection);
// Initializes a HTTPServer and allocates memory for it.
int HTTPServer_InitiatePtr(void* _Context, HTTPServer_OnConnection _OnConnection, HTTPServer** _ServerPtr);

// Disposes a HTTPServer.
void HTTPServer_Dispose(HTTPServer* _Server);
// Disposes a HTTPServer and frees the allocated memory.
void HTTPServer_DisposePtr(HTTPServer** _ServerPtr);

#endif //__HTTPServer_h_
