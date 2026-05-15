// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7496:34 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7490:34 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::get_memory_allocation_function at pugixml.cpp:7873:52 in pugixml.hpp
// pugi::xml_document::load_buffer_inplace_own at pugixml.cpp:7763:46 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::load_buffer_inplace at pugixml.cpp:7756:46 in pugixml.hpp
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
#include <vector>

static pugi::xml_encoding get_random_encoding(const uint8_t* data, size_t size) {
    if (size == 0) return pugi::encoding_auto;
    uint8_t val = data[0] % 10; // 0-9
    return static_cast<pugi::xml_encoding>(val);
}

static unsigned int get_random_options(const uint8_t* data, size_t size, size_t offset = 0) {
    if (size <= offset) return pugi::parse_default;
    // Use up to 4 bytes for options
    unsigned int options = 0;
    size_t bytes_to_copy = std::min(size - offset, sizeof(options));
    std::memcpy(&options, data + offset, bytes_to_copy);
    return options;
}

extern "C" int LLVMFuzzerTestOneInput_62(const uint8_t* Data, size_t Size) {
    if (Size == 0) return 0;

    // 1. Test load_file
    {
        // Write data to file
        std::ofstream file("./dummy_file", std::ios::binary);
        if (file.is_open()) {
            file.write(reinterpret_cast<const char*>(Data), Size);
            file.close();
        }

        pugi::xml_document doc;
        pugi::xml_encoding encoding = get_random_encoding(Data, Size);
        unsigned int options = get_random_options(Data, Size, 1);
        
        if (encoding >= pugi::encoding_auto && encoding <= pugi::encoding_latin1) {
            doc.load_file("./dummy_file", options, encoding);
        }
    }

    // 2. Test load_buffer_inplace_own
    {
        pugi::xml_document doc;
        pugi::xml_encoding encoding = get_random_encoding(Data, Size);
        unsigned int options = get_random_options(Data, Size, 1);
        
        if (encoding >= pugi::encoding_auto && encoding <= pugi::encoding_latin1) {
            // Allocate buffer with pugixml's allocator
            pugi::allocation_function allocate = pugi::get_memory_allocation_function();
            void* buffer = allocate(Size);
            if (buffer) {
                std::memcpy(buffer, Data, Size);
                doc.load_buffer_inplace_own(buffer, Size, options, encoding);
                // Buffer is owned by document, don't use after
            }
        }
    }

    // 3. Test load_buffer
    {
        pugi::xml_document doc;
        pugi::xml_encoding encoding = get_random_encoding(Data, Size);
        unsigned int options = get_random_options(Data, Size, 1);
        
        if (encoding >= pugi::encoding_auto && encoding <= pugi::encoding_latin1) {
            doc.load_buffer(Data, Size, options, encoding);
        }
    }

    // 4. Test load_buffer_inplace
    {
        pugi::xml_document doc;
        pugi::xml_encoding encoding = get_random_encoding(Data, Size);
        unsigned int options = get_random_options(Data, Size, 1);
        
        if (encoding >= pugi::encoding_auto && encoding <= pugi::encoding_latin1) {
            // Allocate buffer that persists for document lifetime
            void* buffer = std::malloc(Size);
            if (buffer) {
                std::memcpy(buffer, Data, Size);
                doc.load_buffer_inplace(buffer, Size, options, encoding);
                // Document uses buffer, must free after document destruction
                std::free(buffer);
            }
        }
    }

    // 5. Test load_string
    {
        pugi::xml_document doc;
        unsigned int options = get_random_options(Data, Size);
        
        // Create null-terminated string
        std::vector<pugi::char_t> str(Size + 1);
        for (size_t i = 0; i < Size; ++i) {
            str[i] = static_cast<pugi::char_t>(Data[i]);
        }
        str[Size] = 0;
        
        doc.load_string(str.data(), options);
    }

    // 6. Test reset
    {
        pugi::xml_document doc1;
        pugi::xml_document doc2;
        
        // Load some data into doc2
        pugi::xml_encoding encoding = get_random_encoding(Data, Size);
        unsigned int options = get_random_options(Data, Size, 1);
        
        if (encoding >= pugi::encoding_auto && encoding <= pugi::encoding_latin1 && Size > 0) {
            doc2.load_buffer(Data, Size, options, encoding);
            
            // Reset doc1 with doc2
            doc1.reset(doc2);
        }
        
        // Also test reset without parameter (clear)
        doc1.reset();
    }

    return 0;
}