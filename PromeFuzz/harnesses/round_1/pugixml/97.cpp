// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5735:34 in pugixml.hpp
// pugi::xml_node::traverse at pugixml.cpp:6789:30 in pugixml.hpp
// pugi::xml_node::traverse at pugixml.cpp:6789:30 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5735:34 in pugixml.hpp
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

static class TestWalker : public pugi::xml_tree_walker {
public:
    virtual bool begin(pugi::xml_node& node) override {
        // Store the root node for later comparison
        m_beginNode = node;
        return true; // Continue traversal
    }
    
    virtual bool for_each(pugi::xml_node& node) override {
        // Required pure virtual function - minimal implementation
        return true;
    }
    
    virtual bool end(pugi::xml_node& node) override {
        // Verify end node matches begin node
        m_endNode = node;
        return true;
    }
    
    pugi::xml_node m_beginNode;
    pugi::xml_node m_endNode;
};

extern "C" int LLVMFuzzerTestOneInput_97(const uint8_t *Data, size_t Size) {
    // Parse XML from fuzzer input
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer(Data, Size, pugi::parse_default | pugi::parse_fragment);
    
    // Test on valid document
    if (result) {
        // Get document root
        pugi::xml_node root = doc.root();
        
        // Test xml_node::root()
        pugi::xml_node root_result = root.root();
        
        // Test xml_node::parent() on various nodes
        pugi::xml_node parent_result = root.parent();
        
        // Test on child nodes if they exist
        for (pugi::xml_node child = root.first_child(); child; child = child.next_sibling()) {
            pugi::xml_node child_root = child.root();
            pugi::xml_node child_parent = child.parent();
            
            // Test xml_node::next_sibling() with and without name filter
            std::string_view name_filter = "";
            if (Size > 0) {
                // Use first byte as simple name filter (mod 26 for letters)
                char c = 'a' + (Data[0] % 26);
                name_filter = std::string_view(&c, 1);
            }
            pugi::xml_node next_sibling = child.next_sibling();
            pugi::xml_node next_sibling_named = child.next_sibling(name_filter.data());
            
            // Test traversal with custom walker
            TestWalker walker;
            child.traverse(walker);
            
            // Test walker begin/end callbacks were invoked
            bool begin_called = walker.m_beginNode == child;
            bool end_called = walker.m_endNode == child;
        }
        
        // Test traversal on root
        TestWalker root_walker;
        root.traverse(root_walker);
    }
    
    // Test on empty/null nodes
    pugi::xml_node null_node;
    pugi::xml_node null_root = null_node.root();
    pugi::xml_node null_parent = null_node.parent();
    pugi::xml_node null_next_sibling = null_node.next_sibling();
    
    // Test with empty string_view
    std::string_view empty_sv;
    pugi::xml_node null_next_sibling_empty = null_node.next_sibling(empty_sv.data());
    
    return 0;
}