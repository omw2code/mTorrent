#include <Node.hpp>

namespace protocol
{
namespace dht
{

Node::Node()
    : uuid{}
    , routing_table_{}
{
    /// generate the unique id at random from 
    /// the same 160 bit space as BitTorrent infohashes
    uuid_ = generateUid();
}

uint8_t Node::generateUid()
{

}

} /// namespace dht
} /// namespace protocol
