// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::traverse at pugixml.cpp:6789:30 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::append_copy at pugixml.cpp:6397:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
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
        // Test root() and parent() during begin callback
        (void)node.root();
        (void)node.parent();
        return true;
    }
    
    bool for_each(pugi::xml_node& node) override {
        // Required override but not in target list, minimal implementation
        (void)node.root();
        (void)node.parent();
        return true;
    }
    
    bool end(pugi::xml_node& node) override {
        // Test root() and parent() during end callback
        (void)node.root();
        (void)node.parent();
        return true;
    }
} fuzz_walker;

extern "C" int LLVMFuzzerTestOneInput_112(const uint8_t *Data, size_t Size) {
    // Create XML document from fuzzer input
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer(Data, Size, pugi::parse_default | pugi::parse_fragment);
    
    if (!result) {
        // If parsing fails, create minimal document for API testing
        doc.append_child("root").append_child("child");
    }
    
    // Get root node for testing
    pugi::xml_node root = doc.root();
    
    // Test xml_node::root()
    pugi::xml_node test_root = root.root();
    (void)test_root;
    
    // Test xml_node::parent()
    if (root.first_child()) {
        pugi::xml_node parent = root.first_child().parent();
        (void)parent;
    }
    
    // Test xml_node::traverse() with xml_tree_walker
    root.traverse(fuzz_walker);
    
    // Test xml_node::append_copy()
    if (root.first_child()) {
        pugi::xml_node copy = root.append_copy(root.first_child());
        (void)copy;
    }
    
    // Test with empty nodes
    pugi::xml_node empty;
    (void)empty.root();
    (void)empty.parent();
    
    // Test walker callbacks on empty node (should handle gracefully)
    fuzz_walker.begin(empty);
    fuzz_walker.end(empty);
    
    return 0;
}