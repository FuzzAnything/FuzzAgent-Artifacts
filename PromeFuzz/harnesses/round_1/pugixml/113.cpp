// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::traverse at pugixml.cpp:6789:30 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5735:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5735:34 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
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
#include <string_view>

// Custom tree walker to test traversal functions
class TestWalker : public pugi::xml_tree_walker {
public:
    TestWalker() = default;
    
    virtual bool begin(pugi::xml_node& node) override {
        // Test root() and parent() on the starting node
        (void)node.root();
        (void)node.parent();
        return true; // Continue traversal
    }
    
    virtual bool for_each(pugi::xml_node& node) override {
        // Test various functions on each node during traversal
        (void)node.root();
        (void)node.parent();
        (void)node.next_sibling();
        
        // Test next_sibling with string_view
        std::string_view sv;
        (void)node.next_sibling(sv.data());
        
        return true; // Continue traversal
    }
    
    virtual bool end(pugi::xml_node& node) override {
        // Test functions on the root node at the end
        (void)node.root();
        (void)node.parent();
        return true;
    }
};

static void test_node_functions(pugi::xml_node node) {
    // Test root() function
    pugi::xml_node root_node = node.root();
    (void)root_node;
    
    // Test parent() function
    pugi::xml_node parent_node = node.parent();
    (void)parent_node;
    
    // Test next_sibling() function
    pugi::xml_node next_sib = node.next_sibling();
    (void)next_sib;
    
    // Test next_sibling() with string_view
    std::string_view name_sv;
    pugi::xml_node next_sib_named = node.next_sibling(name_sv.data());
    (void)next_sib_named;
}

extern "C" int LLVMFuzzerTestOneInput_113(const uint8_t *Data, size_t Size) {
    static bool initialized = false;
    static const char* dummy_file = "./dummy_file";
    
    // Write fuzzer data to dummy file
    std::ofstream out_file(dummy_file, std::ios::binary);
    if (!out_file.is_open()) {
        return 0;
    }
    out_file.write(reinterpret_cast<const char*>(Data), Size);
    out_file.close();
    
    // Parse XML document from file
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(dummy_file);
    
    if (result) {
        // Get document root
        pugi::xml_node root = doc.root();
        
        // Test node functions on root
        test_node_functions(root);
        
        // Test node functions on all nodes in the document
        for (pugi::xml_node node = doc.first_child(); node; node = node.next_sibling()) {
            test_node_functions(node);
            
            // Recursively test all children
            for (pugi::xml_node child = node.first_child(); child; child = child.next_sibling()) {
                test_node_functions(child);
            }
        }
        
        // Test traversal with custom walker
        TestWalker walker;
        root.traverse(walker);
        
        // Test walker begin and end directly
        pugi::xml_node test_node = root;
        (void)walker.begin(test_node);
        (void)walker.end(test_node);
    }
    
    // Also test with empty/invalid nodes
    pugi::xml_node empty_node;
    test_node_functions(empty_node);
    
    // Test with constructed node from nullptr
    pugi::xml_node null_node(nullptr);
    test_node_functions(null_node);
    
    // Test walker with empty node
    TestWalker empty_walker;
    pugi::xml_node empty_node_for_walker;
    (void)empty_walker.begin(empty_node_for_walker);
    (void)empty_walker.end(empty_node_for_walker);
    
    // Clean up dummy file
    std::remove(dummy_file);
    
    return 0;
}