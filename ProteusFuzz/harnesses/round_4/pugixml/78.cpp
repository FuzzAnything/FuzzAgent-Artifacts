// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load at pugixml.cpp:7697:46 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::load_buffer_inplace at pugixml.cpp:7756:46 in pugixml.hpp
// pugi::xml_document::load_buffer_inplace_own at pugixml.cpp:7763:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7739:46 in pugixml.hpp
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

// Helper to write data to a dummy file for load_file testing
static void write_dummy_file(const uint8_t* data, size_t size) {
    std::ofstream file("./dummy_file", std::ios::binary | std::ios::trunc);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(data), size);
        file.close();
    }
}

// Helper to generate a random encoding from the enum range
static pugi::xml_encoding get_random_encoding(uint32_t seed) {
    // Valid range is 0 to 9 (encoding_latin1)
    return static_cast<pugi::xml_encoding>(seed % 10);
}

extern "C" int LLVMFuzzerTestOneInput_78(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Use the first byte to seed random choices for options and encoding
    uint32_t seed = Data[0];
    unsigned int options = seed; // Random options
    pugi::xml_encoding encoding = get_random_encoding(seed);

    // 1. Test load_buffer (copies data, safe for raw binary)
    {
        pugi::xml_document doc;
        doc.load_buffer(Data, Size, options, encoding);
    }

    // 2. Test load_buffer_inplace (modifies buffer, requires allocation)
    {
        pugi::xml_document doc;
        // Allocate a copy of the input buffer using malloc (pugixml uses malloc internally)
        void* buffer = std::malloc(Size);
        if (buffer) {
            std::memcpy(buffer, Data, Size);
            doc.load_buffer_inplace(buffer, Size, options, encoding);
            // load_buffer_inplace does NOT take ownership. We must free it.
            std::free(buffer);
        }
    }

    // 3. Test load_buffer_inplace_own (takes ownership)
    {
        pugi::xml_document doc;
        // Allocate with malloc; pugixml expects memory allocated by its allocator (usually malloc)
        void* buffer = std::malloc(Size);
        if (buffer) {
            std::memcpy(buffer, Data, Size);
            doc.load_buffer_inplace_own(buffer, Size, options, encoding);
            // Do NOT free buffer here; the document owns it and will free it on destruction.
        }
    }

    // 4. Test load_string (requires null-terminated string)
    {
        pugi::xml_document doc;
        // Create a null-terminated copy
        std::vector<char> str_buf(Size + 1);
        std::memcpy(str_buf.data(), Data, Size);
        str_buf[Size] = '\0';
        
        doc.load_string(reinterpret_cast<const pugi::char_t*>(str_buf.data()), options);
    }

    // 5. Test load_file (requires a file on disk)
    {
        pugi::xml_document doc;
        // Write data to dummy file
        write_dummy_file(Data, Size);
        
        const wchar_t* path = L"./dummy_file";
        doc.load_file(path, options, encoding);
    }

    // 6. Test load (from stream)
    // FIX: std::istringstream constructor taking (const char*, size_t) is not standard C++98/11 in all contexts
    // or might behave unexpectedly if the string is not null-terminated in the buffer.
    // The safest way to feed binary data to a stream without assuming null-termination is to use std::string with explicit size
    // or std::vector<char> and a custom streambuf, but std::string constructor (ptr, size) is safe.
    {
        pugi::xml_document doc;
        // Construct a string from the raw data with explicit size. This does not require a null terminator.
        std::string s(reinterpret_cast<const char*>(Data), Size);
        std::istringstream stream(s, std::ios::binary);
        doc.load(stream, options, encoding);
    }

    return 0;
}