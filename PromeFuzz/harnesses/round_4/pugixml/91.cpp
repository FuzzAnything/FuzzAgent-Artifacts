// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::save_file at pugixml.cpp:7814:34 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7496:34 in pugixml.hpp
// pugi::xml_document::load at pugixml.cpp:7724:46 in pugixml.hpp
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
#include <cstddef>
#include <fstream>
#include <sstream>
#include <cstring>

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

extern "C" int LLVMFuzzerTestOneInput_91(const uint8_t* Data, size_t Size) {
    // Write data to dummy file for file-based operations
    writeDummyFile(Data, Size);
    
    // Initialize random parameters from input data
    unsigned int options = 0;
    pugi::xml_encoding encoding = pugi::encoding_auto;
    
    if (Size >= 2) {
        options = Data[0];
        uint8_t enc_byte = Data[1] % 10; // Ensure valid encoding (0-9)
        encoding = static_cast<pugi::xml_encoding>(enc_byte);
    }
    
    // Create XML documents
    pugi::xml_document doc1;
    pugi::xml_document doc2;
    
    // Test load_string with the input data (ensure null termination)
    if (Size > 0) {
        // Create a null-terminated string from input data
        char* str = new char[Size + 1];
        memcpy(str, Data, Size);
        str[Size] = '\0';
        
        pugi::xml_parse_result result1 = doc1.load_string(str, options);
        
        // Test save_file if load succeeded
        if (result1) {
            bool save_result = doc1.save_file("./dummy_file", "\t", pugi::format_default, encoding);
            (void)save_result; // Use result to avoid unused variable warning
        }
        
        delete[] str;
    }
    
    // Test load_file with dummy file
    if (isValidEncoding(encoding)) {
        pugi::xml_parse_result result2 = doc2.load_file("./dummy_file", options, encoding);
        (void)result2;
    }
    
    // Test reset function
    if (!doc1.empty()) {
        doc2.reset(doc1);
    }
    
    // Test load (deprecated string overload)
    if (Size > 0) {
        char* str2 = new char[Size + 1];
        memcpy(str2, Data, Size);
        str2[Size] = '\0';
        
        pugi::xml_document doc3;
        pugi::xml_parse_result result3 = doc3.load(str2, options);
        (void)result3;
        
        delete[] str2;
    }
    
    // Test load (stream overload)
    if (Size > 0) {
        std::istringstream stream(std::string(reinterpret_cast<const char*>(Data), Size));
        pugi::xml_document doc4;
        
        if (isValidEncoding(encoding)) {
            pugi::xml_parse_result result4 = doc4.load(stream, options, encoding);
            (void)result4;
        }
    }
    
    return 0;
}