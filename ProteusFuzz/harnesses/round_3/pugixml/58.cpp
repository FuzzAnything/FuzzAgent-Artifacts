// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::as_utf8 at pugixml.cpp:7842:44 in pugixml.hpp
// pugi::as_wide at pugixml.cpp:7861:59 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::as_wide at pugixml.cpp:7861:59 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7739:46 in pugixml.hpp
// pugi::xml_document::save_file at pugixml.cpp:7814:34 in pugixml.hpp
// pugi::xml_document::save_file at pugixml.cpp:7822:34 in pugixml.hpp
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

extern "C" int LLVMFuzzerTestOneInput_58(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Helper to generate random values from input data
    auto get_random_int = [Data, Size](size_t offset) -> unsigned int {
        if (offset + 4 > Size) return 0;
        unsigned int val = 0;
        std::memcpy(&val, Data + offset, sizeof(val));
        return val;
    };

    auto get_random_byte = [Data, Size](size_t offset) -> unsigned char {
        if (offset >= Size) return 0;
        return Data[offset];
    };

    // 1. Test pugi::as_utf8
    std::wstring wide_str;
    size_t wide_len = (Size > 1000) ? 1000 : Size;
    wide_str.reserve(wide_len);
    for (size_t i = 0; i < wide_len; ++i) {
        wide_str.push_back(static_cast<wchar_t>(Data[i] & 0xFF));
    }

    const wchar_t* wide_cstr = wide_str.c_str();
    std::string utf8_result = pugi::as_utf8(wide_cstr);
    // Removed pugi::as_utf8(nullptr) call as it triggers an assertion failure in debug builds

    // 2. Test pugi::as_wide
    std::string narrow_str(reinterpret_cast<const char*>(Data), Size);
    std::wstring wide_result = pugi::as_wide(narrow_str);

    // 3. Test pugi::xml_document::load_file
    pugi::xml_document doc;
    const char* dummy_file = "./dummy_file";
    FILE* f = fopen(dummy_file, "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }

    unsigned int options = get_random_int(0);
    int enc_idx = get_random_byte(1) % 10;
    pugi::xml_encoding encoding = static_cast<pugi::xml_encoding>(enc_idx);

    // Overload 1: const char* path
    pugi::xml_parse_result res1 = doc.load_file(dummy_file, options, encoding);

    // Overload 2: const wchar_t* path
    std::wstring wide_path = pugi::as_wide(std::string(dummy_file));
    pugi::xml_parse_result res2 = doc.load_file(wide_path.c_str(), options, encoding);

    // 4. Test pugi::xml_document::save_file
    unsigned int flags = get_random_int(2);
    
    // Prepare indent based on char_t type to ensure type safety
    // We use PUGIXML_TEXT macro to handle the char_t type correctly
    const pugi::char_t* indent = PUGIXML_TEXT("\t");

    // Overload 1: const char* path
    // Note: indent is const char_t*, which matches the signature
    bool save_res1 = doc.save_file(dummy_file, indent, flags, encoding);

    // Overload 2: const wchar_t* path
    bool save_res2 = doc.save_file(wide_path.c_str(), indent, flags, encoding);

    // Cleanup
    std::remove(dummy_file);

    return 0;
}