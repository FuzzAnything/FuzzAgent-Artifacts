// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_text::set at pugixml.cpp:7040:30 in pugixml.hpp
// pugi::xml_text::as_bool at pugixml.cpp:7014:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7040:30 in pugixml.hpp
// pugi::xml_text::as_bool at pugixml.cpp:7014:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7040:30 in pugixml.hpp
// pugi::xml_text::as_string at pugixml.cpp:6974:39 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::empty at pugixml.cpp:6961:30 in pugixml.hpp
// pugi::xml_text::get at pugixml.cpp:6966:39 in pugixml.hpp
// pugi::xml_text::as_bool at pugixml.cpp:7014:30 in pugixml.hpp
// pugi::xml_text::as_string at pugixml.cpp:6974:39 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7119:30 in pugixml.hpp
// pugi::xml_text::empty at pugixml.cpp:6961:30 in pugixml.hpp
// pugi::xml_text::get at pugixml.cpp:6966:39 in pugixml.hpp
// pugi::xml_text::as_bool at pugixml.cpp:7014:30 in pugixml.hpp
// pugi::xml_text::as_string at pugixml.cpp:6974:39 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7040:30 in pugixml.hpp
// pugi::xml_text::empty at pugixml.cpp:6961:30 in pugixml.hpp
// pugi::xml_text::get at pugixml.cpp:6966:39 in pugixml.hpp
// pugi::xml_text::as_bool at pugixml.cpp:7014:30 in pugixml.hpp
// pugi::xml_text::as_string at pugixml.cpp:6974:39 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7040:30 in pugixml.hpp
// pugi::xml_text::get at pugixml.cpp:6966:39 in pugixml.hpp
// pugi::xml_text::empty at pugixml.cpp:6961:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7040:30 in pugixml.hpp
// pugi::xml_text::as_bool at pugixml.cpp:7014:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7040:30 in pugixml.hpp
// pugi::xml_text::as_bool at pugixml.cpp:7014:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7040:30 in pugixml.hpp
// pugi::xml_text::as_bool at pugixml.cpp:7014:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7040:30 in pugixml.hpp
// pugi::xml_text::as_bool at pugixml.cpp:7014:30 in pugixml.hpp
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include "pugixml.hpp"
#include <cstdint>
#include <cstring>
#include <string_view>

static void test_xml_text_api(const uint8_t *data, size_t size) {
    // Create a document and a root node to hold text
    pugi::xml_document doc;
    pugi::xml_node root = doc.append_child("root");
    
    // Create a text node
    pugi::xml_node text_node = root.append_child("text");
    pugi::xml_text text = text_node.text();

    // 1. Test xml_text::empty() on initially empty text
    bool is_empty = text.empty();
    (void)is_empty;

    // 2. Test xml_text::get() on initially empty text
    const pugi::char_t* get_result = text.get();
    (void)get_result;

    // 3. Test xml_text::as_bool() on initially empty text
    bool bool_result = text.as_bool(true); // Default true
    (void)bool_result;

    // 4. Test xml_text::as_string() on initially empty text
    const pugi::char_t* str_result = text.as_string(PUGIXML_TEXT("default"));
    (void)str_result;

    // Prepare data for setting text
    std::string_view input_view(reinterpret_cast<const char*>(data), size);
    
    // 5. Test xml_text::set(bool rhs)
    // Use first byte to determine boolean value
    bool bool_val = size > 0 ? (data[0] != 0) : false;
    bool set_bool_res = text.set(bool_val);
    (void)set_bool_res;

    // Verify after set(bool)
    (void)text.empty();
    (void)text.get();
    (void)text.as_bool(false);
    (void)text.as_string(PUGIXML_TEXT(""));

    // 6. Test xml_text::set(const char_t* rhs)
    // We need a null-terminated string. If input is not null-terminated, we handle it.
    // We'll use a small buffer to ensure safety or use the input directly if safe.
    // Since fuzz input might not be null-terminated, we construct a safe string.
    // We limit the length to avoid huge allocations.
    size_t max_len = 1024;
    if (size > max_len) size = max_len;
    
    // Create a temporary buffer with null terminator
    pugi::char_t buffer[1025];
    if (size > 0) {
        std::memcpy(buffer, data, size);
        buffer[size] = PUGIXML_CHAR(0);
    } else {
        buffer[0] = PUGIXML_CHAR(0);
    }
    
    bool set_str_res = text.set(buffer);
    (void)set_str_res;

    // Verify after set(string)
    (void)text.empty();
    (void)text.get();
    (void)text.as_bool(false);
    (void)text.as_string(PUGIXML_TEXT(""));

    // Additional diverse calls to explore states
    // Set to empty string
    text.set(PUGIXML_TEXT(""));
    (void)text.get();
    (void)text.empty();
    
    // Set to boolean true string
    text.set(PUGIXML_TEXT("1"));
    (void)text.as_bool(false);
    
    // Set to boolean false string
    text.set(PUGIXML_TEXT("0"));
    (void)text.as_bool(false);
    
    // Set to various boolean-like strings
    text.set(PUGIXML_TEXT("t"));
    (void)text.as_bool(false);
    text.set(PUGIXML_TEXT("T"));
    (void)text.as_bool(false);
    text.set(PUGIXML_TEXT("y"));
    (void)text.as_bool(false);
    text.set(PUGIXML_TEXT("Y"));
    (void)text.as_bool(false);
    
    // Test as_string with different defaults
    text.set(PUGIXML_TEXT(""));
    (void)text.as_string(PUGIXML_TEXT("fallback"));
}

extern "C" int LLVMFuzzerTestOneInput_56(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    // Handle potential file requirement if any (none strictly required for these APIs, but good practice)
    // No file operations needed for these specific text API calls.
    
    test_xml_text_api(Data, Size);
    
    return 0;
}