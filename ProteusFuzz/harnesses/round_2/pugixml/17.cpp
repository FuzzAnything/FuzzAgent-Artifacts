// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::get at pugixml.cpp:6966:39 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7040:30 in pugixml.hpp
// pugi::xml_text::data at pugixml.cpp:7212:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7040:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7040:30 in pugixml.hpp
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
#include "pugixml.hpp"
#include <cstdint>
#include <cstring>
#include <string>

extern "C" int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    pugi::xml_document doc;
    // Create a minimal valid XML structure to ensure nodes exist for traversal
    // We use a fixed template to guarantee the existence of a child named "test"
    // The fuzzer data will be used to set text content.
    std::string xml_template = "<root><test/></root>";
    if (!doc.load_string(xml_template.c_str())) {
        return 0;
    }

    pugi::xml_node root = doc.child("root");
    if (!root) {
        return 0;
    }

    // 1. pugi::xml_node::child
    // Find the child node named "test"
    pugi::xml_node target_node = root.child("test");
    if (!target_node) {
        return 0;
    }

    // 2. pugi::xml_node::text
    // Get the text proxy for the target node
    pugi::xml_text text_proxy = target_node.text();

    // 3. pugi::xml_text::get
    // Retrieve current text (should be empty or null-terminated string)
    const pugi::char_t* current_text = text_proxy.get();
    (void)current_text; // Suppress unused variable warning

    // 4. pugi::xml_text::set
    // Set text using the fuzzer input. 
    // We ensure null-termination by creating a std::string or using a safe copy.
    // Since set takes string_view_t (const char_t*), we construct a temporary string.
    // We limit the size to avoid potential issues with extremely large inputs in a simple loop,
    // though pugixml handles large allocations internally.
    if (Size > 0) {
        // Create a temporary string to ensure null termination
        std::string input_str(reinterpret_cast<const char*>(Data), Size);
        text_proxy.set(input_str.c_str());
    }

    // 5. pugi::xml_text::data
    // Get the underlying data node
    pugi::xml_node data_node = text_proxy.data();
    (void)data_node; // Suppress unused variable warning

    // 6. pugi::xml_text::set (Second call)
    // Set text again with a modified or different value to explore state changes.
    // We can try setting an empty string or a specific byte pattern.
    if (Size > 0) {
        // Try setting a substring or a fixed string to vary state
        std::string second_input = "fuzzed_data";
        text_proxy.set(second_input.c_str());
    } else {
        text_proxy.set("");
    }

    // 7. pugi::xml_node::last_child
    // Get the last child of the target node (which should be the text node we just manipulated)
    pugi::xml_node last_child = target_node.last_child();
    (void)last_child; // Suppress unused variable warning

    return 0;
}