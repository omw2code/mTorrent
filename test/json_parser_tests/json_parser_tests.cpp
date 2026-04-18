#include <gtest/gtest.h>
#include <string_view>
#include <string>
#include <JsonParser.hpp>

TEST(JsonParsing, getValidStringTest)
{
    const std::string_view json_str = R"({
        \"BitTorrent\":
        {
            \"TCP\": {
                \"port\": 8080,
                \"ip\": \"192.168.1.5\"
            },
            \"UDP\":{
                \"port\": 8080,
                \"ip\": \"192.168.1.5\"
            }
        }
    })";

    JsonParser parser(json_str);
    auto val = parser.getJsonValue<std::string>("BitTorrent/TCP/port");
}