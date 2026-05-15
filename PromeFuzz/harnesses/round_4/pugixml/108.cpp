// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::traverse at pugixml.cpp:6789:30 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::traverse at pugixml.cpp:6789:30 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
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

static class test_walker : public pugi::xml_tree_walker {
public:
    virtual bool begin(pugi::xml_node& node) override {
        // Return true to continue traversal, false to cancel
        return true;
    }
    
    virtual bool for_each(pugi::xml_node& node) override {
        // Return true to continue traversal, false to stop
        return true;
    }
    
    virtual bool end(pugi::xml_node& node) override {
        // Return true to finish traversal normally
        return true;
    }
} g_walker;

extern "C" int LLVMFuzzerTestOneInput_108(const uint8_t *Data, size_t Size) {
    // Parse XML from fuzzer input
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer(Data, Size);
    
    if (result.status == pugi::status_ok) {
        // Get root node
        pugi::xml_node root = doc.root();
        
        // Test xml_node::parent
        if (root) {
            pugi::xml_node parent = root.parent();
        }
        
        // Test xml_node::next_sibling
        if (root) {
            pugi::xml_node next_sib = root.next_sibling();
        }
        
        // Test xml_node::root
        if (root) {
            pugi::xml_node doc_root = root.root();
        }
        
        // Test traversal on root node
        if (root) {
            bool traverse_result = root.traverse(g_walker);
        }
        
        // Test on child nodes
        for (pugi::xml_node child = root.first_child(); child; child = child.next_sibling()) {
            // Test parent on child
            pugi::xml_node child_parent = child.parent();
            
            // Test next_sibling on child
            pugi::xml_node child_next_sib = child.next_sibling();
            
            // Test root on child
            pugi::xml_node child_root = child.root();
            
            // Test traversal on child
            bool child_traverse = child.traverse(g_walker);
        }
        
        // Test on null/empty nodes
        pugi::xml_node empty_node;
        pugi::xml_node empty_parent = empty_node.parent();
        pugi::xml_node empty_next_sib = empty_node.next_sibling();
        pugi::xml_node empty_root = empty_node.root();
        
        // Test walker begin/end directly
        g_walker.begin(root);
        g_walker.end(root);
        
        // Test with different node types
        pugi::xml_node node_from_ptr(nullptr);
        pugi::xml_node node_parent = node_from_ptr.parent();
        pugi::xml_node node_next_sib = node_from_ptr.next_sibling();
        pugi::xml_node node_root = node_from_ptr.root();
    }
    
    // Test with empty document
    pugi::xml_document empty_doc;
    pugi::xml_node empty_doc_root = empty_doc.root();
    if (empty_doc_root) {
        pugi::xml_node empty_doc_parent = empty_doc_root.parent();
        pugi::xml_node empty_doc_next_sib = empty_doc_root.next_sibling();
        pugi::xml_node empty_doc_root2 = empty_doc_root.root();
    }
    
    return 0;
}