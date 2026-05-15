// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load at pugixml.cpp:7724:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::save at pugixml.cpp:7806:34 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::save_file at pugixml.cpp:7814:34 in pugixml.hpp
// pugi::xml_document::save_file at pugixml.cpp:7822:34 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
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
#include <cstdlib>

static void writeDummyFile(const uint8_t* data, size_t size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(data), size);
        file.close();
    }
}

static bool isValidEncoding(pugi::xml_encoding encoding) {
    return encoding >= pugi::encoding_auto && encoding <= pugi::encoding_latin1;
}

extern "C" int LLVMFuzzerTestOneInput_83(const uint8_t* Data, size_t Size) {
    // Write fuzzer input to dummy file for file operations
    writeDummyFile(Data, Size);
    
    // Initialize parameters from fuzzer input
    unsigned int options = 0;
    unsigned int flags = 0;
    pugi::xml_encoding encoding = pugi::encoding_auto;
    
    if (Size >= 1) {
        options = Data[0];
        flags = Data[0];
    }
    
    if (Size >= 2) {
        uint8_t enc_val = Data[1] % 10; // Ensure value 0-9
        if (enc_val <= static_cast<uint8_t>(pugi::encoding_latin1)) {
            encoding = static_cast<pugi::xml_encoding>(enc_val);
        }
    }
    
    // Use PUGIXML_TEXT macro for proper char_t type
    const pugi::char_t* indent = PUGIXML_TEXT("\t");
    
    // Test save_file overloads
    {
        pugi::xml_document doc;
        // Try to load some XML first to have content to save
        const char* test_xml = "<root><child/></root>";
        doc.load_string(test_xml);
        
        // char* path version
        doc.save_file("./dummy_file", indent, flags, encoding);
        
        // wchar_t* path version
        doc.save_file(L"./dummy_file", indent, flags, encoding);
    }
    
    // Test load_file
    {
        pugi::xml_document doc;
        if (isValidEncoding(encoding)) {
            pugi::xml_parse_result result = doc.load_file("./dummy_file", options, encoding);
            (void)result; // Use result to avoid unused variable warning
        }
    }
    
    // Test load_string
    {
        pugi::xml_document doc;
        // Create null-terminated string from fuzzer input
        if (Size > 0) {
            char* str = new char[Size + 1];
            memcpy(str, Data, Size);
            str[Size] = '\0';
            
            pugi::xml_parse_result result = doc.load_string(str, options);
            (void)result;
            
            delete[] str;
        }
    }
    
    // Test load (deprecated)
    {
        pugi::xml_document doc;
        if (Size > 0) {
            char* str = new char[Size + 1];
            memcpy(str, Data, Size);
            str[Size] = '\0';
            
            pugi::xml_parse_result result = doc.load(str, options);
            (void)result;
            
            delete[] str;
        }
    }
    
    // Test save to stream
    {
        pugi::xml_document doc;
        // Load some content first
        const char* test_xml = "<root><child/></root>";
        doc.load_string(test_xml);
        
        std::basic_ostringstream<wchar_t> wstream;
        doc.save(wstream, indent, flags);
    }
    
    // Clean up
    std::remove("./dummy_file");
    
    return 0;
}