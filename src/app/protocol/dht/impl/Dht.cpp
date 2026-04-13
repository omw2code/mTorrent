
#include <openssl/sha.h>
#include "Dht.h"

DHT::DHT()
{
    generateUid();
}

void DHT::generateUid()
{
    SHA1()
}
