// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::get at pugixml.cpp:6966:39 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7055:30 in pugixml.hpp
// pugi::xml_text::data at pugixml.cpp:7212:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7055:30 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
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
#include <string>

static bool parse_input_to_document(const uint8_t* data, size_t size, pugi::xml_document& doc) {
    if (size == 0) {
        return false;
    }
    // Ensure we treat the input as a null-terminated string for load_string if it contains nulls
    // But load_string expects a C-string, so we must be careful with binary data.
    // We will create a temporary buffer with a guaranteed null terminator.
    std::vector<char> buffer(data, data + size);
    buffer.push_back('\0');
    
    pugi::xml_parse_result result = doc.load_string(buffer.data());
    return result.status == pugi::status_ok;
}

extern "C" int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    pugi::xml_document doc;
    
    // Attempt to parse the input as XML
    if (!parse_input_to_document(Data, Size, doc)) {
        // If parsing fails, try to create a minimal valid document to test logic
        doc.append_child("root");
    }

    pugi::xml_node root = doc.child("root");
    if (!root) {
        root = doc.first_child();
    }
    
    if (!root) {
        root = doc;
    }

    // 1. pugi::xml_node::child
    // We need a safe null-terminated string for the child name.
    // Since Data is binary, we cannot assume it's null-terminated or a valid string.
    // We will construct a safe name from the input, ensuring it's null-terminated and not too long.
    std::string safe_name;
    if (Size > 0) {
        // Find the first null byte or take up to 100 bytes
        size_t len = Size;
        const char* data_ptr = reinterpret_cast<const char*>(Data);
        size_t null_pos = strnlen(data_ptr, Size);
        if (null_pos < Size) {
            len = null_pos;
        }
        if (len > 100) len = 100;
        safe_name.assign(data_ptr, len);
    }
    if (safe_name.empty()) {
        safe_name = "test";
    }
    const char* child_name = safe_name.c_str();
    
    pugi::xml_node child_node = root.child(child_name);

    // 2. pugi::xml_node::text
    pugi::xml_text text_proxy = child_node.text();

    // 3. pugi::xml_text::get
    const pugi::char_t* text_content = text_proxy.get();
    (void)text_content; // Suppress unused variable warning

    // 4. pugi::xml_text::set (first call)
    // Prepare a string_view_t
    std::string_view new_text_view = "fuzzed_text_1";
    text_proxy.set(new_text_view);

    // 5. pugi::xml_text::data
    pugi::xml_node data_node = text_proxy.data();
    (void)data_node; // Suppress unused variable warning

    // 6. pugi::xml_text::set (second call)
    std::string_view new_text_view_2 = "fuzzed_text_2";
    text_proxy.set(new_text_view_2);

    // 7. pugi::xml_node::last_child
    pugi::xml_node last_child_node = root.last_child();
    (void)last_child_node; // Suppress unused variable warning

    return 0;
}