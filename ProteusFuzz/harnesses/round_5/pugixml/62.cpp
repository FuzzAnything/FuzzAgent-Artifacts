// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5977:30 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5987:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7040:30 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6009:30 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6019:30 in pugixml.hpp
// pugi::xml_node::offset_debug at pugixml.cpp:6873:35 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::offset_debug at pugixml.cpp:6873:35 in pugixml.hpp
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
#include <string_view>

static pugi::xml_document create_document_from_input(const uint8_t* data, size_t size) {
    pugi::xml_document doc;
    if (size == 0) {
        return doc;
    }
    // Ensure the input is null-terminated for parsing
    std::string input(reinterpret_cast<const char*>(data), size);
    // Add a null terminator if not present
    if (input.back() != '\0') {
        input += '\0';
    }
    pugi::xml_parse_result result = doc.load_string(input.c_str());
    return doc;
}

extern "C" int LLVMFuzzerTestOneInput_62(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    pugi::xml_document doc = create_document_from_input(Data, Size);
    pugi::xml_node root;

    if (!doc) {
        // If parsing failed or returned empty, create a simple document manually
        root = doc.append_child("root");
    } else {
        root = doc.document_element();
        // If document parsed but has no root element, create one
        if (!root) {
            root = doc.append_child("root");
        }
    }

    // Prepare test data from input
    std::string test_str(reinterpret_cast<const char*>(Data), Size);
    if (test_str.back() != '\0') {
        test_str += '\0';
    }
    const char* c_str = test_str.c_str();
    size_t str_size = test_str.size() - 1; // Exclude null terminator for size-based calls

    // 1. Test pugi::xml_node::set_name
    // Overload 1: set_name(const char_t* rhs)
    if (root) {
        root.set_name(c_str);
    }
    // Overload 2: set_name(const char_t* rhs, size_t size)
    if (root) {
        root.set_name(c_str, str_size);
    }

    // Create a text node for testing xml_text::set
    pugi::xml_node text_node = root.append_child(pugi::node_pcdata);
    pugi::xml_text text = text_node.text();

    // 2. Test pugi::xml_text::set
    // Overload: set(const char_t* rhs)
    if (text) {
        text.set(c_str);
    }

    // 3. Test pugi::xml_node::set_value
    // Overload 1: set_value(const char_t* rhs)
    if (text_node) {
        text_node.set_value(c_str);
    }
    // Overload 2: set_value(const char_t* rhs, size_t size)
    if (text_node) {
        text_node.set_value(c_str, str_size);
    }

    // 4. Test pugi::xml_node::offset_debug
    // This function is for debugging and depends on the parsing context.
    // We call it on various nodes to trigger internal logic.
    ptrdiff_t offset = root.offset_debug();
    (void)offset; // Suppress unused variable warning

    // Try on child nodes if they exist
    for (pugi::xml_node node = root.first_child(); node; node = node.next_sibling()) {
        (void)node.offset_debug();
    }

    return 0;
}