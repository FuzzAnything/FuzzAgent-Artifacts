// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::append_copy at pugixml.cpp:6397:34 in pugixml.hpp
// pugi::xml_node::traverse at pugixml.cpp:6789:30 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::append_copy at pugixml.cpp:6397:34 in pugixml.hpp
// pugi::xml_node::traverse at pugixml.cpp:6789:30 in pugixml.hpp
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

static class FuzzWalker : public pugi::xml_tree_walker {
public:
    bool begin(pugi::xml_node& node) override {
        // Track that begin was called
        begin_called = true;
        return true;
    }
    
    bool for_each(pugi::xml_node& node) override {
        // Track that for_each was called
        for_each_called = true;
        return true;
    }
    
    bool end(pugi::xml_node& node) override {
        // Track that end was called
        end_called = true;
        return true;
    }
    
    bool begin_called = false;
    bool for_each_called = false;
    bool end_called = false;
};

extern "C" int LLVMFuzzerTestOneInput_112(const uint8_t *Data, size_t Size) {
    // Create document and parse input
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer(Data, Size, pugi::parse_default);
    
    // Get root node
    pugi::xml_node root = doc.root();
    
    // Test parent() function
    pugi::xml_node parent = root.parent();
    
    // Test root() function on various nodes
    pugi::xml_node root_from_root = root.root();
    
    // Test on child nodes if they exist
    for (pugi::xml_node child = root.first_child(); child; child = child.next_sibling()) {
        pugi::xml_node child_parent = child.parent();
        pugi::xml_node child_root = child.root();
        
        // Test append_copy() function
        if (!child.empty()) {
            pugi::xml_node copy = root.append_copy(child);
        }
    }
    
    // Test traverse() function with custom walker
    FuzzWalker walker;
    root.traverse(walker);
    
    // Test begin() and end() directly on walker
    FuzzWalker direct_walker;
    direct_walker.begin(root);
    direct_walker.end(root);
    
    // Test on empty nodes
    pugi::xml_node empty_node;
    pugi::xml_node empty_parent = empty_node.parent();
    pugi::xml_node empty_root = empty_node.root();
    
    // Test append_copy with empty node
    pugi::xml_node empty_copy = root.append_copy(empty_node);
    
    // Test traverse on empty node
    FuzzWalker empty_walker;
    empty_node.traverse(empty_walker);
    
    return 0;
}