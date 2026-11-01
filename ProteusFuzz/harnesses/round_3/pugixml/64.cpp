// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::set_value at pugixml.cpp:6009:30 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6009:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7047:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7047:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7047:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7040:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7040:30 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5987:30 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5987:30 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5987:30 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5977:30 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5977:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5987:30 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5977:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6019:30 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6009:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6019:30 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6009:30 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7047:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7040:30 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5977:30 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6019:30 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6019:30 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6019:30 in pugixml.hpp
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
#include <cstring>
#include <string_view>

// Static helper to parse XML from fuzz input
static pugi::xml_document parse_xml_from_input(const uint8_t* data, size_t size) {
    pugi::xml_document doc;
    if (size == 0) {
        return doc;
    }
    
    // Ensure null termination for safety
    // Create a buffer with explicit null termination
    std::vector<char> buffer(size + 1);
    std::memcpy(buffer.data(), data, size);
    buffer[size] = '\0';
    
    // Replace null bytes in the middle to avoid early termination issues
    for (size_t i = 0; i < size; ++i) {
        if (buffer[i] == '\0') {
            buffer[i] = ' ';
        }
    }
    
    pugi::xml_parse_result result = doc.load_string(buffer.data());
    return doc;
}

// Static helper to get a valid node from the document
static pugi::xml_node get_valid_node(pugi::xml_document& doc) {
    // Try to find a node that can have a value or name
    for (pugi::xml_node node = doc.first_child(); node; node = node.next_sibling()) {
        if (node.type() == pugi::node_element || 
            node.type() == pugi::node_pcdata || 
            node.type() == pugi::node_cdata || 
            node.type() == pugi::node_comment || 
            node.type() == pugi::node_pi || 
            node.type() == pugi::node_doctype) {
            return node;
        }
    }
    return doc;
}

// Static helper to get a valid text node
static pugi::xml_text get_valid_text(pugi::xml_document& doc) {
    for (pugi::xml_node node = doc.first_child(); node; node = node.next_sibling()) {
        if (node.type() == pugi::node_pcdata || node.type() == pugi::node_cdata) {
            return node.text();
        }
    }
    return pugi::xml_text();
}

extern "C" int LLVMFuzzerTestOneInput_64(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Create a document from the input
    pugi::xml_document doc = parse_xml_from_input(Data, Size);
    
    // If parsing failed or no nodes, try to create a simple document
    if (!doc.first_child()) {
        doc.append_child(pugi::node_element);
        doc.first_child().set_name("root");
    }

    // Get a valid node for testing
    pugi::xml_node node = get_valid_node(doc);
    pugi::xml_text text = get_valid_text(doc);

    // Prepare test strings from input with explicit null termination
    // Create a safe buffer with null terminator
    std::vector<char> safe_buffer(Size + 1);
    std::memcpy(safe_buffer.data(), Data, Size);
    safe_buffer[Size] = '\0';
    
    const char* c_str = safe_buffer.data();
    
    // Test set_value overloads
    // Overload 1: set_value(const char_t* rhs, size_t size)
    if (node) {
        node.set_value(c_str, Size);
        // Test with partial size
        if (Size > 10) {
            node.set_value(c_str, 10);
        }
        // Test with size 0
        node.set_value(c_str, 0);
    }
    
    // Overload 2: set_value(const char_t* rhs)
    if (node) {
        node.set_value(c_str);
        // Test with empty string
        node.set_value("");
    }

    // Test set overloads on text node
    if (text) {
        // Overload 1: set(const char_t* rhs, size_t size)
        text.set(c_str, Size);
        if (Size > 10) {
            text.set(c_str, 10);
        }
        text.set(c_str, 0);
        
        // Overload 2: set(const char_t* rhs)
        text.set(c_str);
        text.set("");
    }

    // Test set_name overloads
    if (node) {
        // Overload 1: set_name(const char_t* rhs, size_t size)
        node.set_name(c_str, Size);
        if (Size > 10) {
            node.set_name(c_str, 10);
        }
        node.set_name(c_str, 0);
        
        // Overload 2: set_name(const char_t* rhs)
        node.set_name(c_str);
        node.set_name("");
    }

    // Additional edge cases
    // Test with different node types by creating them
    pugi::xml_node pi_node = doc.append_child(pugi::node_pi);
    if (pi_node) {
        pi_node.set_name("test_pi", 7);
        pi_node.set_name("test_pi");
    }

    pugi::xml_node comment_node = doc.append_child(pugi::node_comment);
    if (comment_node) {
        comment_node.set_value(c_str, Size);
        comment_node.set_value(c_str);
    }

    pugi::xml_node cdata_node = doc.append_child(pugi::node_cdata);
    if (cdata_node) {
        cdata_node.set_value(c_str, Size);
        cdata_node.set_value(c_str);
        pugi::xml_text cdata_text = cdata_node.text();
        if (cdata_text) {
            cdata_text.set(c_str, Size);
            cdata_text.set(c_str);
        }
    }

    return 0;
}