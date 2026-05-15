// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_buffer_inplace at pugixml.cpp:7756:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::load at pugixml.cpp:7697:46 in pugixml.hpp
// pugi::xml_document::load at pugixml.cpp:7704:46 in pugixml.hpp
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

static pugi::xml_encoding get_valid_encoding(uint8_t value) {
    // Map value 0-9 to valid xml_encoding enum values
    value = value % 10; // Ensure 0-9
    return static_cast<pugi::xml_encoding>(value);
}

static unsigned int get_options(const uint8_t* data, size_t size, size_t offset) {
    if (size - offset >= sizeof(unsigned int)) {
        unsigned int options;
        memcpy(&options, data + offset, sizeof(unsigned int));
        return options;
    }
    return pugi::parse_default;
}

extern "C" int LLVMFuzzerTestOneInput_58(const uint8_t* Data, size_t Size) {
    if (Size < 1) return 0;

    // Extract parameters from fuzzer input
    size_t offset = 0;
    
    // Get encoding from first byte
    pugi::xml_encoding encoding = get_valid_encoding(Data[offset++]);
    
    // Get options (use next 4 bytes if available)
    unsigned int options = get_options(Data, Size, offset);
    if (Size - offset >= sizeof(unsigned int)) {
        offset += sizeof(unsigned int);
    }

    // Remaining data is XML content
    const uint8_t* xml_data = Data + offset;
    size_t xml_size = Size - offset;
    
    if (xml_size == 0) return 0;

    // 1. Test load_file
    {
        write_dummy_file(xml_data, xml_size);
        pugi::xml_document doc1;
        pugi::xml_parse_result result = doc1.load_file("./dummy_file", options, encoding);
        (void)result; // Use result to avoid unused variable warning
    }

    // 2. Test load_buffer
    {
        pugi::xml_document doc2;
        pugi::xml_parse_result result = doc2.load_buffer(xml_data, xml_size, options, encoding);
        (void)result;
    }

    // 3. Test load with char stream
    {
        std::istringstream char_stream(std::string(reinterpret_cast<const char*>(xml_data), xml_size));
        pugi::xml_document doc3;
        pugi::xml_parse_result result = doc3.load(char_stream, options, encoding);
        (void)result;
    }

    // 3. Test load with wchar_t stream
    {
        // Convert data to wstring (simple conversion, may produce invalid wchar_t sequences)
        std::wstring wstr;
        for (size_t i = 0; i < xml_size && i + 1 < xml_size; i += 2) {
            wchar_t wc = (static_cast<wchar_t>(xml_data[i]) << 8) | xml_data[i + 1];
            wstr.push_back(wc);
        }
        std::wistringstream wchar_stream(wstr);
        pugi::xml_document doc4;
        pugi::xml_parse_result result = doc4.load(wchar_stream, options);
        (void)result;
    }

    // 4. Test load_buffer_inplace
    {
        // Create a mutable copy of the buffer for in-place parsing
        void* buffer = malloc(xml_size);
        if (buffer) {
            memcpy(buffer, xml_data, xml_size);
            pugi::xml_document doc5;
            pugi::xml_parse_result result = doc5.load_buffer_inplace(buffer, xml_size, options, encoding);
            (void)result;
            free(buffer);
        }
    }

    // 5. Test load_string
    {
        // Create null-terminated string from XML data
        std::string xml_string(reinterpret_cast<const char*>(xml_data), xml_size);
        if (!xml_string.empty()) {
            pugi::xml_document doc6;
            pugi::xml_parse_result result = doc6.load_string(xml_string.c_str(), options);
            (void)result;
        }
    }

    return 0;
}