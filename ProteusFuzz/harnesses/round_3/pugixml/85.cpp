// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7490:34 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7490:34 in pugixml.hpp
// pugi::xml_document::load_buffer_inplace at pugixml.cpp:7756:46 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7490:34 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7490:34 in pugixml.hpp
// pugi::xml_document::load at pugixml.cpp:7697:46 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7490:34 in pugixml.hpp
// pugi::xml_document::load at pugixml.cpp:7704:46 in pugixml.hpp
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

static void write_dummy_file(const uint8_t* data, size_t size) {
    FILE* f = std::fopen("./dummy_file", "wb");
    if (f) {
        std::fwrite(data, 1, size, f);
        std::fclose(f);
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
    static const size_t count = sizeof(encodings) / sizeof(encodings[0]);
    return encodings[rand() % count];
}

extern "C" int LLVMFuzzerTestOneInput_85(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    pugi::xml_document doc;
    unsigned int options = static_cast<unsigned int>(rand());

    // 1. Test load_buffer
    {
        pugi::xml_parse_result result = doc.load_buffer(Data, Size, options, get_random_encoding());
        (void)result;
        doc.reset();
    }

    // 2. Test load_buffer_inplace
    // We need a mutable copy of the data for in-place parsing
    {
        std::vector<uint8_t> buffer(Data, Data + Size);
        pugi::xml_parse_result result = doc.load_buffer_inplace(buffer.data(), buffer.size(), options, get_random_encoding());
        (void)result;
        doc.reset();
    }

    // 3. Test load_string
    // Ensure null termination for safety
    {
        std::vector<uint8_t> str_buf(Data, Data + Size);
        // Append null terminator if not present or if size is 0 (though Size > 0 checked at start)
        if (Size > 0 && str_buf.back() != 0) {
            str_buf.push_back(0);
        } else if (Size == 0) {
            str_buf.push_back(0);
        }
        pugi::xml_parse_result result = doc.load_string(reinterpret_cast<const pugi::char_t*>(str_buf.data()), options);
        (void)result;
        doc.reset();
    }

    // 4. Test load (stream overloads)
    // 4a. std::basic_istream<char>
    // std::istringstream constructor with (const char*, size_t) is not standard in older C++ versions 
    // and can cause issues if the string isn't null-terminated. 
    // We construct a std::string first to ensure safety.
    {
        std::string str(reinterpret_cast<const char*>(Data), Size);
        std::istringstream stream(str);
        pugi::xml_parse_result result = doc.load(stream, options, get_random_encoding());
        (void)result;
        doc.reset();
    }
    
    // 4b. std::basic_istream<wchar_t>
    // Convert bytes to a std::wstring to initialize the stream correctly
    {
        std::wstring wstr;
        wstr.reserve(Size);
        for (size_t i = 0; i < Size; ++i) {
            wstr.push_back(static_cast<wchar_t>(Data[i]));
        }
        std::wistringstream wstream(wstr);
        pugi::xml_parse_result result = doc.load(wstream, options);
        (void)result;
        doc.reset();
    }

    // 5. Test load_file
    // Write data to a dummy file first
    write_dummy_file(Data, Size);
    {
        pugi::xml_parse_result result = doc.load_file("./dummy_file", options, get_random_encoding());
        (void)result;
        doc.reset();
    }

    // Cleanup dummy file
    std::remove("./dummy_file");

    return 0;
}