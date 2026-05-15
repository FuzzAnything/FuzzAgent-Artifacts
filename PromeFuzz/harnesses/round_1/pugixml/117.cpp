// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::traverse at pugixml.cpp:6789:30 in pugixml.hpp
// pugi::xml_node::traverse at pugixml.cpp:6789:30 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
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

static void test_node_functions(pugi::xml_node node) {
    // Test xml_node::root
    pugi::xml_node root = node.root();
    
    // Test xml_node::parent
    pugi::xml_node parent = node.parent();
    
    // Test xml_node::next_sibling
    pugi::xml_node next_sibling = node.next_sibling();
}

class TestWalker : public pugi::xml_tree_walker {
public:
    bool begin(pugi::xml_node& node) override {
        // Test node functions in begin callback
        test_node_functions(node);
        return true;
    }
    
    bool for_each(pugi::xml_node& node) override {
        // Test node functions during traversal
        test_node_functions(node);
        return true;
    }
    
    bool end(pugi::xml_node& node) override {
        // Test node functions in end callback
        test_node_functions(node);
        return true;
    }
};

extern "C" int LLVMFuzzerTestOneInput_117(const uint8_t *Data, size_t Size) {
    // Create XML document
    pugi::xml_document doc;
    
    // Try to parse the input data as XML
    pugi::xml_parse_result result = doc.load_buffer(Data, Size);
    
    // Test functions on the document root
    pugi::xml_node root = doc.root();
    test_node_functions(root);
    
    // Test functions on document element
    pugi::xml_node doc_element = doc.document_element();
    test_node_functions(doc_element);
    
    // Test functions on first child
    pugi::xml_node first_child = root.first_child();
    test_node_functions(first_child);
    
    // Test functions on empty node
    pugi::xml_node empty_node;
    test_node_functions(empty_node);
    
    // Test traversal with custom walker
    TestWalker walker;
    root.traverse(walker);
    
    // Also test traversal on empty node
    empty_node.traverse(walker);
    
    // Test parent chain
    pugi::xml_node current = first_child;
    while (current) {
        test_node_functions(current);
        current = current.parent();
    }
    
    // Test sibling chain
    current = first_child;
    while (current) {
        test_node_functions(current);
        current = current.next_sibling();
    }
    
    return 0;
}