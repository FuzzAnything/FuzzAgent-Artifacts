// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::save at pugixml.cpp:7799:34 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::save_file at pugixml.cpp:7814:34 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::save_file at pugixml.cpp:7822:34 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::load at pugixml.cpp:7697:46 in pugixml.hpp
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
#include <fstream>
#include <sstream>
#include <cstring>

static void write_dummy_file(const uint8_t* data, size_t size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file) {
        file.write(reinterpret_cast<const char*>(data), size);
    }
}

extern "C" int LLVMFuzzerTestOneInput_80(const uint8_t* Data, size_t Size) {
    // Prepare dummy file
    write_dummy_file(Data, Size);
    
    // Extract parameters from fuzzer input
    unsigned int options = 0;
    pugi::xml_encoding encoding = pugi::encoding_auto;
    unsigned int flags = 0;
    
    if (Size >= 1) {
        options = Data[0];
        flags = Data[0];
    }
    if (Size >= 2) {
        uint8_t enc_byte = Data[1] % 10; // Ensure valid encoding (0-9)
        encoding = static_cast<pugi::xml_encoding>(enc_byte);
    }
    
    // Test xml_document::save_file (char* version)
    {
        pugi::xml_document doc;
        // Try to load some content first to have something to save
        if (Size > 2) {
            const char* str_data = reinterpret_cast<const char*>(Data + 2);
            size_t str_size = Size - 2;
            std::vector<char> buffer(str_data, str_data + str_size);
            if (!buffer.empty()) {
                buffer.push_back('\0');
                doc.load_string(&buffer[0], options);
            }
        }
        doc.save_file("./dummy_file_save", PUGIXML_TEXT("\t"), flags, encoding);
    }
    
    // Test xml_document::save_file (wchar_t* version)
    {
        pugi::xml_document doc;
        if (Size > 2) {
            const char* str_data = reinterpret_cast<const char*>(Data + 2);
            size_t str_size = Size - 2;
            std::vector<char> buffer(str_data, str_data + str_size);
            if (!buffer.empty()) {
                buffer.push_back('\0');
                doc.load_string(&buffer[0], options);
            }
        }
        doc.save_file(L"./dummy_file_save_w", PUGIXML_TEXT("\t"), flags, encoding);
    }
    
    // Test xml_document::load_file
    {
        pugi::xml_document doc;
        if (encoding >= pugi::encoding_auto && encoding <= pugi::encoding_latin1) {
            doc.load_file("./dummy_file", options, encoding);
        }
    }
    
    // Test xml_document::load_string
    {
        pugi::xml_document doc;
        if (Size > 0) {
            // Create null-terminated string from input
            std::vector<char> buffer(Data, Data + Size);
            buffer.push_back('\0');
            doc.load_string(&buffer[0], options);
        }
    }
    
    // Test xml_document::load (stream version)
    {
        pugi::xml_document doc;
        if (encoding >= pugi::encoding_auto && encoding <= pugi::encoding_latin1) {
            std::istringstream stream(std::string(reinterpret_cast<const char*>(Data), Size));
            doc.load(stream, options, encoding);
        }
    }
    
    // Test xml_document::save (stream version)
    {
        pugi::xml_document doc;
        // Try to load some content first
        if (Size > 2) {
            const char* str_data = reinterpret_cast<const char*>(Data + 2);
            size_t str_size = Size - 2;
            std::vector<char> buffer(str_data, str_data + str_size);
            if (!buffer.empty()) {
                buffer.push_back('\0');
                doc.load_string(&buffer[0], options);
            }
        }
        std::ostringstream stream;
        doc.save(stream, PUGIXML_TEXT("\t"), flags, encoding);
    }
    
    return 0;
}