// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::load_buffer_inplace at pugixml.cpp:7756:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7739:46 in pugixml.hpp
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
    }
}

extern "C" int LLVMFuzzerTestOneInput_55(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Random seed for options and encoding based on input data
    unsigned int options = 0;
    if (Size >= 4) {
        options = *reinterpret_cast<const uint32_t*>(Data);
    } else if (Size > 0) {
        options = Data[0];
    }

    pugi::xml_encoding encoding = pugi::encoding_auto;
    if (Size >= 1) {
        int enc_idx = Data[Size - 1] % 10; // 0 to 9
        switch (enc_idx) {
            case 0: encoding = pugi::encoding_auto; break;
            case 1: encoding = pugi::encoding_utf8; break;
            case 2: encoding = pugi::encoding_utf16_le; break;
            case 3: encoding = pugi::encoding_utf16_be; break;
            case 4: encoding = pugi::encoding_utf16; break;
            case 5: encoding = pugi::encoding_utf32_le; break;
            case 6: encoding = pugi::encoding_utf32_be; break;
            case 7: encoding = pugi::encoding_utf32; break;
            case 8: encoding = pugi::encoding_wchar; break;
            case 9: encoding = pugi::encoding_latin1; break;
        }
    }

    // 1. Test load_buffer
    {
        pugi::xml_document doc;
        pugi::xml_parse_result result = doc.load_buffer(Data, Size, options, encoding);
        (void)result;
    }

    // 2. Test load_buffer_inplace (ensure buffer lifetime)
    {
        pugi::xml_document doc;
        std::vector<uint8_t> buffer(Size);
        std::memcpy(buffer.data(), Data, Size);
        pugi::xml_parse_result result = doc.load_buffer_inplace(buffer.data(), Size, options, encoding);
        (void)result;
    }

    // 3. Test load_string (ensure null termination)
    {
        pugi::xml_document doc;
        std::vector<char> str(Size + 1);
        std::memcpy(str.data(), Data, Size);
        str[Size] = '\0';
        pugi::xml_parse_result result = doc.load_string(str.data(), options);
        (void)result;
    }

    // 4. Test load_file
    {
        pugi::xml_document doc;
        write_dummy_file(Data, Size);
        pugi::xml_parse_result result = doc.load_file(L"./dummy_file", options, encoding);
        (void)result;
    }

    // 5. Test load from stream
    // Fix: Use std::string constructor that takes size to avoid strlen on non-null-terminated data
    {
        pugi::xml_document doc;
        std::string input(reinterpret_cast<const char*>(Data), Size);
        std::istringstream stream(input, std::ios::binary);
        pugi::xml_parse_result result = doc.load(stream, options, encoding);
        (void)result;
    }

    // 6. Test load_buffer_inplace_own
    // Note: pugixml requires memory allocated via its internal allocator for this function.
    // Since the internal allocator is not exposed, we cannot safely test this without 
    // risking undefined behavior or crashes. We skip this test to maintain stability.
    // If a custom allocator is registered globally, this could be tested, but by default, 
    // we omit it to prevent heap corruption.

    return 0;
}