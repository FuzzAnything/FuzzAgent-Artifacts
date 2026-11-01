// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_document::load_buffer_inplace at pugixml.cpp:7756:46 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_document::load at pugixml.cpp:7724:46 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
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
    // Random value between 0 and 9 (number of enum values - 1)
    int val = rand() % 10;
    return static_cast<pugi::xml_encoding>(val);
}

extern "C" int LLVMFuzzerTestOneInput_105(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Seed random with input data safely. 
    // Only read if Size >= sizeof(unsigned int) to avoid buffer overflow.
    if (Size >= sizeof(unsigned int)) {
        srand(*reinterpret_cast<const unsigned int*>(Data));
    } else {
        // Fallback seed if data is too small
        srand(0);
    }

    pugi::xml_document doc;
    pugi::xml_parse_result result;

    // 1. Test load_buffer (copies data)
    result = doc.load_buffer(Data, Size, rand(), get_random_encoding());
    (void)result.description();

    // 2. Test load_buffer_inplace (modifies buffer, need a copy)
    std::vector<uint8_t> buffer_copy(Data, Data + Size);
    result = doc.load_buffer_inplace(buffer_copy.data(), Size, rand(), get_random_encoding());
    (void)result.description();

    // 3. Test load_string (requires null-terminated string)
    std::vector<uint8_t> str_buffer(Data, Data + Size);
    str_buffer.push_back(0); // Ensure null termination
    const char* c_str = reinterpret_cast<const char*>(str_buffer.data());
    result = doc.load_string(c_str, rand());
    (void)result.description();

    // 4. Test load (deprecated, same as load_string)
    result = doc.load(c_str, rand());
    (void)result.description();

    // 5. Test load_file
    write_dummy_file(Data, Size);
    result = doc.load_file("./dummy_file", rand(), get_random_encoding());
    (void)result.description();

    // 6. Test parsing result description on a fresh result object
    pugi::xml_parse_result fresh_result;
    (void)fresh_result.description();

    return 0;
}