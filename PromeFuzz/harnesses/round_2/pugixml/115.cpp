// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::traverse at pugixml.cpp:6789:30 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::traverse at pugixml.cpp:6789:30 in pugixml.hpp
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
#include <cstdint>
#include <cstring>

static class test_walker : public pugi::xml_tree_walker {
public:
    bool begin(pugi::xml_node& node) override {
        // Test root() and parent() during begin
        (void)node.root();
        (void)node.parent();
        return true;
    }
    
    bool for_each(pugi::xml_node& node) override {
        // Test next_sibling() during traversal
        (void)node.next_sibling();
        return true;
    }
    
    bool end(pugi::xml_node& node) override {
        // Test root() and parent() during end
        (void)node.root();
        (void)node.parent();
        return true;
    }
} g_walker;

extern "C" int LLVMFuzzerTestOneInput_115(const uint8_t *Data, size_t Size) {
    // Write data to dummy file
    FILE* dummy_file = fopen("./dummy_file", "wb");
    if (!dummy_file) return 0;
    fwrite(Data, 1, Size, dummy_file);
    fclose(dummy_file);
    
    pugi::xml_document doc;
    
    // Try to load from the dummy file
    pugi::xml_parse_result result = doc.load_file("./dummy_file");
    
    // Test APIs regardless of parse success
    pugi::xml_node root = doc.root();
    
    // Test root() function
    (void)root.root();
    
    // Test parent() function on various nodes
    if (root) {
        (void)root.parent();
        
        // Test traverse() function
        root.traverse(g_walker);
        
        // Test next_sibling() on children
        for (pugi::xml_node child = root.first_child(); child; child = child.next_sibling()) {
            (void)child.next_sibling();
            (void)child.parent();
            (void)child.root();
        }
    }
    
    // Also test on empty nodes
    pugi::xml_node empty_node;
    (void)empty_node.next_sibling();
    (void)empty_node.parent();
    (void)empty_node.root();
    
    // Create another document with minimal structure for more testing
    pugi::xml_document doc2;
    if (Size > 0) {
        // Use first byte to decide structure
        uint8_t mode = Data[0] % 3;
        
        if (mode == 0) {
            doc2.append_child("root").append_child("child1");
        } else if (mode == 1) {
            pugi::xml_node root2 = doc2.append_child("root");
            root2.append_child("child1");
            root2.append_child("child2");
        } else {
            pugi::xml_node root2 = doc2.append_child("root");
            root2.append_attribute("attr") = "value";
            root2.append_child("child");
        }
        
        // Test traverse on the new document
        doc2.traverse(g_walker);
        
        // Test next_sibling() more extensively
        pugi::xml_node root2 = doc2.root();
        if (root2) {
            pugi::xml_node node = root2.first_child();
            while (node) {
                (void)node.next_sibling();
                node = node.next_sibling();
            }
        }
    }
    
    return 0;
}