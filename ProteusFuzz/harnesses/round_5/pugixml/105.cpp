// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::load at pugixml.cpp:7697:46 in pugixml.hpp
// pugi::xml_document::load at pugixml.cpp:7724:46 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_document::save at pugixml.cpp:7806:34 in pugixml.hpp
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
#include <random>

static void write_dummy_file(const uint8_t* data, size_t size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(data), size);
        file.close();
    }
}

extern "C" int LLVMFuzzerTestOneInput_105(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    std::mt19937 gen(std::hash<const uint8_t*>()(Data) ^ static_cast<uint64_t>(Size));
    std::uniform_int_distribution<unsigned int> options_dist(0, 0xFFFFFFFF);
    // Valid enum values for xml_encoding are 0 to 9. 
    // 0: encoding_auto
    // 1: encoding_utf8
    // 2: encoding_utf16_le
    // 3: encoding_utf16_be
    // 4: encoding_utf16
    // 5: encoding_utf32_le
    // 6: encoding_utf32_be
    // 7: encoding_utf32
    // 8: encoding_wchar
    // 9: encoding_latin1
    // We restrict the range to 0-9 to avoid triggering the "Invalid encoding" assert in pugixml.
    std::uniform_int_distribution<int> encoding_dist(0, 9); 

    pugi::xml_document doc;

    // 1. Fuzz load_buffer
    {
        pugi::xml_parse_result res = doc.load_buffer(Data, Size, options_dist(gen), static_cast<pugi::xml_encoding>(encoding_dist(gen)));
        (void)res;
    }

    // 2. Fuzz load_string (ensure null termination)
    {
        std::vector<char> buf(Size + 1);
        std::copy(Data, Data + Size, buf.begin());
        buf[Size] = '\0';
        pugi::xml_parse_result res = doc.load_string(buf.data(), options_dist(gen));
        (void)res;
    }

    // 3. Fuzz load from std::istream (char)
    {
        std::istringstream stream(std::string(reinterpret_cast<const char*>(Data), Size));
        pugi::xml_parse_result res = doc.load(stream, options_dist(gen), static_cast<pugi::xml_encoding>(encoding_dist(gen)));
        (void)res;
    }

    // 4. Fuzz load from zero-terminated string (deprecated overload)
    {
        std::vector<char> buf(Size + 1);
        std::copy(Data, Data + Size, buf.begin());
        buf[Size] = '\0';
        pugi::xml_parse_result res = doc.load(reinterpret_cast<const pugi::char_t*>(buf.data()), options_dist(gen));
        (void)res;
    }

    // 5. Fuzz load_file
    {
        write_dummy_file(Data, Size);
        pugi::xml_parse_result res = doc.load_file("./dummy_file", options_dist(gen), static_cast<pugi::xml_encoding>(encoding_dist(gen)));
        (void)res;
    }

    // 6. Fuzz save
    // The target signature is: void save(std::basic_ostream<wchar_t>& stream, ...)
    // We must use std::basic_ostream<wchar_t> and ensure the indent matches char_t.
    // PUGIXML_TEXT macro handles the conversion of "\t" to the correct char_t type.
    {
        std::wstringstream wstream;
        doc.save(wstream, PUGIXML_TEXT("\t"), options_dist(gen));
    }

    // Clean up dummy file
    std::remove("./dummy_file");

    return 0;
}