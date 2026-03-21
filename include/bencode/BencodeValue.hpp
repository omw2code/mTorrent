#ifndef BENCODE_VALUE_HPP
#define BENCODE_VALUE_HPP

#include <cstdint>
#include <vector>
#include <variant>
#include <string>
#include <unordered_map>

class BencodeValue
{
public:
    using BencodeList = std::vector<BencodeValue>;
    using BencodeDict = std::unordered_map<std::string, BencodeValue>;
    using variant_type = std::variant<
        int64_t, 
        std::string, 
        BencodeList,
        BencodeDict>;
        
    variant_type value;

    BencodeValue() = default;
    BencodeValue(int64_t num) : value(num) {};
    BencodeValue(std::string str) : value(std::move(str)) {};
    BencodeValue(BencodeList list) : value(std::move(list)) {};
    BencodeValue(BencodeDict dict) : value(std::move(dict)) {};
};

#endif