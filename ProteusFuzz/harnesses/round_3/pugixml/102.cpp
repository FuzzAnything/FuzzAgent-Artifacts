// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load at pugixml.cpp:7724:46 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_document::load_buffer_inplace at pugixml.cpp:7756:46 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
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
#include <cstddef>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <vector>
#include <sstream>

// Helper to convert uint8_t* to pugi::char_t* (handles both narrow and wide modes)
static const pugi::char_t* to_char_t(const uint8_t* data, size_t size) {
    // In PUGIXML_WCHAR_MODE, char_t is wchar_t. We need to convert.
    // For fuzzing, we can try to interpret bytes as UTF-16 or just cast if not wide.
    // A simple approach: if wide mode, convert bytes to wchar_t assuming UTF-16 LE or just cast pairs.
    // However, to keep it simple and robust for fuzzing, we will just cast if not wide,
    // or create a temporary buffer if wide.
    
    #ifdef PUGIXML_WCHAR_MODE
        if (size % 2 != 0) return nullptr; // Invalid for UTF-16
        // Simple cast assuming the fuzzer provides valid UTF-16 or garbage that won't crash
        return reinterpret_cast<const pugi::char_t*>(data);
    #else
        return reinterpret_cast<const pugi::char_t*>(data);
    #endif
}

static void write_dummy_file(const uint8_t* data, size_t size) {
    FILE* f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

extern "C" int LLVMFuzzerTestOneInput_102(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Initialize random seed based on input to vary options/encoding deterministically per input
    unsigned int seed = 0;
    if (Size >= 4) {
        seed = Data[0] | (Data[1] << 8) | (Data[2] << 16) | (Data[3] << 24);
    }
    
    // Generate random options and encoding
    unsigned int options = seed & 0xFFFFFFFF;
    pugi::xml_encoding encoding = static_cast<pugi::xml_encoding>(seed % 10);

    // 1. Test load_buffer (copies data)
    {
        pugi::xml_document doc;
        pugi::xml_parse_result res = doc.load_buffer(Data, Size, options, encoding);
        // Test description()
        const char* desc = res.description();
        (void)desc; // Use to avoid unused warning, but effectively tests the function
    }

    // 2. Test load_buffer_inplace
    // We need a mutable buffer. Allocate one.
    std::vector<uint8_t> buffer(Size);
    if (Size > 0) {
        std::copy(Data, Data + Size, buffer.begin());
    }
    {
        pugi::xml_document doc;
        // Note: load_buffer_inplace takes void*, so we cast. 
        // The buffer must persist. Since buffer is local, it persists until end of scope.
        pugi::xml_parse_result res = doc.load_buffer_inplace(buffer.data(), Size, options, encoding);
        const char* desc = res.description();
        (void)desc;
    }

    // 3. Test load_string
    // Ensure null termination for string functions.
    std::vector<uint8_t> str_buf(Size + 1);
    if (Size > 0) {
        std::copy(Data, Data + Size, str_buf.begin());
    }
    str_buf[Size] = 0; // Null terminate
    
    const pugi::char_t* c_str = to_char_t(str_buf.data(), Size);
    if (c_str) {
        pugi::xml_document doc;
        pugi::xml_parse_result res = doc.load_string(c_str, options);
        const char* desc = res.description();
        (void)desc;
    }

    // 4. Test load (deprecated, but still valid for fuzzing)
    if (c_str) {
        pugi::xml_document doc;
        pugi::xml_parse_result res = doc.load(c_str, options);
        const char* desc = res.description();
        (void)desc;
    }

    // 5. Test load_file
    // Write data to a dummy file first
    write_dummy_file(Data, Size);
    {
        pugi::xml_document doc;
        pugi::xml_parse_result res = doc.load_file("./dummy_file", options, encoding);
        const char* desc = res.description();
        (void)desc;
    }

    // Cleanup: remove dummy file
    std::remove("./dummy_file");

    return 0;
}