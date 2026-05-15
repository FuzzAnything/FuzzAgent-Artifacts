// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_document::load_buffer_inplace_own at pugixml.cpp:7763:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::load at pugixml.cpp:7697:46 in pugixml.hpp
// pugi::xml_document::load_buffer_inplace at pugixml.cpp:7756:46 in pugixml.hpp
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

static void writeDummyFile(const uint8_t* data, size_t size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file) {
        file.write(reinterpret_cast<const char*>(data), size);
    }
}

static pugi::xml_encoding getValidEncoding(uint8_t encoding_byte) {
    if (encoding_byte >= pugi::encoding_auto && encoding_byte <= pugi::encoding_latin1) {
        return static_cast<pugi::xml_encoding>(encoding_byte);
    }
    return pugi::encoding_auto;
}

extern "C" int LLVMFuzzerTestOneInput_65(const uint8_t* data, size_t size) {
    if (size == 0) return 0;

    pugi::xml_document doc;
    unsigned int options = static_cast<unsigned int>(data[0]);
    uint8_t encoding_byte = size > 1 ? data[1] % 10 : 0;
    pugi::xml_encoding encoding = getValidEncoding(encoding_byte);
    
    const uint8_t* content_data = size > 2 ? data + 2 : data;
    size_t content_size = size > 2 ? size - 2 : size;

    // 1. load_buffer
    {
        pugi::xml_document doc1;
        doc1.load_buffer(content_data, content_size, options, encoding);
    }

    // 2. load_file
    {
        pugi::xml_document doc2;
        writeDummyFile(content_data, content_size);
        doc2.load_file("./dummy_file", options, encoding);
    }

    // 3. load_buffer_inplace_own
    {
        pugi::xml_document doc3;
        void* buffer = malloc(content_size);
        if (buffer) {
            memcpy(buffer, content_data, content_size);
            doc3.load_buffer_inplace_own(buffer, content_size, options, encoding);
        }
    }

    // 4. load_string
    {
        pugi::xml_document doc4;
        std::string str(content_data, content_data + content_size);
        if (!str.empty()) {
            doc4.load_string(str.c_str(), options);
        }
    }

    // 5. load (stream)
    {
        pugi::xml_document doc5;
        std::stringstream stream;
        stream.write(reinterpret_cast<const char*>(content_data), content_size);
        stream.seekg(0);
        doc5.load(stream, options, encoding);
    }

    // 6. load_buffer_inplace
    {
        pugi::xml_document doc6;
        void* buffer = malloc(content_size);
        if (buffer) {
            memcpy(buffer, content_data, content_size);
            doc6.load_buffer_inplace(buffer, content_size, options, encoding);
            free(buffer);
        }
    }

    return 0;
}