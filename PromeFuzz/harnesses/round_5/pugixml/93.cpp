// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::traverse at pugixml.cpp:6789:30 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::traverse at pugixml.cpp:6789:30 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::traverse at pugixml.cpp:6789:30 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::traverse at pugixml.cpp:6789:30 in pugixml.hpp
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
#include <string_view>

// Custom walker class to test begin() and end() callbacks
class test_walker : public pugi::xml_tree_walker {
public:
    test_walker() = default;
    
    virtual bool begin(pugi::xml_node& node) override {
        // Test begin callback - always return true to proceed with traversal
        (void)node; // Mark as used to avoid warnings
        return true;
    }
    
    virtual bool for_each(pugi::xml_node& node) override {
        // Required pure virtual implementation - just continue traversal
        (void)node; // Mark as used to avoid warnings
        return true;
    }
    
    virtual bool end(pugi::xml_node& node) override {
        // Test end callback - always return true
        (void)node; // Mark as used to avoid warnings
        return true;
    }
};

// Static helper function to parse XML from fuzzer input
static pugi::xml_document parse_xml(const uint8_t* data, size_t size) {
    pugi::xml_document doc;
    doc.load_buffer(data, size, pugi::parse_default | pugi::parse_fragment);
    return doc;
}

extern "C" int LLVMFuzzerTestOneInput_93(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    // Test 1: Create empty nodes and test basic functions
    {
        pugi::xml_node empty_node;
        pugi::xml_node parent_result = empty_node.parent();
        pugi::xml_node next_sibling_result = empty_node.next_sibling();
        pugi::xml_node root_result = empty_node.root();
        
        // Create a walker and test callbacks on empty node
        test_walker walker;
        pugi::xml_node test_node;
        bool begin_result = walker.begin(test_node);
        bool end_result = walker.end(test_node);
        (void)begin_result;
        (void)end_result;
        
        // Test traverse on empty node (should return false)
        bool traverse_result = empty_node.traverse(walker);
        (void)traverse_result;
    }
    
    // Test 2: Parse XML from fuzzer input and test functions
    {
        pugi::xml_document doc = parse_xml(Data, Size);
        
        // Get root node
        pugi::xml_node root = doc.root();
        
        // Test root() function
        pugi::xml_node root_result = root.root();
        
        // Test parent() function on root (should be empty for document root)
        pugi::xml_node parent_result = root.parent();
        
        // Test next_sibling() function
        pugi::xml_node next_sibling_result = root.next_sibling();
        
        // Create walker and test traversal
        test_walker walker;
        bool traverse_result = root.traverse(walker);
        (void)traverse_result;
        
        // Test callbacks directly
        bool begin_result = walker.begin(root);
        bool end_result = walker.end(root);
        (void)begin_result;
        (void)end_result;
        
        // Test on child nodes if they exist
        for (pugi::xml_node child = root.first_child(); child; child = child.next_sibling()) {
            // Test parent() on child
            pugi::xml_node child_parent = child.parent();
            
            // Test next_sibling() on child
            pugi::xml_node child_next_sibling = child.next_sibling();
            
            // Test root() on child
            pugi::xml_node child_root = child.root();
            
            // Test traversal on child
            bool child_traverse = child.traverse(walker);
            (void)child_traverse;
            
            // Test a few more siblings
            if (child_next_sibling) {
                pugi::xml_node sibling_next = child_next_sibling.next_sibling();
                (void)sibling_next;
            }
        }
    }
    
    // Test 3: Test with a named sibling search using string_view
    {
        pugi::xml_document doc = parse_xml(Data, Size);
        pugi::xml_node root = doc.root();
        
        // Create a string_view from the first few bytes of input for name matching
        size_t name_length = Size > 100 ? 100 : Size;
        std::string_view name(reinterpret_cast<const char*>(Data), name_length);
        
        // Test next_sibling with name (if available in the API)
        // Note: The target function signature shows next_sibling() without parameters,
        // but the hint mentions string_view. We'll test both if they exist.
        // For now, we'll use the parameterless version as shown in the signature.
        for (pugi::xml_node child = root.first_child(); child; child = child.next_sibling()) {
            pugi::xml_node next = child.next_sibling();
            (void)next;
        }
    }
    
    // Test 4: Create a more complex scenario with nested nodes
    {
        pugi::xml_document doc;
        pugi::xml_node root = doc.append_child("root");
        pugi::xml_node child1 = root.append_child("child1");
        pugi::xml_node child2 = root.append_child("child2");
        pugi::xml_node grandchild = child1.append_child("grandchild");
        
        // Test parent() chain
        pugi::xml_node parent1 = grandchild.parent();  // Should be child1
        pugi::xml_node parent2 = parent1.parent();     // Should be root
        pugi::xml_node parent3 = parent2.parent();     // Should be empty
        
        // Test next_sibling() chain
        pugi::xml_node sibling1 = child1.next_sibling();  // Should be child2
        pugi::xml_node sibling2 = child2.next_sibling();  // Should be empty
        
        // Test root() from different levels
        pugi::xml_node root1 = grandchild.root();  // Should be document root
        pugi::xml_node root2 = child2.root();      // Should be document root
        
        // Test traversal
        test_walker walker;
        bool traverse_result = root.traverse(walker);
        (void)traverse_result;
    }
    
    return 0;
}