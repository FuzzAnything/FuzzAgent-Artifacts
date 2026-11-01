// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load at pugixml.cpp:7724:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_document::load_buffer_inplace at pugixml.cpp:7756:46 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::load at pugixml.cpp:7697:46 in pugixml.hpp
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
    static const pugi::xml_encoding encodings[] = {
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
    return encodings[std::rand() % count];
}

extern "C" int LLVMFuzzerTestOneInput_56(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Seed random with input data to ensure reproducibility for specific inputs
    std::srand(Data[0]);

    // 1. Test load_buffer_inplace
    {
        pugi::xml_document doc;
        // We need a mutable buffer for inplace parsing
        std::vector<uint8_t> buffer(Data, Data + Size);
        pugi::xml_parse_result result = doc.load_buffer_inplace(
            buffer.data(), 
            buffer.size(), 
            std::rand(), 
            get_random_encoding()
        );
        (void)result;
    }

    // 2. Test load_buffer
    {
        pugi::xml_document doc;
        pugi::xml_parse_result result = doc.load_buffer(
            Data, 
            Size, 
            std::rand(), 
            get_random_encoding()
        );
        (void)result;
    }

    // 3. Test load (overloads)
    
    // 3a. load(std::basic_istream<char>&)
    {
        pugi::xml_document doc;
        // Create a temporary buffer with a null terminator to safely construct the stream
        std::vector<char> safe_buffer(Size + 1);
        std::memcpy(safe_buffer.data(), Data, Size);
        safe_buffer[Size] = '\0';
        
        std::istringstream stream(safe_buffer.data(), std::ios::binary);
        pugi::xml_parse_result result = doc.load(
            stream, 
            std::rand(), 
            get_random_encoding()
        );
        (void)result;
    }

    // 3b. load(const char_t*) - deprecated overload
    // This function expects a null-terminated string. 
    // Passing raw binary data without a null terminator causes heap-buffer-overflow
    // because the library calls strlen() internally.
    // We must ensure the buffer passed to this function is null-terminated.
    {
        pugi::xml_document doc;
        std::vector<char> safe_buffer(Size + 1);
        std::memcpy(safe_buffer.data(), Data, Size);
        safe_buffer[Size] = '\0';
        
        const char* cstr = safe_buffer.data();
        pugi::xml_parse_result result = doc.load(cstr, std::rand());
        (void)result;
    }

    // 4. Test load_string
    // Requires null-terminated string. We cannot blindly pass binary data.
    {
        pugi::xml_document doc;
        // Create a temporary buffer with a null terminator to ensure safety
        std::vector<uint8_t> safe_buf(Data, Data + Size);
        safe_buf.push_back(0);
        doc.load_string(reinterpret_cast<const pugi::char_t*>(safe_buf.data()), std::rand());
    }

    // 5. Test load_file
    {
        pugi::xml_document doc;
        write_dummy_file(Data, Size);
        pugi::xml_parse_result result = doc.load_file(
            "./dummy_file", 
            std::rand(), 
            get_random_encoding()
        );
        (void)result;
    }

    return 0;
}