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
    HandshakeManager::Handshake handshake{};
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

    auto buffered_protocol = std::span(buffer).subspan(1, protocol_ver.size());
    for (const auto &[actual, expected] : std::views::zip(buffered_protocol, protocol_ver))
    {
        ASSERT_EQ(actual, expected);
    }
} 
