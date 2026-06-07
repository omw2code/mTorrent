#ifndef __HANDSHAKE_HPP__
#define __HANDSHAKE_HPP__

#include <span>
#include <vector>

namespace bittorrent
{

class HandshakeManager()
{
public:
    
    using ByteBuffer = std::vector<const std::byte>;
    struct Handshake
    {
        ShaHash info_hash{};
        PeerId id{};
    };

    ByteBuffer serialize(const Sync &message);

    Sync deserialize(const std::span<const std::byte> &message);

private:

    const std::byte LEN_PREFIX{'\x13'};

    constexpr std::string_view protocol_{"BitTorrent protocol"};
    
    const std::vector<const std::byte> reserved_('\x00', 8);
      
}; /// class Handshake
}; /// namespace bittorrent 
#endif
