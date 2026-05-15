// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::save_file at pugixml.cpp:7814:34 in pugixml.hpp
// pugi::xml_document::save_file at pugixml.cpp:7822:34 in pugixml.hpp
// pugi::xml_document::load at pugixml.cpp:7724:46 in pugixml.hpp
// pugi::xml_document::load at pugixml.cpp:7697:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
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
#include <fstream>
#include <sstream>
#include <cstdint>
#include <cstring>
#include <string>

extern "C" int LLVMFuzzerTestOneInput_61(const uint8_t *Data, size_t Size) {
    // Create dummy file path
    const char* dummy_file = "./dummy_file";
    const wchar_t* dummy_file_w = L"./dummy_file";
    
    // Write fuzz data to dummy file
    std::ofstream file(dummy_file, std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(Data), Size);
        file.close();
    }
    
    // Initialize parameters using fuzz data
    unsigned int options = 0;
    pugi::xml_encoding encoding = pugi::encoding_auto;
    
    if (Size >= sizeof(unsigned int)) {
        // Use first 4 bytes for options
        std::memcpy(&options, Data, sizeof(unsigned int));
    }
    
    if (Size >= sizeof(unsigned int) + 1) {
        // Use next byte for encoding, validate it's in range 0-9
        uint8_t enc_byte = Data[sizeof(unsigned int)];
        if (enc_byte <= pugi::encoding_latin1) {
            encoding = static_cast<pugi::xml_encoding>(enc_byte);
        }
    }
    
    // Create null-terminated string from fuzz data for string-based functions
    std::string xml_string;
    if (Size > 0) {
        xml_string.assign(reinterpret_cast<const char*>(Data), std::min(Size, static_cast<size_t>(1024)));
        // Ensure null termination for string functions
        if (xml_string.size() < 1024) {
            xml_string.push_back('\0');
        }
    } else {
        xml_string = "";
    }
    
    // Test save_file overloads
    {
        pugi::xml_document doc;
        // Try to load some content first
        doc.load_string("<root/>");
        
        // Test char* version
        doc.save_file(dummy_file, PUGIXML_TEXT("\t"), pugi::format_default, encoding);
        
        // Test wchar_t* version - use PUGIXML_TEXT for indent parameter
        doc.save_file(dummy_file_w, PUGIXML_TEXT("\t"), pugi::format_default, encoding);
    }
    
    // Test load overloads
    {
        pugi::xml_document doc;
        
        // Test deprecated load from string (char_t* version)
        if (!xml_string.empty()) {
            doc.load(xml_string.c_str(), options);
        }
        
        // Test load from stream
        std::istringstream stream(std::string(reinterpret_cast<const char*>(Data), Size));
        doc.load(stream, options, encoding);
    }
    
    // Test load_string
    {
        pugi::xml_document doc;
        if (!xml_string.empty()) {
            doc.load_string(xml_string.c_str(), options);
        }
    }
    
    // Test load_file
    {
        pugi::xml_document doc;
        
        // Validate encoding parameter
        if (encoding >= pugi::encoding_auto && encoding <= pugi::encoding_latin1) {
            doc.load_file(dummy_file, options, encoding);
        }
    }
    
    // Clean up dummy file
    std::remove(dummy_file);
    
    return 0;
}