// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::print at pugixml.cpp:6846:30 in pugixml.hpp
// pugi::xml_node::print at pugixml.cpp:6846:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::remove_attributes at pugixml.cpp:6561:30 in pugixml.hpp
// pugi::xml_node::remove_children at pugixml.cpp:6607:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::remove_attributes at pugixml.cpp:6561:30 in pugixml.hpp
// pugi::xml_node::remove_children at pugixml.cpp:6607:30 in pugixml.hpp
// pugi::xml_node::print at pugixml.cpp:6846:30 in pugixml.hpp
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
#include <fstream>

// Limit recursion depth and node count to prevent stack overflow
static const size_t MAX_DEPTH = 5;
static const size_t MAX_NODES = 20;

static pugi::xml_document create_document_from_input(const uint8_t* data, size_t size) {
    pugi::xml_document doc;
    
    if (size == 0) {
        doc.append_child(pugi::node_element);
        return doc;
    }
    
    // Try to parse as UTF-8 string
    std::string input(reinterpret_cast<const char*>(data), size);
    
    // If parsing fails, create a minimal document
    if (!doc.load_string(input.c_str())) {
        doc.append_child(pugi::node_element);
    }
    
    return doc;
}

static void safe_fuzz_node_operations(pugi::xml_node& node, const uint8_t* data, size_t size, size_t depth) {
    if (node.empty()) {
        return;
    }
    
    // Prevent infinite recursion or deep nesting
    if (depth > MAX_DEPTH) {
        return;
    }
    
    // Test type()
    pugi::xml_node_type type = node.type();
    
    // Test empty()
    bool is_empty = node.empty();
    
    // Test remove_attributes()
    bool remove_attrs_result = node.remove_attributes();
    
    // Test remove_children()
    bool remove_children_result = node.remove_children();
    
    // Test append_child() with different types
    if (size > 0) {
        // Use data to determine node type
        pugi::xml_node_type child_type = static_cast<pugi::xml_node_type>(data[0] % 9);
        pugi::xml_node child = node.append_child(child_type);
        
        if (!child.empty()) {
            // Test type() on child
            pugi::xml_node_type child_type_check = child.type();
            
            // Test empty() on child
            bool child_empty = child.empty();
            
            // Test remove_attributes() on child
            child.remove_attributes();
            
            // Test remove_children() on child
            child.remove_children();
            
            // Recursively fuzz the new child, but limit depth
            safe_fuzz_node_operations(child, data, size, depth + 1);
        }
    }
    
    // Test print()
    std::ostringstream oss;
    pugi::xml_writer_stream writer(oss);
    
    // Use data to determine print parameters
    unsigned int flags = 0;
    if (size > 1) {
        flags = data[1] & 0x0F; // Use lower 4 bits for flags
    }
    
    pugi::xml_encoding encoding = static_cast<pugi::xml_encoding>(size > 2 ? data[2] % 10 : 0);
    
    node.print(writer, "\t", flags, encoding, 0);
}

extern "C" int LLVMFuzzerTestOneInput_87(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    // Create document from input
    pugi::xml_document doc = create_document_from_input(Data, Size);
    
    // Get first child node
    pugi::xml_node root = doc.first_child();
    
    // If no child, create one
    if (root.empty()) {
        root = doc.append_child(pugi::node_element);
    }
    
    // Fuzz operations on root with depth limit
    safe_fuzz_node_operations(root, Data, Size, 0);
    
    // Fuzz operations on document itself
    safe_fuzz_node_operations(doc, Data, Size, 0);
    
    // Create additional nodes and test with limits
    if (Size > 0) {
        pugi::xml_node child = root.append_child(pugi::node_element);
        if (!child.empty()) {
            safe_fuzz_node_operations(child, Data, Size, 1);
        }
        
        // Test with text node
        pugi::xml_node text_node = root.append_child(pugi::node_pcdata);
        if (!text_node.empty()) {
            safe_fuzz_node_operations(text_node, Data, Size, 1);
        }
        
        // Test with comment node
        pugi::xml_node comment_node = root.append_child(pugi::node_comment);
        if (!comment_node.empty()) {
            safe_fuzz_node_operations(comment_node, Data, Size, 1);
        }
    }
    
    // Test print on document
    std::ostringstream oss;
    pugi::xml_writer_stream writer(oss);
    doc.print(writer, "\t", 0, pugi::encoding_auto, 0);
    
    // Test with file writer using FILE*
    FILE* file = fopen("./dummy_file", "wb");
    if (file) {
        pugi::xml_writer_file file_writer(file);
        doc.print(file_writer, "\t", 0, pugi::encoding_auto, 0);
        fclose(file);
    }
    
    return 0;
}