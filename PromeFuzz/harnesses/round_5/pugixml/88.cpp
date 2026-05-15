// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::traverse at pugixml.cpp:6789:30 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
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
#include <fstream>

// Custom walker class to test traversal callbacks
class FuzzWalker : public pugi::xml_tree_walker {
public:
    virtual bool begin(pugi::xml_node& node) override {
        // Test parent() and root() on the starting node
        (void)node.parent();
        (void)node.root();
        return true;
    }
    
    virtual bool for_each(pugi::xml_node& node) override {
        // Test next_sibling() during traversal
        (void)node.next_sibling();
        (void)node.parent();
        (void)node.root();
        return true;
    }
    
    virtual bool end(pugi::xml_node& node) override {
        // Test parent() and root() on the ending node
        (void)node.parent();
        (void)node.root();
        return true;
    }
};

static void test_xml_node_functions(pugi::xml_node& node) {
    // Test parent() function
    pugi::xml_node parent = node.parent();
    (void)parent;
    
    // Test root() function
    pugi::xml_node root = node.root();
    (void)root;
    
    // Test next_sibling() function
    pugi::xml_node next_sibling = node.next_sibling();
    (void)next_sibling;
    
    // Test traverse() function with custom walker
    FuzzWalker walker;
    (void)node.traverse(walker);
}

extern "C" int LLVMFuzzerTestOneInput_88(const uint8_t *Data, size_t Size) {
    // Create XML document
    pugi::xml_document doc;
    
    // Try to parse the input as XML
    pugi::xml_parse_result result = doc.load_buffer(Data, Size);
    
    // Test functions on the document root
    pugi::xml_node root = doc.root();
    test_xml_node_functions(root);
    
    // Test functions on document element (first child)
    pugi::xml_node child = doc.first_child();
    test_xml_node_functions(child);
    
    // Test functions on an empty node
    pugi::xml_node empty_node;
    test_xml_node_functions(empty_node);
    
    // Test walker begin/end methods directly
    FuzzWalker direct_walker;
    pugi::xml_node test_node = root.empty() ? empty_node : root;
    (void)direct_walker.begin(test_node);
    (void)direct_walker.end(test_node);
    
    // Also test with a file-based approach
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(Data), Size);
        file.close();
        
        pugi::xml_document file_doc;
        pugi::xml_parse_result file_result = file_doc.load_file("./dummy_file");
        
        if (file_result) {
            pugi::xml_node file_root = file_doc.root();
            test_xml_node_functions(file_root);
        }
        
        // Clean up the dummy file
        std::remove("./dummy_file");
    }
    
    return 0;
}