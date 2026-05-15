// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::traverse at pugixml.cpp:6789:30 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
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
#include <string_view>

static class test_walker : public pugi::xml_tree_walker {
public:
    bool begin(pugi::xml_node& node) override {
        // Test root() and parent() on the traversal start node
        (void)node.root();
        (void)node.parent();
        return true; // Continue traversal
    }
    
    bool for_each(pugi::xml_node& node) override {
        // Test next_sibling() on each node during traversal
        (void)node.next_sibling();
        // Test root() and parent() on each traversed node
        (void)node.root();
        (void)node.parent();
        return true; // Continue traversal
    }
    
    bool end(pugi::xml_node& node) override {
        // Test root() and parent() on the traversal end node
        (void)node.root();
        (void)node.parent();
        return true;
    }
} g_walker;

extern "C" int LLVMFuzzerTestOneInput_93(const uint8_t *Data, size_t Size) {
    // Parse XML from fuzzer input
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer(Data, Size);
    
    // Only proceed if parsing was successful
    if (result.status == pugi::status_ok) {
        // Get the root node
        pugi::xml_node root = doc.root();
        
        if (root) {
            // Test root() function
            (void)root.root();
            
            // Test parent() function
            (void)root.parent();
            
            // Test next_sibling() function
            (void)root.next_sibling();
            
            // Test traverse() function with our walker
            (void)root.traverse(g_walker);
            
            // Also test on child nodes
            for (pugi::xml_node child = root.first_child(); child; child = child.next_sibling()) {
                (void)child.root();
                (void)child.parent();
                (void)child.next_sibling();
            }
        }
    }
    
    // Also test with empty document
    pugi::xml_document empty_doc;
    pugi::xml_node empty_node = empty_doc.root();
    (void)empty_node.root();
    (void)empty_node.parent();
    (void)empty_node.next_sibling();
    
    return 0;
}