// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::load at pugixml.cpp:7724:46 in pugixml.hpp
// pugi::xml_document::load at pugixml.cpp:7697:46 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_document::save at pugixml.cpp:7799:34 in pugixml.hpp
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
    FILE* f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

extern "C" int LLVMFuzzerTestOneInput_102(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    pugi::xml_document doc;
    pugi::xml_parse_result result;

    // Helper to get random options from data
    auto get_options = [&](size_t idx) -> unsigned int {
        if (idx >= Size) return 0;
        return static_cast<unsigned int>(Data[idx]) % 256;
    };

    auto get_encoding = [&](size_t idx) -> pugi::xml_encoding {
        if (idx >= Size) return pugi::encoding_auto;
        int enc = Data[idx] % 10;
        return static_cast<pugi::xml_encoding>(enc);
    };

    // 1. Fuzz load_string
    // Ensure null termination for load_string
    std::vector<pugi::char_t> string_buf(Size + 1);
    if (sizeof(pugi::char_t) == 1) {
        std::memcpy(string_buf.data(), Data, Size);
        string_buf[Size] = 0;
    } else {
        // For wchar_t, we need to be careful. 
        // We'll just copy valid bytes if possible, or use a safe subset.
        // For simplicity in fuzzing, we'll treat input as bytes and cast, 
        // but ensure null termination.
        // This might produce invalid wide chars, which is good for fuzzing.
        size_t copy_len = Size < sizeof(pugi::char_t) ? Size : sizeof(pugi::char_t);
        std::memcpy(string_buf.data(), Data, copy_len);
        // Fill rest with 0 or simple patterns to avoid huge strings
        for (size_t i = copy_len; i < Size; ++i) {
            string_buf[i] = static_cast<pugi::char_t>(Data[i % 256]);
        }
        string_buf[Size] = 0;
    }

    doc.load_string(string_buf.data(), get_options(0));

    // 2. Fuzz load (deprecated string overload)
    // This is deprecated but we test it as requested
    doc.load(string_buf.data(), get_options(1));

    // 3. Fuzz load (stream overload)
    // FIX: Use std::string constructor that takes (char*, size) to avoid strlen issues
    // on non-null-terminated binary data.
    {
        std::string str(reinterpret_cast<const char*>(Data), Size);
        std::istringstream stream(str, std::ios::binary);
        doc.load(stream, get_options(2), get_encoding(3));
    }

    // 4. Fuzz load_buffer
    // This copies data, so it's safe to pass raw Data
    doc.load_buffer(Data, Size, get_options(4), get_encoding(5));

    // 5. Fuzz load_file
    // Write data to dummy file first
    write_dummy_file(Data, Size);
    doc.load_file("./dummy_file", get_options(6), get_encoding(7));

    // 6. Fuzz save
    // We need a valid document to save. The previous load operations might have populated it.
    // We try to save to a string stream.
    {
        std::basic_ostringstream<pugi::char_t> stream;
        // Try different flags
        unsigned int save_flags = get_options(8);
        // Combine with format_write_bom if the bit is set
        if (save_flags & 1) {
            save_flags |= pugi::format_write_bom;
        }
        doc.save(stream, PUGIXML_TEXT("\t"), save_flags);
    }

    return 0;
}