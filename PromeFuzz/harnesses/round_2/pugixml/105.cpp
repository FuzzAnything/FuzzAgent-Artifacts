// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::traverse at pugixml.cpp:6789:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::append_copy at pugixml.cpp:6397:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::append_copy at pugixml.cpp:6397:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
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
#include <fstream>

static class FuzzWalker : public pugi::xml_tree_walker {
public:
    bool begin(pugi::xml_node& node) override {
        // Custom initialization logic
        begin_called = true;
        return true; // Proceed with traversal
    }
    
    bool for_each(pugi::xml_node& node) override {
        // Required override - just continue traversal
        return true;
    }
    
    bool end(pugi::xml_node& node) override {
        // Custom cleanup logic
        end_called = true;
        return true;
    }
    
    bool begin_called = false;
    bool end_called = false;
};

extern "C" int LLVMFuzzerTestOneInput_105(const uint8_t *Data, size_t Size) {
    // Create XML document from fuzzer input
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer(Data, Size);
    
    // Test xml_node::root()
    pugi::xml_node root_node = doc.root();
    if (!root_node.empty()) {
        pugi::xml_node test_root = root_node.root();
    }
    
    // Test xml_node::parent()
    for (pugi::xml_node child = doc.first_child(); child; child = child.next_sibling()) {
        pugi::xml_node parent = child.parent();
    }
    
    // Test traversal with custom walker
    FuzzWalker walker;
    if (!doc.empty()) {
        doc.traverse(walker);
    }
    
    // Test xml_node::append_copy()
    // Create a simple test document for append_copy operations
    pugi::xml_document test_doc;
    pugi::xml_node root = test_doc.append_child("root");
    pugi::xml_node child1 = root.append_child("child1");
    pugi::xml_node child2 = root.append_child("child2");
    
    // Try to copy nodes
    if (!child1.empty() && !child2.empty()) {
        pugi::xml_node copied = child2.append_copy(child1);
    }
    
    // Test with nodes from parsed document
    if (!doc.empty()) {
        pugi::xml_node doc_root = doc.document_element();
        if (!doc_root.empty()) {
            pugi::xml_node first_child = doc_root.first_child();
            if (!first_child.empty()) {
                pugi::xml_node copied = root.append_copy(first_child);
            }
        }
    }
    
    // Test walker callbacks directly
    if (!doc.empty()) {
        FuzzWalker direct_walker;
        pugi::xml_node test_node = doc.first_child();
        if (!test_node.empty()) {
            direct_walker.begin(test_node);
            direct_walker.end(test_node);
        }
    }
    
    return 0;
}