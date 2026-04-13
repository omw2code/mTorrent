#ifndef _NODE_HPP_
#define _NODE_HPP_

#include <stack>
#include <unordered_map>

namespace protocol
{
namespace dht
{

/// A client/server listening on a UDP port implementing
/// the distributed hash table protocol
class Node
{
public:
    Node();

private:

    uint8_t generateUid();

private:
    struct ContactInfo
    {
        uint8_t id;
        uint8_t distance;
    };


    /// Routing table containing the contact info for a small
    /// number of other nodes
    std::unordered_map<uint8_t, ContactInfo> routing_table_;

    /// Nodes know about many other nodes that have IDs that
    /// are close
    std::stack<ContactInfo> closest_;

    /// Nodes know about a handful of contacts with IDs
    /// that are far away
    std::stack<ContactInfo> far_;
    /// The unique id of this 
    uint8_t uuid_;
};

} /// namespace dht
} /// namespace protocol

#endif
