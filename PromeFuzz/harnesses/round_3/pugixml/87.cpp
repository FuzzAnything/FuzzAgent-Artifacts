// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
// pugi::xml_node::traverse at pugixml.cpp:6789:30 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
// pugi::xml_node::traverse at pugixml.cpp:6789:30 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::traverse at pugixml.cpp:6789:30 in pugixml.hpp
// pugi::xml_node::traverse at pugixml.cpp:6789:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::traverse at pugixml.cpp:6789:30 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
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

// Custom walker class to test traversal
class test_walker : public pugi::xml_tree_walker {
public:
    test_walker() : pugi::xml_tree_walker() {}
    
    virtual bool begin(pugi::xml_node& node) override {
        // Test the begin callback
        (void)node.root();
        (void)node.parent();
        (void)node.next_sibling();
        return true;  // Continue traversal
    }
    
    virtual bool for_each(pugi::xml_node& node) override {
        // Test functions on each node during traversal
        (void)node.root();
        (void)node.parent();
        (void)node.next_sibling();
        return true;  // Continue traversal
    }
    
    virtual bool end(pugi::xml_node& node) override {
        // Test the end callback
        (void)node.root();
        (void)node.parent();
        (void)node.next_sibling();
        return true;
    }
};

// Helper function to create XML document from fuzz data
static pugi::xml_document create_document(const uint8_t* data, size_t size) {
    pugi::xml_document doc;
    
    // Try to parse the data as XML
    if (size > 0) {
        // Create a buffer with null terminator
        char* buffer = new char[size + 1];
        memcpy(buffer, data, size);
        buffer[size] = '\0';
        
        // Try parsing with various options
        pugi::xml_parse_result result = doc.load_buffer(buffer, size);
        (void)result;  // Ignore parse result for fuzzing
        
        delete[] buffer;
    }
    
    return doc;
}

extern "C" int LLVMFuzzerTestOneInput_87(const uint8_t* data, size_t size) {
    // Create XML document from fuzz data
    pugi::xml_document doc = create_document(data, size);
    
    // Test 1: xml_node::root() on various nodes
    {
        // Test on document node
        pugi::xml_node doc_node = doc;
        pugi::xml_node doc_root = doc_node.root();
        (void)doc_root;
        
        // Test on document element
        pugi::xml_node root_element = doc.document_element();
        if (root_element) {
            pugi::xml_node root_from_element = root_element.root();
            (void)root_from_element;
            
            // Test on child nodes
            for (pugi::xml_node child = root_element.first_child(); child; child = child.next_sibling()) {
                pugi::xml_node child_root = child.root();
                (void)child_root;
            }
        }
        
        // Test on empty node
        pugi::xml_node empty_node;
        pugi::xml_node empty_root = empty_node.root();
        (void)empty_root;
    }
    
    // Test 2: xml_node::parent() on various nodes
    {
        pugi::xml_node root_element = doc.document_element();
        if (root_element) {
            // Test parent of root element (should be document node)
            pugi::xml_node parent_of_root = root_element.parent();
            (void)parent_of_root;
            
            // Test parents of all children
            for (pugi::xml_node child = root_element.first_child(); child; child = child.next_sibling()) {
                pugi::xml_node child_parent = child.parent();
                (void)child_parent;
                
                // Test parents of grandchildren
                for (pugi::xml_node grandchild = child.first_child(); grandchild; grandchild = grandchild.next_sibling()) {
                    pugi::xml_node grandchild_parent = grandchild.parent();
                    (void)grandchild_parent;
                }
            }
        }
        
        // Test on empty node
        pugi::xml_node empty_node;
        pugi::xml_node empty_parent = empty_node.parent();
        (void)empty_parent;
    }
    
    // Test 3: xml_node::next_sibling() on various nodes
    {
        pugi::xml_node root_element = doc.document_element();
        if (root_element) {
            // Iterate through all siblings of root element
            pugi::xml_node sibling = root_element;
            while (sibling) {
                pugi::xml_node next = sibling.next_sibling();
                (void)next;
                sibling = next;
            }
            
            // Test on child nodes
            for (pugi::xml_node child = root_element.first_child(); child; child = child.next_sibling()) {
                pugi::xml_node next_sib = child.next_sibling();
                (void)next_sib;
            }
        }
        
        // Test on empty node
        pugi::xml_node empty_node;
        pugi::xml_node empty_next_sibling = empty_node.next_sibling();
        (void)empty_next_sibling;
    }
    
    // Test 4: xml_tree_walker::begin() and xml_tree_walker::end()
    {
        test_walker walker;
        pugi::xml_node root_element = doc.document_element();
        
        if (root_element) {
            // Test begin callback directly
            bool begin_result = walker.begin(root_element);
            (void)begin_result;
            
            // Test end callback directly
            bool end_result = walker.end(root_element);
            (void)end_result;
            
            // Test traversal
            bool traverse_result = root_element.traverse(walker);
            (void)traverse_result;
        }
        
        // Test with empty node
        pugi::xml_node empty_node;
        bool empty_begin_result = walker.begin(empty_node);
        (void)empty_begin_result;
        
        bool empty_end_result = walker.end(empty_node);
        (void)empty_end_result;
    }
    
    // Test 5: xml_node::traverse() with custom walker
    {
        test_walker walker;
        pugi::xml_node root_element = doc.document_element();
        
        if (root_element) {
            // Traverse from root element
            bool traverse_result = root_element.traverse(walker);
            (void)traverse_result;
            
            // Traverse from child nodes
            for (pugi::xml_node child = root_element.first_child(); child; child = child.next_sibling()) {
                bool child_traverse_result = child.traverse(walker);
                (void)child_traverse_result;
            }
        }
        
        // Test traversal on empty node
        pugi::xml_node empty_node;
        bool empty_traverse_result = empty_node.traverse(walker);
        (void)empty_traverse_result;
    }
    
    // Additional state exploration: Create nodes and test functions
    {
        // Create a new document
        pugi::xml_document new_doc;
        
        // Add some nodes
        pugi::xml_node root = new_doc.append_child("root");
        pugi::xml_node child1 = root.append_child("child1");
        pugi::xml_node child2 = root.append_child("child2");
        pugi::xml_node grandchild = child1.append_child("grandchild");
        
        // Test functions on newly created nodes
        (void)root.root();
        (void)root.parent();
        (void)root.next_sibling();
        
        (void)child1.root();
        (void)child1.parent();
        (void)child1.next_sibling();
        
        (void)child2.root();
        (void)child2.parent();
        (void)child2.next_sibling();
        
        (void)grandchild.root();
        (void)grandchild.parent();
        (void)grandchild.next_sibling();
        
        // Test traversal on new document
        test_walker new_walker;
        bool new_traverse_result = root.traverse(new_walker);
        (void)new_traverse_result;
    }
    
    return 0;
}