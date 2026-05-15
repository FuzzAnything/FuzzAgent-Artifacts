// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_document::load at pugixml.cpp:7704:46 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7496:34 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7496:34 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7496:34 in pugixml.hpp
// pugi::xml_document::load_buffer_inplace at pugixml.cpp:7756:46 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
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
#include <vector>
#include <algorithm>

static void test_load_stream(const uint8_t* data, size_t size, unsigned int options) {
    try {
        pugi::xml_document doc;
        std::basic_istringstream<wchar_t> stream;
        
        if (size > 0) {
            // Convert data to wchar_t for the wide stream
            size_t wsize = size / sizeof(wchar_t);
            if (wsize > 0) {
                std::wstring wstr(reinterpret_cast<const wchar_t*>(data), wsize);
                stream.str(wstr);
                stream.seekg(0);
                
                pugi::xml_parse_result result = doc.load(stream, options);
                (void)result;
            }
        }
    } catch (...) {
        // Ignore exceptions
    }
}

static void test_reset(const uint8_t* data, size_t size) {
    try {
        pugi::xml_document doc1;
        pugi::xml_document doc2;
        
        if (size > 0) {
            // Try to load some data into doc1 first
            doc1.load_buffer(data, size);
            
            // Reset doc2 with doc1's contents
            doc2.reset(doc1);
        }
        
        // Also test resetting with empty document
        pugi::xml_document empty_doc;
        doc1.reset(empty_doc);
        doc2.reset(empty_doc);
    } catch (...) {
        // Ignore exceptions
    }
}

static void test_load_buffer_inplace(const uint8_t* data, size_t size, unsigned int options, pugi::xml_encoding encoding) {
    try {
        pugi::xml_document doc;
        
        if (size > 0) {
            // Create a mutable copy of the data for in-place parsing
            void* buffer = malloc(size);
            if (buffer) {
                memcpy(buffer, data, size);
                
                pugi::xml_parse_result result = doc.load_buffer_inplace(buffer, size, options, encoding);
                (void)result;
                
                free(buffer);
            }
        }
    } catch (...) {
        // Ignore exceptions
    }
}

static void test_load_buffer(const uint8_t* data, size_t size, unsigned int options, pugi::xml_encoding encoding) {
    try {
        pugi::xml_document doc;
        
        if (size > 0) {
            pugi::xml_parse_result result = doc.load_buffer(data, size, options, encoding);
            (void)result;
        }
    } catch (...) {
        // Ignore exceptions
    }
}

static void test_load_string(const uint8_t* data, size_t size, unsigned int options) {
    try {
        pugi::xml_document doc;
        
        if (size > 0) {
            // Create null-terminated string
            std::vector<char> str_data(data, data + size);
            str_data.push_back('\0');
            
            pugi::xml_parse_result result = doc.load_string(str_data.data(), options);
            (void)result;
        }
    } catch (...) {
        // Ignore exceptions
    }
}

static void test_load_file(const uint8_t* data, size_t size, unsigned int options, pugi::xml_encoding encoding) {
    try {
        pugi::xml_document doc;
        
        if (size > 0) {
            // Write data to file
            std::ofstream file("./dummy_file", std::ios::binary);
            if (file) {
                file.write(reinterpret_cast<const char*>(data), size);
                file.close();
                
                pugi::xml_parse_result result = doc.load_file("./dummy_file", options, encoding);
                (void)result;
                
                // Remove the file
                std::remove("./dummy_file");
            }
        }
    } catch (...) {
        // Ignore exceptions
    }
}

extern "C" int LLVMFuzzerTestOneInput_84(const uint8_t* data, size_t size) {
    if (size < 1) {
        return 0;
    }
    
    // Extract parameters from the fuzzer input
    unsigned int options = 0;
    pugi::xml_encoding encoding = pugi::encoding_auto;
    
    if (size >= 1) {
        options = static_cast<unsigned int>(data[0]);
    }
    
    if (size >= 2) {
        // Ensure encoding is valid (0-9)
        uint8_t enc_byte = data[1] % 10;
        encoding = static_cast<pugi::xml_encoding>(enc_byte);
    }
    
    // Use remaining data for XML content
    const uint8_t* xml_data = (size >= 3) ? data + 2 : data;
    size_t xml_size = (size >= 3) ? size - 2 : size;
    
    // Test all API functions
    test_load_stream(xml_data, xml_size, options);
    test_reset(xml_data, xml_size);
    test_load_buffer_inplace(xml_data, xml_size, options, encoding);
    test_load_buffer(xml_data, xml_size, options, encoding);
    test_load_string(xml_data, xml_size, options);
    test_load_file(xml_data, xml_size, options, encoding);
    
    return 0;
}