#ifndef __JSON_PARSER_HPP__
#define __JSON_PARSER_HPP__

#include <iostream>
#include <variant>
#include <string_view>
#include <unordered_map>
#include <queue>
#include <nlohmann/json.hpp>

class JsonParser
{
public:
    JsonParser(std::string &filename);
    JsonParser(const std::string_view &literal);
    
    template<typename T>
    T getJsonValue(std::string key_path, std::string delim="/")
    {
        nlohmann::json *node = findNode(&json_, key_path, delim);
        if (node == nullptr || node->is_object())
        {
            std::cerr << "DEBUG: node is nullptr or is an object\n";
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
        using Variant = std::variant<Args...>;
        std::unordered_map<std::string, Variant> kvp_map{};
        nlohmann::json *node = findNode(&json_, key_path, delim);
        if (node == nullptr && !node->is_array() || !node->is_object())
        {
            return {};
        }

        /// Fill out the map
        for (const auto &[key, val] : node->items())
        {
            kvp_map.emplace(key, make_variant<Variant, Args...>(val));
        }
        
        /// return json kvp map
        return kvp_map;
    }

private:
    template<typename Variant, typename T>
    bool canAssign(Variant &variant, const nlohmann::json &node)
    {
        try
        {
            variant = node.get<T>();
        }
        catch(...)
        {
            return false;
        }
        return true;
    }

    template<typename Variant, typename... Args>
    std::variant<Args...> make_variant(const nlohmann::json &node)
    {
        /// Check whether we can grab and assign the type to our variant types
        Variant variant;
        if (!(canAssign<Variant, Args>(variant, node) || ...))
        {
            throw std::runtime_error("Error could not assign json underlying type to specified variant types");
        }

        return variant;
    }

private:

    nlohmann::json* findNode(nlohmann::json *node, std::string &key_path, std::string &delim);

    std::queue<std::string>  tokenize(std::string &str, const std::string delim);

    const std::string_view filename_;

    nlohmann::json json_;
};

#endif