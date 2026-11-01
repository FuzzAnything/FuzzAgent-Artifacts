// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::load_buffer_inplace at pugixml.cpp:7756:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::load at pugixml.cpp:7724:46 in pugixml.hpp
// pugi::xml_document::load at pugixml.cpp:7697:46 in pugixml.hpp
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
    }
}

extern "C" int LLVMFuzzerTestOneInput_73(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Seed random number generator with input data to ensure deterministic behavior
    unsigned int seed = 0;
    if (Size >= 4) {
        seed = *reinterpret_cast<const unsigned int*>(Data);
    }
    std::srand(seed);

    // Generate random options and encoding
    unsigned int options = static_cast<unsigned int>(rand() % 0xFFFFFFFF);
    pugi::xml_encoding encoding = static_cast<pugi::xml_encoding>(rand() % 10);

    // Test load_buffer
    {
        pugi::xml_document doc;
        pugi::xml_parse_result result = doc.load_buffer(Data, Size, options, encoding);
        (void)result; // Suppress unused variable warning
    }

    // Test load_buffer_inplace
    {
        pugi::xml_document doc;
        std::vector<uint8_t> buffer(Size);
        std::memcpy(buffer.data(), Data, Size);
        pugi::xml_parse_result result = doc.load_buffer_inplace(buffer.data(), Size, options, encoding);
        (void)result;
    }

    // Test load_string
    {
        pugi::xml_document doc;
        // Ensure null termination for load_string
        std::string str(reinterpret_cast<const char*>(Data), Size);
        str.push_back('\0');
        pugi::xml_parse_result result = doc.load_string(str.c_str(), options);
        (void)result;
    }

    // Test load (deprecated overload with string)
    {
        pugi::xml_document doc;
        std::string str(reinterpret_cast<const char*>(Data), Size);
        str.push_back('\0');
        pugi::xml_parse_result result = doc.load(str.c_str(), options);
        (void)result;
    }

    // Test load (stream overload)
    {
        pugi::xml_document doc;
        // Create a std::string first to ensure proper handling of the buffer
        std::string str(reinterpret_cast<const char*>(Data), Size);
        std::istringstream stream(str, std::ios::binary);
        pugi::xml_parse_result result = doc.load(stream, options, encoding);
        (void)result;
    }

    // Test load_file
    {
        pugi::xml_document doc;
        write_dummy_file(Data, Size);
        pugi::xml_parse_result result = doc.load_file("./dummy_file", options, encoding);
        (void)result;
    }

    return 0;
}