#include <cstddef>

namespace bittorrent
{

Handshake::ByteBuffer Handshake::serialize(const Sync &message)
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
    switch (state_)
    {
        using namesapce  
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
        if (rx_handshake.protocol.size() > LEN_PREFIX)
        {
            reset();
            throw std::runtime_error("Malformed header: protocol type");
        }
    }
    case PADDING:
    {
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
