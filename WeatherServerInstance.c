#include "WeatherServerInstance.h"
#include <stdlib.h>

//-----------------Internal Functions-----------------
// This function is called by the HTTPServerConnection when a new request is received.
int WeatherServerInstance_OnRequest(void* _Context);
//----------------------------------------------------

int WeatherServerInstance_Initiate(WeatherServerInstance* _Instance, HTTPServerConnection* _Connection) {
    _Instance->connection = _Connection;
    // Set the callback for new requests.
    // The WeatherServerInstance itself is passed as the context.
    HTTPServerConnection_SetCallback(_Instance->connection, _Instance, WeatherServerInstance_OnRequest);
    return 0;
}

int WeatherServerInstance_InitiatePtr(HTTPServerConnection* _Connection, WeatherServerInstance** _InstancePtr) {
    if (_InstancePtr == NULL)
        return -1;

    WeatherServerInstance* _Instance = (WeatherServerInstance*)malloc(sizeof(WeatherServerInstance));
    if (_Instance == NULL)
        return -2;

    int result = WeatherServerInstance_Initiate(_Instance, _Connection);
    if (result != 0) {
        free(_Instance);
        return result;
    }

    *(_InstancePtr) = _Instance;

    return 0;
}

// This function is called by the HTTPServerConnection when a new request is received.
int WeatherServerInstance_OnRequest(void* _Context) {
    // TODO: Implement request handling here.
    // 1. Get the HTTPServerConnection from the _Context.
    // 2. Get the method and url from the HTTPServerConnection.
    // 3. Based on the method and url, decide whether to read from or write to the database.
    // 4. Call the appropriate function in the Database module.
    // 5. Generate an HTTP response based on the result from the database.
    // 6. Use TCPClient_Write to send the response back to the client.
    printf("WeatherServerInstance_OnRequest\n");
    return 0;
}

// This function is called by the WeatherServer's task work function.
// It is currently empty, but this is where the logic for handling the connection would go.
void WeatherServerInstance_Work(WeatherServerInstance* _Server, uint64_t _MonTime) {
}

void WeatherServerInstance_Dispose(WeatherServerInstance* _Instance) {
}

void WeatherServerInstance_DisposePtr(WeatherServerInstance** _InstancePtr) {
    if (_InstancePtr == NULL || *(_InstancePtr) == NULL)
        return;

    WeatherServerInstance_Dispose(*(_InstancePtr));
    free(*(_InstancePtr));
    *(_InstancePtr) = NULL;
}
