// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load at pugixml.cpp:7724:46 in pugixml.hpp
// pugi::xml_document::load at pugixml.cpp:7697:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::save_file at pugixml.cpp:7814:34 in pugixml.hpp
// pugi::xml_document::save_file at pugixml.cpp:7822:34 in pugixml.hpp
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
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

static void write_dummy_file(const uint8_t* data, size_t size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(data), size);
        file.close();
    }
}

static void remove_dummy_file() {
    std::remove("./dummy_file");
}

static pugi::xml_encoding get_random_encoding(uint8_t value) {
    const pugi::xml_encoding encodings[] = {
        pugi::encoding_auto,
        pugi::encoding_utf8,
        pugi::encoding_utf16_le,
        pugi::encoding_utf16_be,
        pugi::encoding_utf16,
        pugi::encoding_utf32_le,
        pugi::encoding_utf32_be,
        pugi::encoding_utf32,
        pugi::encoding_wchar,
        pugi::encoding_latin1
    };
    return encodings[value % (sizeof(encodings) / sizeof(encodings[0]))];
}

extern "C" int LLVMFuzzerTestOneInput_57(const uint8_t* data, size_t size) {
    // Write fuzzer data to dummy file for file-based operations
    write_dummy_file(data, size);
    
    // Extract random values for parameters
    unsigned int options = 0;
    pugi::xml_encoding encoding = pugi::encoding_auto;
    unsigned int flags = 0;
    
    if (size > 0) {
        options = static_cast<unsigned int>(data[0]);
        if (size > 1) {
            encoding = get_random_encoding(data[1]);
            if (size > 2) {
                flags = static_cast<unsigned int>(data[2]);
            }
        }
    }
    
    // Test 1: pugi::xml_document::load_file
    {
        pugi::xml_document doc1;
        pugi::xml_parse_result result = doc1.load_file("./dummy_file", options, encoding);
        (void)result; // Use result to avoid unused variable warning
    }
    
    // Test 2: pugi::xml_document::save_file (both overloads)
    {
        pugi::xml_document doc2;
        // First load some content to save
        if (size > 0) {
            std::string content(reinterpret_cast<const char*>(data), size);
            doc2.load_string(content.c_str(), options);
        }
        
        // char* overload
        bool save_result1 = doc2.save_file("./dummy_file", PUGIXML_TEXT("\t"), flags, encoding);
        (void)save_result1;
        
        // wchar_t* overload
        bool save_result2 = doc2.save_file(L"./dummy_file", PUGIXML_TEXT("\t"), flags, encoding);
        (void)save_result2;
    }
    
    // Test 3: pugi::xml_document::load (both overloads)
    {
        pugi::xml_document doc3;
        
        // First overload (deprecated): load from zero-terminated string
        if (size > 0) {
            // Create null-terminated string
            std::vector<char> buffer(data, data + size);
            buffer.push_back('\0');
            pugi::xml_parse_result result1 = doc3.load(buffer.data(), options);
            (void)result1;
        }
        
        pugi::xml_document doc4;
        // Second overload: load from stream
        if (size > 0) {
            std::string content(reinterpret_cast<const char*>(data), size);
            std::istringstream stream(content);
            pugi::xml_parse_result result2 = doc4.load(stream, options, encoding);
            (void)result2;
        }
    }
    
    // Test 4: pugi::xml_document::load_string
    {
        pugi::xml_document doc5;
        if (size > 0) {
            // Create null-terminated string
            std::vector<char> buffer(data, data + size);
            buffer.push_back('\0');
            pugi::xml_parse_result result = doc5.load_string(buffer.data(), options);
            (void)result;
        }
    }
    
    // Clean up
    remove_dummy_file();
    
    return 0;
}