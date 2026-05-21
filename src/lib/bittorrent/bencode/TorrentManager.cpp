#include <iostream>
#include <algorithm>
#include <string>
#include <bencode/TorrentManager.hpp>
#include <bencode/BencodeValue.hpp>

namespace bittorrent
{

TorrentManager::TorrentManager()
    : torrent_{}
    , decoder_{}
    , meta_info_{}
{}

const TorrentMetaInfo& TorrentManager::getMetaInfo()
{
   return meta_info_;
}

void TorrentManager::loadTorrent(const std::string &torrent)
{
    try
    {
        torrent_ = torrent;
        decoder_.loadTorrent(torrent_);
    }
    catch(const std::runtime_error &err)
    {
        errorDebug(err.what());
    }
}

void TorrentManager::readTorrent()
{
    if (torrent_ == "")
    {
        throw std::runtime_error("Error: Torrent must be loaded before reading");
    }
    auto data = std::get<std::unordered_map<std::string, BencodeValue>>(decoder_.dispatch().value);

    try 
    {
        grabMetaInfo(data);
    }
    catch(const std::runtime_error &err)
    {
        errorDebug(err.what());  
    }
    catch(const std::bad_variant_access &err)
    {
        errorDebug(err.what());
    }
}

void TorrentManager::grabMetaInfo(const std::unordered_map<std::string, BencodeValue> &data)
{
    /// Fill out the meta info
    auto it = data.find("announce");
    if (it == data.end())
    {
        throw std::runtime_error("Failed to find announce key in torrent");
    }
    meta_info_.announce = std::get<std::string>(it->second.value);
    it = data.find("info");
    if (it == data.end())
    {
        throw std::runtime_error("Failed to find info key in torrent");
    }

    auto info_dict = std::get<std::unordered_map<std::string, BencodeValue>>(it->second.value);
    it = info_dict.find("name");
    if (it == info_dict.end())
    {
        throw std::runtime_error("Failed to find name key in torrent");
    }
    meta_info_.name = std::get<std::string>(it->second.value);

    it = info_dict.find("length");
    if (it == info_dict.end())
    {
        std::cout << "DEBUG: length is END ITER\n";
        it = info_dict.find("files");
        if (it == info_dict.end())
        {
            throw std::runtime_error("Failed to find length or files key in torrent");
        }
        auto files = std::get<std::vector<BencodeValue>>(it->second.value);
        std::for_each(files.begin(), files.end(),
            [this](const BencodeValue &file){
                auto file_info = std::get<std::unordered_map<std::string, BencodeValue>>(file.value);
                auto length_it = file_info.find("length");
                if (length_it == file_info.end())
                {
                    throw std::runtime_error("File info length key not found in torrent meta info");
                }
                auto path_it = file_info.find("path");
                if (path_it == file_info.end())
                {
                    throw std::runtime_error("File info path key not found in torrent meta info");
                }
                if (std::get<int64_t>(length_it->second.value) < 0)
                {
                    throw std::runtime_error("Invalid negative file length encountered");
                }

                /// We can safely cast length here
                meta_info_.files.value().push_back({
                    .length = static_cast<uint64_t>(std::get<int64_t>(length_it->second.value)),
                    .path = std::get<std::string>(path_it->second.value)});
        });
    }
    else
    {
        if (std::get<int64_t>(it->second.value) < 0)
        {
            throw std::runtime_error("Invalid negative file length encountered");
        }
        // length should always be positive so we can safely cast this
        meta_info_.length = std::make_optional(static_cast<int64_t>(std::get<int64_t>(it->second.value)));
    }

    it = info_dict.find("piece length");
    if (it == info_dict.end())
    {
        throw std::runtime_error("Failed to find piece length key in torrent");
    }
    if (std::get<int64_t>(it->second.value) < 0)
    {
        throw std::runtime_error("Invalid negative piece length encountered");
    }
    /// We can safely cast this here
    meta_info_.piece_length = std::get<int64_t>(it->second.value);

    it = info_dict.find("pieces");
    if (it == info_dict.end())
    {
        throw std::runtime_error("Failed to find pieces key in torrent");
    }
    /// TODO: clean this part up a bit
    //meta_info_.pieces.push_back({deserialize(std::get<std::string>(it->second.value))});
    auto str = std::get<std::string>(it->second.value);
    std::cout << "DEBUG: my str is = " << str << " and the size is = " << str.size() << "\n";
    //meta_info_.pieces.push_back(std::get<std::string>(it->second.value));
    meta_info_.pieces.push_back(str);
}

uint8_t TorrentManager::deserialize(const std::string &hash)
{
    /// Deserialize the SHA1 hash
}

void TorrentManager::errorDebug(const std::string &err) const
{
    std::cerr << "Malformed torrent: " << err << "\n";
}

} /// namespace bittorrent
