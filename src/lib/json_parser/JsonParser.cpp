#include <fstream>
#include <algorithm>
#include <vector>
#include <ranges>
#include <string_view>
#include <nlohmann/json.hpp>
#include "JsonParser.hpp"

/// TODO: figure out how we can implicitely instantiate this using cmake
template std::string JsonParser::getValue<std::string>(std::string&, std::string);

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

template<typename T>
T JsonParser::getValue(std::string &key, std::string delim)
{
    T val{};
    std::queue<std::string> tokens = tokenize(key, delim);
    std::queue<nlohmann::json*> q{&json_};
    while(!q.empty() && !tokens.empty())
    {
        auto node = q.front();
        q.pop();
        
        if (node.value(tokens.front()))
        {
            /// Still looking for the value
            if (node.is_structured())
            {
                for (auto & child : node.items())
                {
                    q.push(child);
                }
            }
            else 
            {
                /// At the value
                return static_cast<T>(node.value());
            }
            tokens.pop();
        }
        else
        {
            for (auto & child : node.items())
            {
                q.push(child);
            }
        }
    }
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