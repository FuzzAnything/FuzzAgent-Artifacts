// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::value at pugixml.cpp:5700:39 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::value at pugixml.cpp:5700:39 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::traverse at pugixml.cpp:6789:30 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::value at pugixml.cpp:5700:39 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::traverse at pugixml.cpp:6789:30 in pugixml.hpp
// pugi::xml_node::value at pugixml.cpp:5700:39 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
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

// Custom walker to test traversal
static class TestWalker : public pugi::xml_tree_walker {
public:
    bool begin(pugi::xml_node& node) override {
        // Test the begin callback
        (void)node;
        return true; // Continue traversal
    }
    
    bool for_each(pugi::xml_node& node) override {
        // Test node functions during traversal
        (void)node.value();
        (void)node.parent();
        (void)node.name();
        return true; // Continue traversal
    }
    
    bool end(pugi::xml_node& node) override {
        // Test the end callback
        (void)node;
        return true;
    }
} test_walker;

extern "C" int LLVMFuzzerTestOneInput_121(const uint8_t *Data, size_t Size) {
    // Create document and parse input
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer(Data, Size);
    
    // Test API functions on various nodes
    pugi::xml_node root = doc.root();
    
    // Test root node
    (void)root.value();
    (void)root.parent();
    (void)root.name();
    
    // Test traversal if parsing succeeded
    if (result) {
        root.traverse(test_walker);
    }
    
    // Test on child nodes
    for (pugi::xml_node child = root.first_child(); child; child = child.next_sibling()) {
        (void)child.value();
        (void)child.parent();
        (void)child.name();
        
        // Test traversal on child subtree
        child.traverse(test_walker);
    }
    
    // Test on empty node
    pugi::xml_node empty_node;
    (void)empty_node.value();
    (void)empty_node.parent();
    (void)empty_node.name();
    
    // Test walker directly on empty node
    test_walker.begin(empty_node);
    test_walker.end(empty_node);
    
    return 0;
}