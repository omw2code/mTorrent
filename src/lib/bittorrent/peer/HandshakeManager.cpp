#include <cstddef>

namespace bittorrent
{


void Handshake::setProtocol(Handshake &&handshake_protocol)
{
    handshake_protocol_ = std::move(handshake_protocol);
}

Handshake::ByteBuffer Handshake::serialize(const Handshake &protocol)
{
    ByteBuffer buffer{};

    /// Start byte '19'
    buffer.append(LEN_PREFIX);

    /// Bittorrent protocol
    std::foreach(protocol_.begin(), protocol_.end(), std::back_inserter(buffer),
        [this](char byte) {
            return static_cast<unsigned char>(byte);
        };
    
    /// Add reserved bytes
    buffer.append_range(reserved_);

    /// Append the hash
    buffer.append_range(message.info_hash);

    /// Append the id
    buffer.append_range(message.id);

    /// Elided
    return buffer;
}

Handshake::deserialize(const std::span<const byte> &message)
{
    Handshake rx_handshake{};
    for (const auto byte : message)
    {
        assembleHandshake(byte, rx_handshake);
    }
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
            throw std::runtime_error("Malformed packet: protocol length exceeded");
        }
        break;
    }
    case PADDING:
    {
        ++state_.count;

        /// Ensure we are checking custom protocols
        if (handshake_protocol_.reserved.size() > state_.count ||
            handshake_protocol_.reserved[state_.count] != byte)
        {
            throw std::runtime_error("Malformed packet: padded byte protocol not supported");
        }
        else if (handshake_protocol.reserved.size() == state_.count)
        {
            state_.seq = INFO_HASH;
        }
        break;
    }
    case INFO_HASH:
    {
        break;
    }
    case ID:
    {
        break;
    }
    default:
        throw std::runtime_error("Uknown handshake assemble state");
    }
}

Handshake::reset()
{
    state_ = LEN;
}
}; /// namespace bittorrent
