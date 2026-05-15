// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::get_memory_allocation_function at pugixml.cpp:7873:52 in pugixml.hpp
// pugi::xml_document::load_buffer_inplace_own at pugixml.cpp:7763:46 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::load at pugixml.cpp:7697:46 in pugixml.hpp
// pugi::xml_document::load_buffer_inplace at pugixml.cpp:7756:46 in pugixml.hpp
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
#include <sstream>
#include <vector>

static void writeDummyFile(const uint8_t* data, size_t size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(data), size);
        file.close();
    }
}

static pugi::xml_encoding getValidEncoding(uint8_t encoding_byte) {
    pugi::xml_encoding encoding = static_cast<pugi::xml_encoding>(encoding_byte);
    if (encoding >= pugi::encoding_auto && encoding <= pugi::encoding_latin1) {
        return encoding;
    }
    return pugi::encoding_auto;
}

extern "C" int LLVMFuzzerTestOneInput_117(const uint8_t* Data, size_t Size) {
    if (Size < 1) return 0;

    // Extract parameters from fuzzer input
    uint8_t func_selector = Data[0] % 6;
    uint8_t options_byte = (Size > 1) ? Data[1] : 0;
    uint8_t encoding_byte = (Size > 2) ? Data[2] : 0;
    const uint8_t* content_data = (Size > 3) ? Data + 3 : Data;
    size_t content_size = (Size > 3) ? Size - 3 : Size;

    pugi::xml_encoding encoding = getValidEncoding(encoding_byte);
    unsigned int options = static_cast<unsigned int>(options_byte);

    pugi::xml_document doc;

    switch (func_selector) {
        case 0: { // load_file
            writeDummyFile(content_data, content_size);
            pugi::xml_parse_result result = doc.load_file("./dummy_file", options, encoding);
            (void)result;
            break;
        }
        case 1: { // load_buffer_inplace_own
            if (content_size == 0) break;
            void* buffer = pugi::get_memory_allocation_function()(content_size);
            if (!buffer) break;
            std::memcpy(buffer, content_data, content_size);
            pugi::xml_parse_result result = doc.load_buffer_inplace_own(buffer, content_size, options, encoding);
            (void)result;
            break;
        }
        case 2: { // load_buffer
            pugi::xml_parse_result result = doc.load_buffer(content_data, content_size, options, encoding);
            (void)result;
            break;
        }
        case 3: { // load
            std::string content(reinterpret_cast<const char*>(content_data), content_size);
            std::istringstream stream(content);
            pugi::xml_parse_result result = doc.load(stream, options, encoding);
            (void)result;
            break;
        }
        case 4: { // load_buffer_inplace
            if (content_size == 0) break;
            char* buffer = new char[content_size];
            std::memcpy(buffer, content_data, content_size);
            pugi::xml_parse_result result = doc.load_buffer_inplace(buffer, content_size, options, encoding);
            (void)result;
            delete[] buffer;
            break;
        }
        case 5: { // load_string
            if (content_size == 0) break;
            std::vector<char> buffer(content_data, content_data + content_size);
            buffer.push_back('\0');
            pugi::xml_parse_result result = doc.load_string(buffer.data(), options);
            (void)result;
            break;
        }
    }

    return 0;
}