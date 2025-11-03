#include "WeatherServerInstance.h"
#include <stdlib.h>

//-----------------Internal Functions-----------------
// This function is called by the http_connection_t when a new request is received.
int weather_instance_on_request(void* _Context);
//----------------------------------------------------

int weather_instance_init(weather_instance_t* _Instance, http_connection_t* _Connection) {
    _Instance->connection = _Connection;
    // Set the callback for new requests.
    // The weather_instance_t itself is passed as the context.
    http_connection_set_callback(_Instance->connection, _Instance, weather_instance_on_request);
    return 0;
}

int weather_instance_new(http_connection_t* _Connection, weather_instance_t** _InstancePtr) {
    if (_InstancePtr == NULL)
        return -1;

    weather_instance_t* _Instance = (weather_instance_t*)malloc(sizeof(weather_instance_t));
    if (_Instance == NULL)
        return -2;

    int result = weather_instance_init(_Instance, _Connection);
    if (result != 0) {
        free(_Instance);
        return result;
    }

    *(_InstancePtr) = _Instance;

    return 0;
}

// This function is called by the http_connection_t when a new request is received.
int weather_instance_on_request(void* _Context) {
    // TODO: Implement request handling here.
    // 1. Get the http_connection_t from the _Context.
    // 2. Get the method and url from the http_connection_t.
    // 3. Based on the method and url, decide whether to read from or write to the database.
    // 4. Call the appropriate function in the Database module.
    // 5. Generate an HTTP response based on the result from the database.
    // 6. Use tcp_client_write to send the response back to the client.
    printf("weather_instance_on_request\n");
    return 0;
}

// This function is called by the WeatherServer's task work function.
// It is currently empty, but this is where the logic for handling the connection would go.
void weather_instance_work(weather_instance_t* _Server, uint64_t _MonTime) {
}

void weather_instance_dispose(weather_instance_t* _Instance) {
}

void weather_instance_free(weather_instance_t** _InstancePtr) {
    if (_InstancePtr == NULL || *(_InstancePtr) == NULL)
        return;

    weather_instance_dispose(*(_InstancePtr));
    free(*(_InstancePtr));
    *(_InstancePtr) = NULL;
}
