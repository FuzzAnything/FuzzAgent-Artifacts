// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::get_memory_allocation_function at pugixml.cpp:7873:52 in pugixml.hpp
// pugi::xml_document::load_buffer_inplace_own at pugixml.cpp:7763:46 in pugixml.hpp
// pugi::xml_document::load_buffer_inplace at pugixml.cpp:7756:46 in pugixml.hpp
// pugi::xml_document::load at pugixml.cpp:7697:46 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
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
#include <algorithm>

static void write_dummy_file(const uint8_t* data, size_t size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(data), size);
        file.close();
    }
}

static pugi::xml_encoding get_valid_encoding(uint8_t encoding_byte) {
    pugi::xml_encoding encoding = static_cast<pugi::xml_encoding>(encoding_byte % 10);
    if (encoding >= pugi::encoding_auto && encoding <= pugi::encoding_latin1) {
        return encoding;
    }
    return pugi::encoding_auto;
}

extern "C" int LLVMFuzzerTestOneInput_76(const uint8_t* data, size_t size) {
    if (size == 0) {
        return 0;
    }

    // Extract parameters from fuzzer input with bounds checking
    uint8_t function_selector = data[0] % 6;
    uint8_t options_byte = (size > 1) ? data[1] : 0;
    uint8_t encoding_byte = (size > 2) ? data[2] : 0;
    
    // Use remaining bytes as XML content
    const uint8_t* xml_data = data + std::min<size_t>(3, size);
    size_t xml_size = size - std::min<size_t>(3, size);

    pugi::xml_encoding encoding = get_valid_encoding(encoding_byte);
    unsigned int options = static_cast<unsigned int>(options_byte);

    pugi::xml_document doc;
    pugi::xml_parse_result result;

    switch (function_selector) {
        case 0: { // load_buffer_inplace
            if (xml_size > 0) {
                char* buffer = new char[xml_size];
                std::memcpy(buffer, xml_data, xml_size);
                result = doc.load_buffer_inplace(buffer, xml_size, options, encoding);
                delete[] buffer;
            }
            break;
        }
        
        case 1: { // load
            if (xml_size > 0) {
                std::string xml_string(reinterpret_cast<const char*>(xml_data), xml_size);
                std::istringstream stream(xml_string);
                result = doc.load(stream, options, encoding);
            }
            break;
        }
        
        case 2: { // load_buffer
            if (xml_size > 0) {
                result = doc.load_buffer(xml_data, xml_size, options, encoding);
            }
            break;
        }
        
        case 3: { // load_string
            if (xml_size > 0) {
                // Ensure null termination for load_string
                std::vector<char> null_terminated(xml_data, xml_data + xml_size);
                null_terminated.push_back('\0');
                result = doc.load_string(null_terminated.data(), options);
            }
            break;
        }
        
        case 4: { // load_file
            if (xml_size > 0) {
                write_dummy_file(xml_data, xml_size);
                result = doc.load_file(L"./dummy_file", options, encoding);
            }
            break;
        }
        
        case 5: { // load_buffer_inplace_own
            if (xml_size > 0) {
                // Allocate buffer using pugixml's allocator
                void* buffer = pugi::get_memory_allocation_function()(xml_size);
                if (buffer) {
                    std::memcpy(buffer, xml_data, xml_size);
                    result = doc.load_buffer_inplace_own(buffer, xml_size, options, encoding);
                }
            }
            break;
        }
    }

    // Clean up dummy file
    std::remove("./dummy_file");
    
    return 0;
}