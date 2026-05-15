// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_buffer_inplace at pugixml.cpp:7756:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::get_memory_allocation_function at pugixml.cpp:7873:52 in pugixml.hpp
// pugi::xml_document::load_buffer_inplace_own at pugixml.cpp:7763:46 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
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
#include <algorithm>

extern "C" int LLVMFuzzerTestOneInput_112(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Extract parameters from fuzzing input
    // Use first byte for function selector, second for options, third for encoding
    uint8_t func_selector = Size > 0 ? Data[0] % 6 : 0;
    unsigned int options = Size > 1 ? Data[1] : pugi::parse_default;
    uint8_t enc_byte = Size > 2 ? Data[2] : pugi::encoding_auto;
    
    // Ensure encoding is valid (0-9)
    pugi::xml_encoding encoding = pugi::encoding_auto;
    if (enc_byte >= pugi::encoding_auto && enc_byte <= pugi::encoding_latin1) {
        encoding = static_cast<pugi::xml_encoding>(enc_byte);
    }
    
    // Use remaining data for XML content
    const uint8_t* xml_data = Size > 3 ? Data + 3 : Data;
    size_t xml_size = Size > 3 ? Size - 3 : Size;
    
    // Ensure we have at least some data for XML
    if (xml_size == 0) return 0;
    
    // Create document instance
    pugi::xml_document doc;
    
    switch (func_selector) {
        case 0: { // load_file
            // Write XML data to dummy file
            std::ofstream file("./dummy_file", std::ios::binary);
            if (file.is_open()) {
                file.write(reinterpret_cast<const char*>(xml_data), xml_size);
                file.close();
                
                // Call load_file with char* path
                pugi::xml_parse_result result = doc.load_file("./dummy_file", options, encoding);
                (void)result; // Use result to avoid unused variable warning
                
                // Clean up file
                std::remove("./dummy_file");
            }
            break;
        }
        
        case 1: { // load_buffer_inplace_own
            // Allocate buffer using pugixml's allocator
            void* buffer = pugi::get_memory_allocation_function()(xml_size);
            if (buffer) {
                std::memcpy(buffer, xml_data, xml_size);
                
                // Call load_buffer_inplace_own - document takes ownership
                pugi::xml_parse_result result = doc.load_buffer_inplace_own(buffer, xml_size, options, encoding);
                (void)result;
                
                // DO NOT free buffer - document owns it now
            }
            break;
        }
        
        case 2: { // load_buffer
            // Simply pass the buffer - function will copy it
            pugi::xml_parse_result result = doc.load_buffer(xml_data, xml_size, options, encoding);
            (void)result;
            break;
        }
        
        case 3: { // load (from stream)
            // Create string stream from XML data
            std::string xml_string(reinterpret_cast<const char*>(xml_data), xml_size);
            std::istringstream stream(xml_string);
            
            // Call load with stream
            pugi::xml_parse_result result = doc.load(stream, options, encoding);
            (void)result;
            break;
        }
        
        case 4: { // load_buffer_inplace
            // Allocate buffer that persists for document lifetime
            void* buffer = std::malloc(xml_size);
            if (buffer) {
                std::memcpy(buffer, xml_data, xml_size);
                
                // Call load_buffer_inplace
                pugi::xml_parse_result result = doc.load_buffer_inplace(buffer, xml_size, options, encoding);
                (void)result;
                
                // Free buffer after document is destroyed (at end of scope)
                std::free(buffer);
            }
            break;
        }
        
        case 5: { // load_string
            // Ensure null termination for string
            std::vector<char> null_terminated(xml_data, xml_data + xml_size);
            null_terminated.push_back('\0');
            
            // Call load_string
            pugi::xml_parse_result result = doc.load_string(null_terminated.data(), options);
            (void)result;
            break;
        }
    }
    
    return 0;
}