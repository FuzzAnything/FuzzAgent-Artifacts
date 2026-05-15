// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::traverse at pugixml.cpp:6789:30 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::traverse at pugixml.cpp:6789:30 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5735:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
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
#include <cstddef>
#include <string_view>

static pugi::xml_document create_document_from_fuzz_data(const uint8_t* data, size_t size) {
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer(data, size, pugi::parse_default | pugi::parse_fragment, pugi::encoding_utf8);
    if (!result) {
        // If parsing fails, create a minimal valid document to test APIs
        doc.load_string("<root><child1/><child2/><child3/></root>");
    }
    return doc;
}

static class FuzzWalker : public pugi::xml_tree_walker {
public:
    FuzzWalker() : depth(0) {}
    
    virtual bool begin(pugi::xml_node& node) override {
        // Target function 4: xml_tree_walker::begin
        (void)node;
        depth = 0;
        return true;
    }
    
    virtual bool for_each(pugi::xml_node& node) override {
        // Required for traversal but not a target function
        (void)node;
        depth++;
        return true;
    }
    
    virtual bool end(pugi::xml_node& node) override {
        // Target function 6: xml_tree_walker::end
        (void)node;
        depth = 0;
        return true;
    }
    
private:
    int depth;
};

extern "C" int LLVMFuzzerTestOneInput_118(const uint8_t* Data, size_t Size) {
    // Create document from fuzzer input
    pugi::xml_document doc = create_document_from_fuzz_data(Data, Size);
    
    // Get root node
    pugi::xml_node root = doc.root();
    
    // Target function 1: xml_node::root
    pugi::xml_node root_from_root = root.root();
    
    // Test on child nodes if they exist
    pugi::xml_node child = root.first_child();
    if (child) {
        // Target function 1: xml_node::root on child
        pugi::xml_node root_from_child = child.root();
        
        // Target function 2: xml_node::parent on child
        pugi::xml_node parent_of_child = child.parent();
        
        // Target function 3: xml_node::next_sibling
        pugi::xml_node next_sibling = child.next_sibling();
        
        // Test next_sibling with string_view parameter (from hint)
        std::string_view sv = "child";
        pugi::xml_node next_sibling_named = child.next_sibling(sv.data());
        
        // Continue iterating through siblings
        while (next_sibling) {
            // Target function 1: xml_node::root on sibling
            pugi::xml_node root_from_sibling = next_sibling.root();
            
            // Target function 2: xml_node::parent on sibling
            pugi::xml_node parent_of_sibling = next_sibling.parent();
            
            // Get next sibling
            next_sibling = next_sibling.next_sibling();
        }
    }
    
    // Target function 2: xml_node::parent on root (should return empty node)
    pugi::xml_node parent_of_root = root.parent();
    
    // Target function 5: xml_node::traverse with custom walker
    FuzzWalker walker;
    root.traverse(walker);
    
    // Test on null/empty node
    pugi::xml_node empty_node;
    pugi::xml_node root_from_empty = empty_node.root();
    pugi::xml_node parent_of_empty = empty_node.parent();
    pugi::xml_node next_sibling_empty = empty_node.next_sibling();
    
    // Test traversal on empty node
    empty_node.traverse(walker);
    
    // Test on document fragment
    if (doc.first_child() != root) {
        pugi::xml_node fragment_node = doc.first_child();
        if (fragment_node) {
            pugi::xml_node root_from_fragment = fragment_node.root();
            pugi::xml_node parent_of_fragment = fragment_node.parent();
            fragment_node.traverse(walker);
        }
    }
    
    return 0;
}