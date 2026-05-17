#include <string>
#include <bencode/TorrentManager.hpp>

namespace bittorrent
{

TorrentManager::TorrentManager()
    : torrent_{std::string::npos}
    , decoder_{}
    , meta_info_{}
{}

void TorrentManager::loadTorrent(const std::string &torrent)
{
    torrent_ = torrent;
    decoder_.readTorrent(torrent_);
}

void TorrentManager::readTorrent()
{
    if (torrent_ == std::string::npos)
    {
        throw std::runtime_error("Error: Torrent must be loaded before reading");
    }
    auto data = decoder_.dispatch();

    try 
    {
        grabMetaInfo(data);
    }
    catch(std::runtime_error err)
    {
        errorDebug(err);  
    }
}

void TorrentManager::grabMetaInfo(std::unordered_map<std::string, bittorrent::BencodeValue> &data)
{
    /// Fill out the meta info
    auto it = data.find("announce");
    if (it == data.end())
    {
        throw std::runtime_error("Failed to find announce key in torrent");
    }
    meta_info_.announce = std::get<std::string>(it->second.value);

    it = data.find("name");
    if (it == data.end())
    {
        throw std::runtime_error("Failed to find name key in torrent");
    }
    meta_info_.name = std::get<std::string>(it->second.value);

    it = data.find("length");
    if (it == data.end())
    {
        throw std::runtime_error("Failed to find piece_length key in torrent");
    }
    meta_info_.piece_length = std::get<uint64_t>(it->second.value);

    it = data.find("piece_length");
    if (it == data.end())
    {
        throw std::runtime_error("Failed to find piece length key in torrent");
    }
    meta_info_.piece_length = std::get<int64_t>(it->second.value);

    it = data.find("pieces");
    if (it == data.end())
    {
        throw std::runtime_error("Failed to find pieces key in torrent");
    }
    meta_find_.piece_length = deserialize(std::get<std::string>(it->second.value));
}

void TorrentManager::deserialize(std::string &hash) const
{
    /// Deserialize the SHA1 hash
}

void TorrentManager::errorDebug(const std::string &err) const
{
    std::cerr << "Malformed torrent: " << err << "\n";
}
