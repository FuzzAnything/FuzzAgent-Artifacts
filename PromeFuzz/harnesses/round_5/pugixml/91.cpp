// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load at pugixml.cpp:7697:46 in pugixml.hpp
// pugi::xml_document::load_buffer_inplace at pugixml.cpp:7756:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::get_memory_allocation_function at pugixml.cpp:7873:52 in pugixml.hpp
// pugi::xml_document::load_buffer_inplace_own at pugixml.cpp:7763:46 in pugixml.hpp
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
#include <cstring>
#include <fstream>
#include <sstream>
#include <memory>
#include <vector>

static void writeDummyFile(const uint8_t* data, size_t size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(data), size);
        file.close();
    }
}

static pugi::xml_encoding getValidEncoding(uint8_t byte) {
    static const pugi::xml_encoding encodings[] = {
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
    return encodings[byte % 10];
}

extern "C" int LLVMFuzzerTestOneInput_91(const uint8_t* Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Use first byte to determine which function to test
    uint8_t selector = Data[0];
    const uint8_t* content = Data + 1;
    size_t content_size = Size - 1;
    
    // Initialize options and encoding from remaining data
    unsigned int options = 0;
    if (content_size >= sizeof(unsigned int)) {
        memcpy(&options, content, sizeof(unsigned int));
        content += sizeof(unsigned int);
        content_size -= sizeof(unsigned int);
    }
    
    pugi::xml_encoding encoding = getValidEncoding(content_size > 0 ? content[0] : 0);
    
    // Test different API functions based on selector
    switch (selector % 6) {
        case 0: { // load_file
            writeDummyFile(content, content_size);
            pugi::xml_document doc;
            if (encoding >= pugi::encoding_auto && encoding <= pugi::encoding_latin1) {
                doc.load_file("./dummy_file", options, encoding);
            }
            break;
        }
        
        case 1: { // load_buffer_inplace_own
            pugi::xml_document doc;
            if (content_size > 0) {
                // Allocate buffer with pugixml's allocator
                void* buffer = pugi::get_memory_allocation_function()(content_size);
                if (buffer) {
                    memcpy(buffer, content, content_size);
                    if (encoding >= pugi::encoding_auto && encoding <= pugi::encoding_latin1) {
                        doc.load_buffer_inplace_own(buffer, content_size, options, encoding);
                    }
                }
            }
            break;
        }
        
        case 2: { // load_buffer
            pugi::xml_document doc;
            if (content_size > 0) {
                if (encoding >= pugi::encoding_auto && encoding <= pugi::encoding_latin1) {
                    doc.load_buffer(content, content_size, options, encoding);
                }
            }
            break;
        }
        
        case 3: { // load
            pugi::xml_document doc;
            if (content_size > 0) {
                std::stringstream stream;
                stream.write(reinterpret_cast<const char*>(content), content_size);
                if (encoding >= pugi::encoding_auto && encoding <= pugi::encoding_latin1) {
                    doc.load(stream, options, encoding);
                }
            }
            break;
        }
        
        case 4: { // load_buffer_inplace
            pugi::xml_document doc;
            if (content_size > 0) {
                // Allocate buffer that persists for document lifetime
                std::vector<char> buffer(content, content + content_size);
                if (encoding >= pugi::encoding_auto && encoding <= pugi::encoding_latin1) {
                    doc.load_buffer_inplace(buffer.data(), buffer.size(), options, encoding);
                }
            }
            break;
        }
        
        case 5: { // load_string
            pugi::xml_document doc;
            if (content_size > 0) {
                // Create null-terminated string
                std::vector<char> str(content, content + content_size);
                str.push_back('\0');
                doc.load_string(str.data(), options);
            }
            break;
        }
    }
    
    return 0;
}