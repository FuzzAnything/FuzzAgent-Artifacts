// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::traverse at pugixml.cpp:6789:30 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5735:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
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
#include <cstddef>
#include <string>
#include <vector>

static class FuzzWalker : public pugi::xml_tree_walker {
public:
    bool begin(pugi::xml_node& node) override {
        // Test root() and parent() on the traversal starting node
        (void)node.root();
        (void)node.parent();
        return true;
    }
    
    bool for_each(pugi::xml_node& node) override {
        // Test all target functions on each node during traversal
        (void)node.root();
        (void)node.parent();
        (void)node.next_sibling();
        return true;
    }
    
    bool end(pugi::xml_node& node) override {
        // Test root() and parent() on the traversal ending node
        (void)node.root();
        (void)node.parent();
        return true;
    }
} g_walker;

extern "C" int LLVMFuzzerTestOneInput_116(const uint8_t *Data, size_t Size) {
    // Write fuzzer input to a file for XML parsing
    const char* filename = "./dummy_file";
    FILE* fp = fopen(filename, "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    // Parse XML document
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(filename);
    
    if (result) {
        // Test functions on document node
        pugi::xml_node doc_node = doc;
        (void)doc_node.root();
        (void)doc_node.parent();
        (void)doc_node.next_sibling();
        
        // Test functions on document element
        pugi::xml_node root = doc.document_element();
        if (root) {
            (void)root.root();
            (void)root.parent();
            (void)root.next_sibling();
            
            // Test traversal with custom walker
            (void)root.traverse(g_walker);
            
            // Test functions on child nodes
            for (pugi::xml_node child = root.first_child(); child; child = child.next_sibling()) {
                (void)child.root();
                (void)child.parent();
                (void)child.next_sibling();
                
                // Test next_sibling with string_view parameter
                std::string_view name_view(reinterpret_cast<const char*>(Data), Size % 256);
                (void)child.next_sibling(name_view.data());
            }
        }
        
        // Test functions on random nodes by traversing the tree
        std::vector<pugi::xml_node> nodes;
        nodes.push_back(doc_node);
        
        for (size_t i = 0; i < nodes.size() && i < 100; ++i) {
            pugi::xml_node current = nodes[i];
            
            // Test basic navigation functions
            (void)current.root();
            (void)current.parent();
            (void)current.next_sibling();
            
            // Add children to explore more nodes
            for (pugi::xml_node child = current.first_child(); child; child = child.next_sibling()) {
                nodes.push_back(child);
            }
        }
    }
    
    // Test with empty document
    pugi::xml_document empty_doc;
    pugi::xml_node empty_node;
    (void)empty_node.root();
    (void)empty_node.parent();
    (void)empty_node.next_sibling();
    
    // Test with constructed node from null pointer
    pugi::xml_node null_node(nullptr);
    (void)null_node.root();
    (void)null_node.parent();
    (void)null_node.next_sibling();
    
    // Test walker directly on null node
    pugi::xml_node test_node;
    (void)g_walker.begin(test_node);
    (void)g_walker.end(test_node);
    
    return 0;
}