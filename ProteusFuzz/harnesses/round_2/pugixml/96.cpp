// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_document::load_buffer_inplace at pugixml.cpp:7756:46 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::load at pugixml.cpp:7697:46 in pugixml.hpp
// pugi::xml_document::load at pugixml.cpp:7704:46 in pugixml.hpp
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

static void write_dummy_file(const uint8_t *Data, size_t Size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(Data), Size);
        file.close();
    }
}

static pugi::xml_encoding get_random_encoding(const uint8_t *Data, size_t Size) {
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
    size_t hash = 0;
    if (Size > 0) {
        hash = Data[0];
        if (Size > 1) hash ^= Data[1];
        if (Size > 2) hash ^= Data[2];
        if (Size > 3) hash ^= Data[3];
    }
    return encodings[hash % count];
}

static unsigned int get_random_options(const uint8_t *Data, size_t Size) {
    unsigned int hash = 0;
    size_t len = (Size > 4) ? 4 : Size;
    for (size_t i = 0; i < len; ++i) {
        hash = (hash << 8) | Data[i];
    }
    return hash;
}

extern "C" int LLVMFuzzerTestOneInput_96(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // 1. Test load_buffer_inplace
    {
        pugi::xml_document doc;
        std::vector<uint8_t> buffer(Data, Data + Size);
        unsigned int opts = get_random_options(Data, Size);
        pugi::xml_parse_result result = doc.load_buffer_inplace(buffer.data(), buffer.size(), opts, get_random_encoding(Data, Size));
        (void)result;
    }

    // 2. Test load_buffer
    {
        pugi::xml_document doc;
        unsigned int opts = get_random_options(Data, Size);
        pugi::xml_parse_result result = doc.load_buffer(Data, Size, opts, get_random_encoding(Data, Size));
        (void)result;
    }

    // 3. Test load (std::istream<char>)
    {
        pugi::xml_document doc;
        // Use std::string to ensure proper null-termination handling for stream construction
        // or use a vector and construct stream from data pointer with size
        std::string input_str(reinterpret_cast<const char*>(Data), Size);
        std::istringstream stream(input_str, std::ios::binary);
        unsigned int opts = get_random_options(Data, Size);
        pugi::xml_parse_result result = doc.load(stream, opts, get_random_encoding(Data, Size));
        (void)result;
    }

    // 3. Test load (std::istream<wchar_t>)
    {
        pugi::xml_document doc;
        std::vector<wchar_t> wbuffer(Size);
        for (size_t i = 0; i < Size; ++i) {
            wbuffer[i] = static_cast<wchar_t>(Data[i]);
        }
        std::wstring wstr(wbuffer.begin(), wbuffer.end());
        std::basic_istringstream<wchar_t> wstream(wstr);
        unsigned int opts = get_random_options(Data, Size);
        pugi::xml_parse_result result = doc.load(wstream, opts);
        (void)result;
    }

    // 4. Test load_string
    {
        pugi::xml_document doc;
        std::vector<pugi::char_t> str_buffer(Size + 1);
        size_t copy_size = 0;
        for (size_t i = 0; i < Size; ++i) {
            if (Data[i] == 0) {
                copy_size = i;
                break;
            }
            str_buffer[i] = static_cast<pugi::char_t>(Data[i]);
        }
        if (copy_size == 0 && Size > 0) {
            for (size_t i = 0; i < Size; ++i) {
                str_buffer[i] = static_cast<pugi::char_t>(Data[i]);
            }
            copy_size = Size;
        }
        str_buffer[copy_size] = 0;
        
        unsigned int opts = get_random_options(Data, Size);
        pugi::xml_parse_result result = doc.load_string(str_buffer.data(), opts);
        (void)result;
    }

    // 5. Test load_file
    {
        pugi::xml_document doc;
        write_dummy_file(Data, Size);
        unsigned int opts = get_random_options(Data, Size);
        pugi::xml_parse_result result = doc.load_file("./dummy_file", opts, get_random_encoding(Data, Size));
        (void)result;
    }

    return 0;
}