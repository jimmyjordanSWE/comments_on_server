# Software Design and Architecture

This document provides a detailed overview of the software design and architecture of the Weather Server application.

## 1. Core Components

The application is built around a set of core components that provide the basic functionality for a non-blocking, cooperative multitasking server.

### 1.1. Simple Cooperative Multitasking Worker (smw)

The `smw` component is a simple cooperative multitasking worker. It maintains a list of tasks and executes them in a round-robin fashion. The main application loop calls `smw_work` repeatedly to execute the tasks.

- **`smw_task`**: A struct representing a task to be executed. It contains a callback function and a context pointer.
- **`smw_init()`**: Initializes the `smw` worker.
- **`smw_createTask()`**: Creates a new task and adds it to the worker's task list.
- **`smw_work()`**: Executes all registered tasks.

### 1.2. TCP Server (TCPServer)

The `TCPServer` component provides a non-blocking TCP server. It listens for incoming connections on a specified port. When a new connection is accepted, it calls a registered callback function.

- **`TCPServer_Initiate()`**: Initializes the TCP server and starts listening for connections.
- **`TCPServer_OnAccept`**: A callback function that is called when a new connection is accepted.

The `TCPServer` uses the `smw` worker to create a task for handling new connections.

### 1.3. HTTP Server (HTTPServer)

The `HTTPServer` component is built on top of the `TCPServer`. It is responsible for managing HTTP connections. When a new TCP connection is established, the `HTTPServer` creates an `HTTPServerConnection` object and calls a registered callback function.

- **`HTTPServer_Initiate()`**: Initializes the HTTP server.
- **`HTTPServer_OnConnection`**: A callback function that is called when a new HTTP connection is established.

## 2. Application Layer

The application layer consists of the `WeatherServer` and `WeatherServerInstance` components.

### 2.1. Weather Server (WeatherServer)

The `WeatherServer` is the main application server. It initializes and manages the `HTTPServer`. It also maintains a list of active `WeatherServerInstance`s.

- **`WeatherServer_Initiate()`**: Initializes the `WeatherServer`, including the underlying `HTTPServer`.
- **`WeatherServer_OnHTTPConnection()`**: A callback function that is called by the `HTTPServer` when a new connection is established. This function creates a new `WeatherServerInstance` to handle the connection.
- **`WeatherServer_TaskWork()`**: The main task function for the `WeatherServer`. It iterates through the list of `WeatherServerInstance`s and calls their `Work` function.

### 2.2. Weather Server Instance (WeatherServerInstance)

The `WeatherServerInstance` is responsible for handling a single client connection. It receives HTTP requests, processes them, and sends back HTTP responses.

- **`WeatherServerInstance_Initiate()`**: Initializes a `WeatherServerInstance` with a given `HTTPServerConnection`.
- **`WeatherServerInstance_OnRequest()`**: A callback function that is called when a new HTTP request is received. This is where the main application logic for handling requests will be implemented.
- **`WeatherServerInstance_Work()`**: The task function for the `WeatherServerInstance`. This function is currently empty but can be used to perform background work related to the connection.

## 3. Data Flow

1.  The `main` function initializes the `smw` worker and the `WeatherServer`.
2.  The `WeatherServer` initializes the `HTTPServer`, which in turn initializes the `TCPServer` and starts listening for connections.
3.  When a client connects, the `TCPServer` accepts the connection and calls the `HTTPServer`'s `OnAccept` callback.
4.  The `HTTPServer` creates an `HTTPServerConnection` and calls the `WeatherServer`'s `OnHTTPConnection` callback.
5.  The `WeatherServer` creates a `WeatherServerInstance` to handle the connection and adds it to its list of instances.
6.  The `WeatherServerInstance` sets a callback on the `HTTPServerConnection` to be notified of incoming requests.
7.  When the client sends an HTTP request, the `WeatherServerInstance`'s `OnRequest` callback is called.
8.  The `OnRequest` function is responsible for parsing the request, interacting with the (future) database, and sending back an HTTP response.

## 4. Future Development

The current architecture provides a solid foundation for a web server. Future development will focus on the following areas:

- **Database Integration:** A database module will be added to store and retrieve weather data. The `WeatherServer` will be responsible for initializing the database, and a pointer to the database object will be passed to each `WeatherServerInstance`.
- **API Implementation:** A well-defined RESTful API will be implemented in the `WeatherServerInstance_OnRequest` function to handle different types of requests (e.g., getting weather data for a specific location).
- **Client Application:** A client application will be developed to interact with the server's API.
