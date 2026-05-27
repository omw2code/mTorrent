#include <fstream>
#include <iostream>
#include <bencode/BencodeDecoder.hpp>

namespace bittorrent
{

BencodeDecoder::BencodeDecoder()
    : torrent_()
    , pos_{}
    , info_dict_{}
    , callbacks_{}
    , buffer_{}
{}

void BencodeDecoder::setCallbacks(Callbacks &&callbacks)
{
    callbacks_ = std::move(callbacks);
}

void BencodeDecoder::setBuffer(const std::span<const std::byte> &buffer)
{
    buffer_ = buffer;
    pos_ = 0;
}

void BencodeDecoder::decode()
{
    /// Decode the torrent
    auto decoded_torrent = dispatch();
    if (callbacks_.on_decode_callback)
    {
        callbacks_.on_decode_callback(decoded_torrent);
    }
}

BencodeValue BencodeDecoder::dispatch()
{
    if (pos_ >= buffer_.size())
    {
        throw std::runtime_error("Unexpected end of buffer");
    }

    /// Init source range for bencode node
    size_t start = pos_;

    // Iterate through the buffer
    char c = peekBuffer();
    BencodeValue result;
    switch (c)
    {
    case 'i':
        result = handleInt();
        break;
    case 'l':
        result = handleList();
        break;
    case 'd':
        result = handleDict();
        break;
    default:
        if (std::isdigit(static_cast<unsigned char>(c)))
        {
            result = handleString();
        }
        else
        {
           throw std::runtime_error("Invalid or unexpected byte: ");
        }
    }

    /// Save the range for this node
    result.source_range = {
        .start = start,
        .end = pos_
    };

    return result;
}

BencodeValue BencodeDecoder::handleString()
{
    // Reset the length
    int str_len{};

    // Store the string length
    while(pos_ < buffer_.size() &&
          std::isdigit(static_cast<unsigned char>(peekBuffer())))
    {
        str_len = (str_len * 10) + (peekBuffer() - '0');
        ++pos_;
    }
    
    if (pos_ >= buffer_.size() || peekBuffer() != ':')
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
    const char* begin = reinterpret_cast<const char*>(buffer_.data() + pos_);
    std::string str(begin, str_len);

    // Advance the index
    pos_ += str_len;

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
    if (pos_ < buffer_.size() && peekBuffer() == '-')
    {
        negative = true;
        ++pos_;
    }

    bool leading_zero{false};
    while (pos_ < buffer_.size() &&
           std::isdigit(static_cast<unsigned char>(peekBuffer())))
    {
        /// Check if there was a leading zero
        if (leading_zero)
        {
            throw std::runtime_error("Invalid integer, leading zero integers are invalid");
        }
        num = (num * 10) + (peekBuffer() - '0');
        leading_zero = num == 0;
        ++pos_;
    }
    
    // Check bounds
    if (pos_ >= buffer_.size())
    {
        throw std::runtime_error("Int exceeds buffer size");
    }

    // Ensure the data was fully read
    if (peekBuffer() != 'e')
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
    while (pos_ < buffer_.size() && peekBuffer() != 'e')
    {
        list.push_back(dispatch());
    }
    
    if (pos_ >= buffer_.size())
    {
        throw std::runtime_error("Unexpected end of buffer when parsing list");
    }
    
    if (peekBuffer() != 'e')
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
    while(pos_ < buffer_.size() && peekBuffer() != 'e')
    {
        char c = std::to_integer<char>(buffer_[pos_]);
        auto key = std::get<std::string>(dispatch().value); 
        auto val = dispatch();
        dict[key] = val;
    }
    ++pos_;
    return BencodeValue(std::move(dict));
}

char BencodeDecoder::peekBuffer() const 
{
    return std::to_integer<char>(buffer_[pos_]);
}

}; /// namespace bittorrent
