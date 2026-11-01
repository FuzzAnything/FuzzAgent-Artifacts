// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::as_utf8 at pugixml.cpp:7842:44 in pugixml.hpp
// pugi::as_wide at pugixml.cpp:7861:59 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
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
#include <fstream>
#include <algorithm>

static void write_dummy_file(const char* path, const uint8_t* data, size_t size) {
    std::ofstream file(path, std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(data), size);
    }
}

extern "C" int LLVMFuzzerTestOneInput_102(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Prepare data for wide string conversion tests
    std::wstring wide_str_input;
    std::string narrow_str_input;

    // Create wide string from input safely
    if (Size % sizeof(wchar_t) == 0 && Size > 0) {
        size_t wide_len = Size / sizeof(wchar_t);
        wide_str_input.assign(reinterpret_cast<const wchar_t*>(Data), wide_len);
    } else if (Size > 0) {
        // If not aligned, create a small safe wide string
        std::vector<wchar_t> dummy_vec;
        size_t limit = (Size < 10) ? Size : 10;
        dummy_vec.reserve(limit + 1);
        for (size_t i = 0; i < limit; ++i) {
            dummy_vec.push_back(static_cast<wchar_t>(Data[i]));
        }
        dummy_vec.push_back(L'\0'); // Ensure null termination
        wide_str_input.assign(dummy_vec.data(), limit);
    }

    // Create narrow string from input
    narrow_str_input.assign(reinterpret_cast<const char*>(Data), Size);

    // Test pugi::as_utf8
    std::string utf8_result = pugi::as_utf8(wide_str_input.c_str());
    (void)utf8_result;

    // Test pugi::as_wide
    std::wstring wide_result = pugi::as_wide(narrow_str_input);
    (void)wide_result;

    // Prepare file paths
    const char* dummy_path = "./dummy_file";
    
    // Write data to dummy file for load_file tests
    write_dummy_file(dummy_path, Data, Size);

    pugi::xml_document doc;

    // Generate random options and encoding
    unsigned int options = (Size > 0) ? (Data[0] & 0xFF) : 0;
    
    int encoding_val = (Size > 1) ? (Data[1] % 10) : 0;
    pugi::xml_encoding encoding = static_cast<pugi::xml_encoding>(encoding_val);

    // Test pugi::xml_document::load_file (char* overload)
    pugi::xml_parse_result result1 = doc.load_file(dummy_path, options, encoding);
    (void)result1;

    // Test pugi::xml_document::load_file (wchar_t* overload)
    // Create a valid wide string path to avoid buffer overflows in pugixml's internal conversion
    std::wstring wide_path_str = L"./dummy_file";
    pugi::xml_parse_result result2 = doc.load_file(wide_path_str.c_str(), options, encoding);
    (void)result2;

    // Prepare for save_file tests
    unsigned int flags = (Size > 2) ? (Data[2] & 0xFF) : 0;
    int encoding_val_save = (Size > 3) ? (Data[3] % 10) : 0;
    pugi::xml_encoding encoding_save = static_cast<pugi::xml_encoding>(encoding_val_save);

    // Test pugi::xml_document::save_file (char* overload)
    bool save_result1 = doc.save_file(dummy_path, PUGIXML_TEXT("\t"), flags, encoding_save);
    (void)save_result1;

    // Test pugi::xml_document::save_file (wchar_t* overload)
    // Use a valid wide string path
    bool save_result2 = doc.save_file(wide_path_str.c_str(), PUGIXML_TEXT("\t"), flags, encoding_save);
    (void)save_result2;

    // Cleanup
    std::remove(dummy_path);

    return 0;
}