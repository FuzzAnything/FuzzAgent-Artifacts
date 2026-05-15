// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_buffer_inplace at pugixml.cpp:7756:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::load at pugixml.cpp:7697:46 in pugixml.hpp
// pugi::xml_document::load at pugixml.cpp:7704:46 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
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
#include <string>

static void writeDummyFile(const uint8_t* data, size_t size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file) {
        file.write(reinterpret_cast<const char*>(data), size);
    }
}

extern "C" int LLVMFuzzerTestOneInput_78(const uint8_t* data, size_t size) {
    // Ensure we have at least some data to work with
    if (size == 0) {
        return 0;
    }

    // Generate random values for options and encoding
    // Use data to derive values, but ensure they're within valid ranges
    uint8_t options_byte = data[0];
    uint8_t encoding_byte = size > 1 ? data[1] : data[0];
    
    unsigned int options = static_cast<unsigned int>(options_byte);
    pugi::xml_encoding encoding = static_cast<pugi::xml_encoding>(encoding_byte % 10); // 0-9 valid

    // Test load_buffer_inplace
    {
        pugi::xml_document doc1;
        char* buffer = new char[size];
        if (buffer) {
            std::memcpy(buffer, data, size);
            pugi::xml_parse_result result = doc1.load_buffer_inplace(buffer, size, options, encoding);
            (void)result;
            delete[] buffer;
        }
    }

    // Test load_string
    {
        pugi::xml_document doc2;
        // Create null-terminated string from data
        std::string str(reinterpret_cast<const char*>(data), size);
        pugi::xml_parse_result result = doc2.load_string(str.c_str(), options);
        (void)result;
    }

    // Test load with char stream
    {
        pugi::xml_document doc3;
        std::istringstream stream(std::string(reinterpret_cast<const char*>(data), size));
        if (encoding >= pugi::encoding_auto && encoding <= pugi::encoding_latin1) {
            pugi::xml_parse_result result = doc3.load(stream, options, encoding);
            (void)result;
        }
    }

    // Test load with wchar_t stream
    {
        pugi::xml_document doc4;
        std::wstring wstr(size, L' ');
        for (size_t i = 0; i < size; ++i) {
            wstr[i] = static_cast<wchar_t>(data[i]);
        }
        std::wistringstream wstream(wstr);
        pugi::xml_parse_result result = doc4.load(wstream, options);
        (void)result;
    }

    // Test load_file
    {
        pugi::xml_document doc5;
        writeDummyFile(data, size);
        if (encoding >= pugi::encoding_auto && encoding <= pugi::encoding_latin1) {
            pugi::xml_parse_result result = doc5.load_file("./dummy_file", options, encoding);
            (void)result;
        }
    }

    // Test load_buffer
    {
        pugi::xml_document doc6;
        pugi::xml_parse_result result = doc6.load_buffer(data, size, options, encoding);
        (void)result;
    }

    return 0;
}