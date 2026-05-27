#ifndef __TORRENT_MANAGER_HPP__
#define __TORRENT_MANAGER_HPP__

#include <unordered_map>
#include <span>
#include <string>
#include "BencodeValue.hpp"
#include "BencodeDecoder.hpp"
#include "TorrentMetaInfo.hpp"

namespace bittorrent
{
 
class TorrentManager
{
public:
    
    using Sha1Hash = std::array<std::byte, 20>;

    TorrentManager();
    
    const TorrentMetaInfo& getMetaInfo(); 
    
    void loadTorrent(const std::string &torrent);
    
    void readTorrent();

private:
    
    void extractMetaInfo(const BencodeValue &data);
    
    void grabMetaInfo(const std::unordered_map<std::string, BencodeValue> &data);
    
    uint8_t deserialize(const std::string &hash);

    Sha1Hash generateInfoHash(const std::span<const std::byte> &raw_info_bytes); 
    
    void errorDebug(const std::string &err) const; 

private:
    std::string torrent_;
    BencodeDecoder decoder_;
    TorrentMetaInfo meta_info_;
    static constexpr int hash_size_{20};
    std::vector< std::byte> buffer_;
};

} /// namespace bittorrent

#endif
