#include "HTTPServerConnection.h"
#include <stdio.h>
#include <stdlib.h>

//-----------------Internal Functions-----------------
void HTTPServerConnection_TaskWork(void* _Context, uint64_t _MonTime);
//----------------------------------------------------

int HTTPServerConnection_Initiate(HTTPServerConnection* _Connection, int _FD) {
    // Initialize the TCPClient with the file descriptor of the connection.
    TCPClient_Initiate(&_Connection->tcpClient, _FD);
    // Create a task for the smw worker.
    _Connection->task = smw_createTask(_Connection, HTTPServerConnection_TaskWork);
    return 0;
}

int HTTPServerConnection_InitiatePtr(int _FD, HTTPServerConnection** _ConnectionPtr) {
    if (_ConnectionPtr == NULL)
        return -1;

    HTTPServerConnection* _Connection = (HTTPServerConnection*)malloc(sizeof(HTTPServerConnection));
    if (_Connection == NULL)
        return -2;

    int result = HTTPServerConnection_Initiate(_Connection, _FD);
    if (result != 0) {
        free(_Connection);
        return result;
    }

    *(_ConnectionPtr) = _Connection;

    return 0;
}

// Sets the callback that will be called when a new request is received.
void HTTPServerConnection_SetCallback(HTTPServerConnection* _Connection, void* _Context, HTTPServerConnection_OnRequest _OnRequest) {
    _Connection->context = _Context;
    _Connection->onRequest = _OnRequest;
}

// This function is called by the smw_work function.
void HTTPServerConnection_TaskWork(void* _Context, uint64_t _MonTime) {
    HTTPServerConnection* _Connection = (HTTPServerConnection*)_Context;

    // TODO: Implement HTTP parsing here.
    // 1. Read data from the TCPClient into a buffer.
    // 2. Parse the buffer as an HTTP request.
    // 3. If a full request is parsed:
    //    a. Populate the method and url fields in the HTTPServerConnection struct.
    //    b. Call the onRequest callback (WeatherServerInstance_OnRequest).
    //    c. Remove the parsed request from the buffer.
    // 4. If the buffer doesn't contain a full request yet, do nothing and wait for more data to arrive.

    printf("HTTPServerConnection_TaskWork\n");

    // If there is a callback set, call it.
    if (_Connection->onRequest) {
        _Connection->onRequest(_Connection->context);
    }
}

void HTTPServerConnection_Dispose(HTTPServerConnection* _Connection) {
    TCPClient_Dispose(&_Connection->tcpClient);
    smw_destroyTask(_Connection->task);
}

void HTTPServerConnection_DisposePtr(HTTPServerConnection** _ConnectionPtr) {
    if (_ConnectionPtr == NULL || *(_ConnectionPtr) == NULL)
        return;

    HTTPServerConnection_Dispose(*(_ConnectionPtr));
    free(*(_ConnectionPtr));
    *(_ConnectionPtr) = NULL;
}
