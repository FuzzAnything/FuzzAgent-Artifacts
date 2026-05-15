// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::load_buffer_inplace at pugixml.cpp:7756:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::load at pugixml.cpp:7697:46 in pugixml.hpp
// pugi::xml_document::load at pugixml.cpp:7704:46 in pugixml.hpp
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
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <sstream>
#include <string>

static void writeDummyFile(const uint8_t* data, size_t size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file) {
        file.write(reinterpret_cast<const char*>(data), size);
    }
}

static unsigned int getRandomOptions(const uint8_t* data, size_t size, size_t offset) {
    if (offset < size) {
        return static_cast<unsigned int>(data[offset]);
    }
    return pugi::parse_default;
}

static pugi::xml_encoding getRandomEncoding(const uint8_t* data, size_t size, size_t offset) {
    if (offset < size) {
        return static_cast<pugi::xml_encoding>(data[offset] % 10);
    }
    return pugi::encoding_auto;
}

extern "C" int LLVMFuzzerTestOneInput_79(const uint8_t* Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Test load_buffer_inplace
    {
        pugi::xml_document doc1;
        char* buffer = static_cast<char*>(malloc(Size));
        if (buffer) {
            memcpy(buffer, Data, Size);
            unsigned int options = getRandomOptions(Data, Size, 0);
            pugi::xml_encoding encoding = getRandomEncoding(Data, Size, 1);
            doc1.load_buffer_inplace(buffer, Size, options, encoding);
            free(buffer);
        }
    }

    // Test load_string
    {
        pugi::xml_document doc2;
        std::string str(reinterpret_cast<const char*>(Data), Size);
        unsigned int options = getRandomOptions(Data, Size, 0);
        doc2.load_string(str.c_str(), options);
    }

    // Test load for char stream
    {
        pugi::xml_document doc3;
        std::istringstream char_stream(std::string(reinterpret_cast<const char*>(Data), Size));
        unsigned int options = getRandomOptions(Data, Size, 0);
        pugi::xml_encoding encoding = getRandomEncoding(Data, Size, 1);
        if (encoding >= pugi::encoding_auto && encoding <= pugi::encoding_latin1) {
            doc3.load(char_stream, options, encoding);
        }
    }

    // Test load for wchar_t stream
    {
        pugi::xml_document doc4;
        std::wstring wstr(Size / sizeof(wchar_t), L' ');
        if (Size >= sizeof(wchar_t)) {
            size_t wchars = Size / sizeof(wchar_t);
            memcpy(&wstr[0], Data, wchars * sizeof(wchar_t));
        }
        std::wistringstream wchar_stream(wstr);
        unsigned int options = getRandomOptions(Data, Size, 0);
        doc4.load(wchar_stream, options);
    }

    // Test load_file
    {
        pugi::xml_document doc5;
        writeDummyFile(Data, Size);
        unsigned int options = getRandomOptions(Data, Size, 0);
        pugi::xml_encoding encoding = getRandomEncoding(Data, Size, 1);
        if (encoding >= pugi::encoding_auto && encoding <= pugi::encoding_latin1) {
            doc5.load_file("./dummy_file", options, encoding);
        }
        std::remove("./dummy_file");
    }

    // Test load_buffer
    {
        pugi::xml_document doc6;
        unsigned int options = getRandomOptions(Data, Size, 0);
        pugi::xml_encoding encoding = getRandomEncoding(Data, Size, 1);
        doc6.load_buffer(Data, Size, options, encoding);
    }

    return 0;
}