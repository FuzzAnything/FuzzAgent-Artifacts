// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::append_copy at pugixml.cpp:6397:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::traverse at pugixml.cpp:6789:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::append_copy at pugixml.cpp:6397:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::traverse at pugixml.cpp:6789:30 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::traverse at pugixml.cpp:6789:30 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::traverse at pugixml.cpp:6789:30 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::append_copy at pugixml.cpp:6397:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
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
#include "src/pugixml.hpp"
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <fstream>

static void write_dummy_file(const uint8_t* data, size_t size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file) {
        file.write(reinterpret_cast<const char*>(data), size);
    }
}

class CustomWalker : public pugi::xml_tree_walker {
public:
    virtual bool begin(pugi::xml_node& node) override {
        // Test begin callback
        if (node) {
            node.parent();  // Test parent during begin
            node.root();    // Test root during begin
        }
        return true;  // Continue traversal
    }
    
    virtual bool for_each(pugi::xml_node& node) override {
        // Test various operations during traversal
        if (node) {
            node.parent();
            node.root();
        }
        return true;  // Continue traversal
    }
    
    virtual bool end(pugi::xml_node& node) override {
        // Test end callback
        if (node) {
            node.parent();  // Test parent during end
            node.root();    // Test root during end
        }
        return true;
    }
};

extern "C" int LLVMFuzzerTestOneInput_115(const uint8_t *Data, size_t Size) {
    // Write data to dummy file for file-based parsing
    write_dummy_file(Data, Size);
    
    // Test 1: Parse XML from memory
    pugi::xml_document doc1;
    pugi::xml_parse_result result1 = doc1.load_buffer(Data, Size);
    
    if (result1) {
        // Get root node
        pugi::xml_node root1 = doc1.root();
        
        // Test parent() and root() on various nodes
        for (pugi::xml_node node = doc1.first_child(); node; node = node.next_sibling()) {
            pugi::xml_node parent = node.parent();
            pugi::xml_node root = node.root();
            
            // Test traverse() with custom walker
            CustomWalker walker1;
            node.traverse(walker1);
            
            // Test append_copy() within the same document
            for (pugi::xml_node child = node.first_child(); child; child = child.next_sibling()) {
                if (child.type() != pugi::node_document) {  // Document nodes cannot be copied
                    pugi::xml_node copied = node.append_copy(child);
                    // Test operations on the copied node
                    if (copied) {
                        copied.parent();
                        copied.root();
                    }
                }
            }
        }
    }
    
    // Test 2: Parse XML from file
    pugi::xml_document doc2;
    pugi::xml_parse_result result2 = doc2.load_file("./dummy_file");
    
    if (result2) {
        // Test parent() and root() on all nodes
        pugi::xml_node root2 = doc2.root();
        
        // Create a custom walker for traversal
        CustomWalker walker2;
        root2.traverse(walker2);
        
        // Test append_copy() between different documents
        pugi::xml_document doc3;
        pugi::xml_node decl = doc3.append_child(pugi::node_declaration);
        decl.append_attribute("version") = "1.0";
        decl.append_attribute("encoding") = "UTF-8";
        
        pugi::xml_node new_root = doc3.append_child("fuzz_root");
        
        // Copy nodes from doc2 to doc3
        for (pugi::xml_node node = doc2.first_child(); node; node = node.next_sibling()) {
            if (node.type() != pugi::node_document) {
                pugi::xml_node copied = new_root.append_copy(node);
                if (copied) {
                    copied.parent();
                    copied.root();
                }
            }
        }
        
        // Test traverse() on the new document
        CustomWalker walker3;
        doc3.traverse(walker3);
    }
    
    // Test 3: Create minimal document and test functions
    pugi::xml_document doc4;
    pugi::xml_node root4 = doc4.append_child("test_root");
    pugi::xml_node child1 = root4.append_child("child1");
    child1.append_attribute("attr1") = "value1";
    pugi::xml_node child2 = root4.append_child("child2");
    
    // Test parent() and root() on created nodes
    pugi::xml_node parent1 = child1.parent();
    pugi::xml_node root_of_child1 = child1.root();
    
    // Test append_copy() with created nodes
    pugi::xml_node copied_child = root4.append_copy(child1);
    if (copied_child) {
        copied_child.parent();
        copied_child.root();
    }
    
    // Test traverse() on minimal document
    CustomWalker walker4;
    root4.traverse(walker4);
    
    // Test 4: Test on empty/null nodes
    pugi::xml_node empty_node;
    pugi::xml_node empty_parent = empty_node.parent();
    pugi::xml_node empty_root = empty_node.root();
    
    // Test traverse() on empty node (should do nothing)
    CustomWalker walker5;
    empty_node.traverse(walker5);
    
    // Clean up dummy file
    std::remove("./dummy_file");
    
    return 0;
}