// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_file at pugixml.cpp:7739:46 in pugixml.hpp
// pugi::xml_document::load at pugixml.cpp:7697:46 in pugixml.hpp
// pugi::get_memory_allocation_function at pugixml.cpp:7873:52 in pugixml.hpp
// pugi::xml_document::load_buffer_inplace_own at pugixml.cpp:7763:46 in pugixml.hpp
// pugi::xml_document::load_buffer_inplace at pugixml.cpp:7756:46 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
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
#include <cstring>
#include <fstream>
#include <sstream>
#include <vector>
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

static pugi::xml_encoding get_valid_encoding(uint8_t encoding_byte) {
    uint8_t valid_encoding = encoding_byte % 10;
    return static_cast<pugi::xml_encoding>(valid_encoding);
}

extern "C" int LLVMFuzzerTestOneInput_108(const uint8_t* Data, size_t Size) {
    if (Size < 2) return 0;

    // Extract parameters from fuzz input
    uint8_t function_selector = Data[0] % 6;
    uint8_t encoding_selector = Data[1];
    uint32_t options = 0;
    if (Size >= 6) {
        std::memcpy(&options, Data + 2, sizeof(uint32_t));
    }
    
    const uint8_t* xml_content = Data + 6;
    size_t xml_size = (Size >= 6) ? Size - 6 : 0;
    
    pugi::xml_document doc;
    pugi::xml_parse_result result;
    
    switch (function_selector) {
        case 0: { // load from stream
            std::stringstream stream;
            if (xml_size > 0) {
                stream.write(reinterpret_cast<const char*>(xml_content), xml_size);
            }
            pugi::xml_encoding encoding = get_valid_encoding(encoding_selector);
            result = doc.load(stream, options, encoding);
            break;
        }
        
        case 1: { // load_buffer_inplace_own
            if (xml_size > 0) {
                pugi::allocation_function allocate = pugi::get_memory_allocation_function();
                void* buffer = allocate(xml_size);
                if (buffer) {
                    std::memcpy(buffer, xml_content, xml_size);
                    pugi::xml_encoding encoding = get_valid_encoding(encoding_selector);
                    result = doc.load_buffer_inplace_own(buffer, xml_size, options, encoding);
                }
            }
            break;
        }
        
        case 2: { // load_buffer_inplace
            if (xml_size > 0) {
                std::vector<char> buffer(xml_content, xml_content + xml_size);
                pugi::xml_encoding encoding = get_valid_encoding(encoding_selector);
                result = doc.load_buffer_inplace(buffer.data(), buffer.size(), options, encoding);
            }
            break;
        }
        
        case 3: { // load_buffer
            if (xml_size > 0) {
                pugi::xml_encoding encoding = get_valid_encoding(encoding_selector);
                result = doc.load_buffer(xml_content, xml_size, options, encoding);
            }
            break;
        }
        
        case 4: { // load_string
            if (xml_size > 0) {
                std::vector<char> str(xml_content, xml_content + xml_size);
                str.push_back('\0');
                result = doc.load_string(str.data(), options);
            }
            break;
        }
        
        case 5: { // load_file
            write_dummy_file(xml_content, xml_size);
            pugi::xml_encoding encoding = get_valid_encoding(encoding_selector);
            result = doc.load_file(L"./dummy_file", options, encoding);
            remove_dummy_file();
            break;
        }
    }
    
    // Use the result to avoid optimization
    if (result.status != pugi::status_ok) {
        // Do nothing, just prevent dead code elimination
    }
    
    return 0;
}