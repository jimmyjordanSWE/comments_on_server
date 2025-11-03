#include "WeatherServer.h"
#include <stdio.h>
#include <stdlib.h>

//-----------------Callback Functions-----------------
// This function is called by the http_server_t when a new connection is accepted.
int weather_server_on_http_connection(void* _Context, http_connection_t* _Connection) {
    printf("ENTRY weather_server_on_http_connection callback ... ");
    // The context is the weather_server_t itself.
    weather_server_t* _Server = (weather_server_t*)_Context;
    weather_instance_t* instance = NULL;

    // TODO: Pass the Database object to the weather_instance_t here.
    // Create a new weather_instance_t to handle the connection.
    int result = weather_instance_new(_Connection, &instance);
    if (result != 0) {
        printf("weather_server_on_http_connection: Failed to initiate instance\n");
        return -1;
    }

    // Add the new instance to the list of instances.
    LinkedList_append(_Server->instances, instance);

    printf("EXITING.\n");
    return 0;
}
//----------------------------------------------------

//-----------------Internal Functions-----------------
void weather_server_work(void* _Context, uint64_t _MonTime);
int weather_server_on_http_connection(void* _Context, http_connection_t* _Connection);
//----------------------------------------------------

int weather_server_init(weather_server_t* _Server) {
    printf("ENTRY: Creating server ... ");
    // TODO: Initialize the Database object here.

    // Initialize the http_server_t and set the callback for new connections.
    // The weather_server_t itself is passed as the context.
    http_server_init(&_Server->http_server, _Server, weather_server_on_http_connection);
    // Create a linked list to store the weather_instance_t.
    _Server->instances = LinkedList_create();
    // Create a task for the smw worker.
    // The weather_server_t itself is passed as the context.
    _Server->task = smw_create_task(_Server, weather_server_work);
    printf("EXIT: Creating server.");
    return 0;
}

int weather_server_new(weather_server_t** _ServerPtr) {
    if (_ServerPtr == NULL)
        return -1;

    weather_server_t* _Server = (weather_server_t*)malloc(sizeof(weather_server_t));
    if (_Server == NULL)
        return -2;

    int result = weather_server_init(_Server);
    if (result != 0) {
        free(_Server);
        return result;
    }

    *(_ServerPtr) = _Server;

    return 0;
}

// This function is called by the smw_work function.
void weather_server_work(void* _Context, uint64_t _MonTime) {
    // The context is the weather_server_t itself.
    weather_server_t* _Server = (weather_server_t*)_Context;

    // Iterate over all weather_instance_t and call their Work function.
    LinkedList_foreach(_Server->instances, node) {
        weather_instance_t* instance = (weather_instance_t*)node->item;
        weather_instance_work(instance, _MonTime);
    }
}

void weather_server_dispose(weather_server_t* _Server) {
    // TODO: Dispose of the Database object here.
    http_server_dispose(&_Server->http_server);
    smw_destroy_task(_Server->task);
}

void weather_server_free(weather_server_t** _ServerPtr) {
    if (_ServerPtr == NULL || *(_ServerPtr) == NULL)
        return;

    weather_server_dispose(*(_ServerPtr));
    free(*(_ServerPtr));
    *(_ServerPtr) = NULL;
}
