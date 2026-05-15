// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_document::load_buffer_inplace at pugixml.cpp:7756:46 in pugixml.hpp
// pugi::xml_document::load at pugixml.cpp:7697:46 in pugixml.hpp
// pugi::xml_document::load at pugixml.cpp:7704:46 in pugixml.hpp
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
#include <cstdlib>
#include <cstring>
#include <string>
#include <sstream>
#include <fstream>

static unsigned int get_random_uint(const uint8_t* data, size_t size, size_t* offset) {
    if (*offset + sizeof(unsigned int) <= size) {
        unsigned int value;
        memcpy(&value, data + *offset, sizeof(unsigned int));
        *offset += sizeof(unsigned int);
        return value;
    }
    return 0;
}

static pugi::xml_encoding get_random_encoding(const uint8_t* data, size_t size, size_t* offset) {
    if (*offset < size) {
        uint8_t byte = data[(*offset)++];
        // Only use valid encoding values (0-9)
        return static_cast<pugi::xml_encoding>(byte % 10);
    }
    return pugi::encoding_auto;
}

extern "C" int LLVMFuzzerTestOneInput_68(const uint8_t* Data, size_t Size) {
    if (Size == 0) return 0;
    
    size_t offset = 0;
    
    // Extract options and encoding from fuzz data
    unsigned int options = get_random_uint(Data, Size, &offset);
    pugi::xml_encoding encoding = get_random_encoding(Data, Size, &offset);
    
    // Remaining data is XML content
    const uint8_t* xml_data = Data + offset;
    size_t xml_size = Size - offset;
    
    // Test load_buffer_inplace
    {
        pugi::xml_document doc;
        if (xml_size > 0) {
            void* buffer = malloc(xml_size);
            if (buffer) {
                memcpy(buffer, xml_data, xml_size);
                // Ensure encoding is valid before calling
                if (encoding >= pugi::encoding_auto && encoding <= pugi::encoding_latin1) {
                    doc.load_buffer_inplace(buffer, xml_size, options, encoding);
                }
                free(buffer);
            }
        }
    }
    
    // Test load (char stream)
    {
        pugi::xml_document doc;
        if (xml_size > 0) {
            std::string xml_str(reinterpret_cast<const char*>(xml_data), xml_size);
            std::istringstream iss(xml_str);
            // Ensure encoding is valid before calling
            if (encoding >= pugi::encoding_auto && encoding <= pugi::encoding_latin1) {
                doc.load(iss, options, encoding);
            }
        }
    }
    
    // Test load (wchar_t stream)
    {
        pugi::xml_document doc;
        if (xml_size > 0) {
            std::wstring wxml_str(xml_size, L'\0');
            for (size_t i = 0; i < xml_size && i < wxml_str.size(); ++i) {
                wxml_str[i] = static_cast<wchar_t>(xml_data[i]);
            }
            std::wistringstream wiss(wxml_str);
            doc.load(wiss, options);
        }
    }
    
    // Test load_buffer
    {
        pugi::xml_document doc;
        if (xml_size > 0) {
            // Ensure encoding is valid before calling
            if (encoding >= pugi::encoding_auto && encoding <= pugi::encoding_latin1) {
                doc.load_buffer(xml_data, xml_size, options, encoding);
            }
        }
    }
    
    // Test load_string
    {
        pugi::xml_document doc;
        if (xml_size > 0) {
            // Ensure null termination for load_string
            std::vector<char> null_terminated(xml_data, xml_data + xml_size);
            null_terminated.push_back('\0');
            doc.load_string(null_terminated.data(), options);
        }
    }
    
    // Test load_file
    {
        pugi::xml_document doc;
        if (xml_size > 0) {
            const char* filename = "./dummy_file";
            std::ofstream file(filename, std::ios::binary);
            if (file) {
                file.write(reinterpret_cast<const char*>(xml_data), xml_size);
                file.close();
                // Ensure encoding is valid before calling
                if (encoding >= pugi::encoding_auto && encoding <= pugi::encoding_latin1) {
                    doc.load_file(filename, options, encoding);
                }
                std::remove(filename);
            }
        }
    }
    
    return 0;
}