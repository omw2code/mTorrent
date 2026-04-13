#include <string_view>
#include "JsonParser.hpp"

JsonParser::JsonParser(const std::string_view &filename)
    : filename_{filename}
{}