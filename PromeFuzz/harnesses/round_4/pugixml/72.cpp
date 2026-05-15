// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_buffer_inplace at pugixml.cpp:7756:46 in pugixml.hpp
// pugi::xml_document::load_buffer_inplace at pugixml.cpp:7756:46 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::get_memory_allocation_function at pugixml.cpp:7873:52 in pugixml.hpp
// pugi::xml_document::load_buffer_inplace_own at pugixml.cpp:7763:46 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7496:34 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
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

static void write_dummy_file(const uint8_t* data, size_t size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file) {
        file.write(reinterpret_cast<const char*>(data), size);
    }
}

static pugi::xml_encoding get_valid_encoding(uint8_t value) {
    if (value >= pugi::encoding_auto && value <= pugi::encoding_latin1) {
        return static_cast<pugi::xml_encoding>(value);
    }
    return pugi::encoding_auto;
}

extern "C" int LLVMFuzzerTestOneInput_72(const uint8_t* Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Use first byte to decide which function to test
    uint8_t selector = Data[0] % 6;
    const uint8_t* xml_data = Data + 1;
    size_t xml_size = Size - 1;
    
    // Initialize options and encoding from remaining data
    unsigned int options = 0;
    pugi::xml_encoding encoding = pugi::encoding_auto;
    
    if (xml_size >= 1) {
        options = static_cast<unsigned int>(xml_data[0]);
        if (xml_size >= 2) {
            encoding = get_valid_encoding(xml_data[1]);
        }
    }

    pugi::xml_document doc;
    pugi::xml_parse_result result;

    switch (selector) {
        case 0: // load_buffer
            if (xml_size > 0) {
                result = doc.load_buffer(xml_data, xml_size, options, encoding);
            }
            break;
            
        case 1: // load_file
            if (xml_size > 0) {
                write_dummy_file(xml_data, xml_size);
                result = doc.load_file("./dummy_file", options, encoding);
                std::remove("./dummy_file");
            }
            break;
            
        case 2: // load_buffer_inplace_own
            if (xml_size > 0) {
                void* buffer = pugi::get_memory_allocation_function()(xml_size);
                if (buffer) {
                    std::memcpy(buffer, xml_data, xml_size);
                    result = doc.load_buffer_inplace_own(buffer, xml_size, options, encoding);
                }
            }
            break;
            
        case 3: // reset
            {
                pugi::xml_document proto;
                if (xml_size > 0) {
                    proto.load_buffer(xml_data, xml_size, options, encoding);
                }
                doc.reset(proto);
            }
            break;
            
        case 4: // load_string
            if (xml_size > 0) {
                // Create null-terminated string
                char* str = new char[xml_size + 1];
                std::memcpy(str, xml_data, xml_size);
                str[xml_size] = '\0';
                result = doc.load_string(str, options);
                delete[] str;
            }
            break;
            
        case 5: // load_buffer_inplace
            if (xml_size > 0) {
                void* buffer = std::malloc(xml_size);
                if (buffer) {
                    std::memcpy(buffer, xml_data, xml_size);
                    result = doc.load_buffer_inplace(buffer, xml_size, options, encoding);
                    // Buffer must persist for document lifetime - free after document destruction
                    // Create a new scope to ensure doc is destroyed before freeing buffer
                    {
                        pugi::xml_document temp_doc;
                        temp_doc.load_buffer_inplace(buffer, xml_size, options, encoding);
                    }
                    std::free(buffer);
                }
            }
            break;
    }

    return 0;
}