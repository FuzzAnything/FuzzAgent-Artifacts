// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::next_sibling at pugixml.cpp:5797:34 in pugixml.hpp
// pugi::xml_node::traverse at pugixml.cpp:6789:30 in pugixml.hpp
// pugi::xml_node::children at pugixml.cpp:5638:61 in pugixml.hpp
// pugi::xml_node::traverse at pugixml.cpp:6789:30 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::traverse at pugixml.cpp:6789:30 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7490:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::children at pugixml.cpp:5638:61 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
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

// Custom walker class to test traversal
class TestWalker : public pugi::xml_tree_walker {
public:
    bool begin(pugi::xml_node& node) override {
        // Custom initialization logic
        traversal_started = true;
        node_count = 0;
        return true; // Continue traversal
    }
    
    bool for_each(pugi::xml_node& node) override {
        // Count nodes during traversal
        node_count++;
        return true; // Continue traversal
    }
    
    bool end(pugi::xml_node& node) override {
        // Custom cleanup logic
        traversal_completed = true;
        return true;
    }
    
    bool traversal_started = false;
    bool traversal_completed = false;
    int node_count = 0;
};

// Static helper function to create XML document from fuzzer input
static pugi::xml_document create_document(const uint8_t* data, size_t size) {
    pugi::xml_document doc;
    
    // Try parsing as XML string first
    pugi::xml_parse_result result = doc.load_buffer(data, size, 
        pugi::parse_default | pugi::parse_fragment);
    
    // If parsing fails, create a minimal valid XML structure
    if (!result) {
        doc.reset();
        pugi::xml_node root = doc.append_child("root");
        for (size_t i = 0; i < size && i < 10; ++i) {
            char node_name[2] = {static_cast<char>('a' + (data[i] % 26)), 0};
            pugi::xml_node child = root.append_child(node_name);
            child.append_attribute("attr") = "value";
        }
    }
    
    return doc;
}

extern "C" int LLVMFuzzerTestOneInput_86(const uint8_t* Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Create XML document from fuzzer input
    pugi::xml_document doc = create_document(Data, Size);
    
    // Get root node
    pugi::xml_node root = doc.root();
    
    // Test 1: xml_node::root() function
    if (root) {
        pugi::xml_node root_test = root.root();
        // Verify root of root is itself
        (void)root_test;
    }
    
    // Test 2: xml_node::parent() function
    if (root) {
        pugi::xml_node parent = root.parent();
        // Root's parent should be empty
        (void)parent;
        
        // Test parent on child nodes
        for (pugi::xml_node child : root.children()) {
            pugi::xml_node child_parent = child.parent();
            (void)child_parent;
        }
    }
    
    // Test 3: xml_node::next_sibling() function
    if (root) {
        pugi::xml_node first_child = root.first_child();
        if (first_child) {
            // Iterate through siblings
            pugi::xml_node sibling = first_child;
            while (sibling) {
                sibling = sibling.next_sibling();
            }
            
            // Test next_sibling with name filter (using string_view)
            std::string_view name_filter = "test";
            sibling = first_child.next_sibling(name_filter);
            (void)sibling;
        }
    }
    
    // Test 4: xml_node::traverse() and xml_tree_walker functions
    if (root) {
        TestWalker walker;
        
        // Test walker::begin() directly
        bool begin_result = walker.begin(root);
        (void)begin_result;
        
        // Test traversal
        bool traverse_result = root.traverse(walker);
        (void)traverse_result;
        
        // Test walker::end() directly
        bool end_result = walker.end(root);
        (void)end_result;
    }
    
    // Additional tests with different starting nodes
    if (root) {
        // Test traversal from different nodes
        for (pugi::xml_node node : root.children()) {
            TestWalker child_walker;
            node.traverse(child_walker);
            
            // Test root() from child node
            pugi::xml_node node_root = node.root();
            (void)node_root;
            
            // Test parent() from child node
            pugi::xml_node node_parent = node.parent();
            (void)node_parent;
            
            // Test next_sibling() chain
            pugi::xml_node current = node;
            int max_iterations = 10;
            while (current && max_iterations-- > 0) {
                current = current.next_sibling();
            }
        }
    }
    
    // Test with empty/null nodes
    pugi::xml_node empty_node;
    pugi::xml_node empty_root = empty_node.root();
    pugi::xml_node empty_parent = empty_node.parent();
    pugi::xml_node empty_sibling = empty_node.next_sibling();
    (void)empty_root;
    (void)empty_parent;
    (void)empty_sibling;
    
    // Test walker with empty node
    TestWalker empty_walker;
    bool empty_begin = empty_walker.begin(empty_node);
    bool empty_end = empty_walker.end(empty_node);
    (void)empty_begin;
    (void)empty_end;
    
    // Test traversal on empty node
    bool empty_traverse = empty_node.traverse(empty_walker);
    (void)empty_traverse;
    
    return 0;
}