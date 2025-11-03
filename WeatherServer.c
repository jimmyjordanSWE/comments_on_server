#include "WeatherServer.h"
#include <stdio.h>
#include <stdlib.h>

//-----------------Callback Functions-----------------
// This function is called by the HTTPServer when a new connection is accepted.
int WeatherServer_OnHTTPConnection(void* _Context, HTTPServerConnection* _Connection) {
    printf("ENTRY WeatherServer_OnHTTPConnection callback ... ");
    // The context is the WeatherServer itself.
    WeatherServer* _Server = (WeatherServer*)_Context;
    WeatherServerInstance* instance = NULL;

    // TODO: Pass the Database object to the WeatherServerInstance here.
    // Create a new WeatherServerInstance to handle the connection.
    int result = WeatherServerInstance_InitiatePtr(_Connection, &instance);
    if (result != 0) {
        printf("WeatherServer_OnHTTPConnection: Failed to initiate instance\n");
        return -1;
    }

    // Add the new instance to the list of instances.
    LinkedList_append(_Server->instances, instance);

    printf("EXITING.\n");
    return 0;
}
//----------------------------------------------------

//-----------------Internal Functions-----------------
void WeatherServer_TaskWork(void* _Context, uint64_t _MonTime);
int WeatherServer_OnHTTPConnection(void* _Context, HTTPServerConnection* _Connection);
//----------------------------------------------------

int WeatherServer_Initiate(WeatherServer* _Server) {
    printf("ENTRY: Creating server ... ");
    // TODO: Initialize the Database object here.

    // Initialize the HTTPServer and set the callback for new connections.
    // The WeatherServer itself is passed as the context.
    HTTPServer_Initiate(&_Server->httpServer, _Server, WeatherServer_OnHTTPConnection);
    // Create a linked list to store the WeatherServerInstances.
    _Server->instances = LinkedList_create();
    // Create a task for the smw worker.
    // The WeatherServer itself is passed as the context.
    _Server->task = smw_createTask(_Server, WeatherServer_TaskWork);
    printf("EXIT: Creating server.");
    return 0;
}

int WeatherServer_InitiatePtr(WeatherServer** _ServerPtr) {
    if (_ServerPtr == NULL)
        return -1;

    WeatherServer* _Server = (WeatherServer*)malloc(sizeof(WeatherServer));
    if (_Server == NULL)
        return -2;

    int result = WeatherServer_Initiate(_Server);
    if (result != 0) {
        free(_Server);
        return result;
    }

    *(_ServerPtr) = _Server;

    return 0;
}

// This function is called by the smw_work function.
void WeatherServer_TaskWork(void* _Context, uint64_t _MonTime) {
    // The context is the WeatherServer itself.
    WeatherServer* _Server = (WeatherServer*)_Context;

    // Iterate over all WeatherServerInstances and call their Work function.
    LinkedList_foreach(_Server->instances, node) {
        WeatherServerInstance* instance = (WeatherServerInstance*)node->item;
        WeatherServerInstance_Work(instance, _MonTime);
    }
}

void WeatherServer_Dispose(WeatherServer* _Server) {
    // TODO: Dispose of the Database object here.
    HTTPServer_Dispose(&_Server->httpServer);
    smw_destroyTask(_Server->task);
}

void WeatherServer_DisposePtr(WeatherServer** _ServerPtr) {
    if (_ServerPtr == NULL || *(_ServerPtr) == NULL)
        return;

    WeatherServer_Dispose(*(_ServerPtr));
    free(*(_ServerPtr));
    *(_ServerPtr) = NULL;
}
