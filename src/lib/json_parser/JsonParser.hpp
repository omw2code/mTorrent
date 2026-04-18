#ifndef __JSON_PARSER_HPP__
#define __JSON_PARSER_HPP__

#include <variant>
#include <string_view>
#include <unordered_map>
#include <queue>
#include <nlohmann/json.hpp>

class JsonParser
{
public:
    JsonParser(std::ifstream &fs);
    JsonParser(const std::string_view &literal);
    
    template<typename T>
    T getJsonValue(std::string key_path, std::string delim="/")
    {
        std::queue<std::string> tokens = tokenize(key_path, delim);
        nlohmann::json *node = &json_;
        while (!tokens.empty())
        {
            auto search_key = tokens.front();
            tokens.pop();
            if (!node->contains(search_key))
            {
                /// Never going to find the value;
                return T{};
            }
            node = &(*node)[search_key];
        }

        if (node->is_array())
        {
            /// using the wrong api to get a list of values
            return T{};
        }

        /// Return the value we found from the path
        return node->get<T>();
    }

    /// Allowed types
    template<typename ...Args>
    std::unordered_map<std::string, std::variant<Args...>> 
    getJsonArray(std::string key_path, std::string delim="/")
    {
        std::unordered_map<std::string, std::variant<Args...>> kvp_map{};
        std::queue<std::string> tokens = tokenize(key_path, delim);
        nlohmann::json *node = &json_;
        while (!tokens.empty())
        {
            auto token = tokens.front();
            tokens.pop();
            if (!node->contains(token))
            {
                return {};
            }
            node = &(*node)[token];
        }

        if (!node->is_object() && !node->is_array())
        {
            return {};
        }

        try
        {
            /// Fill out the map
            for (const auto &[key, val] : node->items())
            {
                kvp_map.emplace(key, std::variant<decltype(val.type())>(val.get<decltype(val.type())>()));
            }
        }
        catch(...)
        {
            throw std::runtime_error("Error: Invalid type found but not specified in parameter pack");
        }
        
        return kvp_map;
    }

private:
    std::queue<std::string>  tokenize(std::string &str, const std::string delim);

    const std::string_view filename_;

    nlohmann::json json_;
};

#endif