// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::load_buffer_inplace at pugixml.cpp:7756:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::load at pugixml.cpp:7697:46 in pugixml.hpp
// pugi::xml_document::load at pugixml.cpp:7704:46 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <pugixml.hpp>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <sstream>
#include <vector>

static void write_dummy_file(const uint8_t* data, size_t size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(data), size);
    }
}

extern "C" int LLVMFuzzerTestOneInput_92(const uint8_t* Data, size_t Size) {
    if (Size == 0) return 0;

    // Convert fuzz input to unsigned int for options and encoding
    unsigned int options = 0;
    pugi::xml_encoding encoding = pugi::encoding_auto;

    if (Size >= 4) {
        options = *reinterpret_cast<const uint32_t*>(Data);
        Data += 4;
        Size -= 4;
    }

    if (Size >= 1) {
        uint8_t enc_val = Data[0];
        encoding = static_cast<pugi::xml_encoding>(enc_val % 10); // 10 possible encodings
        Data += 1;
        Size -= 1;
    }

    // Ensure at least some data remains for XML content
    if (Size == 0) return 0;

    const uint8_t* xml_data = Data;
    size_t xml_size = Size;

    // 1. Test load_buffer
    {
        pugi::xml_document doc;
        doc.load_buffer(xml_data, xml_size, options, encoding);
    }

    // 2. Test load_buffer_inplace
    {
        pugi::xml_document doc;
        // Make a mutable copy for in-place parsing
        std::vector<uint8_t> buffer(xml_data, xml_data + xml_size);
        doc.load_buffer_inplace(buffer.data(), buffer.size(), options, encoding);
    }

    // 3. Test load_string
    {
        pugi::xml_document doc;
        // Ensure null-termination for load_string
        std::vector<char> str_buf(xml_size + 1);
        std::memcpy(str_buf.data(), xml_data, xml_size);
        str_buf[xml_size] = '\0';
        doc.load_string(str_buf.data(), options);
    }

    // 4. Test load (char stream)
    {
        pugi::xml_document doc;
        std::istringstream stream(std::string(reinterpret_cast<const char*>(xml_data), xml_size));
        doc.load(stream, options, encoding);
    }

    // 5. Test load (wchar stream)
    {
        pugi::xml_document doc;
        std::wstring wstr;
        // Convert to wstring for wchar stream test
        for (size_t i = 0; i < xml_size; ++i) {
            wstr.push_back(static_cast<wchar_t>(xml_data[i]));
        }
        std::wistringstream wstream(wstr);
        doc.load(wstream, options);
    }

    // 6. Test load_file
    {
        pugi::xml_document doc;
        write_dummy_file(xml_data, xml_size);
        doc.load_file("./dummy_file", options, encoding);
    }

    return 0;
}