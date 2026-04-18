#include <fstream>
#include <algorithm>
#include <vector>
#include <ranges>
#include <string_view>
#include <nlohmann/json.hpp>
#include "JsonParser.hpp"

JsonParser::JsonParser(std::ifstream &fs)
{
    /// Read in and initialize the json object
    fs >> json_;
}

JsonParser::JsonParser(const std::string_view &literal)
{
    /// Load the literal 
    json_ = nlohmann::json::parse(literal);
    if (json_.is_discarded())
        throw std::runtime_error("Error invalid json string literal");
}

std::queue<std::string> JsonParser::tokenize(std::string &str, const std::string delim)
{
    size_t pos{};
    std::queue<std::string> tokens{};
    while((pos = str.find(delim)) != std::string::npos)
    {
        tokens.push(str.substr(0, pos));
        str.erase(0, pos + delim.length());
    }
    
    // Elided
    return tokens;
}