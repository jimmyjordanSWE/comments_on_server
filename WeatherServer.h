#ifndef __WeatherServer_h_
#define __WeatherServer_h_

#include "HTTPServer/HTTPServer.h"
#include "WeatherServerInstance.h"
#include "linked_list.h"
#include "smw.h"

// The main WeatherServer struct.
// It contains the HTTPServer, a list of WeatherServerInstances, and a task for the smw worker.
typedef struct {
    // The underlying HTTPServer.
    HTTPServer httpServer;
    // A list of all active WeatherServerInstances.
    LinkedList* instances;
    // The task that will be executed by the smw_work function.
    smw_task* task;

    // TODO: Add a Database object here.
} WeatherServer;

// Initializes a WeatherServer.
int WeatherServer_Initiate(WeatherServer* _Server);
// Initializes a WeatherServer and allocates memory for it.
int WeatherServer_InitiatePtr(WeatherServer** _ServerPtr);

// Disposes a WeatherServer.
void WeatherServer_Dispose(WeatherServer* _Server);
// Disposes a WeatherServer and frees the allocated memory.
void WeatherServer_DisposePtr(WeatherServer** _ServerPtr);

#endif //__WeatherServer_h_
