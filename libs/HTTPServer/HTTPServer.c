#include "HTTPServer.h"
#include <stdio.h>
#include <stdlib.h>
//-----------------Callback Functions-----------------
// This function is called by the tcp_server_t when a new connection is accepted.
int http_server_on_accept(int _FD, void* _Context) {
    // The context is the http_server_t itself.
    http_server_t* _Server = (http_server_t*)_Context;
    http_connection_t* connection = NULL;
    // Create a new http_connection_t to handle the connection.
    int result = http_connection_new(_FD, &connection);
    if (result != 0) {
        printf("http_server_on_accept: Failed to initiate connection\n");
        return -1;
    }
    // Call the on_connection callback to notify the user of the new connection.
    _Server->on_connection(_Server->context, connection);
    return 0;
}
//----------------------------------------------------

//-----------------Internal Functions-----------------
void http_server_work(void* _Context, uint64_t _MonTime);
int http_server_on_accept(int _FD, void* _Context);
//----------------------------------------------------

int http_server_init(http_server_t* _Server, void* _Context, http_server_on_connection_cb _OnConnection) {
    printf("ENTRY http_server_init ... ");
    _Server->context = _Context;
    _Server->on_connection = _OnConnection;
    // Initialize the tcp_server_t and set the callback for new connections.
    // The http_server_t itself is passed as the context.
    tcp_server_init(&_Server->tcp_server, "8080", http_server_on_accept, _Server);
    // Create a task for the smw worker.
    _Server->task = smw_create_task(_Server, http_server_work);
    printf("EXIT http_server_init\n");
    return 0;
}

int http_server_new(void* _Context, http_server_on_connection_cb _OnConnection, http_server_t** _ServerPtr) {
    if (_ServerPtr == NULL)
        return -1;

    http_server_t* _Server = (http_server_t*)malloc(sizeof(http_server_t));
    if (_Server == NULL)
        return -2;

    int result = http_server_init(_Server, _Context, _OnConnection);
    if (result != 0) {
        free(_Server);
        return result;
    }

    *(_ServerPtr) = _Server;

    return 0;
}

// This function is called by the smw_work function.
void http_server_work(void* _Context, uint64_t _MonTime) {
    // The context is the http_server_t itself.

    // http_server_t* _Server = (http_server_t*)_Context;
}

void http_server_dispose(http_server_t* _Server) {
    tcp_server_dispose(&_Server->tcp_server);
    smw_destroy_task(_Server->task);
}

void http_server_free(http_server_t** _ServerPtr) {
    if (_ServerPtr == NULL || *(_ServerPtr) == NULL)
        return;

    http_server_dispose(*(_ServerPtr));
    free(*(_ServerPtr));
    *(_ServerPtr) = NULL;
}
