#ifndef _DHT_HPP_
#define _DHT_HPP_

#include <map>
#include <Node.hpp>

namespace protocol
{

/// Data structure used for storing peer contact information 
/// for trackerless torrents, based on Kademila and implemented
/// over UDP.
/// The DHT is composed of nodes and stores the location of peers.
class DHT
{
public: 
    DHT();

private:
    
    void generateUid();

private:

    struct NodeInfo
    {
        Node node;
        std::string uid;
    };        

};

} /// namespace protocol
#endif
