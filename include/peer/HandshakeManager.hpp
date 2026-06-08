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
        uint8_t prefix_length{19};
        std::string protocol{"BitTorrent protocol"};
        std::vector<const std::byte> reserved('\0x00', 8);
        ShaHash info_hash{};
        PeerId id{};
    };

    ByteBuffer serialize(const Handshake &protocol);

    Sync deserialize(const std::span<const std::byte> &message);

    void setProtocol(Handshake &&handshake);

private:

    enum class HandshakeSeq
    {
        LEN_PREFIX,
        PROTOCOL,
        PADDING,
        INFO_HASH,
        ID,
    };

    struct State
    {
        /// Current state
        HandshakeSeq seq{HandshakeSeq::LEN_PREFIX};

        /// Current helper count agnostic to a state
        uint8_t count{};
    };

    State state_{};

    Handshake handshake_protocol_{};
      
}; /// class Handshake
}; /// namespace bittorrent 
#endif
