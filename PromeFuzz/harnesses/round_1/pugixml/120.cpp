// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::value at pugixml.cpp:5700:39 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::value at pugixml.cpp:5700:39 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
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
        // Test the node passed to begin
        (void)node.value();
        (void)node.parent();
        (void)node.name();
        return true; // Continue traversal
    }
    
    bool for_each(pugi::xml_node& node) override {
        // Test each node during traversal
        (void)node.value();
        (void)node.parent();
        (void)node.name();
        return true; // Continue traversal
    }
    
    bool end(pugi::xml_node& node) override {
        // Test the node passed to end
        (void)node.value();
        (void)node.parent();
        (void)node.name();
        return true;
    }
} testWalker;

extern "C" int LLVMFuzzerTestOneInput_120(const uint8_t *Data, size_t Size) {
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
        root.traverse(testWalker);
    }
    
    // Also test on child nodes
    for (pugi::xml_node child = root.first_child(); child; child = child.next_sibling()) {
        (void)child.value();
        (void)child.parent();
        (void)child.name();
        
        // Test traversal on child subtree
        child.traverse(testWalker);
    }
    
    // Test on empty node
    pugi::xml_node empty;
    (void)empty.value();
    (void)empty.parent();
    (void)empty.name();
    
    // Create a dummy node from null pointer to test edge cases
    pugi::xml_node dummy(static_cast<pugi::xml_node_struct*>(nullptr));
    (void)dummy.value();
    (void)dummy.parent();
    (void)dummy.name();
    
    return 0;
}