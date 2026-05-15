// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::traverse at pugixml.cpp:6789:30 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
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
#include <cstdlib>
#include <string>

// Custom walker to test traversal callbacks
class FuzzWalker : public pugi::xml_tree_walker {
public:
    bool begin(pugi::xml_node& node) override {
        // Test parent() on the root node
        pugi::xml_node parent = node.parent();
        (void)parent;
        
        // Test root() on the root node
        pugi::xml_node root = node.root();
        (void)root;
        
        // Return true to proceed with traversal
        return true;
    }
    
    bool for_each(pugi::xml_node& node) override {
        // Test parent() on each node
        pugi::xml_node parent = node.parent();
        (void)parent;
        
        // Test next_sibling() on each node
        pugi::xml_node next = node.next_sibling();
        (void)next;
        
        // Test root() on each node
        pugi::xml_node root = node.root();
        (void)root;
        
        return true; // continue traversal
    }
    
    bool end(pugi::xml_node& node) override {
        // Test parent() one last time
        pugi::xml_node parent = node.parent();
        (void)parent;
        
        // Test root() one last time
        pugi::xml_node root = node.root();
        (void)root;
        
        return true;
    }
};

static void test_xml_node_functions(pugi::xml_node node) {
    // Test parent()
    pugi::xml_node parent = node.parent();
    (void)parent;
    
    // Test next_sibling()
    pugi::xml_node next = node.next_sibling();
    (void)next;
    
    // Test root()
    pugi::xml_node root = node.root();
    (void)root;
    
    // Test traverse() with custom walker
    FuzzWalker walker;
    node.traverse(walker);
}

extern "C" int LLVMFuzzerTestOneInput_100(const uint8_t *Data, size_t Size) {
    // Create XML document
    pugi::xml_document doc;
    
    // Try to parse the input as XML
    pugi::xml_parse_result result = doc.load_buffer(Data, Size);
    
    if (result) {
        // Get the root node
        pugi::xml_node root = doc.root();
        
        // Test functions on root node
        test_xml_node_functions(root);
        
        // Test functions on all children
        for (pugi::xml_node child = root.first_child(); child; child = child.next_sibling()) {
            test_xml_node_functions(child);
        }
    } else {
        // If parsing fails, create a minimal document and test on it
        pugi::xml_document fallback_doc;
        pugi::xml_node root = fallback_doc.append_child("root");
        root.append_child("child1");
        root.append_child("child2");
        
        // Test functions on the fallback document
        test_xml_node_functions(root);
        for (pugi::xml_node child = root.first_child(); child; child = child.next_sibling()) {
            test_xml_node_functions(child);
        }
    }
    
    return 0;
}