#include <cstddef>
#include <algorithm>
#include <peer/HandshakeManager.hpp>

namespace bittorrent
{
namespace peer
{

void HandshakeManager::setProtocol(Handshake &&handshake_protocol)
{
    handshake_ = std::move(handshake_protocol);
}

HandshakeManager::ByteBuffer HandshakeManager::serialize()
{
    ByteBuffer buffer{};

    /// Start byte
    buffer.push_back(std::byte{handshake_.prefix_length});

    /// Bittorrent protocol
    std::transform(
        handshake_.protocol.begin(), 
        handshake_.protocol.end(), 
        std::back_inserter(buffer),
        [this](char byte) {
            return static_cast<std::byte>(byte);
        });
    
    /// Add reserved bytes
    buffer.append_range(handshake_.reserved);

    /// Append the hash
    if (handshake_.info_hash.has_value())
    {
        buffer.append_range(handshake_.info_hash.value());
    }
    else
    {
        throw std::runtime_error("Error: info hash is required for a handshake");
    }

    /// Append the id
    if (handshake_.id.has_value())
    {
        buffer.append_range(handshake_.id.value());
    }
    else
    {
        throw std::runtime_error("Error: current implementation calls for id to be set in handshake");
    }

    /// Elided
    return buffer;
}

HandshakeManager::Handshake HandshakeManager::deserialize(const std::span<const std::byte> &message)
{
    Handshake rx_handshake{};
    for (const auto byte : message)
    {
        assembleHandshake(byte, rx_handshake);
    }

    /// Elided
    return rx_handshake;
}

void HandshakeManager::assembleHandshake(const std::byte byte, Handshake &rx_handshake)
{
    switch (state_.seq)
    {
        using enum HandshakeManager::HandshakeSeq;

    case LEN_PREFIX:
    {
        if (byte != static_cast<std::byte>(LEN_PREFIX))
        {
            reset();
            throw std::runtime_error("Malformed header: prefix length");
        };
        rx_handshake.prefix_length = std::to_integer<int>(byte);
        state_.seq = PROTOCOL;
        break;
    }
    case PROTOCOL:
    {
        /// Build the protocol we are using
        rx_handshake.protocol.push_back(std::to_integer<char>(byte));

        /// Did we get all our data?
        if (rx_handshake.protocol.size() == rx_handshake.prefix_length &&
            rx_handshake.protocol == handshake_.protocol.data())
        {
            /// Next state
            state_.seq = PADDING;
        }
        else if (handshake_.protocol.size() > rx_handshake.prefix_length)
        {
            reset();
            throw std::runtime_error("Malformed packet: protocol length exceeded");
        }
        break;
    }
    case PADDING:
    {
        ++state_.count;

        /// Ensure we are checking custom protocols
        if (handshake_.reserved.size() < state_.count ||
            handshake_.reserved[state_.count] != byte)
        {
            reset();
            throw std::runtime_error("Malformed packet: padded byte protocol not supported");
        }
        else if (handshake_.reserved.size() == state_.count)
        {
            /// Move onto the next state
            state_.seq = INFO_HASH;
            state_.count = 0;
        }
        break;
    }
    case INFO_HASH:
    {
        ++state_.count;
        if (!rx_handshake.info_hash)
        {
            /// Init the optional
            rx_handshake.info_hash.emplace();
        }

        rx_handshake.info_hash->push_back(byte);

        if (!handshake_.info_hash.has_value())
        {
            reset();
            throw std::runtime_error("Info hash never set! Cannot compare an unknown info hash!");
        }
        else if(state_.count == handshake_.info_hash->size())
        {
            /// Move onto the next state
            state_.seq = ID;
            state_.count = 0;
        }
        break;
    }
    case ID:
    {
        ++state_.count;
        if (!handshake_.id)
        {
            /// Init the optional 
            handshake_.id.emplace();
        }

        /// For the current implementation, ID's are 20 bytes long
        if (state_.count < handshake_.id->size())
        {
            rx_handshake.id->push_back(byte);
        }
        else if (state_.count == handshake_.id->size())
        {
            /// reset the processing for now...
            reset();
        }
        break;
    }
    default:
        throw std::runtime_error("Uknown handshake assemble state");
    }
}

void HandshakeManager::reset()
{
    state_.seq = HandshakeSeq::LEN_PREFIX;
    state_.count = 0;
}
}; /// namsepace peer
}; /// namespace bittorrent
