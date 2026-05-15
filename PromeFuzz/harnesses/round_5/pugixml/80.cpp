// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_text::empty at pugixml.cpp:6961:30 in pugixml.hpp
// pugi::xml_attribute::empty at pugixml.cpp:5360:35 in pugixml.hpp
// pugi::xml_node::remove_attributes at pugixml.cpp:6561:30 in pugixml.hpp
// pugi::xml_node::remove_children at pugixml.cpp:6607:30 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::empty at pugixml.cpp:6961:30 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_attribute::next_attribute at pugixml.cpp:5289:44 in pugixml.hpp
// pugi::xml_attribute::empty at pugixml.cpp:5360:35 in pugixml.hpp
// pugi::xml_node::children at pugixml.cpp:5638:61 in pugixml.hpp
// pugi::xml_object_range::empty at pugixml.hpp:338:8 in pugixml.hpp
// pugi::xml_node::attributes at pugixml.cpp:5648:66 in pugixml.hpp
// pugi::xml_object_range::empty at pugixml.hpp:338:8 in pugixml.hpp
// pugi::xml_node::remove_attributes at pugixml.cpp:6561:30 in pugixml.hpp
// pugi::xml_node::remove_children at pugixml.cpp:6607:30 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
// pugi::xml_node::children at pugixml.cpp:5638:61 in pugixml.hpp
// pugi::xml_node::children at pugixml.cpp:5638:61 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_text::empty at pugixml.cpp:6961:30 in pugixml.hpp
// pugi::xml_attribute::empty at pugixml.cpp:5360:35 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_attribute::empty at pugixml.cpp:5360:35 in pugixml.hpp
// pugi::xml_object_range::empty at pugixml.hpp:338:8 in pugixml.hpp
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

static void test_empty_functions(pugi::xml_node node) {
    // Test xml_node::empty
    bool node_empty = node.empty();
    
    // Test xml_text::empty
    pugi::xml_text text = node.text();
    bool text_empty = text.empty();
    
    // Test xml_attribute::empty
    for (pugi::xml_attribute attr = node.first_attribute(); attr; attr = attr.next_attribute()) {
        bool attr_empty = attr.empty();
    }
    
    // Test xml_object_range::empty for children
    pugi::xml_object_range<pugi::xml_node_iterator> children_range = node.children();
    bool children_range_empty = children_range.empty();
    
    // Test xml_object_range::empty for attributes
    pugi::xml_object_range<pugi::xml_attribute_iterator> attributes_range = node.attributes();
    bool attributes_range_empty = attributes_range.empty();
}

static void test_remove_functions(pugi::xml_node node) {
    // Test remove_attributes
    bool removed_attrs = node.remove_attributes();
    
    // Test remove_children
    bool removed_children = node.remove_children();
}

extern "C" int LLVMFuzzerTestOneInput_80(const uint8_t *Data, size_t Size) {
    // Create a dummy file with the fuzzer input
    FILE* dummy_file = fopen("./dummy_file", "wb");
    if (!dummy_file) {
        return 0;
    }
    fwrite(Data, 1, Size, dummy_file);
    fclose(dummy_file);
    
    // Try to parse the XML from the dummy file
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file("./dummy_file");
    
    if (result) {
        // Traverse the document and test functions on all nodes
        pugi::xml_node root = doc.document_element();
        
        // Test on root node if exists
        if (root) {
            test_empty_functions(root);
            test_remove_functions(root);
            
            // Test on all child nodes
            for (pugi::xml_node child : root.children()) {
                test_empty_functions(child);
                test_remove_functions(child);
                
                // Test on grandchildren
                for (pugi::xml_node grandchild : child.children()) {
                    test_empty_functions(grandchild);
                }
            }
        }
        
        // Also test on the document itself
        test_empty_functions(doc);
        
        // Test with default-constructed objects
        pugi::xml_node default_node;
        pugi::xml_text default_text;
        pugi::xml_attribute default_attr;
        
        bool default_node_empty = default_node.empty();
        bool default_text_empty = default_text.empty();
        bool default_attr_empty = default_attr.empty();
        
        // Test with internal pointer constructors (using nullptr to simulate invalid state)
        pugi::xml_node null_node(static_cast<pugi::xml_node_struct*>(nullptr));
        pugi::xml_attribute null_attr(static_cast<pugi::xml_attribute_struct*>(nullptr));
        
        bool null_node_empty = null_node.empty();
        bool null_attr_empty = null_attr.empty();
        
        // Test xml_object_range with empty iterators
        pugi::xml_node_iterator empty_begin;
        pugi::xml_node_iterator empty_end;
        pugi::xml_object_range<pugi::xml_node_iterator> empty_range(empty_begin, empty_end);
        bool empty_range_result = empty_range.empty();
    } else {
        // Even if parsing fails, test with empty/default objects
        pugi::xml_node empty_node;
        pugi::xml_text empty_text;
        pugi::xml_attribute empty_attr;
        
        bool empty_node_result = empty_node.empty();
        bool empty_text_result = empty_text.empty();
        bool empty_attr_result = empty_attr.empty();
        
        // Test remove functions on empty node
        bool remove_attrs_result = empty_node.remove_attributes();
        bool remove_children_result = empty_node.remove_children();
    }
    
    // Clean up dummy file
    remove("./dummy_file");
    
    return 0;
}