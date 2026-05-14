#include <vector>
#include <iostream>
#include <functional>
#include <algorithm>
#include <networking/tcp/TcpClient.hpp>
#include <networking/tcp/TcpServer.hpp>

class ServerMock
{
public:
    template<typename T>
    using Callback = std::function<void(T)>;
    const std::string settings{
        
    };

    ServerMock()
        : cb{}
        , tcp_server_{settings}
    {};

    void setOnData(Callback<const std::string&> &&cb);

private:
    Callback<const std::string&> cb;
    networking::tcp::TcpServer tcp_server_;
};

int main(int argc, char* argv[])
{
    std::
    /// Parse the args
    std::vector<std::string> args(argv + 1, argv + argc);
    auto it = std::find_if(args.begin(), args.end(),
        [&](const std::string &arg){
            return (arg == "server");
        });
    
    if (it == args.end())
    {
        /// Just run the client as the default with no args

    }
    else
    {
        /// Run the server instead
        auto server = 
    }

}