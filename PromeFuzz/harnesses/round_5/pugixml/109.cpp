// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::load at pugixml.cpp:7697:46 in pugixml.hpp
// pugi::xml_document::load_buffer_inplace at pugixml.cpp:7756:46 in pugixml.hpp
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
#include "src/pugixml.hpp"
#include <cstdint>
#include <cstring>
#include <fstream>
#include <sstream>
#include <cstdlib>

static unsigned int getOptionsFromData(const uint8_t* data, size_t size, size_t& offset) {
    if (offset + sizeof(unsigned int) <= size) {
        unsigned int options;
        memcpy(&options, data + offset, sizeof(unsigned int));
        offset += sizeof(unsigned int);
        return options;
    }
    return pugi::parse_default;
}

static pugi::xml_encoding getEncodingFromData(const uint8_t* data, size_t size, size_t& offset) {
    if (offset < size) {
        uint8_t enc_byte = data[offset] % 10;
        offset++;
        if (enc_byte >= pugi::encoding_auto && enc_byte <= pugi::encoding_latin1) {
            return static_cast<pugi::xml_encoding>(enc_byte);
        }
    }
    return pugi::encoding_auto;
}

static const uint8_t* getContentFromData(const uint8_t* data, size_t size, size_t& offset, size_t& content_size) {
    if (offset < size) {
        content_size = size - offset;
        return data + offset;
    }
    content_size = 0;
    return nullptr;
}

extern "C" int LLVMFuzzerTestOneInput_109(const uint8_t* Data, size_t Size) {
    if (Size == 0) return 0;
    
    size_t offset = 0;
    
    // 1. Test load_file
    {
        pugi::xml_document doc;
        unsigned int options = getOptionsFromData(Data, Size, offset);
        pugi::xml_encoding encoding = getEncodingFromData(Data, Size, offset);
        size_t content_size;
        const uint8_t* content = getContentFromData(Data, Size, offset, content_size);
        
        if (content && content_size > 0) {
            std::ofstream file("./dummy_file", std::ios::binary);
            if (file.is_open()) {
                file.write(reinterpret_cast<const char*>(content), content_size);
                file.close();
                
                if (encoding >= pugi::encoding_auto && encoding <= pugi::encoding_latin1) {
                    pugi::xml_parse_result result = doc.load_file("./dummy_file", options, encoding);
                    (void)result;
                }
                
                std::remove("./dummy_file");
            }
        }
    }
    
    // Reset offset for next test
    offset = 0;
    
    // 2. Test load_buffer_inplace_own
    {
        pugi::xml_document doc;
        unsigned int options = getOptionsFromData(Data, Size, offset);
        pugi::xml_encoding encoding = getEncodingFromData(Data, Size, offset);
        size_t content_size;
        const uint8_t* content = getContentFromData(Data, Size, offset, content_size);
        
        if (content && content_size > 0) {
            // Allocate buffer with operator new as pugixml will use operator delete to free it
            char* buffer = new char[content_size];
            memcpy(buffer, content, content_size);
            pugi::xml_parse_result result = doc.load_buffer_inplace_own(buffer, content_size, options, encoding);
            (void)result;
            // Note: pugixml will free the buffer, so we don't delete it here
        }
    }
    
    // Reset offset for next test
    offset = 0;
    
    // 3. Test load_buffer
    {
        pugi::xml_document doc;
        unsigned int options = getOptionsFromData(Data, Size, offset);
        pugi::xml_encoding encoding = getEncodingFromData(Data, Size, offset);
        size_t content_size;
        const uint8_t* content = getContentFromData(Data, Size, offset, content_size);
        
        if (content && content_size > 0) {
            pugi::xml_parse_result result = doc.load_buffer(content, content_size, options, encoding);
            (void)result;
        }
    }
    
    // Reset offset for next test
    offset = 0;
    
    // 4. Test load (from stream)
    {
        pugi::xml_document doc;
        unsigned int options = getOptionsFromData(Data, Size, offset);
        pugi::xml_encoding encoding = getEncodingFromData(Data, Size, offset);
        size_t content_size;
        const uint8_t* content = getContentFromData(Data, Size, offset, content_size);
        
        if (content && content_size > 0) {
            std::string content_str(reinterpret_cast<const char*>(content), content_size);
            std::istringstream stream(content_str);
            
            if (encoding >= pugi::encoding_auto && encoding <= pugi::encoding_latin1) {
                pugi::xml_parse_result result = doc.load(stream, options, encoding);
                (void)result;
            }
        }
    }
    
    // Reset offset for next test
    offset = 0;
    
    // 5. Test load_buffer_inplace
    {
        pugi::xml_document doc;
        unsigned int options = getOptionsFromData(Data, Size, offset);
        pugi::xml_encoding encoding = getEncodingFromData(Data, Size, offset);
        size_t content_size;
        const uint8_t* content = getContentFromData(Data, Size, offset, content_size);
        
        if (content && content_size > 0) {
            char* buffer = new char[content_size];
            memcpy(buffer, content, content_size);
            pugi::xml_parse_result result = doc.load_buffer_inplace(buffer, content_size, options, encoding);
            (void)result;
            delete[] buffer;
        }
    }
    
    // Reset offset for next test
    offset = 0;
    
    // 6. Test load_string
    {
        pugi::xml_document doc;
        unsigned int options = getOptionsFromData(Data, Size, offset);
        size_t content_size;
        const uint8_t* content = getContentFromData(Data, Size, offset, content_size);
        
        if (content && content_size > 0) {
            // Create null-terminated string
            char* str = new char[content_size + 1];
            memcpy(str, content, content_size);
            str[content_size] = '\0';
            
            pugi::xml_parse_result result = doc.load_string(str, options);
            (void)result;
            
            delete[] str;
        }
    }
    
    return 0;
}