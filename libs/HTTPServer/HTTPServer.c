#include "HTTPServer.h"
#include <stdio.h>
#include <stdlib.h>
//-----------------Callback Functions-----------------
// This function is called by the TCPServer when a new connection is accepted.
int HTTPServer_OnAccept(int _FD, void* _Context) {
    // The context is the HTTPServer itself.
    HTTPServer* _Server = (HTTPServer*)_Context;
    HTTPServerConnection* connection = NULL;
    // Create a new HTTPServerConnection to handle the connection.
    int result = HTTPServerConnection_InitiatePtr(_FD, &connection);
    if (result != 0) {
        printf("HTTPServer_OnAccept: Failed to initiate connection\n");
        return -1;
    }
    // Call the onConnection callback to notify the user of the new connection.
    _Server->onConnection(_Server->context, connection);
    return 0;
}
//----------------------------------------------------

//-----------------Internal Functions-----------------
void HTTPServer_TaskWork(void* _Context, uint64_t _MonTime);
int HTTPServer_OnAccept(int _FD, void* _Context);
//----------------------------------------------------

int HTTPServer_Initiate(HTTPServer* _Server, void* _Context, HTTPServer_OnConnection _OnConnection) {
    printf("ENTRY HTTPServer_Initiate ... ");
    _Server->context = _Context;
    _Server->onConnection = _OnConnection;
    // Initialize the TCPServer and set the callback for new connections.
    // The HTTPServer itself is passed as the context.
    TCPServer_Initiate(&_Server->tcpServer, "8080", HTTPServer_OnAccept, _Server);
    // Create a task for the smw worker.
    _Server->task = smw_createTask(_Server, HTTPServer_TaskWork);
    printf("EXIT HTTPServer_Initiate\n");
    return 0;
}

int HTTPServer_InitiatePtr(void* _Context, HTTPServer_OnConnection _OnConnection, HTTPServer** _ServerPtr) {
    if (_ServerPtr == NULL)
        return -1;

    HTTPServer* _Server = (HTTPServer*)malloc(sizeof(HTTPServer));
    if (_Server == NULL)
        return -2;

    int result = HTTPServer_Initiate(_Server, _Context, _OnConnection);
    if (result != 0) {
        free(_Server);
        return result;
    }

    *(_ServerPtr) = _Server;

    return 0;
}

// This function is called by the smw_work function.
void HTTPServer_TaskWork(void* _Context, uint64_t _MonTime) {
    // The context is the HTTPServer itself.

    // HTTPServer* _Server = (HTTPServer*)_Context;
}

void HTTPServer_Dispose(HTTPServer* _Server) {
    TCPServer_Dispose(&_Server->tcpServer);
    smw_destroyTask(_Server->task);
}

void HTTPServer_DisposePtr(HTTPServer** _ServerPtr) {
    if (_ServerPtr == NULL || *(_ServerPtr) == NULL)
        return;

    HTTPServer_Dispose(*(_ServerPtr));
    free(*(_ServerPtr));
    *(_ServerPtr) = NULL;
}
