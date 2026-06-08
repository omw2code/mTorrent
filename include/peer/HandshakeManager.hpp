#ifndef __HANDSHAKE_HPP__
#define __HANDSHAKE_HPP__

#include <span>
#include <optional>
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
        std::optional<ShaHash> info_hash{};
        std::optional<PeerId> id{};
    };

    /*
    * \brief Serialize the handshake set via setProtocol
    *        or defaulted to version 1 of the bittorrent protocol
    *
    * \return The full encoded handshake
    */
    ByteBuffer serialize();

    /*
    * \brief Deserialize the handshake received from another peer
    *
    * \return The deserialized handshake, throws if handshake
    *         is malformed
    */
    Handshake deserialize(const std::span<const std::byte> &message);

    /*
    *
    * \brief Sets the handshake to use when joining a swarm
    *
    */
    void setProtocol(Handshake &&handshake);

private:

    /*
    *
    * \brief Assemble the handshake recieved from a peer
    *
    */
    void assembleHandshake(const std::byte byte, Handshake &rx_handshake);

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

    /// Internally stored state information
    State state_{};

    /// Handshake protocol to use
    Handshake handshake_protocol_{};
      
}; /// class Handshake
}; /// namespace bittorrent 
#endif
