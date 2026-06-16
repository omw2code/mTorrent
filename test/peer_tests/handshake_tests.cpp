#include <bencode/peer/Handshake.hpp>
#include <gtest/gtest.h>

TEST(HandshakeTests, SerializeDefaultHandshakeTest)
{
    /// Default constructor should init the protocol
    bittorrent::peer::HandshakeManager manager{}

    /// Serialize the handshake
    auto buffer = manager.serialize();

    /// Thing to check
    bittorrent::peer::Handshake handshake{};
    ASSERT_FALSE(buffer.empty());

    std::byte expected_start_byte{handshake.prefix_length};
    ASSERT_EQ(expected_start_byte, buffer.at(0)};

    std::vector<std::byte> protocol_ver{};
    protocol_ver.reserve(handshake.protocol.size());
    std::transform(
        handshake.protocol.begin(),
        handshake.protocol.end(),
        std::back_inserter(protocol_ver),
        [](char c){
            return static_cast<unsigned char>(c);
        });
} 
