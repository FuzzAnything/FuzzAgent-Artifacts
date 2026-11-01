// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::save_file at pugixml.cpp:7822:34 in pugixml.hpp
// pugi::as_utf8 at pugixml.cpp:7849:44 in pugixml.hpp
// pugi::as_wide at pugixml.cpp:7861:59 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7739:46 in pugixml.hpp
// pugi::xml_document::save_file at pugixml.cpp:7814:34 in pugixml.hpp
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

static void write_dummy_file_wide(const wchar_t* path, const uint8_t* data, size_t size) {
    // Use low-level C API for wide file writing to avoid std::wofstream constructor issues
    // Convert wchar_t path to char_t path for fopen if necessary, but here we assume 
    // the environment supports wide paths or we map to a known narrow path for simplicity in fuzzing.
    // However, to strictly follow the wide API requirement without complex locale handling:
    // We will write to a fixed narrow path but simulate the wide call logic if possible,
    // or simply skip the wide file write if the environment doesn't support it easily.
    // For robustness in fuzzing, we'll write to a specific narrow path that we know exists.
    // But to satisfy the API call, we need a file to exist.
    // Let's create a narrow file and use it for both if wide file creation is problematic.
    // Actually, let's just write to a narrow path "dummy_file_wide" as a char path.
    const char* narrow_path = "./dummy_file_wide";
    std::ofstream file(narrow_path, std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(data), size);
        file.close();
    }
}

extern "C" int LLVMFuzzerTestOneInput_62(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // 1. Fuzz pugi::as_utf8
    std::wstring wide_input;
    size_t wide_size = Size / sizeof(wchar_t);
    if (wide_size > 0) {
        wide_input.assign(reinterpret_cast<const wchar_t*>(Data), wide_size);
    }
    std::string utf8_result = pugi::as_utf8(wide_input);
    (void)utf8_result;

    // 2. Fuzz pugi::as_wide
    std::string narrow_input(reinterpret_cast<const char*>(Data), Size);
    std::wstring wide_result = pugi::as_wide(narrow_input);
    (void)wide_result;

    // 3. Fuzz pugi::xml_document::load_file
    pugi::xml_document doc;
    
    unsigned int options = 0;
    if (Size > 0) options = Data[0] & 0xFF;
    if (Size > 1) options |= ((Data[1] & 0xFF) << 8);
    
    pugi::xml_encoding encoding = pugi::encoding_auto;
    if (Size > 2) {
        int enc_val = Data[2] % 10;
        encoding = static_cast<pugi::xml_encoding>(enc_val);
    }

    const char* dummy_path = "./dummy_file";
    const wchar_t* dummy_path_wide = L"./dummy_file_wide";
    
    write_dummy_file(dummy_path, Data, Size);
    write_dummy_file_wide(dummy_path_wide, Data, Size);

    // Invoke narrow overload
    pugi::xml_parse_result res1 = doc.load_file(dummy_path, options, encoding);
    (void)res1;

    // Invoke wide overload
    pugi::xml_parse_result res2 = doc.load_file(dummy_path_wide, options, encoding);
    (void)res2;

    // 4. Fuzz pugi::xml_document::save_file
    unsigned int flags = 0;
    if (Size > 3) flags = Data[3] & 0xFF;
    if (Size > 4) flags |= ((Data[4] & 0xFF) << 8);

    pugi::xml_encoding save_encoding = pugi::encoding_auto;
    if (Size > 5) {
        int enc_val = Data[5] % 10;
        save_encoding = static_cast<pugi::xml_encoding>(enc_val);
    }

    // Define indent string correctly for char_t
    const pugi::char_t* indent = PUGIXML_TEXT("\t");

    // Save to dummy files (narrow and wide paths)
    bool save_res1 = doc.save_file(dummy_path, indent, flags, save_encoding);
    (void)save_res1;

    bool save_res2 = doc.save_file(dummy_path_wide, indent, flags, save_encoding);
    (void)save_res2;

    return 0;
}