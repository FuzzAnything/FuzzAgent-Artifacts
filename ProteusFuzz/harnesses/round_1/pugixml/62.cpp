// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::load at pugixml.cpp:7697:46 in pugixml.hpp
// pugi::xml_document::load at pugixml.cpp:7704:46 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
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
#include <pugixml.hpp>
#include <fstream>

static void write_dummy_file(const uint8_t* data, size_t size) {
    std::ofstream file("./dummy_file", std::ios::binary | std::ios::trunc);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(data), size);
        file.close();
    }
}

extern "C" int LLVMFuzzerTestOneInput_62(const uint8_t* Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Seed random generator based on input to ensure deterministic behavior for same input
    unsigned int seed = 0;
    if (Size >= 4) {
        std::memcpy(&seed, Data, 4);
    }
    // Simple pseudo-random generator for options and encoding
    auto get_random_int = [&](unsigned int max) -> unsigned int {
        seed ^= seed << 13;
        seed ^= seed >> 17;
        seed ^= seed << 5;
        return (seed % max);
    };

    pugi::xml_document doc;
    pugi::xml_parse_result result;

    // 1. Fuzz load_buffer
    result = doc.load_buffer(Data, Size, get_random_int(0xFFFFFFFF), static_cast<pugi::xml_encoding>(get_random_int(10)));

    // 2. Fuzz load (char stream)
    // Fix: Use constructor that takes pointer and length to avoid reading past buffer for null-termination
    {
        std::istringstream char_stream(std::string(reinterpret_cast<const char*>(Data), Size));
        result = doc.load(char_stream, get_random_int(0xFFFFFFFF), static_cast<pugi::xml_encoding>(get_random_int(10)));
    }

    // 3. Fuzz load (wchar stream)
    {
        // Convert input to wchar_t string for testing the wchar overload
        std::vector<wchar_t> wchar_data;
        if (Size % sizeof(wchar_t) == 0) {
            wchar_data.assign(reinterpret_cast<const wchar_t*>(Data), reinterpret_cast<const wchar_t*>(Data) + Size / sizeof(wchar_t));
        } else {
            // Pad to make it valid size
            size_t padded_size = (Size + sizeof(wchar_t) - 1) / sizeof(wchar_t) * sizeof(wchar_t);
            wchar_data.resize(padded_size / sizeof(wchar_t));
            std::memcpy(wchar_data.data(), Data, Size);
            // Zero fill the rest
            std::memset(reinterpret_cast<char*>(wchar_data.data()) + Size, 0, padded_size - Size);
        }

        // Construct wstring from vector to avoid null-termination issues
        std::wstring wstr(wchar_data.begin(), wchar_data.end());
        std::wistringstream wchar_stream(wstr);
        result = doc.load(wchar_stream, get_random_int(0xFFFFFFFF));
    }

    // 4. Fuzz load_file
    write_dummy_file(Data, Size);
    result = doc.load_file("./dummy_file", get_random_int(0xFFFFFFFF), static_cast<pugi::xml_encoding>(get_random_int(10)));

    // 5. Fuzz load_buffer_inplace
    // We need a mutable buffer. Allocate on heap to ensure it persists if needed.
    std::vector<uint8_t> inplace_buf(Data, Data + Size);
    result = doc.load_buffer_inplace(inplace_buf.data(), inplace_buf.size(), get_random_int(0xFFFFFFFF), static_cast<pugi::xml_encoding>(get_random_int(10)));

    // 6. Fuzz load_string
    // Ensure null termination. If the data already has a null, use it. Otherwise, append one.
    std::vector<char> str_buf(Data, Data + Size);
    if (Size > 0 && str_buf[Size - 1] != 0) {
        str_buf.push_back(0);
    } else if (Size == 0) {
        str_buf.push_back(0);
    }

    result = doc.load_string(str_buf.data(), get_random_int(0xFFFFFFFF));

    return 0;
}