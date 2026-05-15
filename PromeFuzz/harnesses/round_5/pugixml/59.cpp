// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::load at pugixml.cpp:7697:46 in pugixml.hpp
// pugi::xml_document::load at pugixml.cpp:7704:46 in pugixml.hpp
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
#include <fstream>
#include <sstream>
#include <memory>
#include <cstdint>
#include <cstring>

static bool isValidEncoding(pugi::xml_encoding enc) {
    return enc >= pugi::encoding_auto && enc <= pugi::encoding_latin1;
}

extern "C" int LLVMFuzzerTestOneInput_59(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    // Extract options and encoding from first two bytes
    unsigned int options = static_cast<unsigned int>(Data[0]);
    uint8_t enc_byte = Data[1] % 10; // Ensure 0-9
    pugi::xml_encoding encoding = static_cast<pugi::xml_encoding>(enc_byte);
    
    // Adjust data pointer and size for XML content
    const uint8_t* xml_data = Data + 2;
    size_t xml_size = Size - 2;

    // Create dummy file for load_file
    {
        std::ofstream file("./dummy_file", std::ios::binary);
        if (file.is_open()) {
            file.write(reinterpret_cast<const char*>(xml_data), xml_size);
            file.close();
        }
    }

    // 1. Test load_file
    {
        pugi::xml_document doc;
        if (isValidEncoding(encoding)) {
            pugi::xml_parse_result result = doc.load_file("./dummy_file", options, encoding);
            (void)result; // Ignore result for fuzzing
        }
    }

    // 2. Test load_buffer
    {
        pugi::xml_document doc;
        if (isValidEncoding(encoding)) {
            pugi::xml_parse_result result = doc.load_buffer(xml_data, xml_size, options, encoding);
            (void)result;
        }
    }

    // 3. Test load with char stream
    {
        pugi::xml_document doc;
        if (isValidEncoding(encoding)) {
            std::istringstream stream(std::string(reinterpret_cast<const char*>(xml_data), xml_size));
            pugi::xml_parse_result result = doc.load(stream, options, encoding);
            (void)result;
        }
    }

    // 4. Test load with wchar_t stream
    {
        pugi::xml_document doc;
        std::wstring wcontent;
        for (size_t i = 0; i < xml_size; ++i) {
            wcontent.push_back(static_cast<wchar_t>(xml_data[i]));
        }
        std::wistringstream stream(wcontent);
        pugi::xml_parse_result result = doc.load(stream, options);
        (void)result;
    }

    // 5. Test load_buffer_inplace
    {
        pugi::xml_document doc;
        if (isValidEncoding(encoding) && xml_size > 0) {
            // Allocate buffer that persists for document lifetime
            std::unique_ptr<char[]> buffer(new char[xml_size]);
            std::memcpy(buffer.get(), xml_data, xml_size);
            pugi::xml_parse_result result = doc.load_buffer_inplace(buffer.get(), xml_size, options, encoding);
            (void)result;
            // Buffer will be freed when unique_ptr goes out of scope
        }
    }

    // 6. Test load_string
    {
        pugi::xml_document doc;
        // Ensure null-terminated string
        std::string content(reinterpret_cast<const char*>(xml_data), xml_size);
        if (!content.empty()) {
            pugi::xml_parse_result result = doc.load_string(content.c_str(), options);
            (void)result;
        }
    }

    // Clean up dummy file
    std::remove("./dummy_file");

    return 0;
}