#include <fstream>
#include <iostream>
#include <bencode/BencodeDecoder.hpp>

namespace bittorrent
{

BencodeDecoder::BencodeDecoder()
    : torrent_()
    , pos_{}
    , info_dict_{}
{}

void BencodeDecoder::loadTorrent(const std::string &filename)
{
    // Safely use binary mode
    std::filesystem::path file_path(filename);
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open())
    {
       throw std::runtime_error("Error opening torrent file");
    }

    // Replace and store the contents of the file into a buffer
    buffer_.assign(std::istreambuf_iterator<char>(file),
                   std::istreambuf_iterator<char>());
}

BencodeValue BencodeDecoder::dispatch()
{
    if (pos_ >= buffer_.size())
        throw std::runtime_error("Unexpected end of buffer");

    // Iterate through the buffer
    char c = buffer_[pos_];
    switch (c)
    {
    case 'i':
        return handleInt();
        break;
    case 'l':
        return handleList();
        break;
    case 'd':
        return handleDict();
        break;
    default:
        if (std::isdigit(static_cast<unsigned char>(c)))
        {
            return handleString();
        }
        else
        {
           throw std::runtime_error("Invalid or unexpected byte: ");
        }
    }
}

BencodeValue BencodeDecoder::handleString()
{
    // Reset the length
    int str_len{};

    // Store the string length
    while(pos_ < buffer_.size() &&
          std::isdigit(static_cast<unsigned char>(buffer_[pos_])))
    {
        str_len = (str_len * 10) + (buffer_[pos_] - '0');
        ++pos_;
    }
    
    if (pos_ >= buffer_.size() || buffer_[pos_] != ':')
    {
        throw std::runtime_error("Malformed string type");
    }
    
    // Skip ':'
    ++pos_;
    
    if (pos_ + str_len > buffer_.size())
    {
        throw std::runtime_error("String length exceeds buffer size");
    }

    // Grab the string
    std::string str(buffer_.begin() + pos_, buffer_.begin() + pos_ + str_len);
    // Advance the index
    pos_ += str_len;

    printf("%.*s", (int)str.size()+ 1, str.c_str());

    

    // Elided
    return BencodeValue(std::move(str));
}

BencodeValue BencodeDecoder::handleInt()
{
    // Move past 'i'
    ++pos_;
    int64_t num{0};

    /// Check for signess 
    bool negative{false};
    if (pos_ < buffer_.size() && buffer_[pos_] == '-')
    {
        negative = true;
        ++pos_;
    }

    bool leading_zero{false};
    while (pos_ < buffer_.size() &&
           std::isdigit(static_cast<unsigned char>(buffer_[pos_])))
    {
        /// Check if there was a leading zero
        if (leading_zero)
        {
            throw std::runtime_error("Invalid integer, leading zero integers are invalid");
        }
        num = (num * 10) + (buffer_[pos_] - '0');
        leading_zero = num == 0;
        ++pos_;
    }
    
    // Check bounds
    if (pos_ >= buffer_.size())
    {
        throw std::runtime_error("Int exceeds buffer size");
    }

    // Ensure the data was fully read
    if (buffer_[pos_] != 'e')
    {
        std::runtime_error("Malformed int type");
    }
    
    // Advance position
    ++pos_;
    
    return BencodeValue(negative ? -1 * num : num);
}

BencodeValue BencodeDecoder::handleList()
{
    // Move pas 'l'
    ++pos_;
   BencodeValue::BencodeList list;
    while (pos_ < buffer_.size() && buffer_[pos_] != 'e')
    {
        list.push_back(dispatch());
        //++pos_;
    }
    
    if (pos_ >= buffer_.size())
    {
        throw std::runtime_error("Unexpected end of buffer when parsing list");
    }
    
    if (buffer_[pos_] != 'e')
    {
        throw std::runtime_error("Malformed list type");
    }

    // Advance position
    ++pos_;

    return BencodeValue(std::move(list));
}

BencodeValue BencodeDecoder::handleDict()
{
    // Move past 'd'
    ++pos_;
    BencodeValue::BencodeDict dict;
    while(pos_ < buffer_.size() && buffer_[pos_] != 'e')
    {
        auto key = std::get<std::string>(dispatch().value);
        auto val = dispatch();
        dict[key] = val;
    }
    return BencodeValue(std::move(dict));
}

void BencodeDecoder::setTorrent(const std::string &filename)
{
    torrent_ = filename;
}

void BencodeDecoder::setBencode(const std::string_view &bencode)
{
    buffer_.assign(bencode.begin(), bencode.end());
    pos_ = 0;
}

}; /// namespace bittorrent
