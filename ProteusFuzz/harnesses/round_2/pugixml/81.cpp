// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7739:46 in pugixml.hpp
// pugi::as_utf8 at pugixml.cpp:7842:44 in pugixml.hpp
// pugi::as_wide at pugixml.cpp:7861:59 in pugixml.hpp
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
#include <vector>

static void write_dummy_file(const std::string& path, const uint8_t* data, size_t size) {
    std::ofstream file(path, std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(data), size);
        file.close();
    }
}

static void write_dummy_file_wide(const std::wstring& path, const uint8_t* data, size_t size) {
    // For fuzzing, we write to a fixed narrow path since wide path support varies by OS
    // and the underlying file system usually handles narrow paths.
    std::string narrow_path = "./dummy_file_wide"; 
    std::ofstream file(narrow_path, std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(data), size);
        file.close();
    }
}

extern "C" int LLVMFuzzerTestOneInput_81(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Prepare dummy file paths
    const char* narrow_path = "./dummy_file";
    const wchar_t* wide_path = L"./dummy_file_wide";

    // Write input data to dummy files
    write_dummy_file(narrow_path, Data, Size);
    write_dummy_file_wide(std::wstring(wide_path), Data, Size);

    pugi::xml_document doc;
    pugi::xml_parse_result result;

    // Fuzz load_file (char* overload)
    unsigned int options_narrow = Size > 0 ? Data[0] : 0;
    pugi::xml_encoding encoding_narrow = Size > 1 ? static_cast<pugi::xml_encoding>(Data[1] % 10) : pugi::encoding_auto;
    result = doc.load_file(narrow_path, options_narrow, encoding_narrow);
    (void)result; // Suppress unused warning

    // Fuzz load_file (wchar_t* overload)
    unsigned int options_wide = Size > 2 ? Data[2] : 0;
    pugi::xml_encoding encoding_wide = Size > 3 ? static_cast<pugi::xml_encoding>(Data[3] % 10) : pugi::encoding_auto;
    result = doc.load_file(wide_path, options_wide, encoding_wide);
    (void)result; // Suppress unused warning

    // Fuzz as_utf8
    if (Size > 4) {
        // Create a wide string from input data (interpret as wide chars if possible)
        std::wstring wide_str;
        size_t wide_len = Size / sizeof(wchar_t);
        if (wide_len > 0) {
            wide_str.assign(reinterpret_cast<const wchar_t*>(Data), wide_len);
        }
        std::string utf8_str = pugi::as_utf8(wide_str.c_str());
        (void)utf8_str; // Suppress unused warning
    }

    // Fuzz as_wide
    if (Size > 4) {
        std::string narrow_str(reinterpret_cast<const char*>(Data), Size);
        std::wstring wide_str_result = pugi::as_wide(narrow_str);
        (void)wide_str_result; // Suppress unused warning
    }

    // Fuzz save_file (char* overload)
    unsigned int flags_narrow = Size > 5 ? Data[5] : 0;
    pugi::xml_encoding save_encoding_narrow = Size > 6 ? static_cast<pugi::xml_encoding>(Data[6] % 10) : pugi::encoding_auto;
    // Use a safe default indent
    const char* indent_narrow = "\t";
    bool save_success_narrow = doc.save_file(narrow_path, indent_narrow, flags_narrow, save_encoding_narrow);
    (void)save_success_narrow; // Suppress unused warning

    // Fuzz save_file (wchar_t* overload)
    unsigned int flags_wide = Size > 8 ? Data[8] : 0;
    pugi::xml_encoding save_encoding_wide = Size > 9 ? static_cast<pugi::xml_encoding>(Data[9] % 10) : pugi::encoding_auto;
    
    // Use PUGIXML_TEXT macro to correctly handle char_t (char or wchar_t)
    // This macro expands to L"\t" if PUGIXML_WCHAR_MODE is defined, else "\t"
    bool save_success_wide = doc.save_file(wide_path, PUGIXML_TEXT("\t"), flags_wide, save_encoding_wide);
    (void)save_success_wide; // Suppress unused warning

    // Cleanup is handled by RAII (xml_document destructor)
    return 0;
}