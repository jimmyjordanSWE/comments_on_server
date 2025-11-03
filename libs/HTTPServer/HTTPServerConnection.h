#ifndef __HTTPServerConnection_h_
#define __HTTPServerConnection_h_

#include "../TCPClient.h"
#include "smw.h"

// Callback function that is called when a new request is received.
// The context is the context that was passed to the HTTPServerConnection_SetCallback function.
typedef int (*HTTPServerConnection_OnRequest)(void* _Context);

// The HTTPServerConnection struct.
// It contains the TCPClient, a callback for new requests, and a task for the smw worker.
typedef struct {
    // The underlying TCPClient.
    TCPClient tcpClient;
    // The context that will be passed to the onRequest callback.
    void* context;
    // The callback that will be called when a new request is received.
    HTTPServerConnection_OnRequest onRequest;
    // The HTTP method of the request.
    char* method;
    // The URL of the request.
    char* url;
    // The task that will be executed by the smw_work function.
    smw_task* task;
} HTTPServerConnection;

// Initializes a HTTPServerConnection.
int HTTPServerConnection_Initiate(HTTPServerConnection* _Connection, int _FD);
// Initializes a HTTPServerConnection and allocates memory for it.
int HTTPServerConnection_InitiatePtr(int _FD, HTTPServerConnection** _ConnectionPtr);

// Sets the callback that will be called when a new request is received.
void HTTPServerConnection_SetCallback(HTTPServerConnection* _Connection, void* _Context, HTTPServerConnection_OnRequest _OnRequest);

// Disposes a HTTPServerConnection.
void HTTPServerConnection_Dispose(HTTPServerConnection* _Connection);
// Disposes a HTTPServerConnection and frees the allocated memory.
void HTTPServerConnection_DisposePtr(HTTPServerConnection** _ConnectionPtr);

#endif //__HTTPServerConnection_h_
