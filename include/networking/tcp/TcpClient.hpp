#ifndef __TCP_CLIENT_HPP__
#define __TCP_CLIENT_HPP__

#include <net/if_arp.h>
#include <sys/socket.h>
#include <string>


namespace networking
{
namespace tcp
{

class TcpClient
{
public:
    TcpClient(const std::string &filename);
    bool create();
private:
    void init();
    void connect();
    void disconnect();
private:
    int socket_;
    sockaddr_in hint_;
    socklen_t size_;
    std::string config_;
};

}; /// namespace tcp
}; /// namespace networking

#endif