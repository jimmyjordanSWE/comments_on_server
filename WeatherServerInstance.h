#ifndef __WeatherServerInstance_h_
#define __WeatherServerInstance_h_

#include "HTTPServer/HTTPServerConnection.h"
#include "smw.h"

// The WeatherServerInstance struct.
// It contains the HTTPServerConnection that it is responsible for.
typedef struct {
    // The underlying HTTPServerConnection.
    HTTPServerConnection* connection;

    // TODO: Add a pointer to the Database object here.
} WeatherServerInstance;

// Initializes a WeatherServerInstance.
int WeatherServerInstance_Initiate(WeatherServerInstance* _Instance, HTTPServerConnection* _Connection);
// Initializes a WeatherServerInstance and allocates memory for it.
int WeatherServerInstance_InitiatePtr(HTTPServerConnection* _Connection, WeatherServerInstance** _InstancePtr);

// This function is called by the WeatherServer's task work function.
void WeatherServerInstance_Work(WeatherServerInstance* _Instance, uint64_t _MonTime);

// Disposes a WeatherServerInstance.
void WeatherServerInstance_Dispose(WeatherServerInstance* _Instance);
// Disposes a WeatherServerInstance and frees the allocated memory.
void WeatherServerInstance_DisposePtr(WeatherServerInstance** _InstancePtr);

#endif //__WeatherServerInstance_h_
