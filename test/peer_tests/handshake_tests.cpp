#include <ranges>
#include <peer/HandshakeManager.hpp>
#include <gtest/gtest.h>

namespace bittorrent::peer
{
class FakeHandshake
{
public:
    FakeHandshake()
    {
        std::vector<std::byte> fake_bytes{
            std::byte{0x01},
            std::byte{0x02},
            std::byte{0x03}};
        handshake_.info_hash = fake_bytes;
        handshake_.id.emplace(fake_bytes.rbegin(), fake_bytes.rend());
    };

    HandshakeManager::Handshake getHandshake() { return handshake_; };

private:

    HandshakeManager::Handshake handshake_;
};
};

TEST(HandshakeTests, SerializeDefaultHandshakeTest)
{
    using namespace bittorrent::peer;
    /// Default constructor should init the protocol
    HandshakeManager manager{};

    /// Create a fake handshake
    FakeHandshake fake_handshake{};
    manager.setProtocol(std::move(fake_handshake.getHandshake()));

    /// Serialize the handshake
    auto buffer = manager.serialize();

    /// Thing to check
    auto handshake = fake_handshake.getHandshake();
    ASSERT_FALSE(buffer.empty());

    /// Make sure first byte is the length
    std::byte expected_start_byte{handshake.prefix_length};
    ASSERT_EQ(expected_start_byte, buffer.at(0));

    /// Make sure the next part of the header is the protocol
    std::vector<std::byte> protocol_ver{};
    protocol_ver.reserve(handshake.protocol.size());
    std::transform(
        handshake.protocol.begin(),
        handshake.protocol.end(),
        std::back_inserter(protocol_ver),
        [](char c){
            return static_cast<std::byte>(c);
        });

    /// Check the protocol type
    std::span<std::byte> span_buf{buffer};
    auto peak_payload = span_buf.subspan(1, protocol_ver.size());
    for (const auto &[actual, expected] : std::views::zip(peak_payload, protocol_ver))
    {
        ASSERT_EQ(actual, expected) << "Protocol version does not match";
    }

    /// Check the reserved bytes
    span_buf = span_buf.subspan(protocol_ver.size() + 1);
    peak_payload = span_buf.subspan(0, 8);
    for (const auto &[actual, expected] : std::views::zip(peak_payload, handshake.reserved))
    {
        ASSERT_EQ(actual, expected) << "Reserved bytes don't match";
    }

    /// Check the hash
    span_buf = span_buf.subspan(8);
    peak_payload = span_buf.subspan(0, handshake.info_hash->size());
    for (const auto &[actual, expected] : std::views::zip(peak_payload, *handshake.info_hash))
    {
        ASSERT_EQ(actual, expected) << "Info hash bytes don't match";
    }

    /// Check the id
    span_buf = span_buf.subspan(handshake.info_hash->size());
    peak_payload = span_buf.subspan(0, handshake.id->size());
    for (const auto &[actual, expected] : std::views::zip(peak_payload, *handshake.id))
    {
        ASSERT_EQ(actual, expected) << "ID bytes do not match";
    }
} 
