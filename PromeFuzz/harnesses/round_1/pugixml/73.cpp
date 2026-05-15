// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_buffer_inplace at pugixml.cpp:7756:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7496:34 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7739:46 in pugixml.hpp
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
#include "src/pugixml.hpp"
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <vector>

static void write_dummy_file(const uint8_t* data, size_t size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file) {
        file.write(reinterpret_cast<const char*>(data), size);
    }
}

extern "C" int LLVMFuzzerTestOneInput_73(const uint8_t* Data, size_t Size) {
    if (Size == 0) return 0;

    // Prepare options and encoding from input data
    unsigned int options = pugi::parse_default;
    pugi::xml_encoding encoding = pugi::encoding_auto;
    if (Size >= sizeof(unsigned int)) {
        options = *reinterpret_cast<const unsigned int*>(Data);
        encoding = static_cast<pugi::xml_encoding>(
            Data[0] % (pugi::encoding_latin1 + 1));
    }

    // Write dummy file for load_file
    write_dummy_file(Data, Size);

    // Test load_buffer_inplace
    {
        pugi::xml_document doc;
        char* buffer = new char[Size];
        if (buffer) {
            std::memcpy(buffer, Data, Size);
            pugi::xml_parse_result result = 
                doc.load_buffer_inplace(buffer, Size, options, encoding);
            delete[] buffer;
        }
    }

    // Test reset
    {
        pugi::xml_document doc1;
        pugi::xml_document doc2;
        // Load some data into doc1 first
        if (Size > 0) {
            std::vector<char> str_data(Data, Data + Size);
            str_data.push_back('\0');
            doc1.load_string(str_data.data());
        }
        doc2.reset(doc1);
    }

    // Test load_buffer
    {
        pugi::xml_document doc;
        pugi::xml_parse_result result = 
            doc.load_buffer(Data, Size, options, encoding);
    }

    // Test load_string
    {
        pugi::xml_document doc;
        if (Size > 0) {
            std::vector<char> str_data(Data, Data + Size);
            str_data.push_back('\0');
            pugi::xml_parse_result result = 
                doc.load_string(str_data.data(), options);
        }
    }

    // Test load_file
    {
        pugi::xml_document doc;
        const wchar_t* path = L"./dummy_file";
        pugi::xml_parse_result result = 
            doc.load_file(path, options, encoding);
    }

    // Test load_buffer_inplace_own
    {
        pugi::xml_document doc;
        void* buffer = std::malloc(Size);
        if (buffer) {
            std::memcpy(buffer, Data, Size);
            pugi::xml_parse_result result = 
                doc.load_buffer_inplace_own(buffer, Size, options, encoding);
        }
    }

    return 0;
}