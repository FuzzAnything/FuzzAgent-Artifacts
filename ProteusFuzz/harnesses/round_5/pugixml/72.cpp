// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::as_wide at pugixml.cpp:7861:59 in pugixml.hpp
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

static const char* DUMMY_FILE_PATH = "./dummy_file";

extern "C" int LLVMFuzzerTestOneInput_72(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a dummy file with the fuzzing data
    FILE* f = fopen(DUMMY_FILE_PATH, "wb");
    if (!f) {
        return 0;
    }
    fwrite(Data, 1, Size, f);
    fclose(f);

    pugi::xml_document doc;

    // Safe access to Data with bounds checking
    unsigned int options = 0;
    pugi::xml_encoding encoding = pugi::encoding_auto;
    
    if (Size > 0) {
        options = Data[0] % 16;
    }
    if (Size > 1) {
        encoding = static_cast<pugi::xml_encoding>(Data[1] % 10);
    }

    // Test load_file (char* overload)
    doc.load_file(DUMMY_FILE_PATH, options, encoding);

    // Test load_file (wchar_t* overload)
    std::wstring wide_path = pugi::as_wide(std::string(DUMMY_FILE_PATH));
    doc.load_file(wide_path.c_str(), options, encoding);

    // Test as_utf8
    std::wstring wide_str;
    if (Size >= sizeof(wchar_t)) {
        // Construct a wstring from input bytes, treating them as wide chars
        // Ensure we don't read past the buffer
        size_t count = Size / sizeof(wchar_t);
        if (count > 0) {
            wide_str.assign(reinterpret_cast<const wchar_t*>(Data), count);
        }
    }
    std::string utf8_str = pugi::as_utf8(wide_str.c_str());
    (void)utf8_str; // Suppress unused variable warning

    // Test as_wide
    std::string narrow_str(reinterpret_cast<const char*>(Data), Size);
    std::wstring wstr = pugi::as_wide(narrow_str);
    (void)wstr; // Suppress unused variable warning

    // Test save_file (char* overload)
    unsigned int save_flags = 0;
    pugi::xml_encoding save_encoding = pugi::encoding_auto;
    if (Size > 2) {
        save_flags = Data[2] % 8;
    }
    if (Size > 3) {
        save_encoding = static_cast<pugi::xml_encoding>(Data[3] % 10);
    }
    doc.save_file(DUMMY_FILE_PATH, PUGIXML_TEXT("\t"), save_flags, save_encoding);

    // Test save_file (wchar_t* overload)
    unsigned int save_flags_w = 0;
    pugi::xml_encoding save_encoding_w = pugi::encoding_auto;
    if (Size > 4) {
        save_flags_w = Data[4] % 8;
    }
    if (Size > 5) {
        save_encoding_w = static_cast<pugi::xml_encoding>(Data[5] % 10);
    }
    doc.save_file(wide_path.c_str(), PUGIXML_TEXT("\t"), save_flags_w, save_encoding_w);

    return 0;
}