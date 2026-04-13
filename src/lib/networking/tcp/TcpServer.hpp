#ifndef ___TCP_SERVER_HPP__
#define ___TCP_SERVER_HPP__

class TcpServer
{
public:
    TcpServer();
    bool create();

private:
    void connect();
    void disconnect();
};