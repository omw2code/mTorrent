#ifndef __TCP_CLIENT_HPP__
#define __TCP_CLIENT_HPP__

#include <string>

namespace networking
{
namespace tcp
{

class TcpClient
{
public:
    TcpClient(const std::string_view &filename);
    bool create();
private:
    bool openConfig(const std::string_view &filename);
    void connect();
    void disconnect();
private:
    int socket_fd_;
};

}; /// namespace tcp
}; /// namespace networking

#endif