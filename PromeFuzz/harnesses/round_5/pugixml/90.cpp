// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::traverse at pugixml.cpp:6789:30 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::traverse at pugixml.cpp:6789:30 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
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
#include <stdint.h>
#include <string>
#include <fstream>

// Custom walker class to test xml_tree_walker functions
class TestWalker : public pugi::xml_tree_walker {
public:
    virtual bool begin(pugi::xml_node& node) override {
        // Test some operations in begin
        if (!node.empty()) {
            node.parent();  // Test parent()
            node.root();    // Test root()
        }
        return true;  // Continue traversal
    }
    
    virtual bool for_each(pugi::xml_node& node) override {
        // Test operations during traversal
        if (!node.empty()) {
            node.parent();  // Test parent()
            node.next_sibling();  // Test next_sibling()
            node.root();  // Test root()
        }
        return true;  // Continue traversal
    }
    
    virtual bool end(pugi::xml_node& node) override {
        // Test operations in end
        if (!node.empty()) {
            node.parent();  // Test parent()
            node.root();    // Test root()
        }
        return true;
    }
};

static void test_node_functions(pugi::xml_node node) {
    if (!node.empty()) {
        // Test parent()
        pugi::xml_node parent = node.parent();
        
        // Test next_sibling()
        pugi::xml_node next_sib = node.next_sibling();
        
        // Test root()
        pugi::xml_node root = node.root();
        
        // Test traverse() with custom walker
        TestWalker walker;
        node.traverse(walker);
    }
}

extern "C" int LLVMFuzzerTestOneInput_90(const uint8_t *Data, size_t Size) {
    // Write data to file for XML parsing
    std::ofstream file("./dummy_file", std::ios::binary);
    if (!file.is_open()) {
        return 0;
    }
    file.write(reinterpret_cast<const char*>(Data), Size);
    file.close();
    
    // Parse XML document
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file("./dummy_file");
    
    if (result) {
        // Get document element
        pugi::xml_node root = doc.document_element();
        
        // Test functions on root node
        test_node_functions(root);
        
        // Test functions on child nodes
        for (pugi::xml_node child = root.first_child(); child; child = child.next_sibling()) {
            test_node_functions(child);
            
            // Test deeper traversal
            for (pugi::xml_node grandchild = child.first_child(); grandchild; grandchild = grandchild.next_sibling()) {
                test_node_functions(grandchild);
            }
        }
        
        // Test empty node
        pugi::xml_node empty_node;
        test_node_functions(empty_node);
        
        // Test custom walker on the entire document
        TestWalker walker;
        doc.traverse(walker);
    }
    
    // Test with empty document
    pugi::xml_document empty_doc;
    pugi::xml_node empty_doc_node = empty_doc.document_element();
    test_node_functions(empty_doc_node);
    
    // Test with manually created nodes
    pugi::xml_document manual_doc;
    pugi::xml_node manual_node = manual_doc.append_child("test");
    manual_node.append_attribute("attr") = "value";
    manual_node.append_child("child");
    
    test_node_functions(manual_node);
    test_node_functions(manual_node.first_child());
    
    return 0;
}