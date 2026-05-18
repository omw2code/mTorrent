#ifndef __TORRENT_META_INFO_HPP__
#define __TORRENT_META_INFO_HPP__

#include <cstdint>
#include <string>
#include <vector>
#include <optional>

namespace bittorrent
{

class TorrentMetaInfo
{
public:
    TorrentMetaInfo()
        : announce{}
        , name {}
        , piece_length{}
        , pieces{}
        , length{}
        , files{}
    {};

    using SHA1hash = std::array<uint8_t, 20>;

    /// For the purposes of the other keys, the multi-file case is 
    /// treated as only having a single file by concatenating the files 
    /// in the order they appear in the files list. The files list is 
    /// the value files maps to, and is a list of dictionaries containing the following keys:
    /// length - The length of the file, in bytes.
    /// path - A list of UTF-8 encoded strings corresponding to subdirectory names, the last 
    /// of which is the actual file name (a zero length list is an error case).
    struct File
    {
        uint64_t length{};
        std::string path{};
    };

    /// URL of the tracker
    std::string announce;
    
    /// UTF-8 encoded string which is the suggested 
    /// name to save the file (or directory) as
    std::string name;
    
    /// maps to the number of bytes in each piece the file is split 
    ///into. For the purposes of transfer, files are split into 
    ///fixed-size pieces which are all the same length except for 
    /// possibly the last one which may be truncated. piece length 
    /// is almost always a power of two, most commonly 2 18 = 256 K 
    /// (BitTorrent prior to version 3.2 uses 2 20 = 1 M as default).
    uint64_t piece_length;

    /// pieces maps to a string whose length is a multiple of 20. 
    /// It is to be subdivided into strings of length 20, each of 
    /// which is the SHA1 hash of the piece at the corresponding index.
    std::vector<SHA1hash> pieces;
    
    /// There is also a key length or a key files, but not both or neither. 
    /// If length is present then the download represents a single file, 
    /// otherwise it represents a set of files which go in a directory structure.
    std::optional<uint64_t> length;
    std::optional<std::vector<File>> files;
};

} /// namespace bittorrent

#endif
