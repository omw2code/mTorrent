#ifndef __TORRENT_MANAGER_HPP__
#define __TORRENT_MANAGER_HPP__

#include <unordered_map>
#include <string>
#include "BencodeValue.hpp"
#include "BencodeDecoder.hpp"
#include "TorrentMetaInfo.hpp"

namespace bittorrent
{
 
class TorrentManager
{
public:
    TorrentManager();

    void loadTorrent(const std::string &torrent);
    void readTorrent();
private:
    void grabMetaInfo(const std::unordered_map<std::string, BencodeValue> &data);
    uint8_t deserialize(const std::string &hash);
    void errorDebug(const std::string &err) const; 
private:
    std::string torrent_;

    BencodeDecoder decoder_;
    TorrentMetaInfo meta_info_;
};

} /// namespace bittorrent

#endif
