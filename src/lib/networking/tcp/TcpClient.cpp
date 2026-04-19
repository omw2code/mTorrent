#include <stdio.h>
#include <cerrno>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <networking/tcp/TcpClient.hpp>
#include <json_parser/JsonParser.hpp>

namespace networking
{
namespace tcp
{

TcpClient::TcpClient(const std::string &filename)
    : socket_{}
    , hint_{}
    , size_{}
    , config_{filename}
{
    init();
}

void TcpClient::init()
{
    /// Json helper
    JsonParser parser{config_};
    auto settings = parser.getJsonArray<std::string, int>("BitTorrent/tcp/client");

    /// Grab initial networking configuration
    auto addr_it = settings.find("ip");
    if (addr_it == settings.end())
    {
        throw std::runtime_error("Error: No network address configured");
    }
    
    auto port_it = settings.find("port");
    if (port_it == settings.end())
    {
        throw std::runtime_error("Error: No network port configured");
    }
    
    /// Let the operating system decide
    int sin_port{};
    if (std::strcmp(std::get<std::string>(port_it->second).c_str(), "any") == 0) 
    {
        /// TODO: this may be implementation defined
        sin_port = 0;
    }
    else
    {
        sin_port = std::stoi(std::get<std::string>(port_it->second));
    }
        
    /// IPv4 only supported as of 4/19/26
    bzero(&hint_, sizeof(hint_));
    hint_.sin_family = AF_INET;
    hint_.sin_port = htons(sin_port);

    /// Let the operating system decide
    if (std::strcmp(std::get<std::string>(addr_it->second).c_str(), "any") == 0)
    {
        hint_.sin_addr.s_addr = htons(INADDR_ANY);
    }
    else if ((inet_pton(AF_INET, std::get<std::string>(addr_it->second).c_str(), &hint_.sin_addr)) == 0)
    {
        throw std::runtime_error("Error converting address to network format");
    }
}

bool TcpClient::create()
{
    /// Create the client
    socket_ = socket(AF_INET, SOCK_STREAM, 0);

    if (socket_ == -1)
    {
        /// Lets print some human readable error
        std::string error{"Error creating socket: "};
        error.append(std::strerror(errno));
        throw std::runtime_error(error);
    }

    return true;
}

}; /// namespace tcp
}; /// namespace networking