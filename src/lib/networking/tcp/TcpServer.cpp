#include <stdio.h>
#include <cerrno>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include <networking/tcp/TcpServer.hpp>
#include <json_parser/JsonParser.hpp>

namespace networking::tcp
{

TcpServer::TcpServer(const std::string& filename)
    : hint_{}
    , socket_{}
    , config_{filename}
{
    init();
}

void TcpServer::init()
{
    /// Json helper
    JsonParser parser{config_};
    auto settings = parser.getJsonArray<std::string, int>("BitTorrent/tcp/server");

    /// Grab initial networking configuration
    auto addr_it = settings.find("ip");
    if (addr_it == settings.end())
    {
        throw std::runtime_error("Error: No network address configured");
    }

    /// Make sure any isn't configured
    if (std::strcmp(std::get<std::string>(addr_it->second).c_str(), "any") == 0)
    {
        throw std::runtime_error("Error, \"any\" address not supported for TCP Server");
    }

    auto port_it = settings.find("port");
    if (port_it == settings.end())
    {
        throw std::runtime_error("Error: No network port configured");
    }

    /// Make sure any isn't configured
    if (std::strcmp(std::get<std::string>(port_it->second).c_str(), "any") == 0)
    {
        throw std::runtime_error("Error, \"any\" port not supported for TCP Server");
    }

    /// Grab the port
    int sin_port = std::stoi(std::get<std::string>(port_it->second));
    
    /// IPv4 only supported as of 4/19/26
    ::bzero(&hint_, sizeof(hint_));
    hint_.sin_family = AF_INET;
    hint_.sin_port = htons(sin_port);

    if ((::inet_pton(AF_INET, std::get<std::string>(addr_it->second).c_str(), &hint_.sin_addr)) == 0)
    {
        throw std::runtime_error("Error converting address to network format");
    }

    /// Init a fd
    socket_ = ::socket(AF_INET, SOCK_STREAM, 0);
    if (socket_ == -1)
    {
        /// Lets print some human readable error
        std::string error{"Error creating socket: "};
        error.append(std::strerror(errno));
        throw std::runtime_error(error);
    }

    /// Allow program to run again even if there are old connections in TIME_WAIT
    constexpr int n{1};
    if (::setsockopt(socket_, SOL_SOCKET, SO_REUSEADDR, (char*)&n, sizeof(n)) < 0)
    {
        std::string error{"Error reusing address: "};
        error.append(std::strerror(errno));
        ::close(socket_);
        throw std::runtime_error(error);
    }

    /// Enable keepalives to make sockets timeout if server goes away
    if (setsockopt(socket_, SOL_SOCKET, SO_KEEPALIVE, (char*)&n, sizeof(n)) < 0)
    {
        std::string error{"Error setting keep alive: "};
        error.append(std::strerror(errno));
        ::close(socket_);
        throw std::runtime_error(error);
    }

    /// Set the close on exec bit
    if (::fcntl(socket_, F_SETFD, n) < 0)
    {
        std::string error{"Error enabling close-exec-bit on the fd: "};
        error.append(std::strerror(errno));
        ::close(socket_);
        throw std::runtime_error(error);
    }

    /// Set non-blocking mode
    if (::fcntl(socket_, F_SETFD, n | O_NONBLOCK) < 0)
    {
        std::string error{"Error setting non-blocking bits: "};
        error.append(std::strerror(errno));
        ::close(socket_);
        throw std::runtime_error(error);
    }
}

void TcpServer::serverStart()
{
    /// Bind the socket to an ip and port
    if (::bind(socket_,(sockaddr*)&hint_, sizeof(hint_)) == -1)
    {
        std::string error{"Error binding to socket"};
        error.append(std::strerror(errno));
        ::close(socket_);
        throw std::runtime_error(error);
    }

    /// listen on a socket with a max connections
    if (::listen(socket_, SOMAXCONN) == -1)
    {
        std::string error{"Error unable to listen on the socket"};
        error.append(std::strerror(errno));
        ::close(socket_);
        throw std::runtime_error(error);
    }
}

};