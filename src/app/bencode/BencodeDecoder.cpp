#include <fstream>
#include <iostream>
#include <bencode/BencodeDecoder.hpp>


BencodeDecoder::BencodeDecoder()
    : torrent_()
    , pos_(0)
{}

void BencodeDecoder::readTorrent(const std::string &filename)
{
    // Safely use binary mode
    std::filesystem::path file_path(filename);
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open())
    {
        std::cerr << "Error opening file" << "\n";
        exit(1);
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
           throw std::runtime_error("Invalid or unexpected byte");
        }
    }
}

BencodeValue BencodeDecoder::handleString()
{
    // Reset the length
    str_len_ = 0;

    // Store the string length
    while(pos_ < buffer_.size() &&
          std::isdigit(static_cast<unsigned char>(buffer_[pos_])))
    {
        str_len_ = (str_len_ * 10) + (buffer_[pos_] - '0');
        ++pos_;
    }
    
    if (pos_ >- buffer_.size() || buffer_[pos_] != ':')
        throw std::runtime_error("Malformed string type");
    
    // Skip ':'
    ++pos_;
    
    if (pos_ + str_len_ > buffer_.size())
        throw std::runtime_error("String length exceeds buffer size");

    // Grab the string
    std::string str;
    str.assign(buffer_.begin() + pos_, buffer_.begin() + pos_ + str_len_);

    // Advance the index
    pos_ += str_len_;

    // Elided
    return BencodeValue(std::move(str));
}

BencodeValue BencodeDecoder::handleInt()
{
    // Move past 'i'
    ++pos_;
    int num = 0;
    while (pos_ < buffer_.size() &&
           std::isdigit(static_cast<unsigned char>(buffer_.at(pos_))))
    {
        num = (num * 10) + (buffer_.at(pos_) - '0');
        ++pos_;
    }
    
    // Check bounds
    if (pos_ >= buffer_.size())
        throw std::runtime_error("Int exceeds buffer size");

    // Ensure the data was fully read
    if (buffer_[pos_] != 'e')
        std::runtime_error("Malformed int type");
    
    // Advance position
    ++pos_;

    return BencodeValue(num);
}

BencodeValue BencodeDecoder::handleList()
{
    // Move pas 'l'
    ++pos_;
    BencodeValue::BencodeList list;
    while (pos_ < buffer_.size() && buffer_[pos_] != 'e')
    {
        list.push_back(dispatch());
        ++pos_;
    }
    
    if (pos_ >= buffer_.size())
        throw std::runtime_error("Unexpected end of buffer when parsing list");
    
    if (buffer_[pos_] != 'e')
        throw std::runtime_error("Malformed list type");

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
        decode_dict_[key] = val;
    }
    return BencodeValue(std::move(dict));
}

void BencodeDecoder::setTorrent(const std::string &filename)
{
    torrent_ = filename;
}
