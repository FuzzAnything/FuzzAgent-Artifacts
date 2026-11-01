// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::load_buffer_inplace at pugixml.cpp:7756:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::load at pugixml.cpp:7697:46 in pugixml.hpp
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
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <sstream>
#include <vector>

static void write_dummy_file(const uint8_t* data, size_t size) {
    FILE* f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

extern "C" int LLVMFuzzerTestOneInput_82(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Helper to generate random options and encoding from input
    unsigned int options = (Size > 0) ? (Data[0] & 0xFF) : 0;
    pugi::xml_encoding encoding = (pugi::xml_encoding)((Size > 1) ? (Data[1] & 0x0F) : 0);
    if (encoding > pugi::encoding_latin1) encoding = pugi::encoding_auto;

    // 1. Test load_buffer (copies data)
    {
        pugi::xml_document doc;
        doc.load_buffer(Data, Size, options, encoding);
    }

    // 2. Test load_buffer_inplace (modifies buffer, caller owns lifetime)
    // We copy the fuzz input to a local buffer to ensure it persists
    std::vector<uint8_t> inplace_buffer(Size);
    if (Size > 0) {
        std::memcpy(inplace_buffer.data(), Data, Size);
    }
    {
        pugi::xml_document doc;
        doc.load_buffer_inplace(inplace_buffer.data(), inplace_buffer.size(), options, encoding);
    }

    // 3. Test load_buffer_inplace_own is skipped due to allocator constraints in standard fuzzing setup.

    // 4. Test load_string (requires null-terminated string)
    if (Size > 0) {
        // Create a temporary buffer with a null terminator to ensure safety
        std::vector<pugi::char_t> temp_buffer(Size + 1);
        for (size_t i = 0; i < Size; ++i) {
            temp_buffer[i] = static_cast<pugi::char_t>(Data[i]);
        }
        temp_buffer[Size] = 0;
        pugi::xml_document doc;
        doc.load_string(temp_buffer.data());
    }

    // 5. Test load (from stream)
    // Fix: Use std::vector<char> to create a buffer with explicit size and copy data,
    // ensuring the stream knows the correct length and doesn't read past the buffer.
    {
        pugi::xml_document doc;
        std::vector<char> buffer_vec(Size);
        if (Size > 0) {
            std::memcpy(buffer_vec.data(), Data, Size);
        }
        std::istringstream stream(std::string(buffer_vec.begin(), buffer_vec.end()), std::ios::binary);
        doc.load(stream, options, encoding);
    }

    // 6. Test load_file (from file)
    // Write data to a dummy file first
    write_dummy_file(Data, Size);
    {
        pugi::xml_document doc;
        doc.load_file(L"./dummy_file", options, encoding);
    }

    // Cleanup: Remove dummy file
    std::remove("./dummy_file");

    return 0;
}