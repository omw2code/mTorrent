#ifndef __JSON_PARSER_HPP__
#define __JSON_PARSER_HPP__

#include <string_view>
#include <queue>
#include <nlohmann/json.hpp>

class JsonParser
{
public:
    JsonParser(std::ifstream &fs);
    JsonParser(const std::string_view &literal);
    
    template<typename T>
    T getValue(std::string &key, std::string delim="\n");

private:
    std::queue<std::string>  tokenize(std::string &str, const std::string delim);

    const std::string_view filename_;
    nlohmann::json json_;
};

#endif