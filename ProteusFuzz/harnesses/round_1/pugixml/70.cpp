// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7490:34 in pugixml.hpp
// pugi::xml_document::load at pugixml.cpp:7697:46 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7490:34 in pugixml.hpp
// pugi::xml_document::load at pugixml.cpp:7724:46 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7490:34 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7490:34 in pugixml.hpp
// pugi::xml_document::load_buffer_inplace at pugixml.cpp:7756:46 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7490:34 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7490:34 in pugixml.hpp
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
#include <random>

static std::mt19937 rng(0);

static unsigned int random_options() {
    return static_cast<unsigned int>(rng());
}

static pugi::xml_encoding random_encoding() {
    return static_cast<pugi::xml_encoding>(rng() % 10);
}

extern "C" int LLVMFuzzerTestOneInput_70(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Prepare a mutable buffer for inplace operations
    std::vector<uint8_t> buffer(Size);
    if (Size > 0) {
        std::memcpy(buffer.data(), Data, Size);
    }

    // Ensure null-termination for string-based functions
    std::vector<char> string_buffer(Size + 1);
    if (Size > 0) {
        std::memcpy(string_buffer.data(), Data, Size);
    }
    string_buffer[Size] = '\0';

    pugi::xml_document doc;
    pugi::xml_parse_result result;

    // 1. Fuzz load_buffer (copies data)
    result = doc.load_buffer(Data, Size, random_options(), random_encoding());
    (void)result;
    doc.reset();

    // 2. Fuzz load (stream overload)
    // FIX: Use std::string to construct the stream to ensure proper length handling
    // and avoid reading past the buffer if it contains nulls or is not null-terminated.
    {
        std::string input_str(reinterpret_cast<const char*>(Data), Size);
        std::istringstream stream(input_str, std::ios::binary);
        result = doc.load(stream, random_options(), random_encoding());
        (void)result;
        doc.reset();
    }

    // 3. Fuzz load (string overload - deprecated but requested)
    result = doc.load(string_buffer.data(), random_options());
    (void)result;
    doc.reset();

    // 4. Fuzz load_file
    const char* dummy_path = "./dummy_file";
    FILE* f = fopen(dummy_path, "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
        
        result = doc.load_file(dummy_path, random_options(), random_encoding());
        (void)result;
        doc.reset();
        
        std::remove(dummy_path);
    }

    // 5. Fuzz load_buffer_inplace
    result = doc.load_buffer_inplace(buffer.data(), Size, random_options(), random_encoding());
    (void)result;
    doc.reset();

    // 6. Fuzz load_string
    result = doc.load_string(string_buffer.data(), random_options());
    (void)result;
    doc.reset();

    return 0;
}