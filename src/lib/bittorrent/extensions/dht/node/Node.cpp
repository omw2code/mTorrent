#include <string>
#include <openssl/sha.h>
#include <protocol/dht/node/Node.hpp>

namespace protocol
{
namespace dht
{

Node::Node()
    : uuid_{}
    , routing_table_{}
{
    /// generate the unique id at random from 
    /// the same 160 bit space as BitTorrent infohashes
    uuid_ = generateUid();
}

uint8_t Node::generateUid()
{
    // return SHA256()
}

} /// namespace dht
} /// namespace protocol
