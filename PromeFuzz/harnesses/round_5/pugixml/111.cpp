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
#include <cstring>
#include <fstream>
#include <sstream>
#include <memory>

extern "C" int LLVMFuzzerTestOneInput_111(const uint8_t *Data, size_t Size) {
    // Ensure we have at least some data for parameters
    if (Size < 1) return 0;

    // Extract parameters from fuzzing data
    // Use first byte for encoding (0-9)
    uint8_t encoding_byte = Data[0] % 10; // Ensure valid encoding enum value
    pugi::xml_encoding encoding = static_cast<pugi::xml_encoding>(encoding_byte);
    
    // Use next 4 bytes for options (if available)
    unsigned int options = 0;
    if (Size >= 5) {
        options = *reinterpret_cast<const unsigned int*>(Data + 1);
    }
    
    // XML content starts after parameters
    const uint8_t* xml_data = Data + (Size >= 5 ? 5 : 1);
    size_t xml_size = Size - (xml_data - Data);
    
    // Validate encoding parameter (as per hints)
    if (encoding < pugi::encoding_auto || encoding > pugi::encoding_latin1) {
        encoding = pugi::encoding_auto;
    }

    // 1. Test load_file
    {
        // Write XML data to dummy file
        std::ofstream file("./dummy_file", std::ios::binary);
        if (file) {
            file.write(reinterpret_cast<const char*>(xml_data), xml_size);
            file.close();
            
            pugi::xml_document doc1;
            pugi::xml_parse_result result = doc1.load_file("./dummy_file", options, encoding);
            (void)result; // Ignore result, just test for crashes
        }
    }

    // 2. Test load_buffer_inplace_own
    {
        if (xml_size > 0) {
            // Allocate buffer with pugixml's allocator
            void* buffer = pugi::get_memory_allocation_function()(xml_size);
            if (buffer) {
                std::memcpy(buffer, xml_data, xml_size);
                
                pugi::xml_document doc2;
                pugi::xml_parse_result result = doc2.load_buffer_inplace_own(buffer, xml_size, options, encoding);
                (void)result;
                // Buffer is owned by document, don't free it
            }
        }
    }

    // 3. Test load_buffer
    {
        if (xml_size > 0) {
            pugi::xml_document doc3;
            pugi::xml_parse_result result = doc3.load_buffer(xml_data, xml_size, options, encoding);
            (void)result;
        }
    }

    // 4. Test load (from stream)
    {
        if (xml_size > 0) {
            std::stringstream stream;
            stream.write(reinterpret_cast<const char*>(xml_data), xml_size);
            stream.seekg(0);
            
            pugi::xml_document doc4;
            pugi::xml_parse_result result = doc4.load(stream, options, encoding);
            (void)result;
        }
    }

    // 5. Test load_buffer_inplace
    {
        if (xml_size > 0) {
            // Allocate buffer that persists for document lifetime
            std::unique_ptr<char[]> buffer(new char[xml_size]);
            std::memcpy(buffer.get(), xml_data, xml_size);
            
            pugi::xml_document doc5;
            pugi::xml_parse_result result = doc5.load_buffer_inplace(buffer.get(), xml_size, options, encoding);
            (void)result;
            // Buffer will be freed when unique_ptr goes out of scope
        }
    }

    // 6. Test load_string
    {
        if (xml_size > 0) {
            // Create null-terminated string
            std::unique_ptr<char[]> str_buffer(new char[xml_size + 1]);
            std::memcpy(str_buffer.get(), xml_data, xml_size);
            str_buffer[xml_size] = '\0';
            
            pugi::xml_document doc6;
            pugi::xml_parse_result result = doc6.load_string(str_buffer.get(), options);
            (void)result;
        }
    }

    return 0;
}