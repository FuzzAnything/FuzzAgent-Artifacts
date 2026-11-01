// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::get at pugixml.cpp:6966:39 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child_value at pugixml.cpp:5946:39 in pugixml.hpp
// pugi::xml_node::child_value at pugixml.cpp:5946:39 in pugixml.hpp
// pugi::xml_node::child_value at pugixml.cpp:5928:39 in pugixml.hpp
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

// Ensure char_t is available by using the namespace or defining it if necessary
// pugixml defines char_t inside the pugi namespace, but it's also often available globally
// depending on configuration. To be safe, we use pugi::char_t.
using pugi::char_t;

extern "C" int LLVMFuzzerTestOneInput_31(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    pugi::xml_document doc;

    // Ensure null-termination for load_string
    // We create a buffer with the input data plus a null terminator
    char_t* input_buffer = new char_t[Size + 1];
    if (!input_buffer) {
        return 0;
    }

    std::memcpy(input_buffer, Data, Size);
    input_buffer[Size] = 0;

    // 1. pugi::xml_document::load_string
    pugi::xml_parse_result result = doc.load_string(input_buffer);
    
    // Cleanup allocated buffer
    delete[] input_buffer;

    // Helper to get a safe string from data for child names
    // We construct a temporary buffer to ensure null termination
    char_t dynamic_name[4];
    dynamic_name[0] = 0;
    dynamic_name[1] = 0;
    dynamic_name[2] = 0;
    dynamic_name[3] = 0;

    // Start traversal
    // We try to get the root node. If the document is empty or root is missing, 
    // current_node will be empty, which is safe for subsequent calls.
    pugi::xml_node current_node = doc.child("root");
    
    // 2-9. pugi::xml_node::child (8 times)
    for (int i = 0; i < 8; ++i) {
        if (Size > 0) {
            // Use a single byte from data as a name (if printable-ish) or just the byte
            // We must ensure null termination.
            dynamic_name[0] = static_cast<char_t>(Data[i % Size]);
            dynamic_name[1] = 0;
            current_node = current_node.child(dynamic_name);
        } else {
            current_node = current_node.child("node");
        }
    }

    // 10. pugi::xml_node::text
    pugi::xml_text text_node = current_node.text();
    // We can try to access the text, but just calling it is enough for coverage.
    const char_t* text_val = text_node.get();
    (void)text_val;

    // 11. pugi::xml_node::child (one more time)
    if (Size > 0) {
        dynamic_name[0] = static_cast<char_t>(Data[(Size - 1) % Size]);
        dynamic_name[1] = 0;
        current_node = current_node.child(dynamic_name);
    } else {
        current_node = current_node.child("next");
    }

    // 12. pugi::xml_node::child_value (overload 1: with name)
    if (Size > 0) {
        dynamic_name[0] = static_cast<char_t>(Data[0]);
        dynamic_name[1] = 0;
        const char_t* val1 = current_node.child_value(dynamic_name);
        (void)val1;
    } else {
        const char_t* val1 = current_node.child_value("val");
        (void)val1;
    }

    // 13. pugi::xml_node::child_value (overload 2: no name)
    const char_t* val2 = current_node.child_value();
    (void)val2;

    return 0;
}