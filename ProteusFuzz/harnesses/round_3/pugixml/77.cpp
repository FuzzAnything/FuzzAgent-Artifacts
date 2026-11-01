// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7490:34 in pugixml.hpp
// pugi::xml_document::load_buffer_inplace at pugixml.cpp:7756:46 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7490:34 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7490:34 in pugixml.hpp
// pugi::xml_document::load at pugixml.cpp:7724:46 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7490:34 in pugixml.hpp
// pugi::xml_document::load at pugixml.cpp:7697:46 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7490:34 in pugixml.hpp
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
#include <fstream>

static void write_dummy_file(const uint8_t* data, size_t size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(data), size);
        file.close();
    }
}

static pugi::xml_encoding get_random_encoding() {
    const int encodings[] = {
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
    const size_t count = sizeof(encodings) / sizeof(encodings[0]);
    return static_cast<pugi::xml_encoding>(encodings[rand() % count]);
}

extern "C" int LLVMFuzzerTestOneInput_77(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    if (Size >= 4) {
        unsigned int seed = Data[0] ^ (Data[1] << 8) ^ (Data[2] << 16) ^ (Data[3] << 24);
        srand(seed);
    }

    pugi::xml_document doc;
    pugi::xml_parse_result result;

    // 1. Test load_buffer
    result = doc.load_buffer(Data, Size, rand(), get_random_encoding());
    (void)result;

    // 2. Test load_buffer_inplace
    std::vector<uint8_t> inplace_buf(Size);
    if (Size > 0) {
        std::memcpy(inplace_buf.data(), Data, Size);
        doc.reset();
        result = doc.load_buffer_inplace(inplace_buf.data(), Size, rand(), get_random_encoding());
        (void)result;
    }

    // 3. Test load_string
    // Ensure null termination by allocating Size + 1 and copying Size bytes
    std::vector<char> str_buf(Size + 1);
    if (Size > 0) {
        std::memcpy(str_buf.data(), Data, Size);
        str_buf[Size] = '\0';
        doc.reset();
        result = doc.load_string(str_buf.data(), rand());
        (void)result;
    }

    // 4. Test load (string overload - deprecated)
    if (Size > 0) {
        doc.reset();
        result = doc.load(str_buf.data(), rand());
        (void)result;
    }

    // 4b. Test load (stream overload)
    // Fix: Create a std::string with explicit size to avoid strlen reading past buffer
    std::string stream_str(reinterpret_cast<const char*>(Data), Size);
    std::istringstream stream(stream_str, std::ios::binary);
    doc.reset();
    result = doc.load(stream, rand(), get_random_encoding());
    (void)result;

    // 5. Test load_file
    write_dummy_file(Data, Size);
    doc.reset();
    result = doc.load_file("./dummy_file", rand(), get_random_encoding());
    (void)result;

    return 0;
}