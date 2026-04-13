#ifndef __JSON_PARSER_HPP__
#define __JSON_PARSER_HPP__

#include <string_view>
#include <nlohmann/json.hpp>

class JsonParser
{
public:
    JsonParser(const std::string_view &filename);
private:
    const std::string_view filename_;
};

#endif