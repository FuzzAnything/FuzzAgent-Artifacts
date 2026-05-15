// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_buffer_inplace_own at pugixml.cpp:7763:46 in pugixml.hpp
// pugi::xml_document::load_buffer_inplace at pugixml.cpp:7756:46 in pugixml.hpp
// pugi::xml_document::load at pugixml.cpp:7697:46 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
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
#include <cstring>
#include <fstream>
#include <sstream>
#include <cstdlib>

static pugi::xml_encoding get_random_encoding(uint8_t value) {
    const pugi::xml_encoding encodings[] = {
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
    return encodings[value % (sizeof(encodings) / sizeof(encodings[0]))];
}

extern "C" int LLVMFuzzerTestOneInput_75(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Generate random options and encoding from input data
    unsigned int options = static_cast<unsigned int>(Data[0]) | (static_cast<unsigned int>(Data[Size > 1 ? 1 : 0]) << 8);
    pugi::xml_encoding encoding = get_random_encoding(Data[Size > 2 ? 2 : 0]);

    // 1. Test load_buffer_inplace
    {
        pugi::xml_document doc1;
        char* buffer = static_cast<char*>(malloc(Size));
        if (buffer) {
            memcpy(buffer, Data, Size);
            pugi::xml_parse_result result = doc1.load_buffer_inplace(buffer, Size, options, encoding);
            (void)result;
            free(buffer);
        }
    }

    // 2. Test load (from stream)
    {
        pugi::xml_document doc2;
        std::istringstream stream(std::string(reinterpret_cast<const char*>(Data), Size));
        pugi::xml_parse_result result = doc2.load(stream, options, encoding);
        (void)result;
    }

    // 3. Test load_buffer
    {
        pugi::xml_document doc3;
        pugi::xml_parse_result result = doc3.load_buffer(Data, Size, options, encoding);
        (void)result;
    }

    // 4. Test load_string
    {
        pugi::xml_document doc4;
        // Create null-terminated string
        char* cstr = static_cast<char*>(malloc(Size + 1));
        if (cstr) {
            memcpy(cstr, Data, Size);
            cstr[Size] = '\0';
            pugi::xml_parse_result result = doc4.load_string(cstr, options);
            (void)result;
            free(cstr);
        }
    }

    // 5. Test load_file
    {
        pugi::xml_document doc5;
        // Write data to dummy file
        std::ofstream outfile("./dummy_file", std::ios::binary);
        if (outfile) {
            outfile.write(reinterpret_cast<const char*>(Data), Size);
            outfile.close();
            
            // Test with char* path
            pugi::xml_parse_result result = doc5.load_file("./dummy_file", options, encoding);
            (void)result;
            
            // Test with wchar_t* path
            pugi::xml_document doc5_w;
            pugi::xml_parse_result result_w = doc5_w.load_file(L"./dummy_file", options, encoding);
            (void)result_w;
            
            // Clean up
            std::remove("./dummy_file");
        }
    }

    // 6. Test load_buffer_inplace_own - must use pugixml's allocator
    {
        pugi::xml_document doc6;
        // Allocate buffer with pugixml's allocation function (malloc)
        char* buffer = static_cast<char*>(malloc(Size));
        if (buffer) {
            memcpy(buffer, Data, Size);
            pugi::xml_parse_result result = doc6.load_buffer_inplace_own(buffer, Size, options, encoding);
            (void)result;
            // Buffer will be automatically freed by pugixml when doc6 is destroyed
        }
    }

    return 0;
}