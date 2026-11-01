// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7739:46 in pugixml.hpp
// pugi::as_wide at pugixml.cpp:7861:59 in pugixml.hpp
// pugi::as_utf8 at pugixml.cpp:7849:44 in pugixml.hpp
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

static void write_dummy_file(const char* path, const uint8_t* data, size_t size) {
    std::ofstream file(path, std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(data), size);
        file.close();
    }
}

extern "C" int LLVMFuzzerTestOneInput_53(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    pugi::xml_document doc;
    
    // Prepare file paths
    const char* narrow_path = "./dummy_file";
    const wchar_t* wide_path = L"./dummy_file";

    // Write input data to dummy file for load_file tests
    write_dummy_file(narrow_path, Data, Size);

    // Randomize options and encoding
    unsigned int options = 0;
    if (Size > 0) {
        options = Data[0];
    }
    pugi::xml_encoding encoding = pugi::encoding_auto;
    if (Size > 1) {
        int enc_idx = Data[1] % 10;
        encoding = static_cast<pugi::xml_encoding>(enc_idx);
    }

    // Test load_file (char overload)
    doc.load_file(narrow_path, options, encoding);

    // Test load_file (wchar_t overload)
    doc.load_file(wide_path, options, encoding);

    // Test as_utf8 and as_wide conversions
    std::string narrow_str(reinterpret_cast<const char*>(Data), Size);
    std::wstring wide_str = pugi::as_wide(narrow_str);
    std::string converted_back = pugi::as_utf8(wide_str);

    // Test save_file (char overload)
    // Ensure indent is a valid, null-terminated string with static lifetime
    const char* indent = "\t";
    
    unsigned int flags = 0;
    if (Size > 3) {
        flags = Data[3];
    }
    pugi::xml_encoding save_encoding = pugi::encoding_auto;
    if (Size > 4) {
        int save_enc_idx = Data[4] % 10;
        save_encoding = static_cast<pugi::xml_encoding>(save_enc_idx);
    }

    doc.save_file(narrow_path, indent, flags, save_encoding);

    // Test save_file (wchar_t overload)
    doc.save_file(wide_path, indent, flags, save_encoding);

    // Cleanup is handled by RAII (xml_document destructor)
    return 0;
}