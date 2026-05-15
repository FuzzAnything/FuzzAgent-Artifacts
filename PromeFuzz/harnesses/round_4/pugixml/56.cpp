// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load at pugixml.cpp:7704:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::save_file at pugixml.cpp:7814:34 in pugixml.hpp
// pugi::xml_document::save_file at pugixml.cpp:7822:34 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
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
#include <cstring>
#include <fstream>
#include <sstream>
#include <string>

extern "C" int LLVMFuzzerTestOneInput_56(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Initialize xml_document
    pugi::xml_document doc;
    
    // Prepare XML content from fuzzer input
    std::string xml_content(reinterpret_cast<const char*>(Data), Size);
    
    // Test load_string
    try {
        pugi::xml_parse_result result = doc.load_string(xml_content.c_str());
        // Result can be checked but we don't need to act on it for fuzzing
    } catch (...) {
        // Ignore exceptions
    }
    
    // Test save_file overloads
    const char* char_path = "./dummy_file";
    const wchar_t* wchar_path = L"./dummy_file";
    
    // Use random values for parameters
    unsigned int random_flags = *reinterpret_cast<const unsigned int*>(Data) % 256;
    unsigned int random_options = *reinterpret_cast<const unsigned int*>(Data) % 256;
    
    // Test save_file with char path
    try {
        bool saved = doc.save_file(char_path, PUGIXML_TEXT("\t"), random_flags, pugi::encoding_auto);
        // Result can be ignored
    } catch (...) {
        // Ignore exceptions
    }
    
    // Test save_file with wchar_t path
    try {
        bool saved = doc.save_file(wchar_path, PUGIXML_TEXT("\t"), random_flags, pugi::encoding_auto);
        // Result can be ignored
    } catch (...) {
        // Ignore exceptions
    }
    
    // Test load_file
    // First write XML content to file
    std::ofstream out_file(char_path, std::ios::binary);
    if (out_file) {
        out_file.write(xml_content.c_str(), xml_content.size());
        out_file.close();
        
        // Generate random encoding value (0-9)
        uint8_t encoding_byte = Data[0] % 10;
        pugi::xml_encoding encoding = static_cast<pugi::xml_encoding>(encoding_byte);
        
        // Validate encoding
        if (encoding >= pugi::encoding_auto && encoding <= pugi::encoding_latin1) {
            try {
                pugi::xml_parse_result result = doc.load_file(char_path, random_options, encoding);
                // Result can be checked but we don't need to act on it for fuzzing
            } catch (...) {
                // Ignore exceptions
            }
        }
    }
    
    // Test load with char stream
    {
        std::istringstream char_stream(xml_content);
        
        // Generate random encoding value (0-9)
        uint8_t encoding_byte = Data[Size > 1 ? 1 : 0] % 10;
        pugi::xml_encoding encoding = static_cast<pugi::xml_encoding>(encoding_byte);
        
        // Validate encoding
        if (encoding >= pugi::encoding_auto && encoding <= pugi::encoding_latin1) {
            try {
                pugi::xml_parse_result result = doc.load(char_stream, random_options, encoding);
                // Result can be checked but we don't need to act on it for fuzzing
            } catch (...) {
                // Ignore exceptions
            }
        }
    }
    
    // Test load with wchar_t stream
    {
        // Convert input to wchar_t (simple conversion for fuzzing)
        std::wstring wxml_content;
        for (size_t i = 0; i < Size && i < 100; ++i) {
            wxml_content.push_back(static_cast<wchar_t>(Data[i]));
        }
        
        std::wistringstream wchar_stream(wxml_content);
        
        try {
            pugi::xml_parse_result result = doc.load(wchar_stream, random_options);
            // Result can be checked but we don't need to act on it for fuzzing
        } catch (...) {
            // Ignore exceptions
        }
    }
    
    // Clean up dummy file
    std::remove(char_path);
    
    return 0;
}