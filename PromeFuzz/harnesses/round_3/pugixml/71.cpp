// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::traverse at pugixml.cpp:6789:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::traverse at pugixml.cpp:6789:30 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5735:34 in pugixml.hpp
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
#include "pugixml.hpp"
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string_view>

static class test_walker : public pugi::xml_tree_walker {
public:
    virtual bool begin(pugi::xml_node& node) override {
        // Do some minimal operations with the node
        (void)node.name();
        (void)node.type();
        return true; // Continue traversal
    }
    
    virtual bool for_each(pugi::xml_node& node) override {
        // Required override - do minimal operations
        (void)node.name();
        (void)node.type();
        return true; // Continue traversal
    }
    
    virtual bool end(pugi::xml_node& node) override {
        // Do some minimal operations with the node
        (void)node.name();
        (void)node.type();
        return true; // Success
    }
} g_walker;

extern "C" int LLVMFuzzerTestOneInput_71(const uint8_t *Data, size_t Size) {
    // First, try to parse XML from the input data
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer(Data, Size, pugi::parse_default | pugi::parse_fragment);
    
    // Test xml_node::root() on various nodes
    pugi::xml_node root_node = doc.root();
    (void)root_node;
    
    // Test on document element
    pugi::xml_node doc_element = doc.document_element();
    if (doc_element) {
        pugi::xml_node root_from_element = doc_element.root();
        (void)root_from_element;
    }
    
    // Test on empty node
    pugi::xml_node empty_node;
    pugi::xml_node root_from_empty = empty_node.root();
    (void)root_from_empty;
    
    // Test xml_node::parent() on various nodes
    if (doc_element) {
        pugi::xml_node parent_of_element = doc_element.parent();
        (void)parent_of_element;
        
        // Try to get children and test parent on them
        for (pugi::xml_node child = doc_element.first_child(); child; child = child.next_sibling()) {
            pugi::xml_node parent_of_child = child.parent();
            (void)parent_of_child;
        }
    }
    
    // Test xml_node::next_sibling() on various nodes
    if (doc_element) {
        pugi::xml_node first_child = doc_element.first_child();
        if (first_child) {
            pugi::xml_node next_sibling = first_child.next_sibling();
            (void)next_sibling;
            
            // Test with name filter using string_view
            std::string_view name_filter = "";
            if (Size > 0) {
                // Use first byte to determine filter length (modulo to avoid overflow)
                size_t filter_len = Data[0] % 16;
                if (filter_len > 0 && filter_len <= Size) {
                    name_filter = std::string_view(reinterpret_cast<const char*>(Data), filter_len);
                }
            }
            pugi::xml_node next_sibling_named = first_child.next_sibling(name_filter.data());
            (void)next_sibling_named;
        }
    }
    
    // Test xml_node::traverse() with our walker
    if (doc_element) {
        bool traverse_result = doc_element.traverse(g_walker);
        (void)traverse_result;
        
        // Also test on empty node
        empty_node.traverse(g_walker);
    }
    
    // Test xml_tree_walker::begin() and end() directly
    if (doc_element) {
        bool begin_result = g_walker.begin(doc_element);
        (void)begin_result;
        
        bool end_result = g_walker.end(doc_element);
        (void)end_result;
    }
    
    // Test with empty node for begin/end
    bool begin_empty_result = g_walker.begin(empty_node);
    (void)begin_empty_result;
    
    bool end_empty_result = g_walker.end(empty_node);
    (void)end_empty_result;
    
    // Additional test: create a node from scratch and test functions
    pugi::xml_node manual_node = doc.append_child("test_node");
    if (manual_node) {
        pugi::xml_node manual_root = manual_node.root();
        (void)manual_root;
        
        pugi::xml_node manual_parent = manual_node.parent();
        (void)manual_parent;
        
        pugi::xml_node manual_next_sibling = manual_node.next_sibling();
        (void)manual_next_sibling;
        
        bool manual_traverse = manual_node.traverse(g_walker);
        (void)manual_traverse;
    }
    
    // Test with a file if needed (though we use buffer parsing above)
    // This is kept as an alternative path
    if (Size > 0 && Size < 1024*1024) { // Limit size for file writing
        FILE* f = fopen("./dummy_file", "wb");
        if (f) {
            fwrite(Data, 1, Size, f);
            fclose(f);
            
            pugi::xml_document file_doc;
            pugi::xml_parse_result file_result = file_doc.load_file("./dummy_file");
            if (file_result) {
                pugi::xml_node file_root = file_doc.root();
                (void)file_root;
                
                // Remove the file after use
                remove("./dummy_file");
            }
        }
    }
    
    return 0;
}