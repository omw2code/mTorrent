
#include <gtest/gtest.h>
#include <bencode/BencodeDecoder.hpp>

/// Strings are length-prefixed base ten followed by a 
/// colon and the string. For example 4:spam corresponds to 'spam'.
TEST(BencodeDecoderString, DecodeString)
{
    /// Set up the thing we want to decode
    constexpr std::string_view bencode_int{"9:the_thing"};
    bittorrent::BencodeDecoder decoder{};
    decoder.setBencode(bencode_int);

    /// Decode the thing
    auto val = decoder.dispatch();

    /// Assert the thing is the thing
    ASSERT_EQ(std::get<std::string>(val.value), "the_thing");
}

/// Integers are represented by an 'i' followed by the number 
/// in base 10 followed by an 'e'. For example i3e corresponds 
/// to 3 and i-3e corresponds to -3. Integers have no size limitation. 
/// i-0e is invalid. All encodings with a leading zero, such as i03e, 
/// are invalid, other than i0e, which of course corresponds to 0.
TEST(BecondeDecoderInt, DecodeInt)
{
    constexpr std::string_view bencode_uInt{"i3e"};
    bittorrent::BencodeDecoder decoder{};
    decoder.setBencode(bencode_uInt);

    /// Check i3e
    auto val = decoder.dispatch();
    ASSERT_EQ(std::get<int64_t>(val.value), 3);

}

TEST(BecondeDecoderInt, DecodeNegative)
{
    constexpr std::string_view bencode_uInt{"i-3e"};
    bittorrent::BencodeDecoder decoder{};
    decoder.setBencode(bencode_uInt);
    auto val = decoder.dispatch();

    // Check i-3e
    ASSERT_EQ(std::get<int64_t>(val.value), -3);
}

TEST(BecondeDecoderInt, DecodeInvalidInt)
{
    /// Leading 0's are invalid 
    constexpr std::string_view bencode_Int{"i-03e"};
    bittorrent::BencodeDecoder decoder{};
    decoder.setBencode(bencode_Int);
    EXPECT_THROW(decoder.dispatch(), std::runtime_error);

    constexpr std::string_view bencode_uInt{"i03e"};
    decoder.setBencode(bencode_uInt);
    EXPECT_THROW(decoder.dispatch(), std::runtime_error);
}


TEST(BencodeDecoderInt, DecodeLargeInt)
{
    constexpr std::string_view bencode_int{"i1000000000000e"};
    bittorrent::BencodeDecoder decoder{};
    decoder.setBencode(bencode_int);
    auto val = decoder.dispatch();

    ASSERT_EQ(std::get<int64_t>(val.value), 1000000000000);

    constexpr std::string_view bencode_neg{"i-1000000000000e"};
    decoder.setBencode(bencode_neg);
    val = decoder.dispatch();

    ASSERT_EQ(std::get<int64_t>(val.value), -1000000000000);
}

/// Lists are encoded as an 'l' followed by their elements (also bencoded) followed
///  by an 'e'. For example l4:spam4:eggse corresponds to ['spam', 'eggs'].
TEST(BencodeDecoderList, DecodeStringList)
{
    constexpr std::string_view bencode_list{"l4:spam4:eggse"};
    bittorrent::BencodeDecoder decoder{};
    decoder.setBencode(bencode_list);
    auto val = decoder.dispatch();

    /// Smoke test assert size is equal to 2
    ASSERT_EQ(std::get<std::vector<bittorrent::BencodeValue>>(val.value).size(), 2);
}


