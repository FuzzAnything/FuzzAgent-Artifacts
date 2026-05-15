// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7496:34 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_document::load at pugixml.cpp:7724:46 in pugixml.hpp
// pugi::xml_document::load at pugixml.cpp:7697:46 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_document::save_file at pugixml.cpp:7814:34 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7490:34 in pugixml.hpp
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include "pugixml.hpp"
#include <cstdint>
#include <cstring>
#include <fstream>
#include <sstream>
#include <vector>

static void write_dummy_file(const uint8_t *Data, size_t Size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file) {
        file.write(reinterpret_cast<const char*>(Data), Size);
    }
}

extern "C" int LLVMFuzzerTestOneInput_83(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Prepare dummy file
    write_dummy_file(Data, Size);

    // Generate parameters from fuzzer input
    unsigned int options = 0;
    if (Size >= sizeof(unsigned int)) {
        memcpy(&options, Data, sizeof(unsigned int));
    }
    
    // Ensure encoding is valid (0-9)
    pugi::xml_encoding encoding = pugi::encoding_auto;
    if (Size >= 1) {
        uint8_t enc_byte = Data[0] % 10; // 0-9
        encoding = static_cast<pugi::xml_encoding>(enc_byte);
    }

    // Create null-terminated string for string APIs
    std::vector<char> buffer(Data, Data + Size);
    buffer.push_back('\0');
    const char* contents = buffer.data();

    try {
        // Test load_string
        {
            pugi::xml_document doc1;
            pugi::xml_parse_result result = doc1.load_string(contents, options);
            (void)result;
        }

        // Test load_file
        {
            pugi::xml_document doc2;
            if (encoding >= pugi::encoding_auto && encoding <= pugi::encoding_latin1) {
                pugi::xml_parse_result result = doc2.load_file("./dummy_file", options, encoding);
                (void)result;
            }
        }

        // Test load (deprecated string overload)
        {
            pugi::xml_document doc3;
            pugi::xml_parse_result result = doc3.load(contents, options);
            (void)result;
        }

        // Test load (stream overload)
        {
            pugi::xml_document doc4;
            std::istringstream stream(std::string(reinterpret_cast<const char*>(Data), Size));
            if (encoding >= pugi::encoding_auto && encoding <= pugi::encoding_latin1) {
                pugi::xml_parse_result result = doc4.load(stream, options, encoding);
                (void)result;
            }
        }

        // Test save_file - first load something, then save
        {
            pugi::xml_document doc5;
            if (encoding >= pugi::encoding_auto && encoding <= pugi::encoding_latin1) {
                pugi::xml_parse_result load_result = doc5.load_file("./dummy_file", options, encoding);
                if (load_result) {
                    bool save_result = doc5.save_file("./dummy_file_save", "\t", options, encoding);
                    (void)save_result;
                }
            }
        }

        // Test reset() without parameter
        {
            pugi::xml_document doc6;
            doc6.load_string(contents, options);
            doc6.reset();
        }

        // Test reset(const xml_document&)
        {
            pugi::xml_document doc7;
            doc7.load_string(contents, options);
            
            pugi::xml_document doc8;
            doc8.reset(doc7);
        }

    } catch (...) {
        // Catch any exceptions to prevent crashes
    }

    return 0;
}