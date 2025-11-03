# Weather Server

This is a simple weather server application written in C. It provides a basic framework for a web server that can be extended to provide weather data.

## Architecture

The server is built on a custom-made, non-blocking, cooperative multitasking framework. The architecture is layered as follows:

- **Cooperative Multitasking (smw):** A simple cooperative multitasking worker that manages and executes tasks.
- **TCP Server (TCPServer):** A non-blocking TCP server that listens for incoming connections.
- **HTTP Server (HTTPServer):** Built on top of the TCP server, this layer handles HTTP connections.
- **Weather Server (WeatherServer):** The main application server that manages weather server instances.
- **Weather Server Instance (WeatherServerInstance):** Handles individual client connections, including request parsing and response generation.

For more details, please refer to the [DESIGN.md](DESIGN.md) file.

## Build Instructions

To build the server, you can use the provided Makefile.

```bash
make
```

This will compile the source code and create an executable named `weather_server` in the project's root directory.

## Usage

To run the server, execute the following command:

```bash
./weather_server
```

The server will start and listen for incoming connections on the default port.

## Dependencies

The project has no external dependencies. It uses a set of custom-built libraries located in the `libs` directory.

## Future Development

The server is currently in a basic state and can be extended with the following features:

- **Database Integration:** Implement a database to store and retrieve weather data.
- **API Implementation:** Define and implement a proper API for weather data retrieval.
- **Client Application:** Develop a client application to interact with the server.
