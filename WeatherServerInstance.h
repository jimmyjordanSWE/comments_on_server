#ifndef __WeatherServerInstance_h_
#define __WeatherServerInstance_h_

#include "HTTPServer/HTTPServerConnection.h"
#include "smw.h"

// The weather_instance_t struct.
// It contains the http_connection_t that it is responsible for.
typedef struct {
    // The underlying http_connection_t.
    http_connection_t* connection;

    // TODO: Add a pointer to the Database object here.
} weather_instance_t;

// Initializes a weather_instance_t.
int weather_instance_init(weather_instance_t* _Instance, http_connection_t* _Connection);
// Initializes a weather_instance_t and allocates memory for it.
int weather_instance_new(http_connection_t* _Connection, weather_instance_t** _InstancePtr);

// This function is called by the WeatherServer's task work function.
void weather_instance_work(weather_instance_t* _Instance, uint64_t _MonTime);

// Disposes a weather_instance_t.
void weather_instance_dispose(weather_instance_t* _Instance);
// Disposes a weather_instance_t and frees the allocated memory.
void weather_instance_free(weather_instance_t** _InstancePtr);

#endif //__WeatherServerInstance_h_
