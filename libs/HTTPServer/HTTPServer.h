#ifndef __HTTPServer_h_
#define __HTTPServer_h_

#include "../TCPServer.h"
#include "HTTPServerConnection.h"
#include "smw.h"

// Callback function that is called when a new connection is accepted.
// The context is the context that was passed to the http_server_init function.
// The connection is the new connection that was accepted.
typedef int (*http_server_on_connection_cb)(void* _Context, http_connection_t* _Connection);

// The http_server_t struct.
// It contains the tcp_server_t, a callback for new connections, and a task for the smw worker.
typedef struct {
    // The context that will be passed to the on_connection callback.
    void* context;
    // The callback that will be called when a new connection is accepted.
    http_server_on_connection_cb on_connection;
    // The underlying tcp_server_t.
    tcp_server_t tcp_server;
    // The task that will be executed by the smw_work function.
    smw_task_t* task;
} http_server_t;

// Initializes a http_server_t.
int http_server_init(http_server_t* _Server, void* _Context, http_server_on_connection_cb _OnConnection);
// Initializes a http_server_t and allocates memory for it.
int http_server_new(void* _Context, http_server_on_connection_cb _OnConnection, http_server_t** _ServerPtr);

// Disposes a http_server_t.
void http_server_dispose(http_server_t* _Server);
// Disposes a http_server_t and frees the allocated memory.
void http_server_free(http_server_t** _ServerPtr);

#endif //__HTTPServer_h_
