#ifndef BENCODE_DECODER_HPP
#define BENCODE_DECODER_HPP

#include <memory>
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

    template<typename T>
    using Callback = std::function<void(T)>;
    struct Callbacks
    {
        Callback<const BencodeValue&> on_decode_callback;
    };

    BencodeDecoder();
    
    void setTorrent(const std::string &filename);

    void setBencode(const std::string_view &beconde);

    void decode();

    void setCallbacks(Callbacks &&callbacks);

    void setBuffer(const std::shared_ptr<std::vector<uint8_t>> &buffer);

    /**
     * \brief Read torrent file to be decoded
     * 
     * \param filename The name of the torrent file
     */
    void loadTorrent(const std::string &filename);

private:
    BencodeValue dispatch();
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

private:
    /// The torrent file
    std::string torrent_;

    /// The url of the tracker
    std::string announce_;

    /// Vector to hold the file contents
    std::shared_ptr<std::vector<uint8_t>> buffer_;

    /// Position in the buffer
    size_t pos_;
    
    /// A dictionary of decoded bencode data
    std::map<std::string, BencodeValue> info_dict_;

    /// Callback block for decoder
    Callbacks callbacks_;
};

}; /// namespace bittorrent
#endif
