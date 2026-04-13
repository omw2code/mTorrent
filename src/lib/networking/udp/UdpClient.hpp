#ifndef __UDP_HPP__
#define __UDP_HPP__

#include <cstdint>
#include <vector>

namespace network
{

class UDP
{
public:
    UDP();
private:

    void loadSettings();

private:

    /// 20 byte BitToreent protocol identifier
    const std::vector<uint8_t> peer_identifier_;

    /// 8 byte flag 
    std::vector<uint8_t> flags_;
};

}; /// namespace network

#endif