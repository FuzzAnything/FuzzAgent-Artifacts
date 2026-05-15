// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_buffer_inplace at pugixml.cpp:7756:46 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7739:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::load at pugixml.cpp:7697:46 in pugixml.hpp
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

static void write_dummy_file(const uint8_t* data, size_t size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file) {
        file.write(reinterpret_cast<const char*>(data), size);
    }
}

static pugi::xml_encoding get_valid_encoding(uint8_t encoding_byte) {
    pugi::xml_encoding encoding = static_cast<pugi::xml_encoding>(encoding_byte % 10);
    if (encoding >= pugi::encoding_auto && encoding <= pugi::encoding_latin1) {
        return encoding;
    }
    return pugi::encoding_auto;
}

extern "C" int LLVMFuzzerTestOneInput_63(const uint8_t* data, size_t size) {
    if (size == 0) {
        return 0;
    }

    // Initialize random parameters from fuzz data
    uint8_t options_byte = data[0];
    uint8_t encoding_byte = data[size > 1 ? 1 : 0];
    unsigned int options = static_cast<unsigned int>(options_byte);
    pugi::xml_encoding encoding = get_valid_encoding(encoding_byte);
    
    const uint8_t* buffer_data = data;
    size_t buffer_size = size;
    
    // Adjust buffer data if we used bytes for parameters
    if (size > 2) {
        buffer_data = data + 2;
        buffer_size = size - 2;
    }

    // Test load_buffer
    {
        pugi::xml_document doc1;
        pugi::xml_parse_result result = doc1.load_buffer(buffer_data, buffer_size, options, encoding);
        (void)result;
    }

    // Test load_file (char* version)
    {
        write_dummy_file(buffer_data, buffer_size);
        pugi::xml_document doc2;
        pugi::xml_parse_result result = doc2.load_file("./dummy_file", options, encoding);
        (void)result;
    }

    // Test load_file (wchar_t* version)
    {
        write_dummy_file(buffer_data, buffer_size);
        pugi::xml_document doc3;
        pugi::xml_parse_result result = doc3.load_file(L"./dummy_file", options, encoding);
        (void)result;
    }

    // Test load_string - ensure null termination
    {
        pugi::xml_document doc4;
        // Create a null-terminated copy of the buffer
        std::string str(reinterpret_cast<const char*>(buffer_data), buffer_size);
        pugi::xml_parse_result result = doc4.load_string(str.c_str(), options);
        (void)result;
    }

    // Test load from stream
    {
        pugi::xml_document doc5;
        std::istringstream stream(std::string(reinterpret_cast<const char*>(buffer_data), buffer_size));
        pugi::xml_parse_result result = doc5.load(stream, options, encoding);
        (void)result;
    }

    // Test load_buffer_inplace - need mutable copy of buffer
    {
        pugi::xml_document doc6;
        void* mutable_buffer = malloc(buffer_size);
        if (mutable_buffer) {
            memcpy(mutable_buffer, buffer_data, buffer_size);
            pugi::xml_parse_result result = doc6.load_buffer_inplace(mutable_buffer, buffer_size, options, encoding);
            (void)result;
            free(mutable_buffer);
        }
    }

    return 0;
}