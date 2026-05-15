// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::append_copy at pugixml.cpp:6397:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::append_copy at pugixml.cpp:6397:34 in pugixml.hpp
// pugi::xml_node::traverse at pugixml.cpp:6789:30 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::traverse at pugixml.cpp:6789:30 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::append_copy at pugixml.cpp:6397:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::traverse at pugixml.cpp:6789:30 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
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
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <fstream>

static void writeDummyFile(const uint8_t* data, size_t size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file) {
        file.write(reinterpret_cast<const char*>(data), size);
    }
}

static class test_walker : public pugi::xml_tree_walker {
public:
    virtual bool begin(pugi::xml_node& node) override {
        // Test root() and parent() on the starting node
        pugi::xml_node r = node.root();
        pugi::xml_node p = node.parent();
        // Also test parent() on the root
        if (r) {
            pugi::xml_node root_parent = r.parent();
        }
        return true;
    }
    
    virtual bool for_each(pugi::xml_node& node) override {
        // Test root() and parent() during traversal
        pugi::xml_node r = node.root();
        pugi::xml_node p = node.parent();
        return true;
    }
    
    virtual bool end(pugi::xml_node& node) override {
        // Test root() and parent() at the end
        pugi::xml_node r = node.root();
        pugi::xml_node p = node.parent();
        return true;
    }
} walker_instance;

extern "C" int LLVMFuzzerTestOneInput_103(const uint8_t* data, size_t size) {
    // Write data to dummy file for potential file-based parsing
    writeDummyFile(data, size);
    
    // Test 1: Parse XML from buffer
    pugi::xml_document doc1;
    pugi::xml_parse_result result1 = doc1.load_buffer(data, size);
    
    if (result1) {
        // Test traverse() with custom walker
        pugi::xml_node root1 = doc1.root();
        if (root1) {
            root1.traverse(walker_instance);
            
            // Test root() function
            pugi::xml_node doc_root = root1.root();
            
            // Test parent() function on various nodes
            for (pugi::xml_node child = root1.first_child(); child; child = child.next_sibling()) {
                pugi::xml_node parent = child.parent();
                
                // Test append_copy() - try to copy a child node
                if (child) {
                    pugi::xml_node copied = root1.append_copy(child);
                    
                    // Test parent() on the copied node
                    if (copied) {
                        pugi::xml_node copied_parent = copied.parent();
                    }
                }
            }
        }
    }
    
    // Test 2: Parse XML from file
    pugi::xml_document doc2;
    pugi::xml_parse_result result2 = doc2.load_file("./dummy_file");
    
    if (result2) {
        pugi::xml_node root2 = doc2.root();
        if (root2) {
            // Create another walker instance
            test_walker walker2;
            root2.traverse(walker2);
            
            // Test append_copy() between different documents
            if (result1 && doc1.root()) {
                pugi::xml_node source_node = doc1.root().first_child();
                if (source_node) {
                    pugi::xml_node copied = root2.append_copy(source_node);
                }
            }
            
            // Test parent() on deeply nested structure
            pugi::xml_node current = root2;
            while (current.first_child()) {
                current = current.first_child();
                pugi::xml_node parent = current.parent();
            }
        }
    }
    
    // Test 3: Create minimal document and test functions
    pugi::xml_document doc3;
    pugi::xml_node node1 = doc3.append_child("node1");
    pugi::xml_node node2 = node1.append_child("node2");
    pugi::xml_node node3 = node2.append_child("node3");
    
    // Test root() on deeply nested node
    pugi::xml_node root3 = node3.root();
    
    // Test parent() chain
    pugi::xml_node parent3 = node3.parent();
    if (parent3) {
        pugi::xml_node parent2 = parent3.parent();
    }
    
    // Test append_copy() within same document
    pugi::xml_node copied_node = node1.append_copy(node3);
    
    // Test traverse() on empty and single-node documents
    test_walker walker3;
    doc3.traverse(walker3);
    
    // Test on empty node
    pugi::xml_node empty_node;
    pugi::xml_node empty_root = empty_node.root();
    pugi::xml_node empty_parent = empty_node.parent();
    
    return 0;
}