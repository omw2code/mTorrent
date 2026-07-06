#include <cstddef>
#include <algorithm>
#include <peer/HandshakeManager.hpp>
#include <iostream>

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
    /// Build up an empty handshake
    Handshake rx_handshake;
    rx_handshake.reset();

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
        if (byte != static_cast<std::byte>(handshake_.prefix_length))
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
            rx_handshake.protocol == handshake_.protocol)
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
        rx_handshake.reserved.push_back(byte);

        /// Ensure we are checking custom protocols
        if (handshake_.reserved[state_.count] != byte)
        {
            reset();
            throw std::runtime_error("Malformed packet: invalid padded byte");
        }
        else if (handshake_.reserved.size() < rx_handshake.reserved.size())
        {
            reset();
            throw std::runtime_error("Malformed packet: padded byte length exceeded");
        }
        else if (rx_handshake.reserved.size() == handshake_.reserved.size())
        {
            /// Move onto the next state
            state_.seq = INFO_HASH;
            state_.count = 0;
            break;
        }

        ++state_.count;
        break;
    }
    case INFO_HASH:
    {
        if (!rx_handshake.info_hash)
        {
            /// Init the optional
            rx_handshake.info_hash.emplace();
        }

        if (rx_handshake.info_hash->size() < state_.count)
        {
            reset();
            throw std::runtime_error("Malformed packet: info hash length exceeded");
        }
        (*rx_handshake.info_hash)[state_.count] = byte;

        /// Make sure transmit set the info hash
        if (!handshake_.info_hash)
        {
            reset();
            throw std::runtime_error("Info hash never set! Cannot compare an unknown info hash!");
        }
        else if (byte != (*handshake_.info_hash)[state_.count])
        {
            reset();
            throw std::runtime_error("Malformed packet: invalid info hash byte");
        }
        else if(state_.count == handshake_.info_hash->size())
        {
            /// Move onto the next state
            state_.seq = ID;
            state_.count = 0;
            break;
        }
        
        ++state_.count;
        break;
    }
    case ID:
    {
        if (!rx_handshake.id)
        {
            /// Init the optional 
            rx_handshake.id.emplace();
        }

        if (rx_handshake.id->size() < state_.count)
        {
            reset();
            throw std::runtime_error("Malformed packet: id length exceeded");
        }
        (*rx_handshake.id)[state_.count] = byte;

        /// For the current implementation, ID's are 20 bytes long
        if (state_.count == handshake_.id->size())
        {
            /// Handshake completed
            break;
        }

        ++state_.count;
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
