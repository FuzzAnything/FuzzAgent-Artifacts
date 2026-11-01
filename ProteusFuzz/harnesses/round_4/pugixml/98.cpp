// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::as_wide at pugixml.cpp:7861:59 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7739:46 in pugixml.hpp
// pugi::xml_document::save_file at pugixml.cpp:7814:34 in pugixml.hpp
// pugi::xml_document::save_file at pugixml.cpp:7822:34 in pugixml.hpp
// pugi::as_utf8 at pugixml.cpp:7842:44 in pugixml.hpp
// pugi::as_utf8 at pugixml.cpp:7842:44 in pugixml.hpp
// pugi::as_wide at pugixml.cpp:7861:59 in pugixml.hpp
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
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

// Helper to generate a random integer from the input data
static uint32_t get_random_uint(const uint8_t* data, size_t size, size_t& offset) {
    if (offset + 4 > size) {
        offset = 0;
        if (size < 4) return 0;
    }
    uint32_t val = 0;
    val |= static_cast<uint32_t>(data[offset++]);
    val |= static_cast<uint32_t>(data[offset++]) << 8;
    val |= static_cast<uint32_t>(data[offset++]) << 16;
    val |= static_cast<uint32_t>(data[offset++]) << 24;
    return val;
}

// Helper to generate a random string from input data
static std::string get_random_string(const uint8_t* data, size_t size, size_t& offset, size_t max_len) {
    if (size == 0) return "";
    size_t len = get_random_uint(data, size, offset) % (max_len + 1);
    if (offset + len > size) {
        len = (size - offset) > max_len ? max_len : (size - offset);
    }
    std::string result;
    result.reserve(len);
    for (size_t i = 0; i < len; ++i) {
        result += static_cast<char>(data[offset++]);
    }
    return result;
}

// Helper to generate a random wide string from input data
static std::wstring get_random_wstring(const uint8_t* data, size_t size, size_t& offset, size_t max_len) {
    if (size == 0) return L"";
    size_t len = get_random_uint(data, size, offset) % (max_len + 1);
    if (offset + len * 2 > size) {
        len = (size - offset) / 2;
        if (len > max_len) len = max_len;
    }
    std::wstring result;
    result.reserve(len);
    for (size_t i = 0; i < len; ++i) {
        if (offset + 2 > size) break;
        wchar_t wc = static_cast<wchar_t>(data[offset++]);
        wc |= static_cast<wchar_t>(data[offset++]) << 8;
        result += wc;
    }
    return result;
}

extern "C" int LLVMFuzzerTestOneInput_98(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    size_t offset = 0;

    // 1. Test pugi::as_utf8
    // Generate a random wide string
    std::wstring wide_str = get_random_wstring(Data, Size, offset, 1024);
    const wchar_t* wide_c_str = wide_str.c_str();
    
    // Call as_utf8 with wide string
    std::string utf8_result = pugi::as_utf8(wide_c_str);
    (void)utf8_result; // Suppress unused variable warning

    // Test as_utf8 with empty string
    const wchar_t* empty_wide = L"";
    utf8_result = pugi::as_utf8(empty_wide);

    // 2. Test pugi::as_wide
    // Generate a random narrow string
    std::string narrow_str = get_random_string(Data, Size, offset, 1024);
    
    // Call as_wide
    std::wstring wide_result = pugi::as_wide(narrow_str);
    (void)wide_result;

    // 3. Test pugi::xml_document::load_file and save_file
    // Create a dummy file with random data
    const char* dummy_file_path = "./dummy_file";
    std::ofstream dummy_file(dummy_file_path, std::ios::binary);
    if (dummy_file.is_open()) {
        // Write some random data to the file, ensuring it's not empty
        size_t file_len = Size > 1024 ? 1024 : Size;
        if (file_len > 0) {
            dummy_file.write(reinterpret_cast<const char*>(Data), file_len);
        } else {
            // Write minimal valid XML if input is too small or empty
            dummy_file.write("<root/>", 6);
        }
        dummy_file.close();
    }

    pugi::xml_document doc;
    
    // Generate random options and encoding
    uint32_t options = get_random_uint(Data, Size, offset);
    // Mask options to keep only valid bits if needed, but pugixml handles unknown flags gracefully usually
    // However, for safety, we can mask to known flags if we knew them, but let's let it be random as per hint
    
    int enc_idx = get_random_uint(Data, Size, offset) % 10; // 0 to 9
    pugi::xml_encoding encoding = static_cast<pugi::xml_encoding>(enc_idx);

    // Test load_file with char* path
    pugi::xml_parse_result result1 = doc.load_file(dummy_file_path, options, encoding);
    (void)result1;

    // Test load_file with wchar_t* path
    std::wstring wide_path = pugi::as_wide(std::string(dummy_file_path));
    pugi::xml_parse_result result2 = doc.load_file(wide_path.c_str(), options, encoding);
    (void)result2;

    // Test save_file with char* path
    // Generate random indent, flags, encoding for save
    std::string indent_str = get_random_string(Data, Size, offset, 16);
    const pugi::char_t* indent = indent_str.c_str();
    uint32_t flags = get_random_uint(Data, Size, offset);
    int enc_idx_save = get_random_uint(Data, Size, offset) % 10;
    pugi::xml_encoding encoding_save = static_cast<pugi::xml_encoding>(enc_idx_save);

    bool save_result1 = doc.save_file(dummy_file_path, indent, flags, encoding_save);
    (void)save_result1;

    // Test save_file with wchar_t* path
    bool save_result2 = doc.save_file(wide_path.c_str(), indent, flags, encoding_save);
    (void)save_result2;

    // Cleanup: Remove dummy file
    std::remove(dummy_file_path);

    return 0;
}