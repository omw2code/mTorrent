#include <fstream>
#include <string_view>
#include "JsonParser.hpp"

JsonParser::JsonParser(std::ifstream &fs)
{
    /// Read in and initialize the json object
    fs >> json_;
}

JsonParser::JsonParser(const std::string_view &literal)
{
    /// Load the literal 
    auto json_ = nlohmann::json::parse(literal);
    if (json_.is_discarded())
        throw std::runtime_error("Error invalid json string literal");
}

template<typename T>
T JsonParser::getValue(const std::string_view &key)
{
    auto val = json_.find(key);
    if (val == json_.end())
        throw std::runtime_error("Error finding json key: " + key);

    return static_cast<T>(val);
}