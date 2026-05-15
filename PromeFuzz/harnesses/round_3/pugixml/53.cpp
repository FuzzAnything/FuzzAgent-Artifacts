// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_buffer_inplace at pugixml.cpp:7756:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::load at pugixml.cpp:7697:46 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7739:46 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::get_memory_allocation_function at pugixml.cpp:7873:52 in pugixml.hpp
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
#include "pugixml.hpp"
#include <cstdint>
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

extern "C" int LLVMFuzzerTestOneInput_53(const uint8_t* Data, size_t Size) {
    if (Size == 0) return 0;

    // Prepare options and encoding from input data
    unsigned int options = 0;
    pugi::xml_encoding encoding = pugi::encoding_auto;
    
    if (Size >= 2) {
        options = Data[0] % 256;
        encoding = static_cast<pugi::xml_encoding>(Data[1] % 10);
    }

    // Prepare XML content (skip first 2 bytes if they exist)
    const uint8_t* xml_data = Data;
    size_t xml_size = Size;
    if (Size >= 2) {
        xml_data += 2;
        xml_size -= 2;
    }

    // Create null-terminated string for load_string
    std::string xml_string(reinterpret_cast<const char*>(xml_data), xml_size);
    if (xml_string.empty()) return 0;

    // 1. Test load_buffer_inplace
    {
        pugi::xml_document doc1;
        char* buffer = new char[xml_size];
        if (buffer) {
            std::memcpy(buffer, xml_data, xml_size);
            doc1.load_buffer_inplace(buffer, xml_size, options, encoding);
            delete[] buffer;
        }
    }

    // 2. Test load_string
    {
        pugi::xml_document doc2;
        doc2.load_string(xml_string.c_str(), options);
    }

    // 3. Test load with stream
    {
        pugi::xml_document doc3;
        std::istringstream stream(xml_string);
        if (encoding >= pugi::encoding_auto && encoding <= pugi::encoding_latin1) {
            doc3.load(stream, options, encoding);
        }
    }

    // 4. Test load_file
    {
        pugi::xml_document doc4;
        writeToDummyFile(xml_data, xml_size);
        if (encoding >= pugi::encoding_auto && encoding <= pugi::encoding_latin1) {
            doc4.load_file(L"./dummy_file", options, encoding);
        }
    }

    // 5. Test load_buffer
    {
        pugi::xml_document doc5;
        doc5.load_buffer(xml_data, xml_size, options, encoding);
    }

    // 6. Test load_buffer_inplace_own
    {
        pugi::xml_document doc6;
        void* buffer = pugi::get_memory_allocation_function()(xml_size);
        if (buffer) {
            std::memcpy(buffer, xml_data, xml_size);
            doc6.load_buffer_inplace_own(buffer, xml_size, options, encoding);
        }
    }

    return 0;
}