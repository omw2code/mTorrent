#ifndef __HANDSHAKE_HPP__
#define __HANDSHAKE_HPP__

#include <span>
#include <optional>
#include <vector>

namespace bittorrent
{
namespace peer
{

class HandshakeManager
{
public:
    
    using ByteBuffer = std::vector<std::byte>;
    using PeerId = std::array<std::byte, 20>;
    using ShaHash = std::array<std::byte, 20>;
    struct Handshake
    {
        uint8_t prefix_length;
        std::string protocol;
        std::vector<std::byte> reserved;
        std::optional<ShaHash> info_hash;
        std::optional<PeerId> id;
        
        Handshake()
            : prefix_length{19}
            , protocol{"BitTorrent protocol"}
            , reserved(8, static_cast<std::byte>('\0x00'))
            , info_hash{}
            , id{}
        {}

        void reset()
        {
            prefix_length = 0;
            protocol = "";
            reserved.clear();
            info_hash.reset();
            id.reset();
        }
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

    /*
    * TODO:
    */
    void setEncoder();

private:

    /*
    *
    * \brief Assemble the handshake recieved from a peer
    *
    */
    void assembleHandshake(const std::byte byte, Handshake &rx_handshake);

    /*
    * \brief Resets the handshake assembly
    */
    void reset();

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
    Handshake handshake_{};
      
}; /// class Handshake
}; /// namesapce peer
}; /// namespace bittorrent 
#endif
