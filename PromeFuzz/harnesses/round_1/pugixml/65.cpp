// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_document::load_buffer_inplace_own at pugixml.cpp:7763:46 in pugixml.hpp
// pugi::xml_document::load_buffer_inplace at pugixml.cpp:7756:46 in pugixml.hpp
// pugi::get_memory_allocation_function at pugixml.cpp:7873:52 in pugixml.hpp
// pugi::get_memory_deallocation_function at pugixml.cpp:7878:54 in pugixml.hpp
// pugi::xml_document::load_buffer_inplace at pugixml.cpp:7756:46 in pugixml.hpp
// pugi::xml_document::load at pugixml.cpp:7697:46 in pugixml.hpp
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
#include <fstream>
#include <sstream>
#include <vector>

static pugi::xml_encoding get_random_encoding(uint8_t value) {
    const pugi::xml_encoding encodings[] = {
        pugi::encoding_auto,
        pugi::encoding_utf8,
        pugi::encoding_utf16_le,
        pugi::encoding_utf16_be,
        pugi::encoding_utf16,
        pugi::encoding_utf32_le,
        pugi::encoding_utf32_be,
        pugi::encoding_utf32,
        pugi::encoding_wchar,
        pugi::encoding_latin1
    };
    return encodings[value % (sizeof(encodings) / sizeof(encodings[0]))];
}

static void* allocate_with_pugixml(size_t size) {
    pugi::allocation_function alloc = pugi::get_memory_allocation_function();
    if (alloc) {
        return alloc(size);
    }
    return nullptr;
}

static void deallocate_with_pugixml(void* ptr) {
    pugi::deallocation_function dealloc = pugi::get_memory_deallocation_function();
    if (dealloc) {
        dealloc(ptr);
    }
}

extern "C" int LLVMFuzzerTestOneInput_65(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Extract options and encoding from the input data
    unsigned int options = 0;
    pugi::xml_encoding encoding = pugi::encoding_auto;
    
    if (Size >= 1) {
        options = Data[0];
    }
    if (Size >= 2) {
        encoding = get_random_encoding(Data[1]);
    }
    
    // Use remaining data as XML content (skip first 2 bytes for options/encoding)
    const uint8_t* xml_data = Data + 2;
    size_t xml_size = (Size >= 2) ? Size - 2 : 0;
    
    // Ensure we have some XML data to parse
    if (xml_size == 0) {
        return 0;
    }

    // Test 1: load_buffer_inplace
    {
        pugi::xml_document doc1;
        void* buffer1 = malloc(xml_size);
        if (buffer1) {
            memcpy(buffer1, xml_data, xml_size);
            pugi::xml_parse_result result1 = doc1.load_buffer_inplace(buffer1, xml_size, options, encoding);
            (void)result1; // Use result to avoid unused variable warning
            free(buffer1);
        }
    }

    // Test 2: load from stream
    {
        pugi::xml_document doc2;
        std::istringstream stream(std::string(reinterpret_cast<const char*>(xml_data), xml_size));
        pugi::xml_parse_result result2 = doc2.load(stream, options, encoding);
        (void)result2;
    }

    // Test 3: load_buffer
    {
        pugi::xml_document doc3;
        pugi::xml_parse_result result3 = doc3.load_buffer(xml_data, xml_size, options, encoding);
        (void)result3;
    }

    // Test 4: load_string
    {
        pugi::xml_document doc4;
        // Ensure null termination for load_string
        std::vector<char> str_data(xml_data, xml_data + xml_size);
        str_data.push_back('\0');
        pugi::xml_parse_result result4 = doc4.load_string(str_data.data(), options);
        (void)result4;
    }

    // Test 5: load_file
    {
        pugi::xml_document doc5;
        // Write data to file
        std::ofstream file("./dummy_file", std::ios::binary);
        if (file) {
            file.write(reinterpret_cast<const char*>(xml_data), xml_size);
            file.close();
            
            // Try both char and wchar_t versions
            pugi::xml_parse_result result5 = doc5.load_file("./dummy_file", options, encoding);
            (void)result5;
            
            // Clean up
            std::remove("./dummy_file");
        }
    }

    // Test 6: load_buffer_inplace_own
    {
        pugi::xml_document doc6;
        void* buffer6 = allocate_with_pugixml(xml_size);
        if (buffer6) {
            memcpy(buffer6, xml_data, xml_size);
            pugi::xml_parse_result result6 = doc6.load_buffer_inplace_own(buffer6, xml_size, options, encoding);
            (void)result6;
            // Buffer will be freed by pugixml when doc6 is destroyed
        }
    }

    // Additional test: Try different option combinations
    if (Size >= 3) {
        pugi::xml_document doc7;
        unsigned int alt_options = Data[2];
        pugi::xml_encoding alt_encoding = get_random_encoding(Data[0]);
        
        void* buffer7 = malloc(xml_size);
        if (buffer7) {
            memcpy(buffer7, xml_data, xml_size);
            pugi::xml_parse_result result7 = doc7.load_buffer_inplace(buffer7, xml_size, alt_options, alt_encoding);
            (void)result7;
            free(buffer7);
        }
    }

    return 0;
}