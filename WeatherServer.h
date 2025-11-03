#ifndef __WeatherServer_h_
#define __WeatherServer_h_

#include "HTTPServer/HTTPServer.h"
#include "WeatherServerInstance.h"
#include "linked_list.h"
#include "smw.h"

// The main weather_server_t struct.
// It contains the http_server_t, a list of weather_instance_t, and a task for the smw worker.
typedef struct {
    // The underlying http_server_t.
    http_server_t http_server;
    // A list of all active weather_instance_t.
    LinkedList* instances;
    // The task that will be executed by the smw_work function.
    smw_task_t* task;

    // TODO: Add a Database object here.
} weather_server_t;

// Initializes a weather_server_t.
int weather_server_init(weather_server_t* _Server);
// Initializes a weather_server_t and allocates memory for it.
int weather_server_new(weather_server_t** _ServerPtr);

// Disposes a weather_server_t.
void weather_server_dispose(weather_server_t* _Server);
// Disposes a weather_server_t and frees the allocated memory.
void weather_server_free(weather_server_t** _ServerPtr);

#endif //__WeatherServer_h_
