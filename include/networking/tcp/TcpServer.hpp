#ifndef ___TCP_SERVER_HPP__
#define ___TCP_SERVER_HPP__

namespace networking
{
namespace tcp 
{

class TcpServer
{
public:
    TcpServer();
    bool create();

private:
    void listen();
    void connect();
    void disconnect();
};

}; /// namespace tcp
}; /// namesapce networking
#endif