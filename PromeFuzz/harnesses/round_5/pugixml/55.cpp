// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load at pugixml.cpp:7697:46 in pugixml.hpp
// pugi::xml_document::save_file at pugixml.cpp:7814:34 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7496:34 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_document::load at pugixml.cpp:7697:46 in pugixml.hpp
// pugi::xml_document::load at pugixml.cpp:7704:46 in pugixml.hpp
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include "src/pugixml.hpp"
#include <cstdint>
#include <cstring>
#include <fstream>
#include <sstream>
#include <string>

extern "C" int LLVMFuzzerTestOneInput_55(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Create XML document objects
    pugi::xml_document doc1;
    pugi::xml_document doc2;
    pugi::xml_document doc3;
    pugi::xml_document doc4;
    pugi::xml_document doc5;
    pugi::xml_document doc6;
    pugi::xml_document doc_reset;

    // Extract parameters from fuzz data
    unsigned int options = 0;
    pugi::xml_encoding encoding = pugi::encoding_auto;
    
    if (Size >= 2) {
        // Use first byte for options (any value is valid)
        options = static_cast<unsigned int>(Data[0]);
        
        // Use second byte for encoding, but limit to valid enum range (0-9)
        uint8_t encoding_byte = Data[1] % 10; // 0-9
        encoding = static_cast<pugi::xml_encoding>(encoding_byte);
    }

    // Prepare XML content (null-terminated string)
    std::string xml_content(reinterpret_cast<const char*>(Data), Size);
    const char* xml_cstr = xml_content.c_str();

    // 1. Test load_string
    try {
        pugi::xml_parse_result result1 = doc1.load_string(xml_cstr, options);
        (void)result1; // Use result to avoid unused variable warning
    } catch (...) {
        // Ignore exceptions
    }

    // 2. Test load_file
    // Write data to dummy file
    static const char* dummy_file = "./dummy_file";
    std::ofstream out_file(dummy_file, std::ios::binary);
    if (out_file) {
        out_file.write(reinterpret_cast<const char*>(Data), Size);
        out_file.close();
        
        try {
            // Validate encoding before use
            if (encoding >= pugi::encoding_auto && encoding <= pugi::encoding_latin1) {
                pugi::xml_parse_result result2 = doc2.load_file(dummy_file, options, encoding);
                (void)result2;
            }
        } catch (...) {
            // Ignore exceptions
        }
    }

    // 3. Test load with char stream
    try {
        std::istringstream char_stream(xml_content);
        if (encoding >= pugi::encoding_auto && encoding <= pugi::encoding_latin1) {
            pugi::xml_parse_result result3 = doc3.load(char_stream, options, encoding);
            (void)result3;
        }
    } catch (...) {
        // Ignore exceptions
    }

    // 4. Test load with wchar_t stream
    try {
        // Convert to wstring (simple conversion, may not be semantically correct)
        std::wstring wxml_content(xml_content.begin(), xml_content.end());
        std::wistringstream wchar_stream(wxml_content);
        pugi::xml_parse_result result4 = doc4.load(wchar_stream, options);
        (void)result4;
    } catch (...) {
        // Ignore exceptions
    }

    // 5. Test save_file
    try {
        // First load some content to save
        std::istringstream char_stream2(xml_content);
        if (encoding >= pugi::encoding_auto && encoding <= pugi::encoding_latin1) {
            pugi::xml_parse_result load_result = doc5.load(char_stream2, options, encoding);
            if (load_result) {
                static const char* save_file = "./dummy_save_file";
                bool save_result = doc5.save_file(save_file, "\t", pugi::format_default, encoding);
                (void)save_result;
            }
        }
    } catch (...) {
        // Ignore exceptions
    }

    // 6. Test reset
    try {
        // Load some content into doc_reset
        pugi::xml_parse_result load_result = doc_reset.load_string(xml_cstr, options);
        if (load_result) {
            doc6.reset(doc_reset);
        }
    } catch (...) {
        // Ignore exceptions
    }

    return 0;
}