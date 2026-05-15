// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load at pugixml.cpp:7697:46 in pugixml.hpp
// pugi::get_memory_allocation_function at pugixml.cpp:7873:52 in pugixml.hpp
// pugi::xml_document::load_buffer_inplace_own at pugixml.cpp:7763:46 in pugixml.hpp
// pugi::xml_document::load_buffer_inplace at pugixml.cpp:7756:46 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
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
#include <cstdint>
#include <cstring>
#include <fstream>
#include <sstream>
#include <cstdlib>

extern "C" int LLVMFuzzerTestOneInput_73(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Generate random options and encoding from input data
    unsigned int options = 0;
    pugi::xml_encoding encoding = pugi::encoding_auto;
    
    if (Size >= 1) {
        options = Data[0];
    }
    
    if (Size >= 2) {
        uint8_t enc_val = Data[1] % 10; // Ensure encoding is 0-9
        encoding = static_cast<pugi::xml_encoding>(enc_val);
    }

    // Create XML content from remaining data
    const char* xml_content = reinterpret_cast<const char*>(Data);
    size_t xml_size = Size;

    // Test 1: load from stream
    {
        pugi::xml_document doc1;
        std::istringstream stream(std::string(xml_content, xml_size));
        if (encoding >= pugi::encoding_auto && encoding <= pugi::encoding_latin1) {
            doc1.load(stream, options, encoding);
        }
    }

    // Test 2: load_buffer_inplace_own
    {
        pugi::xml_document doc2;
        // Allocate buffer with pugixml's allocator
        void* buffer = pugi::get_memory_allocation_function()(xml_size);
        if (buffer) {
            std::memcpy(buffer, xml_content, xml_size);
            if (encoding >= pugi::encoding_auto && encoding <= pugi::encoding_latin1) {
                doc2.load_buffer_inplace_own(buffer, xml_size, options, encoding);
            }
        }
    }

    // Test 3: load_buffer_inplace
    {
        pugi::xml_document doc3;
        // Allocate buffer that persists for document lifetime
        char* buffer = new char[xml_size];
        std::memcpy(buffer, xml_content, xml_size);
        if (encoding >= pugi::encoding_auto && encoding <= pugi::encoding_latin1) {
            doc3.load_buffer_inplace(buffer, xml_size, options, encoding);
        }
        delete[] buffer;
    }

    // Test 4: load_buffer
    {
        pugi::xml_document doc4;
        if (encoding >= pugi::encoding_auto && encoding <= pugi::encoding_latin1) {
            doc4.load_buffer(xml_content, xml_size, options, encoding);
        }
    }

    // Test 5: load_string
    {
        pugi::xml_document doc5;
        // Create null-terminated string
        std::string str(xml_content, xml_size);
        doc5.load_string(str.c_str(), options);
    }

    // Test 6: load_file
    {
        pugi::xml_document doc6;
        // Write data to file
        std::ofstream file("./dummy_file", std::ios::binary);
        if (file.is_open()) {
            file.write(xml_content, xml_size);
            file.close();
            
            if (encoding >= pugi::encoding_auto && encoding <= pugi::encoding_latin1) {
                doc6.load_file("./dummy_file", options, encoding);
            }
            
            // Clean up file
            std::remove("./dummy_file");
        }
    }

    return 0;
}