#ifndef BENCODE_DECODER_HPP
#define BENCODE_DECODER_HPP

#include <string>
#include <vector>
#include <map>
#include <functional>
#include <bencode/BencodeValue.hpp>

namespace bittorrent
{

class BencodeDecoder
{
public:

    using BencodeList = std::vector<BencodeValue>;
    using BencodeDict = std::map<std::string, BencodeValue>;
    using variant_type = std::variant<
        int64_t,
        std::string,
        BencodeList,
        BencodeDict>;

    BencodeDecoder();

    BencodeDecoder(const std::string_view &torrent);
    
    void setTorrent(const std::string &filename);

    void setBencode(const std::string_view &beconde);

    BencodeValue dispatch();

private:
    /**
     * \brief Read torrent file to be decoded
     * 
     * \param filename The name of the torrent file
     */
    void readTorrent(const std::string &filename);

    /**
     * \brief Decode bencoded string types in UTF-8 format
     * 
     * \param str Bencoded string
     */
   BencodeValue handleString();
    
    /**
     * \brief Decode bencoded integral types
     * 
     * \param integral Bencoded integer
     */
    BencodeValue handleInt();

    /**
     * \brief Decode bencoded list types
     * 
     * \param list Bencoded list
     */
    BencodeValue handleList();

    /**
     * \brief Decode bencoded dictionary types
     * 
     * \param dict Bencoded dictionary
     */
    BencodeValue handleDict();

    /**
     * \brief Safely consume digits until a dilimeter
     * 
     * \return If the next byte is expected or not
     */
    bool consumeUntil(const char delimiter);

   BencodeValue parse();

private:
    /// The torrent file
    std::string torrent_;

    /// The url of the tracker
    std::string announce_;

    /// Vector to hold the file contents
    std::vector<uint8_t> buffer_;

    /// Position in the buffer
    int pos_;

    /// The length of the decoded string type
    int str_len_;
    
    /// A dictionary of decoded bencode data
    std::map<std::string, BencodeValue> decode_dict_;
};

}; /// namespace bittorrent
#endif
