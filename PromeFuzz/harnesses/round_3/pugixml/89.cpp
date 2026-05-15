// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5735:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::internal_object at pugixml.cpp:6841:42 in pugixml.hpp
// pugi::xml_node::internal_object at pugixml.cpp:6841:42 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::traverse at pugixml.cpp:6789:30 in pugixml.hpp
// pugi::xml_node::traverse at pugixml.cpp:6789:30 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
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
#include <string_view>

static class test_walker : public pugi::xml_tree_walker {
public:
    virtual bool begin(pugi::xml_node& node) override {
        // Store the root node for later comparison
        begin_root = node.root();
        return true;
    }
    
    virtual bool for_each(pugi::xml_node& node) override {
        // Required pure virtual function implementation
        return true;
    }

    virtual bool end(pugi::xml_node& node) override {
        // Verify the end node matches the begin node
        end_root = node.root();
        return true;
    }

    pugi::xml_node begin_root;
    pugi::xml_node end_root;
} g_walker;

extern "C" int LLVMFuzzerTestOneInput_89(const uint8_t *Data, size_t Size) {
    // Create document and parse input
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer(Data, Size, pugi::parse_default, pugi::encoding_auto);
    
    if (!result) {
        // Try with a dummy file if buffer parsing fails
        const char* dummy_file = "./dummy_file";
        FILE* f = fopen(dummy_file, "wb");
        if (f) {
            fwrite(Data, 1, Size, f);
            fclose(f);
            result = doc.load_file(dummy_file);
            remove(dummy_file);
        }
    }

    // Test API functions if we have a valid document
    if (result) {
        // Get root node
        pugi::xml_node root = doc.root();
        
        // Test 1: xml_node::root()
        if (root) {
            pugi::xml_node test_root = root.root();
            // Test root() on child nodes
            for (pugi::xml_node child = root.first_child(); child; child = child.next_sibling()) {
                pugi::xml_node child_root = child.root();
            }
        }
        
        // Test 2 & 6: xml_tree_walker::begin() and end() via traverse()
        if (root) {
            bool traverse_result = root.traverse(g_walker);
            
            // Reset walker state for next iteration
            g_walker.begin_root = pugi::xml_node();
            g_walker.end_root = pugi::xml_node();
        }
        
        // Test 3: xml_node::traverse() with a local walker
        if (root) {
            class local_walker : public pugi::xml_tree_walker {
                virtual bool begin(pugi::xml_node& node) override { return true; }
                virtual bool for_each(pugi::xml_node& node) override { return true; }
                virtual bool end(pugi::xml_node& node) override { return true; }
            };
            local_walker local;
            root.traverse(local);
        }
        
        // Test 4: xml_node::parent()
        if (root) {
            // Test parent() on all nodes in document
            for (pugi::xml_node node = root.first_child(); node; node = node.next_sibling()) {
                pugi::xml_node parent = node.parent();
                // Recursively test children
                for (pugi::xml_node child = node.first_child(); child; child = child.next_sibling()) {
                    pugi::xml_node child_parent = child.parent();
                }
            }
        }
        
        // Test 5: xml_node::next_sibling()
        if (root) {
            // Test with empty string_view
            std::string_view empty_name;
            
            // Iterate through all siblings
            for (pugi::xml_node sibling = root.first_child(); sibling; sibling = sibling.next_sibling()) {
                // Test next_sibling() with and without name filter
                pugi::xml_node next = sibling.next_sibling();
                
                // Test with specific name if node has a name
                if (sibling.name() && strlen(sibling.name()) > 0) {
                    std::string_view node_name(sibling.name());
                    pugi::xml_node next_named = sibling.next_sibling(node_name.data());
                }
            }
        }
        
        // Additional state exploration: test on various node types
        pugi::xml_node null_node;
        pugi::xml_node test_null_root = null_node.root();
        pugi::xml_node test_null_parent = null_node.parent();
        pugi::xml_node test_null_sibling = null_node.next_sibling();
        
        // Test on document node itself
        pugi::xml_node doc_as_node = doc;
        pugi::xml_node doc_root = doc_as_node.root();
        pugi::xml_node doc_parent = doc_as_node.parent();
        
        // Create a node from internal pointer (if we can get one)
        if (root.internal_object()) {
            pugi::xml_node from_ptr(root.internal_object());
            pugi::xml_node ptr_root = from_ptr.root();
        }
    }
    
    return 0;
}