#ifndef _ROUTING_TABLE_HPP_
#define _ROUTING_TABLE_HPP_

#include <cstdint>

namespace protocol
{
namespace dht
{

class RoutingTable
{
public: 
    RoutingTable();
private:
    struct Contactinfo
    {
        uint8_t id;
        uint8_t distrance;
    };
};


} /// Namespace dht
} /// Namespace protocol
#endif

