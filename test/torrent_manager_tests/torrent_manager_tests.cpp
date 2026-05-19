#include <gtest/gtest.h>
#include <Bencode/TorrentManager.hpp>
#include <Bencode/TorrentMetaData.hpp>

TEST(TorrentManagerTests, readOneFileTorrent)
{
    bittorrent::TorrentManager manager;
    manager.loadTorrent("test.torrent");
    manager.readTorrent();
    auto meta_data = manager.getMetaData(); 

   ASSERT(meta_data.announce, "http://tracker.example.com") 
   ASSERT(meta_data.length, 12345); 
   ASSERT(meta_data.name, "test.txt");
   ASSERT(meta_data.piece_length, 13684); 
   ASSERT(meta_data.pieces.size(), 1);
   ASSERT(meta_data.pieces[1],"12345678901234567890"); 
}
