#include <filesystem>
#include <gtest/gtest.h>
#include <bencode/TorrentManager.hpp>
#include <bencode/TorrentMetaInfo.hpp>

TEST(TorrentManagerTests, readOneFileTorrent)
{
    namespace fs = std::filesystem;
    bittorrent::TorrentManager manager{};
    fs::path test_file = fs::path(TEST_DATA_DIR) / "single_file.torrent";
    manager.loadTorrent(test_file.string());
    manager.readTorrent();
    auto meta_info = manager.getMetaInfo(); 

   ASSERT_EQ(meta_info.announce, "http://tracker.example.com"); 
   ASSERT_EQ(meta_info.length, 12345); 
   ASSERT_EQ(meta_info.name, "test.txt");
   ASSERT_EQ(meta_info.piece_length, 16384); 
   ASSERT_EQ(meta_info.pieces.size(), 1);
   ASSERT_EQ(meta_info.pieces[0],"12345678901234567890"); 
}

TEST(TorrentManagerTests, readMultiFileTorrent)
{
    bittorrent::TorrentManager manager{};
    manager.loadTorrent("multi_file.torrent");
    manager.readTorrent();
    auto meta_info = manager.getMetaInfo();

    ASSERT_EQ(meta_info.announce, "http://tracker.example.com");
}
