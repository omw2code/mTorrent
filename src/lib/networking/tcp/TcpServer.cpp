#include <iostream>
#include <cerrno>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <networking/tcp/TcpServer.hpp>

namespace networking::tcp
{

TcpServer::TcpServer()
{}

void TcpServer::listen()
{
    int listening = socket(AF_INET, SOCK_STREAM, 0);
    if (listening == -1)
    {
        /// Lets print some human readable error
        std::cerr << "Error creating socket: " << std::strerror(errno) << "\n";
    }

    /// Bind the socket to a port
    sockaddr_in hint;
    hint.sin_family = AF_INET;

}

};