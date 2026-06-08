#include <cstddef>

namespace bittorrent
{


void Handshake::setProtocol(Handshake &&handshake_protocol)
{
    handshake_protocol_ = std::move(handshake_protocol);
}

Handshake::ByteBuffer Handshake::serialize()
{
    ByteBuffer buffer{};

    /// Start byte
    buffer.append(std::byte{handshake_protocol_.prefix_length});

    /// Bittorrent protocol
    std::foreach(
        handshake_protocol_.protocol_.begin(), 
        handshake_protocol_.protocol_.end(), 
        std::back_inserter(buffer),
        [this](char byte) {
            return static_cast<unsigned char>(byte);
        });
    
    /// Add reserved bytes
    buffer.append_range(handshake_protocol_.reserved_);

    /// Append the hash
    if (handshake_protocol_.info_hash.has_value())
    {
        buffer.append_range(handshake_protocol_.info_hash);
    }
    else
    {
        throw std::runtime_error("Error: info hash is required for a handshake");
    }

    /// Append the id
    if (handshake_protocol_.id.has_value())
    {
        buffer.append_range(handshake_protocol_.id);
    }
    else
    {
        throw std::runtime_error("Error: current implementation calls for id to be set in handshake");
    }

    /// Elided
    return buffer;
}

HandshakeManager::Handshake HandshakeManager::deserialize(const std::span<const byte> &message)
{
    Handshake rx_handshake{};
    for (const auto byte : message)
    {
        assembleHandshake(byte, rx_handshake);
    }

    /// Elided
    return rx_handshake;
}

Handshake::assembleHandshake(const std::byte byte, Handshake &rx_handshake)
{
    switch (state_.seq)
    {
        using namesapce HandshakeSeq;

    case LEN_PREFIX:
    {
        if (byte != LEN_PREFIX)
        {
            reset();
            throw std::runtime_error("Malformed header: prefix length");
        };
        rx_handshake.prefix_length = std::to_integer<int>(byte);
        state_ = PROTOCOL;
        break;
    }
    case PROTOCOL:
    {
        /// Build the protocol we are using
        rx_handshake.protocol.append(std::to_integer<char>(byte);

        /// Did we get all our data?
        if (rx_handshake.protocol.size() == rx_handshake.prefix_length &&
            rx_handshake.protocol == protocol_.data())
        {
            /// Next state
            state_ = PADDING;
        }
        else if (handshake.protocol().size() > rx_handshake.prefix_length
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
        if (handshake_protocol_.reserved.size() < state_.count ||
            handshake_protocol_.reserved[state_.count] != byte)
        {
            reset();
            throw std::runtime_error("Malformed packet: padded byte protocol not supported");
        }
        else if (handshake_protocol.reserved.size() == state_.count)
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
        rx_handshake.info_hash.append(byte);

        if (!handshake_protocol_.info_hash.has_value())
        {
            reset();
            throw std::runtime_error("Info hash never set! Cannot compare an unknown info hash!");
        }
        else if(state.count == handshake_protocol_.info_hash.size())
        {
            /// Move onto the next state
            state_.seq = ID;
            state_count = 0;
        }
        break;
    }
    case ID:
    {
        ++state_.count;

        /// For the current implementation, ID's are 20 bytes long
        if (state_.count < handshake_protocol_.id.size())
        {
            rx_handshake.id.append(byte);
        }
        else if (state_.count == handshake_protocol.id.size())
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

Handshake::reset()
{
    state_.seq = HandshakeSeq::LEN_PREFIX;
    state_.count = 0;
}
}; /// namespace bittorrent
