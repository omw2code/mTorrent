#include <networking/udp/UdpClient.hpp>

namespace network
{
namespace udp
{

UDP::UDP()
    : peer_identifier_{
        0x13, 
        0x42,
        0x69,
        0x74,
        0x54,
        0x6f,
        0x72,
        0x72,
        0x65, 
        0x6e, 
        0x74, 
        0x20,
        0x50, 
        0x72, 
        0x6f, 
        0x74, 
        0x6f, 
        0x63, 
        0x6f, 
        0x6c}
    , flags_{0x00, 8}
{}

void UDP::loadSettings()
{
    /// TODO: Load some json settings for the initial client setup
}


}; /// namespace udp
}; /// namespace network