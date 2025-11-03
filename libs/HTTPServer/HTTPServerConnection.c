#include "HTTPServerConnection.h"
#include <stdio.h>
#include <stdlib.h>

//-----------------Internal Functions-----------------
void http_connection_work(void* _Context, uint64_t _MonTime);
//----------------------------------------------------

int http_connection_init(http_connection_t* _Connection, int _FD) {
    // Initialize the tcp_client_t with the file descriptor of the connection.
    tcp_client_init(&_Connection->tcp_client, _FD);
    // Create a task for the smw worker.
    _Connection->task = smw_create_task(_Connection, http_connection_work);
    return 0;
}

int http_connection_new(int _FD, http_connection_t** _ConnectionPtr) {
    if (_ConnectionPtr == NULL)
        return -1;

    http_connection_t* _Connection = (http_connection_t*)malloc(sizeof(http_connection_t));
    if (_Connection == NULL)
        return -2;

    int result = http_connection_init(_Connection, _FD);
    if (result != 0) {
        free(_Connection);
        return result;
    }

    *(_ConnectionPtr) = _Connection;

    return 0;
}

// Sets the callback that will be called when a new request is received.
void http_connection_set_callback(http_connection_t* _Connection, void* _Context, http_connection_on_request_cb _OnRequest) {
    _Connection->context = _Context;
    _Connection->on_request = _OnRequest;
}

// This function is called by the smw_work function.
void http_connection_work(void* _Context, uint64_t _MonTime) {
    http_connection_t* _Connection = (http_connection_t*)_Context;

    // TODO: Implement HTTP parsing here.
    // 1. Read data from the tcp_client_t into a buffer.
    // 2. Parse the buffer as an HTTP request.
    // 3. If a full request is parsed:
    //    a. Populate the method and url fields in the http_connection_t struct.
    //    b. Call the on_request callback (weather_instance_on_request).
    //    c. Remove the parsed request from the buffer.
    // 4. If the buffer doesn't contain a full request yet, do nothing and wait for more data to arrive.

    printf("http_connection_work\n");

    // If there is a callback set, call it.
    if (_Connection->on_request) {
        _Connection->on_request(_Connection->context);
    }
}

void http_connection_dispose(http_connection_t* _Connection) {
    tcp_client_dispose(&_Connection->tcp_client);
    smw_destroy_task(_Connection->task);
}

void http_connection_free(http_connection_t** _ConnectionPtr) {
    if (_ConnectionPtr == NULL || *(_ConnectionPtr) == NULL)
        return;

    http_connection_dispose(*(_ConnectionPtr));
    free(*(_ConnectionPtr));
    *(_ConnectionPtr) = NULL;
}
