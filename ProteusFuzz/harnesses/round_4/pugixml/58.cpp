// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::load at pugixml.cpp:7724:46 in pugixml.hpp
// pugi::xml_document::load at pugixml.cpp:7697:46 in pugixml.hpp
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

static void write_dummy_file(const uint8_t* data, size_t size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(data), size);
        file.close();
    }
}

extern "C" int LLVMFuzzerTestOneInput_58(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    pugi::xml_document doc;
    pugi::xml_parse_result result;

    // 1. Test load_buffer (Primary method for raw binary fuzzing)
    unsigned int options = (Size > 0) ? (Data[0] & 0xFF) : 0;
    pugi::xml_encoding encoding = static_cast<pugi::xml_encoding>(Size > 1 ? (Data[1] & 0x0F) : 0);
    result = doc.load_buffer(Data, Size, options, encoding);

    // 2. Test load_string and load (const char_t*)
    // Ensure input is null-terminated for string-based loaders
    // We construct a string from the buffer, ensuring it doesn't read past Size
    std::string input_str;
    input_str.reserve(Size + 1);
    input_str.assign(reinterpret_cast<const char*>(Data), Size);
    input_str.push_back('\0'); // Ensure null termination
    const pugi::char_t* null_terminated_cstr = input_str.c_str();

    unsigned int options2 = (Size > 2) ? (Data[2] & 0xFF) : 0;
    result = doc.load_string(null_terminated_cstr, options2);

    // 3. Test load(const char_t* contents) [Deprecated]
    unsigned int options3 = (Size > 3) ? (Data[3] & 0xFF) : 0;
    result = doc.load(null_terminated_cstr, options3);

    // 4. Test load(std::basic_istream<char>& stream)
    // Use the null-terminated string to avoid reading past buffer bounds
    std::istringstream char_stream(input_str);
    unsigned int options4 = (Size > 4) ? (Data[4] & 0xFF) : 0;
    pugi::xml_encoding encoding4 = static_cast<pugi::xml_encoding>(Size > 5 ? (Data[5] & 0x0F) : 0);
    result = doc.load(char_stream, options4, encoding4);

    // 5. Test load_file
    write_dummy_file(Data, Size);
    unsigned int options5 = (Size > 6) ? (Data[6] & 0xFF) : 0;
    pugi::xml_encoding encoding5 = static_cast<pugi::xml_encoding>(Size > 7 ? (Data[7] & 0x0F) : 0);
    result = doc.load_file("./dummy_file", options5, encoding5);

    // 6. Test save(std::basic_ostream<wchar_t>& stream)
    std::wostringstream wstream;
    const pugi::char_t* indent = (Size > 8) ? (Data[8] == 0 ? PUGIXML_TEXT("\t") : PUGIXML_TEXT("  ")) : PUGIXML_TEXT("\t");
    unsigned int save_flags = (Size > 9) ? (Data[9] & 0xFF) : 0;
    
    doc.save(wstream, indent, save_flags);

    return 0;
}