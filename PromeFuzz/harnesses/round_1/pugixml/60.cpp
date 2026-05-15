// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_buffer_inplace at pugixml.cpp:7756:46 in pugixml.hpp
// pugi::xml_document::load at pugixml.cpp:7697:46 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_document::load_buffer_inplace_own at pugixml.cpp:7763:46 in pugixml.hpp
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
#include <algorithm>

static void write_dummy_file(const uint8_t* data, size_t size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file) {
        file.write(reinterpret_cast<const char*>(data), size);
    }
}

static void remove_dummy_file() {
    std::remove("./dummy_file");
}

extern "C" int LLVMFuzzerTestOneInput_60(const uint8_t *Data, size_t Size) {
    // Ensure we have at least some data for string operations
    size_t effective_size = std::max(Size, static_cast<size_t>(1));
    
    // Generate random options and encoding from input data
    unsigned int options = 0;
    pugi::xml_encoding encoding = pugi::encoding_auto;
    
    if (Size >= 2) {
        options = static_cast<unsigned int>(Data[0]) | (static_cast<unsigned int>(Data[1]) << 8);
        encoding = static_cast<pugi::xml_encoding>((Data[0] + Data[1]) % (pugi::encoding_latin1 + 1));
    } else if (Size == 1) {
        options = static_cast<unsigned int>(Data[0]);
        encoding = static_cast<pugi::xml_encoding>(Data[0] % (pugi::encoding_latin1 + 1));
    }
    
    // 1. Test load_buffer_inplace
    {
        pugi::xml_document doc1;
        char* buffer1 = static_cast<char*>(malloc(effective_size));
        if (buffer1) {
            memcpy(buffer1, Data, effective_size);
            doc1.load_buffer_inplace(buffer1, effective_size, options, encoding);
            free(buffer1);
        }
    }
    
    // 2. Test load (stream)
    {
        pugi::xml_document doc2;
        std::stringstream stream;
        stream.write(reinterpret_cast<const char*>(Data), effective_size);
        stream.seekg(0);
        doc2.load(stream, options, encoding);
    }
    
    // 3. Test load_buffer
    {
        pugi::xml_document doc3;
        doc3.load_buffer(Data, effective_size, options, encoding);
    }
    
    // 4. Test load_string
    {
        pugi::xml_document doc4;
        // Ensure null-terminated string
        char* str = static_cast<char*>(malloc(effective_size + 1));
        if (str) {
            memcpy(str, Data, effective_size);
            str[effective_size] = '\0';
            doc4.load_string(str, options);
            free(str);
        }
    }
    
    // 5. Test load_file
    {
        pugi::xml_document doc5;
        write_dummy_file(Data, effective_size);
        doc5.load_file("./dummy_file", options, encoding);
        remove_dummy_file();
    }
    
    // 6. Test load_buffer_inplace_own
    {
        pugi::xml_document doc6;
        // Allocate with malloc (pugixml's default allocator)
        void* buffer2 = malloc(effective_size);
        if (buffer2) {
            memcpy(buffer2, Data, effective_size);
            doc6.load_buffer_inplace_own(buffer2, effective_size, options, encoding);
        }
    }
    
    return 0;
}