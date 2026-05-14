
#include <gtest/gtest.h>
#include <bencode/BencodeDecoder.hpp>

TEST(BencodeDecoderTests, DecodeString)
{
    /// Set up the thing we want to decode
    constexpr std::string_view string_torrent{"4:spam"};
    bittorrent::BencodeDecoder decoder(string_torrent);

    /// Decode the thing
    auto val = decoder.dispatch();

    /// Assert the thing is the thing
    ASSERT_EQ(std::get<std::string>(val.value), "spam");

}
