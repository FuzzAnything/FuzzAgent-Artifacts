// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::offset_debug at pugixml.cpp:6873:35 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
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
#include <cstddef>
#include <string_view>

static void test_node_functions(pugi::xml_node node) {
    // Test parent() function
    pugi::xml_node parent = node.parent();
    
    // Test type() function
    pugi::xml_node_type node_type = node.type();
    
    // Test first_child() function
    pugi::xml_node first_child = node.first_child();
    
    // Test last_child() function
    pugi::xml_node last_child = node.last_child();
    
    // Test offset_debug() function
    ptrdiff_t offset = node.offset_debug();
    
    // Use the results to avoid optimization
    (void)parent;
    (void)node_type;
    (void)first_child;
    (void)last_child;
    (void)offset;
}

static void test_append_child(pugi::xml_node node, const uint8_t *Data, size_t Size) {
    if (Size == 0) return;
    
    // Use first byte to determine node type (modulo number of node types)
    pugi::xml_node_type type = static_cast<pugi::xml_node_type>(Data[0] % 9);
    
    // Append child with the determined type
    pugi::xml_node new_child = node.append_child(type);
    
    // Test functions on the new child
    test_node_functions(new_child);
}

extern "C" int LLVMFuzzerTestOneInput_98(const uint8_t *Data, size_t Size) {
    // Test with empty node
    pugi::xml_node empty_node;
    test_node_functions(empty_node);
    
    // Test with document node
    pugi::xml_document doc;
    
    // Try to parse the input as XML
    pugi::xml_parse_result result = doc.load_buffer(Data, Size, pugi::parse_default | pugi::parse_fragment);
    
    if (result) {
        // Get document node
        pugi::xml_node root = doc;
        test_node_functions(root);
        
        // Test append_child on document node
        test_append_child(root, Data, Size);
        
        // Traverse the tree and test functions on all nodes
        for (pugi::xml_node child = root.first_child(); child; child = child.next_sibling()) {
            test_node_functions(child);
            
            // Test append_child on child nodes
            test_append_child(child, Data, Size);
            
            // Test traversal functions
            pugi::xml_node first = child.first_child();
            pugi::xml_node last = child.last_child();
            
            if (first) test_node_functions(first);
            if (last) test_node_functions(last);
        }
    }
    
    // Test with a newly created document
    pugi::xml_document new_doc;
    pugi::xml_node new_root = new_doc.append_child(pugi::node_element);
    
    // Test functions on the new root
    test_node_functions(new_root);
    
    // Test append_child multiple times with different node types
    for (size_t i = 0; i < Size && i < 10; ++i) {
        test_append_child(new_root, Data + i, Size - i);
    }
    
    // Test parent() on children
    for (pugi::xml_node child = new_root.first_child(); child; child = child.next_sibling()) {
        pugi::xml_node parent = child.parent();
        test_node_functions(parent);
    }
    
    return 0;
}