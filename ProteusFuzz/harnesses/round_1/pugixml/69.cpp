// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::set_value at pugixml.cpp:6009:30 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6019:30 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6009:30 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6019:30 in pugixml.hpp
// pugi::as_utf8 at pugixml.cpp:7849:44 in pugixml.hpp
// pugi::as_utf8 at pugixml.cpp:7842:44 in pugixml.hpp
// pugi::as_wide at pugixml.cpp:7861:59 in pugixml.hpp
// pugi::as_wide at pugixml.cpp:7854:59 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
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
#include <cstring>
#include <string>
#include <vector>
#include <sstream>

// Helper to convert input data to a valid wide string for as_utf8 testing
static std::wstring create_wide_string(const uint8_t* data, size_t size) {
    std::wstring result;
    if (size == 0) return result;
    // Treat input as wchar_t, ensuring alignment and valid size
    if (size % sizeof(wchar_t) != 0) {
        // Pad to make it a multiple of wchar_t size
        size_t new_size = (size / sizeof(wchar_t) + 1) * sizeof(wchar_t);
        std::vector<uint8_t> padded(new_size, 0);
        std::memcpy(padded.data(), data, size);
        const wchar_t* wide_data = reinterpret_cast<const wchar_t*>(padded.data());
        size_t count = new_size / sizeof(wchar_t);
        result.assign(wide_data, count);
    } else {
        const wchar_t* wide_data = reinterpret_cast<const wchar_t*>(data);
        size_t count = size / sizeof(wchar_t);
        result.assign(wide_data, count);
    }
    return result;
}

// Helper to create a valid UTF-8 string from input for as_wide testing
static std::string create_utf8_string(const uint8_t* data, size_t size) {
    // Ensure the string is null-terminated for C-string overloads
    std::string result(reinterpret_cast<const char*>(data), size);
    // Remove potential null bytes in the middle to avoid early termination issues for C-string API
    // But for fuzzing, we want to test raw data, so we pass as-is to std::string overload
    // For C-string overload, we ensure a null terminator at the end if not present
    return result;
}

extern "C" int LLVMFuzzerTestOneInput_69(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // 1. Test pugi::as_utf8
    {
        // Overload 1: as_utf8(const std::basic_string<wchar_t>& str)
        std::wstring wide_str = create_wide_string(Data, Size);
        std::string utf8_result1 = pugi::as_utf8(wide_str);
        (void)utf8_result1; // Suppress unused variable warning

        // Overload 2: as_utf8(const wchar_t* str)
        // Create a null-terminated wide string
        std::wstring wide_str_nt = wide_str + L'\0';
        const wchar_t* wide_c_str = wide_str_nt.c_str();
        std::string utf8_result2 = pugi::as_utf8(wide_c_str);
        (void)utf8_result2;
    }

    // 2. Test pugi::as_wide
    {
        // Overload 1: as_wide(const std::basic_string<char>& str)
        std::string utf8_str = create_utf8_string(Data, Size);
        std::wstring wide_result1 = pugi::as_wide(utf8_str);
        (void)wide_result1;

        // Overload 2: as_wide(const char* str)
        // Ensure null termination for C-string overload
        std::string utf8_str_nt = utf8_str + '\0';
        const char* utf8_c_str = utf8_str_nt.c_str();
        std::wstring wide_result2 = pugi::as_wide(utf8_c_str);
        (void)wide_result2;
    }

    // 3. Test pugi::xml_node::set_value
    {
        // Create a minimal XML document to get a valid node
        pugi::xml_document doc;
        if (!doc.load_string("<root><node/></root>")) {
            // If parsing fails, try a simpler document
            doc.load_string("<root/>");
        }
        
        pugi::xml_node node = doc.child("root").child("node");
        if (!node) {
            // Fallback: create a text node or use root
            node = doc.child("root");
            if (!node) {
                // If still no node, create one
                node = doc.append_child("test_node");
            }
        }

        // Prepare test data for set_value
        std::string test_data(reinterpret_cast<const char*>(Data), Size);
        
        // Overload 1: set_value(const char_t* rhs)
        // Ensure null termination
        std::string test_data_nt = test_data + '\0';
        const char* c_str = test_data_nt.c_str();
        bool res1 = node.set_value(c_str);
        (void)res1;

        // Overload 2: set_value(const char_t* rhs, size_t size)
        // Use original size, but ensure we don't pass null if size > 0
        const char* c_str_raw = test_data.empty() ? "" : test_data.data();
        bool res2 = node.set_value(c_str_raw, Size);
        (void)res2;

        // Test with empty string
        bool res3 = node.set_value("");
        (void)res3;
        
        // Test with empty string and size 0
        bool res4 = node.set_value("", 0);
        (void)res4;
    }

    return 0;
}