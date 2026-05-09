#ifndef ___TCP_SERVER_HPP__
#define ___TCP_SERVER_HPP__

#include <sys/socket.h>
#include <string>

namespace networking
{
namespace tcp 
{

class TcpServer
{
public:
    TcpServer(const std::string& filename);
    bool crweate();

private:
    void init();
    void serverStart();
    void listen();
    void connect();
    void disconnect();
private:
    sockaddr_in hint_;
    int socket_;
    std::string config_;

};

}; /// namespace tcp
}; /// namesapce networking
#endif