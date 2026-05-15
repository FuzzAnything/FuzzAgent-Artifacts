// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_tree_walker::begin at pugixml.cpp:5227:37 in pugixml.hpp
// pugi::xml_tree_walker::end at pugixml.cpp:5232:37 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5797:34 in pugixml.hpp
// pugi::xml_node::traverse at pugixml.cpp:6789:30 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
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

// Custom walker to test begin/end callbacks
class test_walker : public pugi::xml_tree_walker {
public:
    bool begin(pugi::xml_node& node) override {
        // Test base implementation - just return true
        return pugi::xml_tree_walker::begin(node);
    }
    
    bool for_each(pugi::xml_node& node) override {
        // Required implementation for traversal
        return true; // Continue traversal
    }
    
    bool end(pugi::xml_node& node) override {
        // Test base implementation - just return true
        return pugi::xml_tree_walker::end(node);
    }
};

static void test_node_functions(pugi::xml_node node) {
    // Test root() function
    pugi::xml_node root_node = node.root();
    
    // Test parent() function
    pugi::xml_node parent_node = node.parent();
    
    // Test next_sibling() function
    pugi::xml_node next_sibling_node = node.next_sibling();
    
    // Test next_sibling() with string_view parameter (if available)
    // Note: This tests the overloaded version mentioned in the hint
    std::string_view sv;
    pugi::xml_node next_sibling_named = node.next_sibling(sv);
}

static void test_traversal(pugi::xml_node node) {
    // Create walker and test traversal
    test_walker walker;
    
    // Test begin() callback directly
    bool begin_result = walker.begin(node);
    
    // Test traverse() function
    bool traverse_result = node.traverse(walker);
    
    // Test end() callback directly
    bool end_result = walker.end(node);
}

extern "C" int LLVMFuzzerTestOneInput_119(const uint8_t *Data, size_t Size) {
    // Create XML document
    pugi::xml_document doc;
    
    // Try to parse the input as XML
    pugi::xml_parse_result result = doc.load_buffer(Data, Size, pugi::parse_default | pugi::parse_fragment);
    
    if (result) {
        // Document parsed successfully, test functions on various nodes
        
        // Test on document node
        pugi::xml_node doc_node = doc;
        test_node_functions(doc_node);
        test_traversal(doc_node);
        
        // Test on document element (root element)
        pugi::xml_node root = doc.document_element();
        if (root) {
            test_node_functions(root);
            test_traversal(root);
            
            // Test on first child
            pugi::xml_node first_child = root.first_child();
            if (first_child) {
                test_node_functions(first_child);
                test_traversal(first_child);
            }
            
            // Test on children nodes (not attributes)
            for (pugi::xml_node child = root.first_child(); child; child = child.next_sibling()) {
                test_node_functions(child);
                test_traversal(child);
            }
        }
        
        // Also test on empty/null nodes
        pugi::xml_node empty_node;
        test_node_functions(empty_node);
        test_traversal(empty_node);
        
        // Test with constructed node from nullptr
        pugi::xml_node null_node(nullptr);
        test_node_functions(null_node);
        test_traversal(null_node);
    } else {
        // Even if parsing fails, test with empty document
        test_node_functions(doc);
        test_traversal(doc);
        
        // Also test with a manually created node structure
        // Create a simple tree to test navigation functions
        pugi::xml_document test_doc;
        pugi::xml_node root = test_doc.append_child("root");
        pugi::xml_node child1 = root.append_child("child1");
        pugi::xml_node child2 = root.append_child("child2");
        
        test_node_functions(root);
        test_node_functions(child1);
        test_node_functions(child2);
        
        test_traversal(root);
        test_traversal(child1);
        test_traversal(child2);
    }
    
    return 0;
}