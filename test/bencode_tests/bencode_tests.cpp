
#include <ranges>
#include <gtest/gtest.h>
#include <bencode/BencodeDecoder.hpp>

/// Strings are length-prefixed base ten followed by a 
/// colon and the string. For example 4:spam corresponds to 'spam'.
TEST(BencodeDecoderString, DecodeString)
{
    /// Set up the thing we want to decode
    constexpr std::span<const char> bencode_lit{"9:the_thing"};
    const std::span<const std::byte> bencode_bytes{std::as_bytes(bencode_lit)}; 
    bittorrent::BencodeDecoder decoder{};
    decoder.setBuffer(bencode_bytes);
    
    /// Our values we want to check
    bittorrent::BencodeValue actual_data;

    /// Set up the callback
    bittorrent::BencodeDecoder::Callbacks callbacks;
    callbacks.on_decode_callback = [&](const bittorrent::BencodeValue &data){ actual_data = data; };
    decoder.setCallbacks(std::move(callbacks));

    /// Decode the thing
    decoder.decode();

    /// Assert the thing is the thing
    ASSERT_EQ(std::get<std::string>(actual_data.value), "the_thing");
}

/*
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

    /// Decoded value we want to catch
    bittorrent::BencodeValue actual_data;

    /// Set up callback
    bittorrent::BencodeDecoder::Callbacks callbacks{};
    callbacks.on_decode_callback = [&](const bittorrent::BencodeValue &data){ actual_data = data; };
    decoder.setCallbacks(std::move(callbacks));

    /// Read the data
    decoder.decode();

    /// Check i3e
    ASSERT_EQ(std::get<int64_t>(actual_data.value), 3);

}

TEST(BecondeDecoderInt, DecodeNegative)
{
    constexpr std::string_view bencode_uInt{"i-3e"};
    bittorrent::BencodeDecoder decoder{};
    decoder.setBencode(bencode_uInt);

    /// Actual data we'll check
    bittorrent::BencodeValue actual_data;

    /// Set up callback
    bittorrent::BencodeDecoder::Callbacks callbacks;
    callbacks.on_decode_callback = [&](const bittorrent::BencodeValue &data){ actual_data = data; };
    decoder.setCallbacks(std::move(callbacks));

    /// Read the data
    decoder.decode();

    // Check i-3e
    ASSERT_EQ(std::get<int64_t>(actual_data.value), -3);
}

TEST(BecondeDecoderInt, DecodeInvalidInt)
{
    /// Leading 0's are invalid 
    constexpr std::string_view bencode_Int{"i-03e"};
    bittorrent::BencodeDecoder decoder{};
    decoder.setBencode(bencode_Int);

    // Thing to check
    bittorrent::BencodeValue actual_data;

    /// Callbacks
    bittorrent::BencodeDecoder::Callbacks callbacks;
    callbacks.on_decode_callback = [&](const bittorrent::BencodeValue &data){ actual_data = data; };
    decoder.setCallbacks(std::move(callbacks));

    /// This should throw
    EXPECT_THROW(decoder.decode(), std::runtime_error);

    /// Change it up with a positive number
    constexpr std::string_view bencode_uInt{"i03e"};
    decoder.setBencode(bencode_uInt);
    EXPECT_THROW(decoder.decode(), std::runtime_error);
}


TEST(BencodeDecoderInt, DecodeLargeInt)
{
    constexpr std::string_view bencode_int{"i1000000000000e"};
    bittorrent::BencodeDecoder decoder{};
    decoder.setBencode(bencode_int);
   
    /// Checking this value
    bittorrent::BencodeValue actual_data;

    /// Set up callbacks
    bittorrent::BencodeDecoder::Callbacks callbacks;
    callbacks.on_decode_callback = [&](const bittorrent::BencodeValue &data){ actual_data = data; };
    decoder.setCallbacks(std::move(callbacks));

    /// Decode and assert this
    decoder.decode();
    ASSERT_EQ(std::get<int64_t>(actual_data.value), 1000000000000);

    /// Set a new piece of data
    constexpr std::string_view bencode_neg{"i-1000000000000e"};
    decoder.setBencode(bencode_neg);

    /// Decode and assert
    decoder.decode();
    ASSERT_EQ(std::get<int64_t>(actual_data.value), -1000000000000);
}

/// Lists are encoded as an 'l' followed by their elements (also bencoded) followed
///  by an 'e'. For example l4:spam4:eggse corresponds to ['spam', 'eggs'].
TEST(BencodeDecoderList, DecodeStringList)
{
    constexpr std::string_view bencode_list{"l4:spam4:eggse"};
    bittorrent::BencodeDecoder decoder{};
    decoder.setBencode(bencode_list);

    /// Thing to check
    bittorrent::BencodeValue actual_data;

    // Callbacks
    bittorrent::BencodeDecoder::Callbacks callbacks;
    callbacks.on_decode_callback = [&](const bittorrent::BencodeValue &data){ actual_data = data; };
    decoder.setCallbacks(std::move(callbacks));

    // Decode
    decoder.decode();

    /// Smoke test assert size is equal to 2
    using bittorrent_vec = std::vector<bittorrent::BencodeValue>;
    std::vector<std::string> expected{"spam", "eggs"};
    ASSERT_EQ(std::get<bittorrent_vec>(actual_data.value).size(), expected.size());
    for (const auto& [ben_val, real_val]  : std::views::zip(std::get<bittorrent_vec>(actual_data.value), expected))
    {
        using BenBaseType = std::decay_t<decltype(std::get<std::string>(ben_val.value))>;
         static_assert(std::is_same<
             BenBaseType , 
             std::string>::value); 
         
         ASSERT_EQ(std::get<std::string>(ben_val.value), real_val);
    }
}


TEST(BencodeDecoderList, DecodeMultiTypeList)
{
    constexpr std::string_view bencode_list{"l4:spami4e4:eggse"};
    bittorrent::BencodeDecoder decoder{};
    decoder.setBencode(bencode_list);

    /// Thing to check
    bittorrent::BencodeValue actual_data;

    /// Set up callbacks
    bittorrent::BencodeDecoder::Callbacks callbacks;
    callbacks.on_decode_callback = [&](const bittorrent::BencodeValue &data){ actual_data = data; };
    decoder.setCallbacks(std::move(callbacks));

    /// Decode the data
    decoder.decode();

    using BittorrentVec = std::vector<bittorrent::BencodeValue>;
    auto decoded_vec = std::get<BittorrentVec>(actual_data.value);
    ASSERT_EQ(decoded_vec.size(), 3);

    /// Just grab each value directly and don't worry about iterating through the
    /// vectors cause that would be very ugly
    using BenBaseTypeString = std::decay_t<decltype(std::get<std::string>(decoded_vec[0].value))>;
    static_assert(std::is_same<BenBaseTypeString, std::string>::value);
    ASSERT_EQ(std::get<std::string>(decoded_vec[0].value), "spam");

    using BenBaseTypeInt = std::decay_t<decltype(std::get<int64_t>(decoded_vec[1].value))>;
    static_assert(std::is_same<BenBaseTypeInt, int64_t>::value); 
    ASSERT_EQ(std::get<int64_t>(decoded_vec[1].value), 4);

    using BenBaseTypeString = std::decay_t<decltype(std::get<std::string>(decoded_vec[2].value))>;
    static_assert(std::is_same<BenBaseTypeString, std::string>::value);
    ASSERT_EQ(std::get<std::string>(decoded_vec[2].value), "eggs");
}

/// Dictionaries are encoded as a 'd' followed by a list of alternating keys and their corresponding values 
/// followed by an 'e'. For example, d3:cow3:moo4:spam4:eggse corresponds to {'cow': 'moo', 'spam': 'eggs'} 
/// and d4:spaml1:a1:bee corresponds to {'spam': ['a', 'b']}. Keys must be strings and appear in sorted order 
/// (sorted as raw strings, not alphanumerics)
TEST(BencodeDecoderDict, DecodeSimpleDict)
{
    constexpr std::string_view bencode_dict{"d3:cow3:moo4:spam4:eggse"}; 
    bittorrent::BencodeDecoder decoder{};
    decoder.setBencode(bencode_dict);

    /// Set up callbacks
    bittorrent::BencodeValue actual_data;
    bittorrent::BencodeDecoder::Callbacks callbacks;
    callbacks.on_decode_callback = [&](const bittorrent::BencodeValue &data){ actual_data = data; };
    decoder.setCallbacks(std::move(callbacks));

    /// Decode the data
    decoder.decode();
    
    using StrMap = std::map<std::string, std::string>;
    using BenMap = std::unordered_map<std::string, bittorrent::BencodeValue>;
    StrMap expected { {"cow", "moo"}, {"spam", "eggs"}};
    ASSERT_EQ(std::get<BenMap>(actual_data.value).size(), expected.size());
    for (const auto &[ben_kvp, real_kvp] : std::views::zip(std::get<BenMap>(actual_data.value), expected))
    {
        ASSERT_EQ(std::get<std::string>(ben_kvp.second.value), real_kvp.second);
    }
}
*/
