#ifndef __HTTPServerConnection_h_
#define __HTTPServerConnection_h_

#include "../TCPClient.h"
#include "smw.h"

// Callback function that is called when a new request is received.
// The context is the context that was passed to the http_connection_set_callback function.
typedef int (*http_connection_on_request_cb)(void* _Context);

// The http_connection_t struct.
// It contains the tcp_client_t, a callback for new requests, and a task for the smw worker.
typedef struct {
    // The underlying tcp_client_t.
    tcp_client_t tcp_client;
    // The context that will be passed to the on_request callback.
    void* context;
    // The callback that will be called when a new request is received.
    http_connection_on_request_cb on_request;
    // The HTTP method of the request.
    char* method;
    // The URL of the request.
    char* url;
    // The task that will be executed by the smw_work function.
    smw_task_t* task;
} http_connection_t;

// Initializes a http_connection_t.
int http_connection_init(http_connection_t* _Connection, int _FD);
// Initializes a http_connection_t and allocates memory for it.
int http_connection_new(int _FD, http_connection_t** _ConnectionPtr);

// Sets the callback that will be called when a new request is received.
void http_connection_set_callback(http_connection_t* _Connection, void* _Context, http_connection_on_request_cb _OnRequest);

// Disposes a http_connection_t.
void http_connection_dispose(http_connection_t* _Connection);
// Disposes a http_connection_t and frees the allocated memory.
void http_connection_free(http_connection_t** _ConnectionPtr);

#endif //__HTTPServerConnection_h_
