// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load at pugixml.cpp:7697:46 in pugixml.hpp
// pugi::get_memory_allocation_function at pugixml.cpp:7873:52 in pugixml.hpp
// pugi::xml_document::load_buffer_inplace_own at pugixml.cpp:7763:46 in pugixml.hpp
// pugi::xml_document::load_buffer_inplace at pugixml.cpp:7756:46 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7739:46 in pugixml.hpp
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
#include <cstring>
#include <fstream>
#include <sstream>
#include <vector>

static void test_load_stream(pugi::xml_document& doc, const uint8_t* data, size_t size, unsigned int options, pugi::xml_encoding encoding) {
    if (encoding >= pugi::encoding_auto && encoding <= pugi::encoding_latin1) {
        std::stringstream stream;
        stream.write(reinterpret_cast<const char*>(data), size);
        doc.load(stream, options, encoding);
    }
}

static void test_load_buffer_inplace_own(pugi::xml_document& doc, const uint8_t* data, size_t size, unsigned int options, pugi::xml_encoding encoding) {
    if (size == 0) return;
    
    // Allocate buffer with pugixml's allocator (default is operator new)
    void* buffer = pugi::get_memory_allocation_function()(size);
    if (!buffer) return;
    
    std::memcpy(buffer, data, size);
    doc.load_buffer_inplace_own(buffer, size, options, encoding);
}

static void test_load_buffer_inplace(pugi::xml_document& doc, const uint8_t* data, size_t size, unsigned int options, pugi::xml_encoding encoding) {
    if (size == 0) return;
    
    // Allocate buffer that persists for document lifetime
    std::vector<char> buffer(data, data + size);
    doc.load_buffer_inplace(buffer.data(), buffer.size(), options, encoding);
}

static void test_load_buffer(pugi::xml_document& doc, const uint8_t* data, size_t size, unsigned int options, pugi::xml_encoding encoding) {
    doc.load_buffer(data, size, options, encoding);
}

static void test_load_string(pugi::xml_document& doc, const uint8_t* data, size_t size, unsigned int options) {
    if (size == 0) return;
    
    // Create null-terminated string
    std::vector<char> str(data, data + size);
    str.push_back('\0');
    doc.load_string(str.data(), options);
}

static void test_load_file(pugi::xml_document& doc, const uint8_t* data, size_t size, unsigned int options, pugi::xml_encoding encoding) {
    if (encoding >= pugi::encoding_auto && encoding <= pugi::encoding_latin1) {
        std::ofstream file("./dummy_file", std::ios::binary);
        if (file) {
            file.write(reinterpret_cast<const char*>(data), size);
            file.close();
            
            doc.load_file(L"./dummy_file", options, encoding);
            
            // Clean up the file
            std::remove("./dummy_file");
        }
    }
}

extern "C" int LLVMFuzzerTestOneInput_102(const uint8_t* data, size_t size) {
    if (size < 2) return 0;
    
    // Use first byte for options, second byte for encoding (mod 10 to get valid enum)
    unsigned int options = data[0];
    pugi::xml_encoding encoding = static_cast<pugi::xml_encoding>(data[1] % 10);
    
    // Skip the first two bytes used for options and encoding
    const uint8_t* xml_data = data + 2;
    size_t xml_size = size - 2;
    
    // Test each load function with the same document
    pugi::xml_document doc1;
    test_load_stream(doc1, xml_data, xml_size, options, encoding);
    
    pugi::xml_document doc2;
    test_load_buffer_inplace_own(doc2, xml_data, xml_size, options, encoding);
    
    pugi::xml_document doc3;
    test_load_buffer_inplace(doc3, xml_data, xml_size, options, encoding);
    
    pugi::xml_document doc4;
    test_load_buffer(doc4, xml_data, xml_size, options, encoding);
    
    pugi::xml_document doc5;
    test_load_string(doc5, xml_data, xml_size, options);
    
    pugi::xml_document doc6;
    test_load_file(doc6, xml_data, xml_size, options, encoding);
    
    return 0;
}