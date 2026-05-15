// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load at pugixml.cpp:7697:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::save_file at pugixml.cpp:7814:34 in pugixml.hpp
// pugi::xml_document::save_file at pugixml.cpp:7822:34 in pugixml.hpp
// pugi::xml_document::save at pugixml.cpp:7806:34 in pugixml.hpp
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
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <sstream>
#include <string>

static void writeDummyFile(const uint8_t* data, size_t size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(data), size);
        file.close();
    }
}

extern "C" int LLVMFuzzerTestOneInput_69(const uint8_t* data, size_t size) {
    // Ensure we have at least some data for parameters
    if (size < 1) return 0;
    
    // Use first byte for parameter randomization
    uint8_t param_seed = data[0];
    
    // Initialize parameters using the seed
    unsigned int options = static_cast<unsigned int>(param_seed);
    
    // Fix: Ensure encoding is valid (0-9 as per xml_encoding enum)
    uint8_t encoding_val = param_seed % 10; // 10 valid encodings (0-9)
    pugi::xml_encoding encoding = static_cast<pugi::xml_encoding>(encoding_val);
    
    // Prepare XML content (skip first byte used for parameters)
    const char* xml_content = reinterpret_cast<const char*>(data + 1);
    size_t xml_size = size > 1 ? size - 1 : 0;
    
    // Create a null-terminated string for load_string
    std::string xml_string;
    if (xml_size > 0) {
        xml_string.assign(xml_content, xml_size);
    } else {
        xml_string = "";
    }
    
    // Write dummy file for file-based operations
    writeDummyFile(data, size);
    
    // Test 1: load from stream
    {
        pugi::xml_document doc1;
        std::istringstream stream(xml_string);
        pugi::xml_parse_result result = doc1.load(stream, options, encoding);
        (void)result; // Use result to avoid unused variable warning
    }
    
    // Test 2: load_string
    {
        pugi::xml_document doc2;
        pugi::xml_parse_result result = doc2.load_string(xml_string.c_str(), options);
        (void)result;
        
        // Only try save operations if loading succeeded
        if (result.status == pugi::status_ok) {
            // Test save_file with char* path
            bool saved1 = doc2.save_file("./dummy_file_out", PUGIXML_TEXT("\t"), pugi::format_default, encoding);
            (void)saved1;
            
            // Test save_file with wchar_t* path
            bool saved2 = doc2.save_file(L"./dummy_file_out", PUGIXML_TEXT("\t"), pugi::format_default, encoding);
            (void)saved2;
            
            // Test save to stream
            std::wostringstream woss;
            doc2.save(woss, PUGIXML_TEXT("\t"), pugi::format_default);
        }
    }
    
    // Test 3: load_file
    {
        pugi::xml_document doc3;
        pugi::xml_parse_result result = doc3.load_file("./dummy_file", options, encoding);
        (void)result;
    }
    
    // Test 4: Another load from stream with different parameters
    {
        pugi::xml_document doc4;
        std::istringstream stream2(xml_string);
        // Use different options by XORing with size
        unsigned int options2 = options ^ static_cast<unsigned int>(size);
        pugi::xml_parse_result result = doc4.load(stream2, options2, encoding);
        (void)result;
    }
    
    return 0;
}