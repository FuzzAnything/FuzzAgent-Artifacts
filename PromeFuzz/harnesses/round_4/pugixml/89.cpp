// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::next_sibling at pugixml.cpp:5735:34 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
// pugi::xml_node::traverse at pugixml.cpp:6789:30 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
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
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
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
#include <string_view>

// Custom walker class for testing traverse functionality
class FuzzWalker : public pugi::xml_tree_walker {
public:
    FuzzWalker() : current_depth(0), node_count(0) {}
    
    virtual bool begin(pugi::xml_node& node) override {
        current_depth = 0;
        node_count = 0;
        // Return true to proceed with traversal, false to cancel
        return true;
    }
    
    virtual bool for_each(pugi::xml_node& node) override {
        node_count++;
        current_depth = this->depth();
        return true;
    }
    
    virtual bool end(pugi::xml_node& node) override {
        return true;
    }
    
    int current_depth;
    int node_count;
};

static void test_xml_node_functions(pugi::xml_node node) {
    // Test parent() function
    pugi::xml_node parent = node.parent();
    
    // Test next_sibling() function
    pugi::xml_node next_sibling = node.next_sibling();
    
    // Test root() function
    pugi::xml_node root = node.root();
    
    // Test traverse() function with custom walker
    FuzzWalker walker;
    node.traverse(walker);
}

extern "C" int LLVMFuzzerTestOneInput_89(const uint8_t *Data, size_t Size) {
    // Write data to a dummy file for XML parsing
    const char* filename = "./dummy_file";
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        return 0;
    }
    file.write(reinterpret_cast<const char*>(Data), Size);
    file.close();
    
    // Parse XML document
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(filename);
    
    if (result) {
        // Get document element (root node)
        pugi::xml_node root = doc.document_element();
        
        if (root) {
            // Test functions on root node
            test_xml_node_functions(root);
            
            // Test functions on child nodes
            for (pugi::xml_node child = root.first_child(); child; child = child.next_sibling()) {
                test_xml_node_functions(child);
                
                // Test functions on grandchildren
                for (pugi::xml_node grandchild = child.first_child(); grandchild; grandchild = grandchild.next_sibling()) {
                    test_xml_node_functions(grandchild);
                }
            }
            
            // Test functions with empty nodes
            pugi::xml_node empty_node;
            test_xml_node_functions(empty_node);
            
            // Test parent() on document node
            pugi::xml_node doc_node = doc;
            pugi::xml_node doc_parent = doc_node.parent();
            
            // Test root() on various nodes
            pugi::xml_node root_from_child = root.first_child().root();
            pugi::xml_node root_from_grandchild = root.first_child().first_child().root();
            
            // Test next_sibling() with name filter using string_view
            if (Size > 0) {
                // Use first byte of input as a simple name filter
                char name_char = static_cast<char>(Data[0] % 26 + 'a');
                std::string name_str(1, name_char);
                std::string_view name_sv = name_str;
                
                pugi::xml_node next_named = root.next_sibling(name_sv.data());
            }
        }
        
        // Test traverse on empty document
        pugi::xml_document empty_doc;
        pugi::xml_node empty_root = empty_doc.document_element();
        if (empty_root) {
            FuzzWalker empty_walker;
            empty_root.traverse(empty_walker);
        }
    }
    
    // Test with memory buffer parsing as well
    pugi::xml_document mem_doc;
    pugi::xml_parse_result mem_result = mem_doc.load_buffer(Data, Size);
    
    if (mem_result) {
        pugi::xml_node mem_root = mem_doc.document_element();
        if (mem_root) {
            test_xml_node_functions(mem_root);
        }
    }
    
    // Clean up dummy file
    std::remove(filename);
    
    return 0;
}