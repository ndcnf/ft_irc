# ft_irc
First explaination of Server.
1. Server Class: This class represents the server and contains methods for socket creation, handling client connections, and processing commands.
2. Member Variables:
    - `_socket`: Holds the server's socket file descriptor.
    - `_validity`: Represents the validity of the server.
    - `_port`: The port number on which the server listens for incoming connections.
    - `_sockets`: A vector that stores the file descriptors of connected clients.
    - `_addr`: Represents the server's address structure.
    - `_pfds`: A vector of `pollfd` structures used for polling file descriptors.
    - `_clients`: A vector that stores instances of the `Client` class representing connected clients.
    - `_password`: Stores the server's password.
3. Member Functions:
    - `setPort()` and `getPort()`: Used to set and retrieve the server's port number.
    - `createSocket()`: Creates and configures the server socket.
    - `allSockets()`: Prints information about all connected sockets.
    - `getSocket()`: Returns the server's socket file descriptor.
    - `connection()`: Handles client connections and manages communication with clients.
    - `cmdSelection()`: Processes commands received from clients.
    - `inputClient()`: Handles input from clients and decides whether it's a command or text.
    - `addClient()`: Adds a new client to the server's list of connected clients.
4. Additional Functionality:
    - The `cmdSelection()` function seems to handle the processing of different command types.
    - There is a commented-out `capOrNOt()` function, which might have been intended to handle CAP LS command and negotiate capabilities with clients.
    - The `getCap()` function returns a vector of capabilities supported by the server.
