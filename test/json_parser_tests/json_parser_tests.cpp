#include <string_view>
#include <string>
#include <type_traits>
#include <gtest/gtest.h>
#include <JsonParser.hpp>

TEST(JsonParsing, getValidJsonValueTest)
{
    const std::string_view json_str = R"({
        "BitTorrent":
        {
            "TCP": {
                "port": 8080,
                "ip": "192.168.1.5"
            },
            "UDP":{
                "port": 8080,
                "ip": "192.168.1.5"
            }
        }
    })";

    JsonParser parser(json_str);
    int actual = parser.getJsonValue<int>("BitTorrent/TCP/port");

    ASSERT_EQ(actual, 8080);
}

TEST(JsonParsing, getValidJsonArrayTest)
{
    const std::string_view json_str = R"({
        "BitTorrent":
        {
            "TCP": {
                "port": 8080,
                "ip": "192.168.1.5"
            },
            "UDP":{
                "port": 8080,
                "ip": "192.168.1.5"
            }
        }
    })";

    /// Init the parser, grab the array
    JsonParser parser(json_str);
    auto actual_map = parser.getJsonArray<int, std::string>("BitTorrent/TCP");
    
    /// Asserts for the tests 
    ASSERT_EQ(actual_map.size(), 2);
    for (const auto &[key, val] : actual_map)
    {
        if constexpr (std::is_same<decltype(val), int>::value)
        {
            ASSERT_EQ(key, "port");
            ASSERT_EQ(val, 8080);
        }
        else if constexpr (std::is_same<decltype(val), std::string>::value)
        {
            ASSERT_EQ(key, "ip");
            ASSERT_EQ(val, "192.168.1.5");
        }
    }
}