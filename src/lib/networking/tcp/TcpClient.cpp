#include <sys/socket.h>
#include <iostream>
// #include <json_parser>
#include "TcpClient.hpp"

namespace networking
{
namespace tcp
{

TcpClient::TcpClient(const std::string_view &filename)
    : socket_fd_{}
{
    openConfig(filename);
}

bool TcpClient::openConfig(const std::string_view &filename)
{

}

bool TcpClient::create()
{
    /// Create the client
    socket_fd_ = socket(AF_INET, SOCK_STREAM, 0);

    if (socket_fd_ == -1)
    {
        std::cerr << "Error creating tcp socket: " << errno << "\n";
    }

    return true;
}

}; /// namespace tcp
}; /// namespace networking