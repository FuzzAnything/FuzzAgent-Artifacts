// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::save at pugixml.cpp:7799:34 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::load at pugixml.cpp:7697:46 in pugixml.hpp
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
#include "pugixml.hpp"
#include <fstream>
#include <sstream>
#include <cstdint>
#include <cstring>

static void writeDummyFile(const uint8_t* data, size_t size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file) {
        file.write(reinterpret_cast<const char*>(data), size);
    }
}

static void removeDummyFile() {
    std::remove("./dummy_file");
}

extern "C" int LLVMFuzzerTestOneInput_52(const uint8_t* Data, size_t Size) {
    // Write input to dummy file for file-based operations
    writeDummyFile(Data, Size);
    
    // Initialize parameters from fuzzer input
    unsigned int options = 0;
    pugi::xml_encoding encoding = pugi::encoding_auto;
    
    if (Size > 0) {
        options = Data[0];
    }
    if (Size > 1) {
        // Ensure encoding is within valid range (0-9)
        encoding = static_cast<pugi::xml_encoding>(Data[1] % 10);
    }
    
    // Test load_string
    {
        pugi::xml_document doc1;
        // Create null-terminated string from input
        std::string str(reinterpret_cast<const char*>(Data), Size);
        pugi::xml_parse_result result = doc1.load_string(str.c_str(), options);
        (void)result; // Use result to avoid unused variable warning
    }
    
    // Test load from stream
    {
        pugi::xml_document doc2;
        std::istringstream stream(std::string(reinterpret_cast<const char*>(Data), Size));
        if (encoding >= pugi::encoding_auto && encoding <= pugi::encoding_latin1) {
            pugi::xml_parse_result result = doc2.load(stream, options, encoding);
            (void)result;
        }
    }
    
    // Test load_file
    {
        pugi::xml_document doc3;
        if (encoding >= pugi::encoding_auto && encoding <= pugi::encoding_latin1) {
            pugi::xml_parse_result result = doc3.load_file("./dummy_file", options, encoding);
            (void)result;
        }
    }
    
    // Test save_file - first create a document to save
    {
        pugi::xml_document doc4;
        std::string str(reinterpret_cast<const char*>(Data), Size);
        doc4.load_string(str.c_str(), options);
        
        // Generate save parameters from input
        unsigned int flags = pugi::format_default;
        pugi::xml_encoding save_encoding = pugi::encoding_auto;
        
        if (Size > 2) {
            flags = Data[2];
        }
        if (Size > 3) {
            save_encoding = static_cast<pugi::xml_encoding>(Data[3] % 10);
        }
        
        // Test both overloads of save_file
        if (save_encoding >= pugi::encoding_auto && save_encoding <= pugi::encoding_latin1) {
            bool saved1 = doc4.save_file("./dummy_file_save", PUGIXML_TEXT("\t"), flags, save_encoding);
            (void)saved1;
            
            bool saved2 = doc4.save_file(L"./dummy_file_save_w", PUGIXML_TEXT("\t"), flags, save_encoding);
            (void)saved2;
            
            // Clean up saved files
            std::remove("./dummy_file_save");
            std::remove("./dummy_file_save_w");
        }
    }
    
    // Test save to stream
    {
        pugi::xml_document doc5;
        std::string str(reinterpret_cast<const char*>(Data), Size);
        doc5.load_string(str.c_str(), options);
        
        unsigned int flags = pugi::format_default;
        pugi::xml_encoding save_encoding = pugi::encoding_auto;
        
        if (Size > 2) {
            flags = Data[2];
        }
        if (Size > 3) {
            save_encoding = static_cast<pugi::xml_encoding>(Data[3] % 10);
        }
        
        if (save_encoding >= pugi::encoding_auto && save_encoding <= pugi::encoding_latin1) {
            std::ostringstream out_stream;
            doc5.save(out_stream, PUGIXML_TEXT("\t"), flags, save_encoding);
        }
    }
    
    // Clean up dummy file
    removeDummyFile();
    
    return 0;
}