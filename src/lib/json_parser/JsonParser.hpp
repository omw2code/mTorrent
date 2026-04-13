#ifndef __JSON_PARSER_HPP__
#define __JSON_PARSER_HPP__

#include <string_view>
#include <nlohmann/json.hpp>

class JsonParser
{
public:
    JsonParser(std::ifstream &fs);
    JsonParser(const std::string_view &literal);
    
    template<typename T>
    T getValue(const std::string_view &key);
private:
    const std::string_view filename_;
    nlohmann::json json_;
};

#endif