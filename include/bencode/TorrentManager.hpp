#ifndef __TORRENT_MANAGER_HPP__
#define __TORRENT_MANAGER_HPP__

namespace bittorrent
{
 
class TorrentManager
{
public:
    TorrentManager();

    loadTorrent(std::string &torrent);
private:
    void grabMetainfo(std::unordered_map<std::string, BencodeValue> &data);
    void deserialize(std::string &hash) const;
    void errorDebug(const std::string &err) const; 
private:
    std::string torrent_;

    BencodeDecoder decoder_;
    TorrentMetaInfo meta_info_;
};

} /// namespace bittorrent
