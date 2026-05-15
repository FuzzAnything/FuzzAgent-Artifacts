// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7490:34 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7490:34 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7490:34 in pugixml.hpp
// pugi::xml_document::load at pugixml.cpp:7697:46 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7490:34 in pugixml.hpp
// pugi::xml_document::load at pugixml.cpp:7704:46 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7490:34 in pugixml.hpp
// pugi::xml_document::load_buffer_inplace at pugixml.cpp:7756:46 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7490:34 in pugixml.hpp
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

static void writeToDummyFile(const uint8_t* data, size_t size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file) {
        file.write(reinterpret_cast<const char*>(data), size);
    }
}

static unsigned int getRandomOptions(const uint8_t* data, size_t size, size_t offset) {
    if (offset + sizeof(unsigned int) <= size) {
        unsigned int options;
        std::memcpy(&options, data + offset, sizeof(unsigned int));
        return options;
    }
    return pugi::parse_default;
}

static pugi::xml_encoding getRandomEncoding(const uint8_t* data, size_t size, size_t offset) {
    if (offset < size) {
        uint8_t encoding_byte = data[offset] % 10;
        if (encoding_byte >= pugi::encoding_auto && encoding_byte <= pugi::encoding_latin1) {
            return static_cast<pugi::xml_encoding>(encoding_byte);
        }
    }
    return pugi::encoding_auto;
}

extern "C" int LLVMFuzzerTestOneInput_78(const uint8_t* Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Initialize document
    pugi::xml_document doc;
    
    // Test load with char stream
    {
        std::string xml_content(reinterpret_cast<const char*>(Data), Size);
        std::istringstream stream(xml_content);
        size_t options_offset = 0;
        size_t encoding_offset = sizeof(unsigned int);
        unsigned int options = getRandomOptions(Data, Size, options_offset);
        pugi::xml_encoding encoding = getRandomEncoding(Data, Size, encoding_offset);
        if (encoding >= pugi::encoding_auto && encoding <= pugi::encoding_latin1) {
            pugi::xml_parse_result result = doc.load(stream, options, encoding);
            (void)result;
        }
        doc.reset();
    }
    
    // Test load with wchar_t stream
    {
        std::wstring wxml_content;
        for (size_t i = 0; i < Size; ++i) {
            wxml_content.push_back(static_cast<wchar_t>(Data[i]));
        }
        std::wistringstream wstream(wxml_content);
        size_t options_offset = 0;
        unsigned int options = getRandomOptions(Data, Size, options_offset);
        pugi::xml_parse_result result = doc.load(wstream, options);
        (void)result;
        doc.reset();
    }
    
    // Test load_buffer_inplace
    {
        size_t options_offset = 0;
        size_t encoding_offset = sizeof(unsigned int);
        unsigned int options = getRandomOptions(Data, Size, options_offset);
        pugi::xml_encoding encoding = getRandomEncoding(Data, Size, encoding_offset);
        
        if (encoding >= pugi::encoding_auto && encoding <= pugi::encoding_latin1) {
            void* buffer = std::malloc(Size);
            if (buffer) {
                std::memcpy(buffer, Data, Size);
                pugi::xml_parse_result result = doc.load_buffer_inplace(buffer, Size, options, encoding);
                (void)result;
                std::free(buffer);
            }
        }
        doc.reset();
    }
    
    // Test load_buffer
    {
        size_t options_offset = 0;
        size_t encoding_offset = sizeof(unsigned int);
        unsigned int options = getRandomOptions(Data, Size, options_offset);
        pugi::xml_encoding encoding = getRandomEncoding(Data, Size, encoding_offset);
        
        if (encoding >= pugi::encoding_auto && encoding <= pugi::encoding_latin1) {
            pugi::xml_parse_result result = doc.load_buffer(Data, Size, options, encoding);
            (void)result;
        }
        doc.reset();
    }
    
    // Test load_string
    {
        size_t options_offset = 0;
        unsigned int options = getRandomOptions(Data, Size, options_offset);
        
        // Create null-terminated string
        std::string xml_string(reinterpret_cast<const char*>(Data), Size);
        if (!xml_string.empty()) {
            pugi::xml_parse_result result = doc.load_string(xml_string.c_str(), options);
            (void)result;
        }
        doc.reset();
    }
    
    // Test load_file
    {
        writeToDummyFile(Data, Size);
        
        size_t options_offset = 0;
        size_t encoding_offset = sizeof(unsigned int);
        unsigned int options = getRandomOptions(Data, Size, options_offset);
        pugi::xml_encoding encoding = getRandomEncoding(Data, Size, encoding_offset);
        
        if (encoding >= pugi::encoding_auto && encoding <= pugi::encoding_latin1) {
            pugi::xml_parse_result result = doc.load_file("./dummy_file", options, encoding);
            (void)result;
        }
        doc.reset();
    }
    
    return 0;
}