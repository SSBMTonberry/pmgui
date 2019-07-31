//
// Created by robin on 31.07.2019.
//

#define CATCH_CONFIG_MAIN
#include "../external_libs/include/catch.hpp"
#include <filesystem>

namespace fs = std::filesystem;

TEST_CASE( "Use existing base path if it exists, then get parent", "[simple][filesystem]" ) {
    fs::path path = fs::path("/home/robin/Music/NSFE");
    fs::path expected_parent_path = fs::path("/home/robin/Music");
    fs::path parent_path = path.parent_path();

    if(!fs::exists(path) || !fs::exists(expected_parent_path))
        SUCCEED("Cannot test: Paths in test are non-existent...");
    else
    {
        REQUIRE( expected_parent_path == parent_path );
    }
}

//TEST_CASE( "Get parent several times from specific paths - expect right parent", "[simple][filesystem]" ) {
//    //fs::path path = fs::path("/home/robin/Music/NSFE");
//    fs::path path = fs::path("/home/robin/Projects");
//    fs::path expected_parent_path = fs::path("/home/robin/Music");
//
//    if(!fs::exists(path) || !fs::exists(expected_parent_path))
//        SUCCEED("Cannot test: Paths in test are non-existent...");
//
//    std::string pathStr1 = path.u8string();
//    path = path.parent_path();
//    std::string pathStr2 = path.u8string();
//    fs::path newPath1 = fs::path("/home/robin/Music");
//    path = newPath1;
//    std::string pathStr3 = path.u8string();
//    fs::path newPath2 = fs::path("/home/robin/Music/NSFE");
//    path = newPath2;
//    std::string pathStr4 = path.u8string();
//    path = path.parent_path();
//    std::string pathStr5 = path.u8string();
//
//    REQUIRE( path == expected_parent_path );
//}