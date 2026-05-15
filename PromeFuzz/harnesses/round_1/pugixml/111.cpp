// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5735:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::traverse at pugixml.cpp:6789:30 in pugixml.hpp
// pugi::xml_node::traverse at pugixml.cpp:6789:30 in pugixml.hpp
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
#include <cstdlib>
#include <cstring>
#include <string_view>

static class test_walker : public pugi::xml_tree_walker {
public:
    bool begin(pugi::xml_node& node) override {
        // Record that begin was called
        begin_called = true;
        // Return true to continue traversal, but sometimes return false based on fuzz data
        return should_continue;
    }
    
    bool for_each(pugi::xml_node& node) override {
        // Required pure virtual implementation - do minimal work
        return true;
    }
    
    bool end(pugi::xml_node& node) override {
        // Record that end was called
        end_called = true;
        return true;
    }
    
    bool begin_called = false;
    bool end_called = false;
    bool should_continue = true;
} walker_instance;

extern "C" int LLVMFuzzerTestOneInput_111(const uint8_t *Data, size_t Size) {
    // Parse XML from fuzzer input
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer(Data, Size, pugi::parse_default | pugi::parse_fragment);
    
    // Get root node
    pugi::xml_node root = doc.root();
    
    // Test root() function on various nodes
    if (root) {
        // root() should return the same node for document root
        pugi::xml_node root_of_root = root.root();
        
        // Test parent() on root node (should be empty)
        pugi::xml_node parent_of_root = root.parent();
        
        // Test next_sibling() on root node
        pugi::xml_node next_sibling = root.next_sibling();
        
        // Test on first child if exists
        pugi::xml_node first_child = root.first_child();
        if (first_child) {
            pugi::xml_node child_root = first_child.root();
            pugi::xml_node child_parent = first_child.parent();
            pugi::xml_node child_next_sibling = first_child.next_sibling();
            
            // Test next_sibling with name filter if we have enough data
            if (Size > 0) {
                std::string_view name_filter(reinterpret_cast<const char*>(Data), Size % 100);
                pugi::xml_node named_sibling = first_child.next_sibling(name_filter.data());
            }
        }
        
        // Test traversal if we have a non-empty document
        if (root.first_child()) {
            // Reset walker state
            walker_instance.begin_called = false;
            walker_instance.end_called = false;
            walker_instance.should_continue = (Size % 2) == 0; // Vary based on input
            
            // Test traverse() function
            bool traversal_result = root.traverse(walker_instance);
            
            // Also test on a child node
            if (first_child) {
                test_walker child_walker;
                child_walker.should_continue = (Size % 3) == 0;
                first_child.traverse(child_walker);
            }
        }
    }
    
    // Test on empty node
    pugi::xml_node empty_node;
    pugi::xml_node empty_root = empty_node.root();
    pugi::xml_node empty_parent = empty_node.parent();
    pugi::xml_node empty_next_sibling = empty_node.next_sibling();
    
    // Test with a constructed node from nullptr
    pugi::xml_node null_node(nullptr);
    pugi::xml_node null_root = null_node.root();
    pugi::xml_node null_parent = null_node.parent();
    pugi::xml_node null_next_sibling = null_node.next_sibling();
    
    // Test walker with empty node
    if (Size % 5 == 0) {
        test_walker empty_walker;
        empty_walker.should_continue = true;
        bool empty_begin = empty_walker.begin(empty_node);
        bool empty_end = empty_walker.end(empty_node);
    }
    
    // Test with a dummy file if parsing failed but we want to test other paths
    if (!result && Size > 0) {
        // Write data to dummy file and try to parse
        FILE* f = fopen("./dummy_file", "wb");
        if (f) {
            fwrite(Data, 1, Size, f);
            fclose(f);
            
            pugi::xml_document doc2;
            doc2.load_file("./dummy_file");
            
            // Remove dummy file
            remove("./dummy_file");
        }
    }
    
    return 0;
}