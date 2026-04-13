
#include <openssl/sha.h>
#include "Dht.hpp"

namespace protocol
{

DHT::DHT()
{
    generateUid();
}

void DHT::generateUid()
{
    SHA1()
}

}; /// namespace protocol