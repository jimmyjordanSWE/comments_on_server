#include "WeatherServer.h"
#include "smw.h"
#include "utils.h"
#include <stdio.h>

int main() {
    // Initialize the simple cooperative multitasking worker.
    smw_init();

    // Create a WeatherServer instance.
    WeatherServer server;
    // Initialize the WeatherServer. This will also initialize the underlying HTTPServer and TCPServer.
    WeatherServer_Initiate(&server);

    // Main application loop.
    while (1) {
        // smw_work will call the work function for all registered tasks.
        // In this case, it will call TCPServer_TaskWork and WeatherServer_TaskWork.
        smw_work(SystemMonotonicMS());
        sleep(1);
    }

    // Dispose the WeatherServer and the smw worker.
    WeatherServer_Dispose(&server);
    smw_dispose();
    return 0;
}
